#include "data_base.h"
#include "consts.h"
read_status data_base::read_base(const char *filename)
{
	FILE *fp;
	list2_node *curr;
	list2_node *tail;
	read_status res;
	search_returns er_code;
	int grp;
	if(data_list.head != nullptr)
	{
		this->delete_data_base();
	}
	if(!(fp = fopen(filename, "r")))
	{
		return read_status::open;
	}
	curr = new list2_node;
	if(curr == nullptr)
	{
		fclose(fp);
		return read_status::memory;
	}
	res = curr->read(fp);
	if(res != read_status::success)
	{
		if(res == read_status::eof)
		{
			fclose(fp);
			delete curr;
			return read_status::success;
		}
		else
		{
			fclose(fp);
			delete curr;
			return res;
		}
	}
	data_list.head = tail = curr;
	grp = curr->get_group();
	er_code = g_search[grp].add_node(curr);
	switch(er_code)
	{
		case search_returns::success:
		break;
		case search_returns::memory:
		fclose(fp);
		this->delete_data_base();
		return read_status::memory;
		case search_returns::balance:
		fclose(fp);
		this->delete_data_base();
		return read_status::balance;
		case search_returns::repeated_addition:
		fclose(fp);
		this->delete_data_base();
		return read_status::unknown;
		case search_returns::unknown:
		fclose(fp);
		this->delete_data_base();
		return read_status::unknown;
	}
	er_code = n_p_search.add_node(curr);
	switch(er_code)
	{
		case search_returns::success:
		break;
		case search_returns::memory:
		fclose(fp);
		this->delete_data_base();
		return read_status::memory;
		case search_returns::balance:
		fclose(fp);
		this->delete_data_base();
		return read_status::balance;
		case search_returns::repeated_addition:
		fclose(fp);
		this->delete_data_base();
		return read_status::unknown;
		case search_returns::unknown:
		fclose(fp);
		this->delete_data_base();
		return read_status::unknown;
	}
	curr = new list2_node;
	if(curr == nullptr)
	{
		fclose(fp);
		this->delete_data_base();
		return read_status::memory;
	}
	while((res = curr->read(fp)) == read_status::success)
	{
		grp = curr->get_group();
		if(grp >= MAX_GROUP)
		{
			delete curr;
			fclose(fp);
			this->delete_data_base();
			return read_status::group;
		}
		er_code = g_search[grp].add_node(curr);
		switch(er_code)
		{
			case search_returns::success:
			er_code = n_p_search.add_node(curr);
			switch(er_code)
			{
				case search_returns::success:
				curr->prev = tail;
				tail->next = curr;
				tail = curr;
				break;
				case search_returns::memory:
				fclose(fp);
				this->delete_data_base();
				return read_status::memory;
				case search_returns::balance:
				fclose(fp);
				this->delete_data_base();
				return read_status::balance;
				case search_returns::repeated_addition:
				delete curr;
				break;
				case search_returns::unknown:
				fclose(fp);
				this->delete_data_base();
				return read_status::unknown;
			}
			break;
			case search_returns::memory:
			fclose(fp);
			this->delete_data_base();
			return read_status::memory;
			case search_returns::balance:
			fclose(fp);
			this->delete_data_base();
			return read_status::balance;
			case search_returns::repeated_addition:
			delete curr;
			break;
			case search_returns::unknown:
			fclose(fp);
			this->delete_data_base();
			return read_status::unknown;
		}
		curr = new list2_node;
		if(curr == nullptr)
		{
			fclose(fp);
			this->delete_data_base();
			return read_status::memory;
		}
	}
	delete curr;
	if(!feof(fp))
	{
		fclose(fp);
		this->delete_data_base();
		return read_status::format;
	}
	fclose(fp);
	return read_status::success;
}
int data_base::command_apply(command &x, int fd)
{
	int i;
	int res = 0;
	int grp;
	search_returns er_code;
	command com;
	char buf[10];
	int len_num, nbytes;
	char num[INT_LEN];
	condition c_name = x.get_c_name();
	condition c_phone = x.get_c_phone();
	condition c_group = x.get_c_group();
	list2_node *curr = nullptr;
	list2_node *curr_n = nullptr;
	list2 search_list_name;
	list2_node *curr_search = nullptr;
	list2_node *tail_name = nullptr;
	list2 search_list_phone;
	list2_node *tail_phone = nullptr;
	list2 search_list_group;
	list2_node *tail_group = nullptr;
	list2 search_list_name_phone;
	list2 search_list;
	switch(x.get_type())
	{
		case command_type::stop:
		return -2;
		case command_type::quit:
		return -1;
		case command_type::insert:
		er_code = g_search[x.get_group()].insert_record(x, curr);
		switch(er_code)
		{
			case search_returns::success:
			if(!curr)
			{
				break;
			}
			er_code = n_p_search.add_node(curr);
			switch(er_code)
			{
				case search_returns::success:
				break;
				case search_returns::memory:
				return -3;
				case search_returns::balance:
				return -4;
				case search_returns::unknown:
				return -5;
				case search_returns::repeated_addition:
				break;
			}
			break;
			case search_returns::memory:
			return -3;
			case search_returns::balance:
			return -4;
			case search_returns::unknown:
			return -5;
			case search_returns::repeated_addition:
			break;
		}
		if(curr != nullptr)
		{
			if(data_list.head)
			{
				data_list.head->prev = curr;
				curr->next = data_list.head;
			}
			data_list.head = curr;
		}
		res = 0;
		// Формируем ответ в байтовой записи
		for(i = 0; i < INT_LEN; i++)
		{
			num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
		}
		// Пересылаем ответ
		len_num = INT_LEN;
		for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
		{
			nbytes = write(fd, num + i, len_num);
			if(nbytes < 0)
			{
				perror("write length");
				exit(EXIT_FAILURE);
			}
			else 
			{
				if(nbytes == 0)
				{
					perror ("write truncated");
					exit (EXIT_FAILURE);
				}
			}
		}
		return res;
		case command_type::select:
		if(x.get_op() != operation::lor)
		{
			if(c_group == condition::eq)
			{
				res = g_search[x.get_group()].search_group(c_name, c_phone, x, search_list_group, tail_group);
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				if(tail_group)
				{
					search_list.Neumann_search_group_sort(x);
					for(curr = search_list_group.head; curr != nullptr; curr = curr->next_search_group)
					{
						if(curr->print(fd, x.get_order()))
						{
							return -1;
						}
					}
				}
				search_list_group.head = nullptr;
				return res;
			}
			if(c_name == condition::eq)
			{
				res = n_p_search.search_name(c_name, x, search_list_name, tail_name);
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				if(tail_name)
				{
					search_list_name.Neumann_search_name_sort(x);
					for(curr = search_list_name.head; curr != nullptr; curr = curr->next_search_name)
					{
						if(curr->print(fd, x.get_order()))
						{
							return -1;
						}
					}
				}
				search_list_name.head = nullptr;
				return res;	
			}
			if(c_phone == condition::eq)
			{
				res = n_p_search.search_phone(c_phone, x, search_list_phone, tail_phone);
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				if(tail_phone)
				{
					search_list_phone.Neumann_search_phone_sort(x);
					for(curr = search_list_phone.head; curr != nullptr; curr = curr->next_search_phone)
					{
						if(curr->print(fd, x.get_order()))
						{
							return -1;
						}
					}
				}
				search_list_phone.head = nullptr;
				return res;
			}
			res = n_p_search.search_name(c_name, x, search_list_name, tail_name);
			if(res >= 0)
			{
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				if(tail_name)
				{
					search_list_name.Neumann_search_name_sort(x);
					for(curr = search_list_name.head; curr != nullptr; curr = curr->next_search_name)
					{
						if(curr->print(fd, x.get_order()))
						{
							return -1;
						}
					}
				}
				search_list_name.head = nullptr;
				return res;
			}
			res = -1;
			switch(c_group)
			{
				case condition::eq:
				res = g_search[x.get_group()].search_group(c_name, c_phone, x, search_list_group, tail_group);
				break;
				case condition::ne:
				break;
				case condition::lt:
				res = 0;
				for(i = 0; i < x.get_group(); i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::gt:
				res = 0;
				for(i = x.get_group() + 1; i < MAX_GROUP; i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::le:
				res = 0;
				for(i = 0; i <= x.get_group(); i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::ge:
				res = 0;
				for(i = x.get_group(); i < MAX_GROUP; i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::like:
				break;
				case condition::nlike:
				break;
				case condition::none:
				break;
			}
			if(res >= 0)
			{
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				if(tail_group)
				{
					search_list.Neumann_search_group_sort(x);
					for(curr = search_list_group.head; curr != nullptr; curr = curr->next_search_group)
					{
						if(curr->print(fd, x.get_order()))
						{
							return -1;
						}
					}
				}
				search_list_group.head = nullptr;
				return res;
			}
			res = n_p_search.search_phone(c_phone, x, search_list_phone, tail_phone);
			if(res >= 0)
			{
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				if(tail_phone)
				{
					search_list_phone.Neumann_search_phone_sort(x);
					for(curr = search_list_phone.head; curr != nullptr; curr = curr->next_search_phone)
					{
						if(curr->print(fd, x.get_order()))
						{
							return -1;
						}
					}
				}
				search_list_phone.head = nullptr;
				return res;
			}
			return data_list.command_apply(x, fd);
		}
		else
		{
			if(c_name == condition::like || c_name == condition::nlike || c_name == condition::ne)
			{
				return data_list.command_apply(x, fd);
			}
			if(c_phone == condition::ne || c_group == condition::ne)
			{
				return data_list.command_apply(x, fd);
			}
			res = 0;
			if(c_name != condition::none)
			{
				n_p_search.search_name(c_name, x, search_list_name, tail_name);
			}
			if(c_phone != condition::none)
			{
				n_p_search.search_phone(c_phone, x, search_list_phone, tail_phone);
			}
			switch(c_group)
			{
				case condition::eq:
				g_search[x.get_group()].search_group(c_name, c_phone, x, search_list_group, tail_group);
				break;
				case condition::ne:
				break;
				case condition::lt:
				res = 0;
				for(i = 0; i < x.get_group(); i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::gt:
				res = 0;
				for(i = x.get_group() + 1; i < MAX_GROUP; i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::le:
				res = 0;
				for(i = 0; i <= x.get_group(); i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::ge:
				res = 0;
				for(i = x.get_group(); i < MAX_GROUP; i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::like:
				break;
				case condition::nlike:
				break;
				case condition::none:
				break;
			}
			com.parse("select * order by phone, group, name", buf);
			search_list_name.Neumann_search_name_sort(com);
			search_list_phone.Neumann_search_phone_sort(com);
			search_list_group.Neumann_search_group_sort(com);
			search_list_name_phone.merge_name_phone(search_list_name, search_list_phone);
			res = search_list.merge_name_phone_group(search_list_name_phone, search_list_group);
			search_list.Neumann_search_sort(x);
			// Формируем ответ в байтовой записи
			for(i = 0; i < INT_LEN; i++)
			{
				num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
			}
			// Пересылаем ответ
			len_num = INT_LEN;
			for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
			{
				nbytes = write(fd, num + i, len_num);
				if(nbytes < 0)
				{
					perror("write length");
					exit(EXIT_FAILURE);
				}
				else 
				{
					if(nbytes == 0)
					{
						perror ("write truncated");
						exit (EXIT_FAILURE);
					}
				}
			}
			if(search_list.print_search(fd, x.get_order()))
			{
				return -1;
			}
			search_list_name.head = nullptr;
			search_list_phone.head = nullptr;
			search_list_group.head = nullptr;
			search_list_name_phone.head = nullptr;
			search_list.head = nullptr;
			return res;
		}
		return data_list.command_apply(x, fd);
		case command_type::del:
		if(x.get_op() != operation::lor)
		{
			if(c_group == condition::eq)
			{
				grp = x.get_group();
				g_search[grp].search_group(c_name, c_phone, x, search_list_group, tail_group);
				//if(res >= 0)
				{
					if(tail_group)
					{
						curr = search_list_group.head;
						while(curr != nullptr)
						{
							curr_n = curr->next_search_group;
							er_code = n_p_search.remove_record(*curr);
							switch(er_code)
							{
								case search_returns::success:
								break;
								case search_returns::memory:
								search_list_group.head = nullptr;
								return -3;
								case search_returns::balance:
								search_list_group.head = nullptr;
								return -4;
								case search_returns::unknown:
								search_list_group.head = nullptr;
								return -5;
								case search_returns::repeated_addition:
								search_list_group.head = nullptr;
								return -5;
							}
							er_code = g_search[grp].remove_record(*curr);
							switch(er_code)
							{
								case search_returns::success:
								break;
								case search_returns::memory:
								search_list_group.head = nullptr;
								return -3;
								case search_returns::balance:
								search_list_group.head = nullptr;
								return -4;
								case search_returns::unknown:
								search_list_group.head = nullptr;
								return -5;
								case search_returns::repeated_addition:
								search_list_group.head = nullptr;
								return -5;
							}
							if(curr->prev)
							{
								curr->prev->next = curr->next;
							}
							else
							{
								data_list.head = curr->next; 
							}
							if(curr->next)
							{
								curr->next->prev = curr->prev;
							}
							delete curr;
							curr = curr_n;
						}
					}
					search_list_group.head = nullptr;
					res = 0;
					// Формируем ответ в байтовой записи
					for(i = 0; i < INT_LEN; i++)
					{
						num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
					}
					// Пересылаем ответ
					len_num = INT_LEN;
					for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
					{
						nbytes = write(fd, num + i, len_num);
						if(nbytes < 0)
						{
							perror("write length");
							exit(EXIT_FAILURE);
						}
						else 
						{
							if(nbytes == 0)
							{
								perror ("write truncated");
								exit (EXIT_FAILURE);
							}
						}
					}
					return res;
				}	
			}
			if(c_name == condition::eq)
			{
				n_p_search.search_name(c_name, x, search_list_name, tail_name);
				//if(res >= 0)
				{
					if(tail_name)
					{
						curr = search_list_name.head;
						while(curr != nullptr)
						{
							curr_n = curr->next_search_name;
							er_code = n_p_search.remove_record(*curr);
							switch(er_code)
							{
								case search_returns::success:
								break;
								case search_returns::memory:
								search_list_name.head = nullptr;
								return -3;
								case search_returns::balance:
								search_list_name.head = nullptr;
								return -4;
								case search_returns::unknown:
								search_list_name.head = nullptr;
								return -5;
								case search_returns::repeated_addition:
								search_list_name.head = nullptr;
								return -5;
							}
							er_code = g_search[curr->get_group()].remove_record(*curr);
							switch(er_code)
							{
								case search_returns::success:
								break;
								case search_returns::memory:
								search_list_name.head = nullptr;
								return -3;
								case search_returns::balance:
								search_list_name.head = nullptr;
								return -4;
								case search_returns::unknown:
								search_list_name.head = nullptr;
								return -5;
								case search_returns::repeated_addition:
								search_list_name.head = nullptr;
								return -5;
							}
							if(curr->prev)
							{
								curr->prev->next = curr->next;
							}
							else
							{
								data_list.head = curr->next; 
							}
							if(curr->next)
							{
								curr->next->prev = curr->prev;
							}
							delete curr;
							curr = curr_n;
						}
					}
					search_list_name.head = nullptr;
					res = 0;
					// Формируем ответ в байтовой записи
					for(i = 0; i < INT_LEN; i++)
					{
						num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
					}
					// Пересылаем ответ
					len_num = INT_LEN;
					for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
					{
						nbytes = write(fd, num + i, len_num);
						if(nbytes < 0)
						{
							perror("write length");
							exit(EXIT_FAILURE);
						}
						else 
						{
							if(nbytes == 0)
							{
								perror ("write truncated");
								exit (EXIT_FAILURE);
							}
						}
					}
					return res;
				}	
			}
			if(c_phone == condition::eq)
			{
				n_p_search.search_phone(c_phone, x, search_list_phone, tail_phone);
				//if(res >= 0)
				{
					if(tail_phone)
					{
						curr = search_list_phone.head;
						while(curr != nullptr)
						{
							curr_n = curr->next_search_phone;
							er_code = n_p_search.remove_record(*curr);
							switch(er_code)
							{
								case search_returns::success:
								break;
								case search_returns::memory:
								search_list_phone.head = nullptr;
								return -3;
								case search_returns::balance:
								search_list_phone.head = nullptr;
								return -4;
								case search_returns::unknown:
								search_list_phone.head = nullptr;
								return -5;
								case search_returns::repeated_addition:
								search_list_phone.head = nullptr;
								return -5;
							}
							er_code = g_search[curr->get_group()].remove_record(*curr);
							switch(er_code)
							{
								case search_returns::success:
								break;
								case search_returns::memory:
								search_list_phone.head = nullptr;
								return -3;
								case search_returns::balance:
								search_list_phone.head = nullptr;
								return -4;
								case search_returns::unknown:
								search_list_phone.head = nullptr;
								return -5;
								case search_returns::repeated_addition:
								search_list_phone.head = nullptr;
								return -5;
							}
							if(curr->prev)
							{
								curr->prev->next = curr->next;
							}
							else
							{
								data_list.head = curr->next; 
							}
							if(curr->next)
							{
								curr->next->prev = curr->prev;
							}
							delete curr;
							curr = curr_n;
						}
					}
					search_list_phone.head = nullptr;
					res = 0;
					// Формируем ответ в байтовой записи
					for(i = 0; i < INT_LEN; i++)
					{
						num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
					}
					// Пересылаем ответ
					len_num = INT_LEN;
					for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
					{
						nbytes = write(fd, num + i, len_num);
						if(nbytes < 0)
						{
							perror("write length");
							exit(EXIT_FAILURE);
						}
						else 
						{
							if(nbytes == 0)
							{
								perror ("write truncated");
								exit (EXIT_FAILURE);
							}
						}
					}
					return res;
				}
			}
			res = n_p_search.search_name(c_name, x, search_list_name, tail_name);
			if(res >= 0)
			{
				if(tail_name)
				{
					curr = search_list_name.head;
					while(curr != nullptr)
					{
						curr_n = curr->next_search_name;
						er_code = n_p_search.remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list_name.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list_name.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list_name.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list_name.head = nullptr;
							return -5;
						}
						er_code = g_search[curr->get_group()].remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list_name.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list_name.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list_name.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list_name.head = nullptr;
							return -5;
						}
						if(curr->prev)
						{
							curr->prev->next = curr->next;
						}
						else
						{
							data_list.head = curr->next; 
						}
						if(curr->next)
						{
							curr->next->prev = curr->prev;
						}
						delete curr;
						curr = curr_n;
					}
				}
				search_list_name.head = nullptr;
				res = 0;
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				return res;
			}
			res = -1;
			switch(c_group)
			{
				case condition::eq:
				res = g_search[x.get_group()].search_group(c_name, c_phone, x, search_list_group, tail_group);
				break;
				case condition::ne:
				break;
				case condition::lt:
				res = 0;
				for(i = 0; i < x.get_group(); i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::gt:
				res = 0;
				for(i = x.get_group() + 1; i < MAX_GROUP; i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::le:
				res = 0;
				for(i = 0; i <= x.get_group(); i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::ge:
				res = 0;
				for(i = x.get_group(); i < MAX_GROUP; i++)
				{
					res += g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::like:
				break;
				case condition::nlike:
				break;
				case condition::none:
				break;
			}
			if(res >= 0)
			{
				if(tail_group)
				{
					curr = search_list_group.head;
					while(curr != nullptr)
					{
						curr_n = curr->next_search_group;
						er_code = n_p_search.remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list_group.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list_group.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list_group.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list_group.head = nullptr;
							return -5;
						}
						er_code = g_search[curr->get_group()].remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list_group.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list_group.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list_group.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list_group.head = nullptr;
							return -5;
						}
						if(curr->prev)
						{
							curr->prev->next = curr->next;
						}
						else
						{
							data_list.head = curr->next; 
						}
						if(curr->next)
						{
							curr->next->prev = curr->prev;
						}
						delete curr;
						curr = curr_n;
					}
				}
				search_list_group.head = nullptr;
				res = 0;
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				return res;
			}
			res = n_p_search.search_phone(c_phone, x, search_list_phone, tail_phone);
			if(res >= 0)
			{
				if(tail_phone)
				{
					curr = search_list_phone.head;
					while(curr != nullptr)
					{
						curr_n = curr->next_search_phone;
						er_code = n_p_search.remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list_phone.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list_phone.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list_phone.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list_phone.head = nullptr;
							return -5;
						}
						er_code = g_search[curr->get_group()].remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list_phone.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list_phone.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list_phone.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list_phone.head = nullptr;
							return -5;
						}
						if(curr->prev)
						{
							curr->prev->next = curr->next;
						}
						else
						{
							data_list.head = curr->next; 
						}
						if(curr->next)
						{
							curr->next->prev = curr->prev;
						}
						delete curr;
						curr = curr_n;
					}
				}
				search_list_phone.head = nullptr;
				res = 0;
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				return res;
			}
			for(curr = data_list.head; curr != nullptr; curr = curr->next)
			{
				if(x.apply(*curr))
				{
					if(!search_list.head)
					{
						curr_search = search_list.head = curr;
					}
					else
					{
						curr_search->next_search = curr;
						curr_search = curr;
					}
				}
			}
			if(curr_search)
			{
				curr_search->next_search = nullptr;
				curr = search_list.head;
				while(curr != nullptr)
				{
					curr_n = curr->next_search;
					er_code = n_p_search.remove_record(*curr);
					switch(er_code)
					{
						case search_returns::success:
						break;
						case search_returns::memory:
						search_list.head = nullptr;
						return -3;
						case search_returns::balance:
						search_list.head = nullptr;
						return -4;
						case search_returns::unknown:
						search_list.head = nullptr;
						return -5;
						case search_returns::repeated_addition:
						search_list.head = nullptr;
						return -5;
					}
					er_code = g_search[curr->get_group()].remove_record(*curr);
					switch(er_code)
					{
						case search_returns::success:
						break;
						case search_returns::memory:
						search_list.head = nullptr;
						return -3;
						case search_returns::balance:
						search_list.head = nullptr;
						return -4;
						case search_returns::unknown:
						search_list.head = nullptr;
						return -5;
						case search_returns::repeated_addition:
						search_list.head = nullptr;
						return -5;
					}
					if(curr->prev)
					{
						curr->prev->next = curr->next;
					}
					else
					{
						data_list.head = curr->next; 
					}
					if(curr->next)
					{
						curr->next->prev = curr->prev;
					}
					delete curr;
					curr = curr_n;
				}
			}
			search_list.head = nullptr;
			res = 0;
			// Формируем ответ в байтовой записи
			for(i = 0; i < INT_LEN; i++)
			{
				num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
			}
			// Пересылаем ответ
			len_num = INT_LEN;
			for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
			{
				nbytes = write(fd, num + i, len_num);
				if(nbytes < 0)
				{
					perror("write length");
					exit(EXIT_FAILURE);
				}
				else 
				{
					if(nbytes == 0)
					{
						perror ("write truncated");
						exit (EXIT_FAILURE);
					}
				}
			}
			return res;
		}
		else
		{
			if(c_name == condition::like || c_name == condition::nlike || c_name == condition::ne || c_phone == condition::ne || c_group == condition::ne)
			{
				for(curr = data_list.head; curr != nullptr; curr = curr->next)
				{
					if(x.apply(*curr))
					{
						if(!search_list.head)
						{
							curr_search = search_list.head = curr;
						}
						else
						{
							curr_search->next_search = curr;
							curr_search = curr;
						}
					}
				}
				if(curr_search)
				{
					curr_search->next_search = nullptr;
					curr = search_list.head;
					while(curr != nullptr)
					{
						curr_n = curr->next_search;
						er_code = n_p_search.remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list.head = nullptr;
							return -5;
						}
						er_code = g_search[curr->get_group()].remove_record(*curr);
						switch(er_code)
						{
							case search_returns::success:
							break;
							case search_returns::memory:
							search_list.head = nullptr;
							return -3;
							case search_returns::balance:
							search_list.head = nullptr;
							return -4;
							case search_returns::unknown:
							search_list.head = nullptr;
							return -5;
							case search_returns::repeated_addition:
							search_list.head = nullptr;
							return -5;
						}
						if(curr->prev)
						{
							curr->prev->next = curr->next;
						}
						else
						{
							data_list.head = curr->next; 
						}
						if(curr->next)
						{
							curr->next->prev = curr->prev;
						}
						delete curr;
						curr = curr_n;
					}
				}
				search_list.head = nullptr;
				res = 0;
				// Формируем ответ в байтовой записи
				for(i = 0; i < INT_LEN; i++)
				{
					num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
				}
				// Пересылаем ответ
				len_num = INT_LEN;
				for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
				{
					nbytes = write(fd, num + i, len_num);
					if(nbytes < 0)
					{
						perror("write length");
						exit(EXIT_FAILURE);
					}
					else 
					{
						if(nbytes == 0)
						{
							perror ("write truncated");
							exit (EXIT_FAILURE);
						}
					}
				}
				return res;
			}
			res = 0;
			if(c_name != condition::none)
			{
				n_p_search.search_name(c_name, x, search_list_name, tail_name);
			}
			if(c_phone != condition::none)
			{
				n_p_search.search_phone(c_phone, x, search_list_phone, tail_phone);
			}
			switch(c_group)
			{
				case condition::eq:
				g_search[x.get_group()].search_group(c_name, c_phone, x, search_list_group, tail_group);
				break;
				case condition::ne:
				break;
				case condition::lt:
				res = 0;
				for(i = 0; i < x.get_group(); i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::gt:
				res = 0;
				for(i = x.get_group() + 1; i < MAX_GROUP; i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::le:
				res = 0;
				for(i = 0; i <= x.get_group(); i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::ge:
				res = 0;
				for(i = x.get_group(); i < MAX_GROUP; i++)
				{
					g_search[i].search_group(c_name, c_phone, x, search_list_group, tail_group);
				}
				break;
				case condition::like:
				break;
				case condition::nlike:
				break;
				case condition::none:
				break;
			}
			com.parse("select * order by phone, group, name", buf);
			search_list_name.Neumann_search_name_sort(com);
			search_list_phone.Neumann_search_phone_sort(com);
			search_list_group.Neumann_search_group_sort(com);
			search_list_name_phone.merge_name_phone(search_list_name, search_list_phone);
			search_list.merge_name_phone_group(search_list_name_phone, search_list_group);
			curr = search_list.head;
			while(curr != nullptr)
			{
				curr_n = curr->next_search;
				er_code = n_p_search.remove_record(*curr);
				switch(er_code)
				{
					case search_returns::success:
					break;
					case search_returns::memory:
					search_list.head = nullptr;
					return -3;
					case search_returns::balance:
					search_list.head = nullptr;
					return -4;
					case search_returns::unknown:
					search_list.head = nullptr;
					return -5;
					case search_returns::repeated_addition:
					search_list.head = nullptr;
					return -5;
				}
				er_code = g_search[curr->get_group()].remove_record(*curr);
				switch(er_code)
				{
					case search_returns::success:
					break;
					case search_returns::memory:
					search_list.head = nullptr;
					return -3;
					case search_returns::balance:
					search_list.head = nullptr;
					return -4;
					case search_returns::unknown:
					search_list.head = nullptr;
					return -5;
					case search_returns::repeated_addition:
					search_list.head = nullptr;
					return -5;
				}
				if(curr->prev)
				{
					curr->prev->next = curr->next;
				}
				else
				{
					data_list.head = curr->next; 
				}
				if(curr->next)
				{
					curr->next->prev = curr->prev;
				}
				delete curr;
				curr = curr_n;
			}
		}
		search_list_name.head = nullptr;
		search_list_phone.head = nullptr;
		search_list_group.head = nullptr;
		search_list_name_phone.head = nullptr;
		search_list.head = nullptr;
		res = 0;
		if(write(fd, &res, sizeof (int)) != (int) sizeof (int))
		{
			perror("write result");
			return -1;
		}
		return res;
		case command_type::none:
		res = 0;
		// Формируем ответ в байтовой записи
		for(i = 0; i < INT_LEN; i++)
		{
			num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
		}
		// Пересылаем ответ
		len_num = INT_LEN;
		for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
		{
			nbytes = write(fd, num + i, len_num);
			if(nbytes < 0)
			{
				perror("write length");
				exit(EXIT_FAILURE);
			}
			else 
			{
				if(nbytes == 0)
				{
					perror ("write truncated");
					exit (EXIT_FAILURE);
				}
			}
		}
		return res;
	}
	// Формируем ответ в байтовой записи
	for(i = 0; i < INT_LEN; i++)
	{
		num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
	}
	// Пересылаем ответ
	len_num = INT_LEN;
	for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
	{
		nbytes = write(fd, num + i, len_num);
		if(nbytes < 0)
		{
			perror("write length");
			exit(EXIT_FAILURE);
		}
		else 
		{
			if(nbytes == 0)
			{
				perror ("write truncated");
				exit (EXIT_FAILURE);
			}
		}
	}
	return res;
}