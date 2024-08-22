#include <string.h>
#include "command.h"
#define LEN 1234
bool command::parse(const char *string, char *remainder)
{
	char word[LEN];
	const char *cond = nullptr;
	int i;
	type = command_type::none;
	c_name = condition::none;
	c_phone = condition::none;
	c_group = condition::none;
	op = operation::none;
	for(i = 0; i < max_items; i++)
	{
		order[i] = order_by[i] = ordering::none;
	}
	init(nullptr, 0, 0);
	cond = string;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		cond += i;
		rem_write(cond, remainder);
		return false;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "quit"))
	{
		type = command_type::quit;
	}
	else
	{
		if(!strcmp(word, "insert"))
		{
			type = command_type::insert;
		}
		else
		{
			if(!strcmp(word, "select"))
			{
				type = command_type::select;
			}
			else
			{
				if(!strcmp(word, "delete"))
				{
					type = command_type::del;
				}
				else
				{
					if(!strcmp(word, "stop"))
					{
						type = command_type::stop;
					}
					else
					{
						rem_write(cond, remainder);
						return false;
					}
				}
			}
		}
	}
	cond += i;
	switch(type)
	{
		case command_type::stop:
		for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
		if(*cond != ';')
		{
			rem_write(cond, remainder);
			return false;
		}
		rem_write(cond, remainder);
		return true;
		case command_type::quit:
		for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
		if(*cond != ';')
		{
			rem_write(cond, remainder);
			return false;
		}
		rem_write(cond, remainder);
		return true;
		case command_type::insert:
		return insert_parse(cond, remainder);
		case command_type::select:
		return select_parse(cond, remainder);
		case command_type::del:
		return delete_parse(cond, remainder);
		case command_type::none:
		rem_write(cond, remainder);
		return false;
	}
	rem_write(cond, remainder);
	return false;
}
bool command::insert_parse(const char *string, char *remainder)
{
	int i;
	int ins_phone = 0;
	int ins_group = 0;
	const char *cond = nullptr;
	char word[LEN];
	cond = string;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(*cond != '(')
	{
		rem_write(cond, remainder);
		return false;
	}
	cond++;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	for(i = 0; cond[i] != ',' && cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		rem_write(cond, remainder);
		return false; //let name be empty?
	}
	strncpy(word, cond, i);
	word[i] = 0;
	cond += i;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(*cond != ',')
	{
		rem_write(cond, remainder);
		return false;
	}
	cond++;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(sscanf(cond, "%d", &ins_phone) != 1)
	{
		rem_write(cond, remainder);
		return false;
	}
	for(; *cond >= '0' && *cond <= '9'; cond++);
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(*cond != ',')
	{
		rem_write(cond, remainder);
		return false;
	}
	cond++;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(sscanf(cond, "%d", &ins_group) != 1)
	{
		rem_write(cond, remainder);
		return false;
	}
	for(; *cond >= '0' && *cond <= '9'; cond++);
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(*cond != ')')
	{
		rem_write(cond, remainder);
		return false;
	}
	cond++;
	for(; *cond == ' ' || *cond == '\t'|| *cond == '\n'; cond++);
	if(*cond != ';')
	{
		rem_write(cond, remainder);
		return false;
	}
	if(init(word, ins_phone, ins_group))
	{
		rem_write(cond, remainder);
		return false;
	}
	rem_write(cond, remainder);
	return true;
}
bool command::order_parse(const char* &cond, char *word, char *remainder)
{
	int i;
	for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ','&& cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		rem_write(cond, remainder);
		return false;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "*"))
	{
		this->order[0] = ordering::name;
		this->order[1] = ordering::phone;
		this->order[2] = ordering::group;
		cond++;
	}
	else
	{
		if(!strcmp(word, "name"))
		{
			this->order[0] = ordering::name;
		}
		else
		{
			if(!strcmp(word, "phone"))
			{
				this->order[0] = ordering::phone;
			}
			else
			{
				if(!strcmp(word, "group"))
				{
					this->order[0] = ordering::group;
				}
				else
				{
					rem_write(cond, remainder);
					return false;
				}
			}
		}
		cond += i;
		for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
		if(*cond == ',')
		{
			cond++;
			for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
			for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ','&& cond[i] != ';' && cond[i]; i++);
			if(!i)
			{
				rem_write(cond, remainder);
				return false;
			}
			strncpy(word, cond, i);
			word[i] = 0;
			if(!strcmp(word, "name"))
			{
				if(this->order[0] == ordering::name)
				{
					rem_write(cond, remainder);
					return false;
				}
				this->order[1] = ordering::name;
			}
			else
			{
				if(!strcmp(word, "phone"))
				{
					if(this->order[0] == ordering::phone)
					{
						rem_write(cond, remainder);
						return false;
					}
					this->order[1] = ordering::phone;
				}
				else
				{
					if(!strcmp(word, "group"))
					{
						if(this->order[0] == ordering::group)
						{
							rem_write(cond, remainder);
							return false;
						}
						this->order[1] = ordering::group;
					}
					else
					{
						rem_write(cond, remainder);
						return false;
					}
				}
			}
			cond += i;
			for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
			if(*cond == ',')
			{
				cond++;
				for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
				for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ','&& cond[i] != ';' && cond[i]; i++);
				if(!i)
				{
					rem_write(cond, remainder);
					return false;
				}
				strncpy(word, cond, i);
				word[i] = 0;
				if(!strcmp(word, "name"))
				{
					if(this->order[0] == ordering::name || this->order[1] == ordering::name)
					{
						rem_write(cond, remainder);
						return false;
					}
					this->order[2] = ordering::name;
				}
				else
				{
					if(!strcmp(word, "phone"))
					{
						if(this->order[0] == ordering::phone || this->order[1] == ordering::phone)
						{
							rem_write(cond, remainder);
							return false;
						}
						this->order[2] = ordering::phone;
					}
					else
					{
						if(!strcmp(word, "group"))
						{
							if(this->order[0] == ordering::group || this->order[1] == ordering::group)
							{
								rem_write(cond, remainder);
								return false;
							}
							this->order[2] = ordering::group;
						}
						else
						{
							rem_write(cond, remainder);
							return false;
						}
					}
				}
				cond += i;
			}
		}
	}
	return true;
}
bool command::order_by_parse(const char* &cond, char *word, char *remainder)
{
	int i;
	for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ','&& cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		rem_write(cond, remainder);
		return false;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "name"))
	{
		this->order_by[0] = ordering::name;
	}
	else
	{
		if(!strcmp(word, "phone"))
		{
			this->order_by[0] = ordering::phone;
		}
		else
		{
			if(!strcmp(word, "group"))
			{
				this->order_by[0] = ordering::group;
			}
			else
			{
				rem_write(cond, remainder);
				return false;
			}
		}
	}
	cond += i;
	for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
	if(*cond == ',')
	{
		cond++;
		for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
		for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ','&& cond[i] != ';' && cond[i]; i++);
		if(!i)
		{
			rem_write(cond, remainder);
			return false;
		}
		strncpy(word, cond, i);
		word[i] = 0;
		if(!strcmp(word, "name"))
		{
			if(this->order_by[0] == ordering::name)
			{
				rem_write(cond, remainder);
				return false;
			}
			this->order_by[1] = ordering::name;
		}
		else
		{
			if(!strcmp(word, "phone"))
			{
				if(this->order_by[0] == ordering::phone)
				{
					rem_write(cond, remainder);
					return false;
				}
				this->order_by[1] = ordering::phone;
			}
			else
			{
				if(!strcmp(word, "group"))
				{
					if(this->order_by[0] == ordering::group)
					{
						rem_write(cond, remainder);
						return false;
					}
					this->order_by[1] = ordering::group;
				}
				else
				{
					rem_write(cond, remainder);
					return false;
				}
			}
		}
		cond += i;
		for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
		if(*cond == ',')
		{
			cond++;
			for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
			for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ','&& cond[i] != ';' && cond[i]; i++);
			if(!i)
			{
				rem_write(cond, remainder);
				return false;
			}
			strncpy(word, cond, i);
			word[i] = 0;
			if(!strcmp(word, "name"))
			{
				if(this->order_by[0] == ordering::name || this->order_by[1] == ordering::name)
				{
					rem_write(cond, remainder);
					return false;
				}
				this->order_by[2] = ordering::name;
			}
			else
			{
				if(!strcmp(word, "phone"))
				{
					if(this->order_by[0] == ordering::phone || this->order_by[1] == ordering::phone)
					{
						rem_write(cond, remainder);
						return false;
					}
					this->order_by[2] = ordering::phone;
				}
				else
				{
					if(!strcmp(word, "group"))
					{
						if(this->order_by[0] == ordering::group || this->order_by[1] == ordering::group)
						{
							rem_write(cond, remainder);
							return false;
						}
						this->order_by[2] = ordering::group;
					}
					else
					{
						rem_write(cond, remainder);
						return false;
					}
				}
			}
			cond += i;
			for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
		}
	}
	if(*cond != ';')
	{
		rem_write(cond, remainder);
		return false;
	}
	rem_write(cond, remainder);
	return true;
}
bool command::select_parse(const char *string, char *remainder)
{
	int i;
	int sel_phone = 0;
	int sel_group = 0;
	char sel_name[LEN];
	char word[LEN];
	const char *cond = nullptr;
	field_type first_field_type = field_type::none;
	field_type second_field_type = field_type::none;
	field_type third_field_type = field_type::none;
	cond = string;
	if(!order_parse(cond, word, remainder))
	{
		rem_write(cond, remainder);
		return false;
	}
	for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(strcmp(word, "where"))
	{
		if(!strcmp(word, "order"))
		{
			cond += i;
			for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
			for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
			if(!i)
			{
				rem_write(cond, remainder);
				return false;
			}
			strncpy(word, cond, i);
			word[i] = 0;
			if(!strcmp(word, "by"))
			{
				cond += i;
				return order_by_parse(cond, word, remainder);
			}
			else
			{
				rem_write(cond, remainder);
				return false;
			}
		}
		else
		{
			rem_write(cond, remainder);
			return false;
		}
	}
	cond += i;
	if(!first_condition_parse(cond, word, sel_name, sel_phone, sel_group, first_field_type, i))
	{
		rem_write(cond, remainder);
		return false;
	}
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	if(!strcmp(word, "and"))
	{
		op = operation::land;
	}
	else
	{
		if(!strcmp(word, "or"))
		{
			op = operation::lor;
		}
		else
		{
			if(!strcmp(word, "order"))
			{
				cond += i;
				for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
				for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
				if(!i)
				{
					rem_write(cond, remainder);
					return false;
				}
				strncpy(word, cond, i);
				word[i] = 0;
				if(!strcmp(word, "by"))
				{
					cond += i;
					if(first_field_type == field_type::name || second_field_type == field_type::name || third_field_type == field_type::name)
					{
						if(init(sel_name, sel_phone, sel_group))
						{
							rem_write(cond, remainder);
							return false;
						}
					}
					else
					{
						if(init(nullptr, sel_phone, sel_group))
						{
							rem_write(cond, remainder);
							return false;
						}
					}
					return order_by_parse(cond,  word, remainder);
				}
				else
				{
					rem_write(cond, remainder);
					return false;
				}
			}
			else
			{
				rem_write(cond, remainder);
				return false;
			}
		}
	}
	cond += i;
	if(!second_condition_parse(cond, word, sel_name, sel_phone, sel_group, first_field_type, second_field_type, i))
	{
		rem_write(cond, remainder);
		return false;
	}
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	if(!strcmp(word, "and"))
	{
		if(op != operation::land)
		{
			rem_write(cond, remainder);
			return false;
		}
	}
	else
	{
		if(!strcmp(word, "or"))
		{
			if(op != operation::lor)
			{
				rem_write(cond, remainder);
				return false;
			}
		}
		else
		{
			if(!strcmp(word, "order"))
			{
				cond += i;
				for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
				for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
				if(!i)
				{
					rem_write(cond, remainder);
					return false;
				}
				strncpy(word, cond, i);
				word[i] = 0;
				if(!strcmp(word, "by"))
				{
					cond += i;
					if(first_field_type == field_type::name || second_field_type == field_type::name || third_field_type == field_type::name)
					{
						if(init(sel_name, sel_phone, sel_group))
						{
							rem_write(cond, remainder);
							return false;
						}
					}
					else
					{
						if(init(nullptr, sel_phone, sel_group))
						{
							rem_write(cond, remainder);
							return false;
						}
					}
					return order_by_parse(cond, word, remainder);
				}
				else
				{
					rem_write(cond, remainder);
					return false;
				}
			}
			else
			{
				rem_write(cond, remainder);
				return false;
			}
		}
	}
	cond += i;
	if(!third_condition_parse(cond, word, sel_name, sel_phone, sel_group, first_field_type, second_field_type, third_field_type, i))
	{
		rem_write(cond, remainder);
		return false;
	}
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "order"))
	{
		cond += i;
		for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
		for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
		if(!i)
		{
			rem_write(cond, remainder);
			return false;
		}
		strncpy(word, cond, i);
		word[i] = 0;
		if(!strcmp(word, "by"))
		{
			cond += i;
			return order_by_parse(cond, word, remainder);
		}
		else
		{
			rem_write(cond, remainder);
			return false;
		}
	}
	else
	{
		rem_write(cond, remainder);
		return false;
	}
	if(c_name == condition::none && c_phone == condition::none && c_group == condition::none)
	{
		rem_write(cond, remainder);
		return false;
	}
	rem_write(cond, remainder);
	return true;
}
bool command::delete_parse(const char *string, char *remainder)
{
	int i;
	char del_name[LEN];
	int del_phone = 0;
	int del_group = 0;
	char word[LEN];
	const char *cond = nullptr;
	field_type first_field_type = field_type::none;
	field_type second_field_type = field_type::none;
	field_type third_field_type = field_type::none;
	cond = string;
	for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(strcmp(word, "where"))
	{
		rem_write(cond, remainder);
		return false;
	}
	cond += i;
	if(!first_condition_parse(cond, word, del_name, del_phone, del_group, first_field_type, i))
	{
		rem_write(cond, remainder);
		return false;
	}
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	if(!strcmp(word, "and"))
	{
		op = operation::land;
	}
	else
	{
		if(!strcmp(word, "or"))
		{
			op = operation::lor;
		}
		else
		{
			rem_write(cond, remainder);
			return false;
		}
	}
	cond += i;
	if(!second_condition_parse(cond, word, del_name, del_phone, del_group, first_field_type, second_field_type, i))
	{
		rem_write(cond, remainder);
		return false;
	}
	if(!i)
	{
		rem_write(cond, remainder);
		return true;
	}
	if(!strcmp(word, "and"))
	{
		if(op != operation::land)
		{
			rem_write(cond, remainder);
			return false;
		}
	}
	else
	{
		if(!strcmp(word, "or"))
		{
			if(op != operation::lor)
			{
				rem_write(cond, remainder);
				return false;
			}
		}
		else
		{
			rem_write(cond, remainder);
			return false;
		}
	}
	cond += i;
	if(!third_condition_parse(cond, word, del_name, del_phone, del_group, first_field_type, second_field_type, third_field_type, i))
	{
		rem_write(cond, remainder);
		return false;
	}
	if(*cond == ';')
	{
		rem_write(cond, remainder);
		return true;
	}
	else
	{
		rem_write(cond, remainder);
		return false;
	}
	if(c_name == condition::none && c_phone == condition::none && c_group == condition::none)
	{
		rem_write(cond, remainder);
		return false;
	}
	rem_write(cond, remainder);
	return true;
}
void command::print(FILE *fp) const
{
	if(c_name != condition::none)
	{
		switch(c_name)
		{
			case condition::eq:
				fprintf(fp, "name = %s ", get_name());
				break;
			case condition::ne:
				fprintf(fp, "name != %s ", get_name());
				break;
			case condition::lt:
				fprintf(fp, "name < %s ", get_name());
				break;
			case condition::gt:
				fprintf(fp, "name > %s ", get_name());
				break;
			case condition::le:
				fprintf(fp, "name <= %s ", get_name());
				break;
			case condition::ge:
				fprintf(fp, "name >= %s ", get_name());
				break;
			case condition::like:
				fprintf(fp, "name like %s ", get_name());
				break;
			case condition::nlike:
				fprintf(fp, "name not like %s ", get_name());
				break;
			case condition::none:
				fprintf(fp, "No condition found!\n");
				break;
		}
	}
	if(c_phone != condition::none)
	{
		switch(c_phone)
		{
			case condition::eq:
				fprintf(fp, "phone = %d ", get_phone());
				break;
			case condition::ne:
				fprintf(fp, "phone != %d ", get_phone());
				break;
			case condition::lt:
				fprintf(fp, "phone < %d ", get_phone());
				break;
			case condition::gt:
				fprintf(fp, "phone > %d ", get_phone());
				break;
			case condition::le:
				fprintf(fp, "phone <= %d ", get_phone());
				break;
			case condition::ge:
				fprintf(fp, "phone >= %d ", get_phone());
				break;
			case condition::like:
				fprintf(fp, "operator \"like\" prohibited for phone!\n");
				break;
			case condition::nlike:
				fprintf(fp, "operator \"nlike\" prohibited for phone!\n");
				break;
			case condition::none:
				fprintf(fp, "No condition found!\n");
				break;
		}
	}
	if(c_group != condition::none)
	{
		switch(c_group)
		{
			case condition::eq:
				fprintf(fp, "group = %d", get_group());
				break;
			case condition::ne:
				fprintf(fp, "group != %d", get_group());
				break;
			case condition::lt:
				fprintf(fp, "group < %d", get_group());
				break;
			case condition::gt:
				fprintf(fp, "group > %d", get_group());
				break;
			case condition::le:
				fprintf(fp, "group <= %d", get_group());
				break;
			case condition::ge:
				fprintf(fp, "group >= %d", get_group());
				break;
			case condition::like:
				fprintf(fp, "operator \"like\" prohibited for group!\n");
				break;
			case condition::nlike:
				fprintf(fp, "operator \"nlike\" prohibited for group!\n");
				break;
			case condition::none:
				fprintf(fp, "No condition found!\n");
				break;
		}
	}
	fprintf(fp, "\n");
}
bool command::apply(const record& x) const
{
	bool nm = false;
	bool ph = false;
	bool gr = false;
	bool check_n = false;
	bool check_p = false;
	bool check_g = false;
	if(c_name != condition::none)
	{
		nm = x.compare_name(c_name, *this);
		check_n = true;
	}
	if(c_phone != condition::none)
	{
		ph = x.compare_phone(c_phone, *this);
		check_p = true;
	}
	if(c_group != condition::none)
	{
		gr = x.compare_group(c_group, *this);
		check_g = true;
	}
	switch(this->op)
	{
		case operation::land:
		return (nm || !check_n) && (ph || !check_p) && (gr || !check_g);
		case operation::lor:
		return nm || ph || gr;
		case operation::none:
		return (nm || ph || gr) || (!check_n && !check_p && !check_g);
	}
	return false;
}
int command::record_order_cmp(const record& x, const record& y) const
{
	int i;
	int dif = 0;
	for(i = 0; i < max_items && !dif; i++)
	{
		switch(order_by[i])
		{
			case ordering::name:
			dif = strcmp(x.get_name(), y.get_name());
			break;
			case ordering::phone:
			dif = x.get_phone() - y.get_phone();
			break;
			case ordering::group:
			dif = x.get_group() - y.get_group();
			break;
			case ordering::none:
			return 0;
		}
	}
	return dif;
}
void command::rem_write(const char *string, char *remainder)
{
		for(; *string != ';' && *string; string++);
		if(*string == ';')
		{
			string++;
		}
		for(; *string == ' ' || *string == '\t'|| *string == '\n'; string++);
		strcpy(remainder, string);	
}
bool command::condition_parse(const char* &cond, char *word, field_type fld)
{
	int i;
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] == '=' || cond[i] == '<'|| cond[i] == '>'; i++);
	if(i)
	{
		strncpy(word, cond, i);
		word[i] = 0;
		if(!strcmp(word, "="))
		{
			
			switch(fld)
			{
				case field_type::name:
				c_name = condition::eq;
				break;
				case field_type::phone:
				c_phone = condition::eq;
				break;
				case field_type::group:
				c_group = condition::eq;
				break;
				case field_type::none:
				return false;
			}
		}
		else
		{
			if(!strcmp(word, "<>"))
			{
				switch(fld)
				{
					case field_type::name:
					c_name = condition::ne;
					break;
					case field_type::phone:
					c_phone = condition::ne;
					break;
					case field_type::group:
					c_group = condition::ne;
					break;
					case field_type::none:
					return false;
				}
			}
			else
			{
				if(!strcmp(word, ">"))
				{
					switch(fld)
					{
						case field_type::name:
						c_name = condition::gt;
						break;
						case field_type::phone:
						c_phone = condition::gt;
						break;
						case field_type::group:
						c_group = condition::gt;
						break;
						case field_type::none:
						return false;
					}
				}
				else
				{
					if(!strcmp(word, "<"))
					{
						switch(fld)
						{
							case field_type::name:
							c_name = condition::lt;
							break;
							case field_type::phone:
							c_phone = condition::lt;
							break;
							case field_type::group:
							c_group = condition::lt;
							break;
							case field_type::none:
							return false;
						}
					}
					else
					{
						if(!strcmp(word, ">="))
						{
							switch(fld)
							{
								case field_type::name:
								c_name = condition::ge;
								break;
								case field_type::phone:
								c_phone = condition::ge;
								break;
								case field_type::group:
								c_group = condition::ge;
								break;
								case field_type::none:
								return false;
							}
						}
						else
						{
							if(!strcmp(word, "<="))
							{
								switch(fld)
								{
									case field_type::name:
									c_name = condition::le;
									break;
									case field_type::phone:
									c_phone = condition::le;
									break;
									case field_type::group:
									c_group = condition::le;
									break;
									case field_type::none:
									return false;
								}
							}
							else
							{
								return false;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
		if(!i)
		{
			return false;
		}
		strncpy(word, cond, i);
		word[i] = 0;
		if(!strcmp(word, "like"))
		{
			switch(fld)
			{
				case field_type::name:
				c_name = condition::like;
				break;
				case field_type::phone:
				return false;
				case field_type::group:
				return false;
				case field_type::none:
				return false;
			}
		}
		else
		{
			if(!strcmp(word, "not"))
			{
				cond += i;
				for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
				for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
				if(!i)
				{
					return false;
				}
				strncpy(word, cond, i);
				word[i] = 0;
				if(!strcmp(word, "like"))
				{
					switch(fld)
					{
						case field_type::name:
						c_name = condition::nlike;
						break;
						case field_type::phone:
						return false;
						case field_type::group:
						return false;
						case field_type::none:
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	cond += i;
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	return true;
}
bool command::field_read(const char* &cond, char *n, int &p, int &g, field_type fld)
{
	int i;
	switch(fld)
	{
		case field_type::name:
		for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n'&& cond[i] != ';' && cond[i]; i++);
		strncpy(n, cond, i);
		n[i] = 0;
		cond += i;
		return true;
		case field_type::phone:
		if(sscanf(cond, "%d", &p) != 1)
		{
			return false;
		}
		for(; *cond >= '0' && *cond <= '9'; cond++);
		return true;
		case field_type::group:
		if(sscanf(cond, "%d", &g) != 1)
		{
			return false;
		}
		for(; *cond >= '0' && *cond <= '9'; cond++);
		return true;
		case field_type::none:
		return false;
	}
	return false;
}
bool command::first_condition_parse(const char* &cond, char *word, char *n, int &p, int &g, field_type &first_field_type, int &i)
{
	for(; *cond == ' '|| *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != '=' && cond[i] != '<'&& cond[i] != '>' && cond[i] && cond[i] != ';'; i++);	
	if(!i)
	{
		return false;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "name"))
	{
		first_field_type = field_type::name;
	}
	else
	{
		if(!strcmp(word, "phone"))
		{
			first_field_type = field_type::phone;
		}
		else
		{
			if(!strcmp(word, "group"))
			{
				first_field_type = field_type::group;
			}
			else
			{
				return false;
			}
		}
	}
	cond += i;
	if(!condition_parse(cond, word, first_field_type))
	{
		return false;
	}
	if(!field_read(cond, n, p, g, first_field_type))
	{
		return false;
	}
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		if(first_field_type == field_type::name)
		{
			if(init(n, p, g))
			{
				return false;
			}
		}
		else
		{
			if(init(nullptr, p, g))
			{
				return false;
			}
		}
		return true;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	return true;
}
bool command::second_condition_parse(const char* &cond, char *word, char *n, int &p, int &g, field_type &first_field_type, field_type &second_field_type, int &i)
{
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != '=' && cond[i] != '<'&& cond[i] != '>' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		return false;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "name"))
	{
		if(first_field_type == field_type::name)
		{
			return false;
		}
		else
		{
			second_field_type = field_type::name;
		}
	}
	else
	{
		if(!strcmp(word, "phone"))
		{
			if(first_field_type == field_type::phone)
			{
				return false;
			}
			else
			{
				second_field_type = field_type::phone;
			}
		}
		else
		{
			if(!strcmp(word, "group"))
			{
				if(first_field_type == field_type::group)
				{
					return false;
				}
				else
				{
					second_field_type = field_type::group;
				}
			}
			else
			{
				return false;
			}
		}
	}
	cond += i;
	if(!condition_parse(cond, word, second_field_type))
	{
		return false;
	}
	if(!field_read(cond, n, p, g, second_field_type))
	{
		return false;
	}
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		if(first_field_type == field_type::name || second_field_type == field_type::name)
		{
			if(init(n, p, g))
			{
				return false;
			}
		}
		else
		{
			if(init(nullptr, p, g))
			{
				return false;
			}
		}
		return true;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	return true;
}
bool command::third_condition_parse(const char* &cond, char *word, char *n, int &p, int &g, field_type &first_field_type, field_type &second_field_type, field_type &third_field_type,int &i)
{
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	for(i = 0; cond[i] != ' ' && cond[i] != '\t' && cond[i] != '\n' && cond[i] != '=' && cond[i] != '<'&& cond[i] != '>' && cond[i] != ';' && cond[i]; i++);
	if(!i)
	{
		return false;
	}
	strncpy(word, cond, i);
	word[i] = 0;
	if(!strcmp(word, "name"))
	{
		if(first_field_type == field_type::name || second_field_type == field_type::name)
		{
			return false;
		}
		else
		{
			third_field_type = field_type::name;
		}
	}
	else
	{
		if(!strcmp(word, "phone"))
		{
			if(first_field_type == field_type::phone || second_field_type == field_type::phone)
			{
				return false;
			}
			else
			{
				third_field_type = field_type::phone;
			}
		}
		else
		{
			if(!strcmp(word, "group"))
			{
				if(first_field_type == field_type::group || second_field_type == field_type::group)
				{
					return false;
				}
				else
				{
					third_field_type = field_type::group;
				}
			}
			else
			{
				return false;
			}
		}
	}
	cond += i;
	if(!condition_parse(cond, word, third_field_type))
	{
		return false;
	}
	if(!field_read(cond, n, p, g, third_field_type))
	{
		return false;
	}
	for(; *cond == ' ' || *cond == '\t' || *cond == '\n'; cond++);
	if(first_field_type == field_type::name || second_field_type == field_type::name || third_field_type == field_type::name)
	{
		if(init(n, p, g))
		{
			return false;
		}
	}
	else
	{
		if(init(nullptr, p, g))
		{
			return false;
		}
	}
	return true;
}