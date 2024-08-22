#include <string.h>
#include "search_structs.h"
void name_avl_tree::delete_tree(name_avl_tree_node *p)
{
	name_avl_tree_node *curr;
	name_avl_tree_node *curr_n;
	if(p == nullptr)
	{
		p = root;
		root = nullptr;
		if(p == nullptr)
		{
			return;
		}
	}
	for(curr = p->next_name; curr != nullptr; curr = curr_n)
	{
		curr_n = curr->next_name;
		delete curr;
	}
	if(p->left != nullptr)
	{
		this->delete_tree(p->left);
	}
	if(p->right != nullptr)
	{
		this->delete_tree(p->right);
	}
	delete p;
}
int name_avl_tree::insert_record(command &x, list2_node *&new_node)
{
	int res;
	int grow = 0;
	list2_node *new_rec = nullptr;
	name_avl_tree_node *new_root = nullptr;
	if(root == nullptr)
	{
		new_root = new name_avl_tree_node;
		if(!new_root)
		{
			return name_avl_tree::ERROR_MEMORY;
		}
		root = new_root;
		new_rec = new list2_node;
		if(!new_rec)
		{
			return name_avl_tree::ERROR_MEMORY;
		}
		if(new_rec->init(x.get_name(), x.get_phone(), x.get_group()))
		{
			delete new_rec;
			return name_avl_tree::ERROR_MEMORY;
		} 
		new_node = new_root->rec = new_rec;
		return name_avl_tree::SUCCESS;
	}
	res = insert_in_subtree(x, root, new_root, grow, new_node);
	if(res != name_avl_tree::SUCCESS)
	{
		return res;
	}
	root = new_root;
	return name_avl_tree::SUCCESS;
}
int name_avl_tree::insert_in_subtree(command &x, name_avl_tree_node *p, name_avl_tree_node *&new_offspring, int &grow, list2_node *&new_node)
{
	name_avl_tree_node *s = nullptr;
	name_avl_tree_node *curr;
	list2_node *new_rec = nullptr;
	int res;
	int diff = strcmp(x.get_name(), p->rec->get_name());
	if(diff < 0)//*x < *p
	{
		if(p->left == nullptr)
		{
			s = new name_avl_tree_node; //p->left = x;
			if(!s)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			new_rec = new list2_node;
			if(!new_rec)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			if(new_rec->init(x.get_name(), x.get_phone(), x.get_group()))
			{
				delete new_rec;
				return name_avl_tree::ERROR_MEMORY;
			}
			new_node = s->rec = new_rec;
			p->left = s;
			/*if(s->init(x.get_name(), x.get_phone(), x.get_group()))
			{
				delete s;
				return name_avl_tree::ERROR_MEMORY;
			}*/
			grow = 1 - p->balance;
			p->balance--;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		res = insert_in_subtree(x, p->left, s, grow, new_node);
		if(res != SUCCESS)
		{
			return res;
		}
		if(grow == 0)
		{
			p->left = s;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		switch(p->balance)
		{
			case 1:
			p->left = s;
			p->balance = 0;
			grow = 0;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case 0:
			p->left = s;
			p->balance = -1;
			grow = 1;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case -1:
			switch(s->balance)
			{
				case -1:
				p->left = s->right;
				s->right = p;
				p->balance = s->balance = 0;
				grow = 0;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case 0:
				p->left = s->right;
				s->right = p;
				p->balance = -1;
				s->balance = 1;
				grow = 1;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case 1:
				new_offspring = s->right;
				s->right = new_offspring->left;
				p->left = new_offspring->right;
				new_offspring->left = s;
				new_offspring->right = p;
				switch(new_offspring->balance)
				{
					case 0:
					p->balance = s->balance = new_offspring->balance = 0;
					break;
					case 1:
					p->balance = new_offspring->balance = 0;
					s->balance = -1;
					break;
					case -1:
					p->balance = 1;
					s->balance = new_offspring->balance = 0;
					break;
					default:
					return name_avl_tree::WRONG_BALANCE;
				} 
				grow = 0;
				return name_avl_tree::SUCCESS;
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
			default:
			return name_avl_tree::WRONG_BALANCE;
		}
	}
	if(diff > 0)
	{
		if(p->right == nullptr)
		{
			s = new name_avl_tree_node; //p->right = x;
			if(!s)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			new_rec = new list2_node;
			if(!new_rec)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			if(new_rec->init(x.get_name(), x.get_phone(), x.get_group()))
			{
				delete new_rec;
				return name_avl_tree::ERROR_MEMORY;
			}
			new_node = s->rec = new_rec;
			p->right = s;
			/*if(s->init(x.get_name(), x.get_phone(), x.get_group()))
			{
				delete s;
				return name_avl_tree::ERROR_MEMORY;
			}*/
			grow = 1 + p->balance;
			p->balance++;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		res = insert_in_subtree(x, p->right, s, grow, new_node);
		if(res != SUCCESS)
		{
			return res;
		}
		if(grow == 0)
		{
			p->right = s;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		switch(p->balance)
		{
			case -1:
			p->right = s;
			p->balance = 0;
			grow = 0;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case 0:
			p->right = s;
			p->balance = 1;
			grow = 1;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case 1:
			switch(s->balance)
			{
				case 1:
				p->right = s->left;
				s->left = p;
				p->balance = s->balance = 0;
				grow = 0;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case 0:
				p->right = s->left;
				s->left = p;
				p->balance = 1;
				s->balance = -1;
				grow = 1;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case -1:
				new_offspring = s->left;
				s->left = new_offspring->right;
				p->right = new_offspring->left;
				new_offspring->left = p;
				new_offspring->right = s;
				switch(new_offspring->balance)
				{
					case 0:
					p->balance = s->balance = new_offspring->balance = 0;
					break;
					case 1:
					s->balance = new_offspring->balance = 0;
					p->balance = -1;
					break;
					case -1:
					s->balance = 1;
					p->balance = new_offspring->balance = 0;
					break;
					default:
					return name_avl_tree::WRONG_BALANCE;
				} 
				grow = 0;
				return name_avl_tree::SUCCESS;
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
			default:
			return name_avl_tree::WRONG_BALANCE;
		}
	}
	for(curr = p; curr->next_name != nullptr; curr = curr->next_name)
	{
		if(curr->rec->compare_phone(condition::eq, x) && curr->rec->compare_group(condition::eq, x)) 
		{
			new_node = nullptr;
			break;
		}
	}
	if(curr->next_name == nullptr)
	{
		if(!(curr->rec->compare_phone(condition::eq, x) && curr->rec->compare_group(condition::eq, x)))
		{
			s = new name_avl_tree_node; //p->right = x;
			if(!s)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			new_rec = new list2_node;
			if(!new_rec)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			if(new_rec->init(x.get_name(), x.get_phone(), x.get_group()))
			{
				delete new_rec;
				return name_avl_tree::ERROR_MEMORY;
			}
			new_node = s->rec = new_rec;
			curr->next_name = s;
			/*if(s->init(x.get_name(), x.get_phone(), x.get_group()))
			{
				delete s;
				return name_avl_tree::ERROR_MEMORY;
			}*/
		}
		else
		{
			new_node = nullptr;
		}
	}
	new_offspring = p;
	grow = 0;
	return name_avl_tree::SUCCESS;
}
int name_avl_tree::add_node(list2_node *x)
{
	int res;
	int grow = 0;
	name_avl_tree_node *new_root = nullptr;
	if(root == nullptr)
	{
		root = new name_avl_tree_node;
		if(!root)
		{
			return name_avl_tree::ERROR_MEMORY;
		}
		root->rec = x;
		return name_avl_tree::SUCCESS;
	}
	res = add_in_subtree(x, root, new_root, grow);
	if(res != name_avl_tree::SUCCESS)
	{
		return res;
	}
	root = new_root;
	return name_avl_tree::SUCCESS;
}
int name_avl_tree::add_in_subtree(list2_node *x, name_avl_tree_node *p, name_avl_tree_node *&new_offspring, int &grow)
{
	name_avl_tree_node *s = nullptr;
	name_avl_tree_node *r = nullptr;
	int res;
	int diff = strcmp(x->get_name(), p->rec->get_name());
	if(diff < 0)//*x < *p
	{
		if(p->left == nullptr)
		{
			s = new name_avl_tree_node;
			if(!s)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			s->rec = x;
			p->left = s;
			grow = 1 - p->balance;
			p->balance--;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		res = add_in_subtree(x, p->left, s, grow);
		if(res != SUCCESS)
		{
			return res;
		}
		if(grow == 0)
		{
			p->left = s;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		switch(p->balance)
		{
			case 1:
			p->left = s;
			p->balance = 0;
			grow = 0;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case 0:
			p->left = s;
			p->balance = -1;
			grow = 1;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case -1:
			switch(s->balance)
			{
				case -1:
				p->left = s->right;
				s->right = p;
				p->balance = s->balance = 0;
				grow = 0;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case 0:
				p->left = s->right;
				s->right = p;
				p->balance = -1;
				s->balance = 1;
				grow = 1;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case 1:
				new_offspring = s->right;
				s->right = new_offspring->left;
				p->left = new_offspring->right;
				new_offspring->left = s;
				new_offspring->right = p;
				switch(new_offspring->balance)
				{
					case 0:
					p->balance = s->balance = new_offspring->balance = 0;
					break;
					case 1:
					p->balance = new_offspring->balance = 0;
					s->balance = -1;
					break;
					case -1:
					p->balance = 1;
					s->balance = new_offspring->balance = 0;
					break;
					default:
					return name_avl_tree::WRONG_BALANCE;
				} 
				grow = 0;
				return name_avl_tree::SUCCESS;
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
			default:
			return name_avl_tree::WRONG_BALANCE;
		}
	}
	if(diff > 0)
	{
		if(p->right == nullptr)
		{
			s = new name_avl_tree_node;
			if(!s)
			{
				return name_avl_tree::ERROR_MEMORY;
			}
			s->rec = x;
			p->right = s;
			grow = 1 + p->balance;
			p->balance++;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		res = add_in_subtree(x, p->right, s, grow);
		if(res != SUCCESS)
		{
			return res;
		}
		if(grow == 0)
		{
			p->right = s;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
		}
		switch(p->balance)
		{
			case -1:
			p->right = s;
			p->balance = 0;
			grow = 0;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case 0:
			p->right = s;
			p->balance = 1;
			grow = 1;
			new_offspring = p;
			return name_avl_tree::SUCCESS;
			case 1:
			switch(s->balance)
			{
				case 1:
				p->right = s->left;
				s->left = p;
				p->balance = s->balance = 0;
				grow = 0;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case 0:
				p->right = s->left;
				s->left = p;
				p->balance = 1;
				s->balance = -1;
				grow = 1;
				new_offspring = s;
				return name_avl_tree::SUCCESS;
				case -1:
				new_offspring = s->left;
				s->left = new_offspring->right;
				p->right = new_offspring->left;
				new_offspring->left = p;
				new_offspring->right = s;
				switch(new_offspring->balance)
				{
					case 0:
					p->balance = s->balance = new_offspring->balance = 0;
					break;
					case 1:
					s->balance = new_offspring->balance = 0;
					p->balance = -1;
					break;
					case -1:
					s->balance = 1;
					p->balance = new_offspring->balance = 0;
					break;
					default:
					return name_avl_tree::WRONG_BALANCE;
				} 
				grow = 0;
				return name_avl_tree::SUCCESS;
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
			default:
			return name_avl_tree::WRONG_BALANCE;
		}
	}
	for(s = p; s->next_name != nullptr; s = s->next_name)
	{
		if(s->rec->compare_phone(condition::eq, *x) && s->rec->compare_group(condition::eq, *x))
		{
			//new_offspring = p;
			//grow = 0;
			return name_avl_tree::SECOND_INIT;
		}
	}
	r = new name_avl_tree_node;
	if(!r)
	{
		return name_avl_tree::ERROR_MEMORY;
	}
	r->rec = x;
	s->next_name = r;
	new_offspring = p;
	grow = 0;
	return name_avl_tree::SUCCESS;
}
int name_avl_tree::remove_record(record &x)
{
	int res;
	int grow = 0;
	name_avl_tree_node *new_root = nullptr;
	if(root == nullptr)
	{
		return name_avl_tree::SUCCESS;
	}
	res = remove_from_subtree(x, root, new_root, grow);
	if(res != name_avl_tree::SUCCESS)
	{
		return res;
	}
	root = new_root;
	return name_avl_tree::SUCCESS;
}
int name_avl_tree::remove_from_subtree(record &x, name_avl_tree_node *p, name_avl_tree_node *&new_offspring, int &grow)
{
	if(p == nullptr)
	{
		new_offspring = nullptr;
		grow = 0;
		return name_avl_tree::SUCCESS;
	}
	int res;
	int buf_bal;
	int diff = strcmp(x.get_name(), p->rec->get_name());
	name_avl_tree_node *s = nullptr;
	name_avl_tree_node *r = nullptr;
	name_avl_tree_node *curr;
	name_avl_tree_node *curr_p;
	name_avl_tree_node *r_p;
	if(diff < 0)
	{
		res = remove_from_subtree(x, p->left, s, grow);
		if(res != name_avl_tree::SUCCESS)
		{
			return res;
		}
		if(grow < 0)
		{
			//balance_left
			switch(p->balance)
			{
				case -1:
				p->left = s;
				new_offspring = p;
				p->balance = 0;
				grow = -1;
				return name_avl_tree::SUCCESS;
				case 0:
				p->left = s;
				new_offspring = p;
				p->balance = 1;
				grow = 0;
				return name_avl_tree::SUCCESS;
				case 1:
				switch(p->right->balance)
				{
					case 1:
					new_offspring = p->right;
					p->right = new_offspring->left;
					p->left = s;
					new_offspring->left = p;
					p->balance = new_offspring->balance = 0;
					grow = -1;
					return name_avl_tree::SUCCESS;
					case 0:
					new_offspring = p->right;
					p->right = new_offspring->left;
					p->left = s;
					new_offspring->left = p;
					p->balance = 1;
					new_offspring->balance = -1;
					grow = 0;
					return name_avl_tree::SUCCESS;
					case -1:
					r = p->right;
					new_offspring = r->left;
					r->left = new_offspring->right;
					new_offspring->right = r;
					p->left = s;
					p->right = new_offspring->left;
					new_offspring->left = p;
					switch(new_offspring->balance)
					{
						case 0:
						p->balance = r->balance = new_offspring->balance = 0;
						grow = -1;
						return name_avl_tree::SUCCESS;
						case 1:
						r->balance = new_offspring->balance = 0;
						p->balance = -1;
						grow = -1;
						return name_avl_tree::SUCCESS;
						case -1:
						p->balance = new_offspring->balance = 0;
						r->balance = 1;
						grow = -1;
						return name_avl_tree::SUCCESS;
						default:
						return name_avl_tree::WRONG_BALANCE;
					}
					default:
					return name_avl_tree::WRONG_BALANCE;
				}
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
		}
		p->left = s;
		new_offspring = p;
		return name_avl_tree::SUCCESS;
	}
	if(diff > 0)
	{
		res = remove_from_subtree(x, p->right, s, grow);
		if(res != name_avl_tree::SUCCESS)
		{
			return res;
		}
		if(grow < 0)
		{
			//balance_right
			switch(p->balance)
			{
				case 1:
				p->right = s;
				new_offspring = p;
				p->balance = 0;
				grow = -1;
				return name_avl_tree::SUCCESS;
				case 0:
				p->right = s;
				new_offspring = p;
				p->balance = -1;
				grow = 0;
				return name_avl_tree::SUCCESS;
				case -1:
				switch(p->left->balance)
				{
					case -1:
					new_offspring = p->left;
					p->left = new_offspring->right;
					p->right = s;
					new_offspring->right = p;
					p->balance = new_offspring->balance = 0;
					grow = -1;
					return name_avl_tree::SUCCESS;
					case 0:
					new_offspring = p->left;
					p->left = new_offspring->right;
					p->right = s;
					new_offspring->right = p;
					p->balance = -1;
					new_offspring->balance = 1;
					grow = 0;
					return name_avl_tree::SUCCESS;
					case 1:
					r = p->left;
					new_offspring = r->right;
					r->right = new_offspring->left;
					new_offspring->left = r;
					p->right = s;
					p->left = new_offspring->right;
					new_offspring->right = p;
					switch(new_offspring->balance)
					{
						case 0:
						p->balance = r->balance = new_offspring->balance = 0;
						grow = -1;
						return name_avl_tree::SUCCESS;
						case -1:
						r->balance = new_offspring->balance = 0;
						p->balance = 1;
						grow = -1;
						return name_avl_tree::SUCCESS;
						case 1:
						p->balance = new_offspring->balance = 0;
						r->balance = -1;
						grow = -1;
						return name_avl_tree::SUCCESS;
						default:
						return name_avl_tree::WRONG_BALANCE;
					}
					default:
					return name_avl_tree::WRONG_BALANCE;
				}
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
		}
		p->right = s;
		new_offspring = p;
		return name_avl_tree::SUCCESS;
	}
	if(p->next_name != nullptr)
	{
		if(p->rec->compare_phone(condition::eq, x) && p->rec->compare_group(condition::eq, x))//(p == x)
		{
			curr = p->next_name;
			curr->right = p->right;
			curr->left = p->left;
			/*if(p->rec->prev)
			{
				p->rec->prev->next = p->rec->next;
			}
			else
			{
				data_list.head = p->rec->next;
			}
			if(p->rec->next)
			{
				p->rec->next->prev = p->rec->prev;
			}*/
			curr->balance = p->balance;
			//delete p->rec;
			delete p;
			new_offspring = curr;
			grow = 0;
			return name_avl_tree::SUCCESS;
		}
		for(curr_p = p, curr = p->next_name; curr != nullptr; curr_p = curr, curr = curr->next_name)
		{
			if(curr->rec->compare_phone(condition::eq, x) && curr->rec->compare_group(condition::eq, x))//(curr == x)
			{
				break;
			}
		}
		if(curr)
		{
			curr_p->next_name = curr->next_name;
			/*if(curr->rec->prev)
			{
				curr->rec->prev->next = curr->rec->next;
			}
			else
			{
				data_list.head = curr->rec->next;
			}
			if(curr->rec->next)
			{
				curr->rec->next->prev = curr->rec->prev;
			}
			delete curr->rec;*/
			delete curr;
		}
		new_offspring = p;
		grow = 0;
		return name_avl_tree::SUCCESS;
	}
	if(!(p->rec->compare_phone(condition::eq, x) && p->rec->compare_group(condition::eq, x)))//(p != x)
	{
		new_offspring = p;
		grow = 0;
		return name_avl_tree::SUCCESS;
	}
	if(p->left == nullptr)
	{
		new_offspring = p->right;
		/*if(p->rec->prev)
		{
			p->rec->prev->next = p->rec->next;
		}
		else
		{
			data_list.head = p->rec->next;
		}
		if(p->rec->next)
		{
			p->rec->next->prev = p->rec->prev;
		}
		delete p->rec;*/
		delete p;
		grow = -1;
		return name_avl_tree::SUCCESS;
	}
	if(p->right == nullptr)
	{
		new_offspring = p->left;
		/*if(p->rec->prev)
		{
			p->rec->prev->next = p->rec->next;
		}
		else
		{
			data_list.head = p->rec->next;
		}
		if(p->rec->next)
		{
			p->rec->next->prev = p->rec->prev;
		}
		delete p->rec;*/
		delete p;
		grow = -1;
		return name_avl_tree::SUCCESS;
	}
	for(r_p = p, r = p->left; r->right != nullptr; r_p = r, r = r->right);
	if(r_p != p)
	{
		r_p->right = p;
		s = p->left;
		p->left = r->left;
		r->left = s;
	}
	else
	{
		p->left = r->left;
		r->left = p;
	}
	r->right = p->right;
	p->right = nullptr;
	buf_bal = p->balance;
	p->balance = r->balance;
	r->balance = buf_bal;
	p = r;
	res = remove_from_subtree(x, p->left, s, grow);
	if(res != name_avl_tree::SUCCESS)
	{
		return res;
	}
	if(grow < 0)
	{
		switch(p->balance)
		{
			case -1:
			p->left = s;
			new_offspring = p;
			p->balance = 0;
			grow = -1;
			return name_avl_tree::SUCCESS;
			case 0:
			p->left = s;
			new_offspring = p;
			p->balance = 1;
			grow = 0;
			return name_avl_tree::SUCCESS;
			case 1:
			switch(p->right->balance)
			{
				case 1:
				new_offspring = p->right;
				p->right = new_offspring->left;
				p->left = s;
				new_offspring->left = p;
				p->balance = new_offspring->balance = 0;
				grow = -1;
				return name_avl_tree::SUCCESS;
				case 0:
				new_offspring = p->right;
				p->right = new_offspring->left;
				p->left = s;
				new_offspring->left = p;
				p->balance = 1;
				new_offspring->balance = -1;
				grow = 0;
				return name_avl_tree::SUCCESS;
				case -1:
				r = p->right;
				new_offspring = r->left;
				r->left = new_offspring->right;
				new_offspring->right = r;
				p->left = s;
				p->right = new_offspring->left;
				new_offspring->left = p;
				switch(new_offspring->balance)
				{
					case 0:
					p->balance = r->balance = new_offspring->balance = 0;
					grow = -1;
					return name_avl_tree::SUCCESS;
					case 1:
					r->balance = new_offspring->balance = 0;
					p->balance = -1;
					grow = -1;
					return name_avl_tree::SUCCESS;
					case -1:
					p->balance = new_offspring->balance = 0;
					r->balance = 1;
					grow = -1;
					return name_avl_tree::SUCCESS;
					default:
					return name_avl_tree::WRONG_BALANCE;
				}
				default:
				return name_avl_tree::WRONG_BALANCE;
			}
			default:
			return name_avl_tree::WRONG_BALANCE;
		}
	}
	else
	{
		p->left = s;//if(s) s->parent = p;
		new_offspring = p;
	}
	return name_avl_tree::SUCCESS;
}
int name_avl_tree::search(command &x, list2 &search_list, list2_node *&tail)
{
	int res;
	if(root == nullptr)
	{
		return 0;
	}
	res = search(x, root, search_list, tail);
	if(tail)
	{
		tail->next_search_name = nullptr;
	}
	return res;
}
int name_avl_tree::search(command &x, name_avl_tree_node *p, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	int diff = strcmp(x.get_name(), p->rec->get_name());
	name_avl_tree_node *curr; 
	switch(x.get_c_name())
	{
		case condition::eq:
		if(diff < 0)
		{
			if(p->left != nullptr)
			{
				return search(x, p->left, search_list, tail);
			}
			else
			{
				return 0;
			}
		}	
		if(diff > 0)
		{
			if(p->right != nullptr)
			{
				return search(x, p->right, search_list, tail);
			}
			else
			{
				return 0;
			}
		}
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_name = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		return res;
		case condition::lt:
		if(diff < 0)
		{
			if(p->left != nullptr)
			{
				return search(x, p->left, search_list, tail);
			}
			else
			{
				return 0;
			}
		}	
		if(diff > 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_name = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search(x, p->right, search_list, tail);
			}
			return res;
		}
		if(p->left != nullptr)
		{
			res += search(x, p->left, search_list, tail);
		}
		return res;
		case condition::gt:
		if(diff < 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_name = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search(x, p->right, search_list, tail);
			}
			return res;
		}	
		if(diff > 0)
		{
			if(p->right != nullptr)
			{
				return search(x, p->right, search_list, tail);
			}
			else
			{
				return 0;
			}
		}
		if(p->right != nullptr)
		{
			res += search(x, p->right, search_list, tail);
		}
		return res;
		case condition::le:
		if(diff < 0)
		{
			if(p->left != nullptr)
			{
				return search(x, p->left, search_list, tail);
			}
			else
			{
				return 0;
			}
		}	
		if(diff > 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_name = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search(x, p->right, search_list, tail);
			}
			return res;
		}
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_name = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->left != nullptr)
		{
			res += search(x, p->left, search_list, tail);
		}
		return res;
		case condition::ge:
		if(diff < 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_name = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search(x, p->right, search_list, tail);
			}
			return res;
		}	
		if(diff > 0)
		{
			if(p->right != nullptr)
			{
				return search(x, p->right, search_list, tail);
			}
			else
			{
				return 0;
			}
		}
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_name = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->right != nullptr)
		{
			res += search(x, p->right, search_list, tail);
		}
		return res;
		case condition::ne:
		return 0;
		case condition::like:
		return 0;
		case condition::nlike:
		return 0;
		case condition::none:
		return 0;
	}
	return 0;
}
int name_avl_tree::search_g(command &x, list2 &search_list, list2_node *&tail)
{
	int res;
	if(root == nullptr)
	{
		return 0;
	}
	res = search_g(x, root, search_list, tail);
	if(tail)
	{
		tail->next_search_group = nullptr;
	}
	return res;
}
int name_avl_tree::search_g(command &x, name_avl_tree_node *p, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	int diff = 0;
	condition c_name = x.get_c_name();
	name_avl_tree_node *curr;
	if(c_name != condition::like && c_name != condition::nlike && c_name != condition::none)
	{
		diff = strcmp(x.get_name(), p->rec->get_name());
	}
	switch(c_name)
	{
		case condition::eq:
		if(diff < 0)
		{
			if(p->left != nullptr)
			{
				return search_g(x, p->left, search_list, tail);
			}
			else
			{
				return 0;
			}
		}	
		if(diff > 0)
		{
			if(p->right != nullptr)
			{
				return search_g(x, p->right, search_list, tail);
			}
			else
			{
				return 0;
			}
		}
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		return res;
		case condition::lt:
		if(diff < 0)
		{
			if(p->left != nullptr)
			{
				return search_g(x, p->left, search_list, tail);
			}
			else
			{
				return 0;
			}
		}	
		if(diff > 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search_g(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search_g(x, p->right, search_list, tail);
			}
			return res;
		}
		if(p->left != nullptr)
		{
			res += search_g(x, p->left, search_list, tail);
		}
		return res;
		case condition::gt:
		if(diff < 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search_g(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search_g(x, p->right, search_list, tail);
			}
			return res;
		}	
		if(diff > 0)
		{
			if(p->right != nullptr)
			{
				return search_g(x, p->right, search_list, tail);
			}
			else
			{
				return 0;
			}
		}
		if(p->right != nullptr)
		{
			res += search_g(x, p->right, search_list, tail);
		}
		return res;
		case condition::le:
		if(diff < 0)
		{
			if(p->left != nullptr)
			{
				return search_g(x, p->left, search_list, tail);
			}
			else
			{
				return 0;
			}
		}	
		if(diff > 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search_g(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search_g(x, p->right, search_list, tail);
			}
			return res;
		}
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->left != nullptr)
		{
			res += search_g(x, p->left, search_list, tail);
		}
		return res;
		case condition::ge:
		if(diff < 0)
		{
			for(curr = p; curr != nullptr; curr = curr->next_name)
			{
				if(x.apply(*curr->rec))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->rec;
						tail = curr->rec;
					}
					else
					{
						tail = search_list.head = curr->rec; 
					}
				}
			}
			if(p->left != nullptr)
			{
				res += search_g(x, p->left, search_list, tail);
			}
			if(p->right != nullptr)
			{
				res += search_g(x, p->right, search_list, tail);
			}
			return res;
		}	
		if(diff > 0)
		{
			if(p->right != nullptr)
			{
				return search_g(x, p->right, search_list, tail);
			}
			else
			{
				return 0;
			}
		}
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->right != nullptr)
		{
			res += search_g(x, p->right, search_list, tail);
		}
		return res;
		case condition::ne:
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->left != nullptr)
		{
			res += search_g(x, p->left, search_list, tail);
		}
		if(p->right != nullptr)
		{
			res += search_g(x, p->right, search_list, tail);
		}
		return res;
		case condition::like:
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->left != nullptr)
		{
			res += search_g(x, p->left, search_list, tail);
		}
		if(p->right != nullptr)
		{
			res += search_g(x, p->right, search_list, tail);
		}
		return res;
		case condition::nlike:
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->left != nullptr)
		{
			res += search_g(x, p->left, search_list, tail);
		}
		if(p->right != nullptr)
		{
			res += search_g(x, p->right, search_list, tail);
		}
		return res;
		case condition::none: //without check?
		for(curr = p; curr != nullptr; curr = curr->next_name)
		{
			if(x.apply(*curr->rec))
			{
				res++;
				if(tail)
				{
					tail->next_search_group = curr->rec;
					tail = curr->rec;
				}
				else
				{
					tail = search_list.head = curr->rec; 
				}
			}
		}
		if(p->left != nullptr)
		{
			res += search_g(x, p->left, search_list, tail);
		}
		if(p->right != nullptr)
		{
			res += search_g(x, p->right, search_list, tail);
		}
		return res;
	}
	return 0;
}
int name_avl_tree::full_walk_g(list2 &search_list, list2_node *&tail)
{
	int res;
	if(root == nullptr)
	{
		return 0;
	}
	res = walk_in_subtree_g(root, search_list, tail);
	if(tail)
	{
		tail->next_search_group = nullptr;
	}
	return res;
}
int name_avl_tree::walk_in_subtree_g(name_avl_tree_node *p, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	name_avl_tree_node *curr;
	for(curr = p; curr != nullptr; curr = curr->next_name)
	{
		res++;
		if(tail)
		{
			tail->next_search_group = curr->rec;
			tail = curr->rec;
		}
		else
		{
			tail = search_list.head = curr->rec; 
		}
	}
	if(p->left != nullptr)
	{
		res += walk_in_subtree_g(p->left, search_list, tail);
	}
	if(p->right != nullptr)
	{
		res += walk_in_subtree_g(p->right, search_list, tail);
	}
	return res;
}
/*void name_avl_tree::print_node(int max_level, int level, name_avl_tree_node *p)
{
	int i;
	int spaces = (level<<1);
	//name_avl_tree_node *curr;
	if(max_level <= level)
	{
		return;
	}
	printf("%d ", p->balance);
	for(i = 0; i < (p->balance >= 0 ? spaces: spaces - 1); i++)
	{
		printf(" ");
	}
	p->rec->print();*/
	/*for(curr = p; curr != nullptr; curr = curr->next_name)
	{
		curr->print();
	}*/
	/*if(p->left != nullptr)
	{
		print_node(max_level, level + 1, p->left);
	}
	if(p->right != nullptr)
	{
		print_node(max_level, level + 1, p->right);
	}
}
void name_avl_tree::print(int r)
{
	if(root != nullptr)
	{
		print_node(r, 0, root);
	}
}*/
int vector_rec::bin_search_left(record &x)
{
	int L = 0;
	int R = size;
	while(L < R)
	{
		if(arr[(L+R)>>1]->pgn_compare(x) < 0)//arr[(L+R)>>1] < x
		{
			L = ((L+R)>>1) + 1;
		}
		else
		{
			R = ((L+R)>>1);
		}
	}
	return R;
}
int vector_rec::bin_search_right(record &x)
{
	int L = 0;
	int R = size;
	while(L < R)
	{
		if(arr[(L+R)>>1]->pgn_compare(x) <= 0)//arr[(L+R)>>1] < x
		{
			L = ((L+R)>>1) + 1;
		}
		else
		{
			R = ((L+R)>>1);
		}
	}
	return R;
}
int vector_rec::bin_search_left(command &x)
{
	int L = 0;
	int R = size;
	while(L < R)
	{
		if(arr[(L+R)>>1]->compare_phone(condition::lt, x))//arr[(L+R)>>1] < x
		{
			L = ((L+R)>>1) + 1;
		}
		else
		{
			R = ((L+R)>>1);
		}
	}
	return R;
}
int vector_rec::bin_search_right(command &x)
{
	int L = 0;
	int R = size;
	while(L < R)
	{
		if(arr[(L+R)>>1]->compare_phone(condition::le, x))//arr[(L+R)>>1] <= x
		{
			L = ((L+R)>>1) + 1;
		}
		else
		{
			R = ((L+R)>>1);
		}
	}
	return R;
}
int v_vector_rec::add_record(list2_node &x)
{
	vector_rec *curr;
	int R;
	int i;
	for(curr = head; curr != nullptr; curr = curr->next)
	{
		if(curr->size < max_size)
		{
			R = curr->bin_search_right(x);
			for(i = curr->size; i > R; i--)
			{
				curr->arr[i] = curr->arr[i - 1];
			}
			curr->arr[i] = &x;
			curr->size++;
			return 0;
		}
	}
	if(!curr)
	{
		curr = new vector_rec;
		if(curr->init(max_size))
		{
			return -1;
		}
		curr->arr[0] = &x;
		curr->size = 1;
		curr->next = head;
		head = curr;
	}
	return 0;
}
void v_vector_rec::delete_v_vector_rec()
{
	vector_rec *curr = head;
	vector_rec *curr_n;
	while(curr != nullptr)
	{
		curr_n = curr->next;
		delete curr;
		curr = curr_n;
	}
	head = nullptr;
}
void v_vector_rec::remove_record(record &x)
{
	vector_rec *curr;
	int R;
	int i;
	for(curr = head; curr != nullptr; curr = curr->next)
	{
		R = curr->bin_search_right(x);
		if(R && curr->arr[R - 1] == &x)//(R && !curr->arr[R - 1]->pgn_compare(x)) without pgn_compare here?
		{
			for(i = R - 1; i < curr->size - 1; i++)
			{
				curr->arr[i] = curr->arr[i + 1];
			}
			curr->arr[i] = nullptr;
			curr->size--;
			return;
		}
	}
}
int v_vector_rec::search(command &x, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	int R;
	int L;
	int i;
	vector_rec *curr;
	switch(x.get_c_phone())
	{
		case condition::eq:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			L = curr->bin_search_left(x);
			for(i = R - 1; i >= L; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::lt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::gt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::le:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::ge:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::ne:
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	if(tail)
	{
		tail->next_search_phone = nullptr;
	}
	return 0;
}
int v_vector_rec::search_g(command &x, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	int R;
	int L;
	int i;
	vector_rec *curr;
	switch(x.get_c_phone())
	{
		case condition::eq:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			L = curr->bin_search_left(x);
			for(i = R - 1; i >= L; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::lt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::gt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::le:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::ge:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::ne:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			L = curr->bin_search_left(x);
			for(i = 0; i < L; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
			for(i = R; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	if(tail)
	{
		tail->next_search_group = nullptr;
	}
	return 0;
}
int v_vector_rec_g::add_record(list2_node &x)
{
	vector_rec *curr;
	int R;
	int i;
	for(curr = head; curr != nullptr; curr = curr->next)
	{
		if(curr->size < max_size)
		{
			R = curr->bin_search_right(x);
			for(i = curr->size; i > R; i--)
			{
				curr->arr[i] = curr->arr[i - 1];
			}
			curr->arr[i] = &x;
			curr->size++;
			return 0;
		}
	}
	if(!curr)
	{
		curr = new vector_rec;
		if(curr->init(max_size))
		{
			return -1;
		}
		curr->arr[0] = &x;
		curr->size = 1;
		curr->next = head;
		head = curr;
	}
	return 0;
}
void v_vector_rec_g::delete_v_vector_rec_g()
{
	vector_rec *curr = head;
	vector_rec *curr_n;
	while(curr != nullptr)
	{
		curr_n = curr->next;
		delete curr;
		curr = curr_n;
	}
	head = nullptr;
}
void v_vector_rec_g::remove_record(record &x)
{
	vector_rec *curr;
	int R;
	int i;
	for(curr = head; curr != nullptr; curr = curr->next)
	{
		R = curr->bin_search_right(x);
		if(R && curr->arr[R - 1] == &x)//(R && !curr->arr[R - 1]->pgn_compare(x)) without pgn_compare here?
		{
			for(i = R - 1; i < curr->size - 1; i++)
			{
				curr->arr[i] = curr->arr[i + 1];
			}
			curr->arr[i] = nullptr;
			curr->size--;
			return;
		}
	}
}
int v_vector_rec_g::search(command &x, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	int R;
	int L;
	int i;
	vector_rec *curr;
	switch(x.get_c_phone())
	{
		case condition::eq:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			L = curr->bin_search_left(x);
			for(i = R - 1; i >= L; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::lt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::gt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::le:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::ge:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_phone = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_phone = nullptr;
		}
		return res;
		case condition::ne:
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	if(tail)
	{
		tail->next_search_phone = nullptr;
	}
	return 0;
}
int v_vector_rec_g::search_g(command &x, list2 &search_list, list2_node *&tail)
{
	int res = 0;
	int R;
	int L;
	int i;
	vector_rec *curr;
	switch(x.get_c_phone())
	{
		case condition::eq:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			L = curr->bin_search_left(x);
			for(i = R - 1; i >= L; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::lt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::gt:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::le:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			for(i = R - 1; i >= 0; i--)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::ge:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			L = curr->bin_search_left(x);
			for(i = L; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::ne:
		for(curr = head; curr != nullptr; curr = curr->next)
		{
			R = curr->bin_search_right(x);
			L = curr->bin_search_left(x);
			for(i = 0; i < L; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
			for(i = R; i < curr->size; i++)
			{
				if(x.apply(*curr->arr[i]))
				{
					res++;
					if(tail)
					{
						tail->next_search_group = curr->arr[i];
						tail = curr->arr[i];
					}
					else
					{
						tail = search_list.head = curr->arr[i]; 
					}
				}
			}
		}
		if(tail)
		{
			tail->next_search_group = nullptr;
		}
		return res;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	if(tail)
	{
		tail->next_search_group = nullptr;
	}
	return 0;
}
search_returns search_structs::add_node(list2_node *x)
{
	int er_code;
	er_code = name_tree.add_node(x);
	switch(er_code)
	{
		case name_avl_tree::SUCCESS:
		break;
		case name_avl_tree::ERROR_MEMORY:
		return search_returns::memory;
		case name_avl_tree::WRONG_BALANCE:
		return search_returns::balance;
		case name_avl_tree::SECOND_INIT:
		return search_returns::repeated_addition;
		default:
		return search_returns::unknown;
	}
	if(!phone_hash_arr)
	{
		phone_hash_arr = new v_vector_rec[pow_k];
		if(!phone_hash_arr)
		{
			return search_returns::memory;
		}
	}
	er_code = phone_hash_arr[x->phone_hash(pow_k)].add_record(*x);
	if(er_code)
	{
		return search_returns::memory;
	}
	return search_returns::success;
}
search_returns search_structs::insert_record(command &x, list2_node *&new_node)
{
	search_returns hash_ret;
	int er_code;
	er_code = name_tree.insert_record(x, new_node);
	if(er_code != name_avl_tree::SUCCESS)
	{
		switch(er_code)
		{
			case name_avl_tree::ERROR_MEMORY:
			return search_returns::memory;
			case name_avl_tree::WRONG_BALANCE:
			return search_returns::balance;
			default:
			return search_returns::unknown;
		}
	}
	if(new_node != nullptr)
	{
		if(!phone_hash_arr)
		{
			hash_ret = init();
			if(hash_ret != search_returns::success)
			{
				return hash_ret;
			}
		}
		er_code = phone_hash_arr[new_node->phone_hash(pow_k)].add_record(*new_node);
		if(er_code)
		{
			return search_returns::memory;
		}
	}
	return search_returns::success;
}
int search_structs::search_name(condition c_name, command &x, list2 &search_list_name, list2_node *&tail_name)
{
	int res = -1;
	switch(c_name)
	{
		case condition::eq:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::ne:
		break;
		case condition::lt:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::gt:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::le:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::ge:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	return res;
}
int search_structs::search_phone(condition c_phone, command &x, list2 &search_list_phone, list2_node *&tail_phone)
{
	int res = -1;
	int i;
	switch(c_phone)
	{
		case condition::eq:
		res = phone_hash_arr[x.phone_hash(pow_k)].search(x, search_list_phone, tail_phone);
		break;
		case condition::ne:
		break;
		case condition::lt:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::gt:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::le:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::ge:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	return res;
}
int search_structs::search_phone_g(condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group)
{
	int res = -1;
	int i;
	switch(c_phone)
	{
		case condition::eq:
		res = phone_hash_arr[x.phone_hash(pow_k)].search_g(x, search_list_group, tail_group);
		break;
		case condition::ne:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::lt:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::gt:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::le:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::ge:
		res = 0;
		for(i = 0; i < pow_k; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	return res;
}
search_returns search_structs::remove_record(record &x)
{
	int er_code;
	phone_hash_arr[x.phone_hash(pow_k)].remove_record(x);
	er_code = name_tree.remove_record(x);
	switch(er_code)
	{
		case name_avl_tree::SUCCESS:
		return search_returns::success;
		case name_avl_tree::WRONG_BALANCE:
		return search_returns::balance;
		default:
		return search_returns::unknown;
	}
	return search_returns::unknown;
}
int search_structs::search_group(condition c_name, condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group)
{
	int res = 0;
	if(phone_hash_arr == nullptr)
	{
		return 0;
	}
	if(c_name == condition::eq)
	{
		res = name_tree.search_g(x, search_list_group, tail_group);
	}
	if(c_phone == condition::eq)
	{
		res = this->search_phone_g(c_phone, x, search_list_group, tail_group);
	}
	if(c_name != condition::like && c_name != condition::nlike && c_name != condition::none)
	{
		res = name_tree.search_g(x, search_list_group, tail_group);
	}
	if(c_phone != condition::none)
	{
		res = this->search_phone_g(c_phone, x, search_list_group, tail_group);
	}
	res = name_tree.search_g(x, search_list_group, tail_group);
	return res;
}
search_returns search_structs_g::add_node(list2_node *x)
{
	int er_code;
	er_code = name_tree.add_node(x);
	switch(er_code)
	{
		case name_avl_tree::SUCCESS:
		break;
		case name_avl_tree::ERROR_MEMORY:
		return search_returns::memory;
		case name_avl_tree::WRONG_BALANCE:
		return search_returns::balance;
		case name_avl_tree::SECOND_INIT:
		return search_returns::repeated_addition;
		default:
		return search_returns::unknown;
	}
	if(!phone_hash_arr)
	{
		phone_hash_arr = new v_vector_rec_g[pow_g];
		if(!phone_hash_arr)
		{
			return search_returns::memory;
		}
	}
	er_code = phone_hash_arr[x->phone_hash(pow_g)].add_record(*x);
	if(er_code)
	{
		return search_returns::memory;
	}
	return search_returns::success;
}
search_returns search_structs_g::insert_record(command &x, list2_node *&new_node)
{
	search_returns hash_ret;
	int er_code;
	er_code = name_tree.insert_record(x, new_node);
	if(er_code != name_avl_tree::SUCCESS)
	{
		switch(er_code)
		{
			case name_avl_tree::ERROR_MEMORY:
			return search_returns::memory;
			case name_avl_tree::WRONG_BALANCE:
			return search_returns::balance;
			default:
			return search_returns::unknown;
		}
	}
	if(new_node != nullptr)
	{
		if(!phone_hash_arr)
		{
			hash_ret = init();
			if(hash_ret != search_returns::success)
			{
				return hash_ret;
			}
		}
		er_code = phone_hash_arr[new_node->phone_hash(pow_g)].add_record(*new_node);
		if(er_code)
		{
			return search_returns::memory;
		}
	}
	return search_returns::success;
}
int search_structs_g::search_name(condition c_name, command &x, list2 &search_list_name, list2_node *&tail_name)
{
	int res = -1;
	switch(c_name)
	{
		case condition::eq:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::ne:
		break;
		case condition::lt:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::gt:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::le:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::ge:
		res = name_tree.search(x, search_list_name, tail_name);
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	return res;
}
int search_structs_g::search_phone(condition c_phone, command &x, list2 &search_list_phone, list2_node *&tail_phone)
{
	int res = -1;
	int i;
	switch(c_phone)
	{
		case condition::eq:
		res = phone_hash_arr[x.phone_hash(pow_g)].search(x, search_list_phone, tail_phone);
		break;
		case condition::ne:
		break;
		case condition::lt:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::gt:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::le:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::ge:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search(x, search_list_phone, tail_phone);
		}
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	return res;
}
int search_structs_g::search_phone_g(condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group)
{
	int res = -1;
	int i;
	switch(c_phone)
	{
		case condition::eq:
		res = phone_hash_arr[x.phone_hash(pow_g)].search_g(x, search_list_group, tail_group);
		break;
		case condition::ne:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::lt:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::gt:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::le:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::ge:
		res = 0;
		for(i = 0; i < pow_g; i++)
		{
			res += phone_hash_arr[i].search_g(x, search_list_group, tail_group);
		}
		break;
		case condition::like:
		break;
		case condition::nlike:
		break;
		case condition::none:
		break;
	}
	return res;
}
search_returns search_structs_g::remove_record(record &x)
{
	int er_code;
	phone_hash_arr[x.phone_hash(pow_g)].remove_record(x);
	er_code = name_tree.remove_record(x);
	switch(er_code)
	{
		case name_avl_tree::SUCCESS:
		return search_returns::success;
		case name_avl_tree::WRONG_BALANCE:
		return search_returns::balance;
		default:
		return search_returns::unknown;
	}
	return search_returns::unknown;
}
int search_structs_g::search_group(condition c_name, condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group)
{
	if(phone_hash_arr == nullptr)
	{
		return 0;
	}
	if(x.get_op() == operation::lor)
	{
		return name_tree.full_walk_g(search_list_group, tail_group);
	}
	if(c_name == condition::eq)
	{
		return name_tree.search_g(x, search_list_group, tail_group);
	}
	if(c_phone == condition::eq)
	{
		return this->search_phone_g(c_phone, x, search_list_group, tail_group);
	}
	if(c_name != condition::like && c_name != condition::nlike && c_name != condition::none)
	{
		return name_tree.search_g(x, search_list_group, tail_group);
	}
	if(c_phone != condition::none)
	{
		return this->search_phone_g(c_phone, x, search_list_group, tail_group);
	}
	return name_tree.search_g(x, search_list_group, tail_group);
}