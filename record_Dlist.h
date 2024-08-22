#ifndef record_Dlist_H
#define record_Dlist_H
#define MAX_PRINT 30
#include <iostream>
#include "command.h"
class list2;
class name_avl_tree;
class v_vector_rec;
class search_structs;
class v_vector_rec_g;
class search_structs_g;
class data_base;
class list2_node : public record
{
private:
	list2_node *next = nullptr;
	list2_node *prev = nullptr;
	list2_node *next_search = nullptr;
	list2_node *next_search_name = nullptr;
	list2_node *next_search_phone = nullptr;
	list2_node *next_search_group = nullptr;
public:
    list2_node *get_next() const
	{
		return this->next;
	}
	list2_node *get_prev() const
	{
		return this->prev;
	}
	int set_next(list2_node *lst_nd)
	{
		if(this->next == nullptr)
		{
			this->next = lst_nd;
			return SUCCESS;
		}
		else
		{
			return NODE_LOST;
		}
	}
	int set_prev(list2_node *lst_nd)
	{
		if(this->prev == nullptr)
		{
			this->prev = lst_nd;
			return SUCCESS;
		}
		else
		{
			return NODE_LOST;
		}
	}
	list2_node() = default;
	list2_node(const list2_node &x) = delete;
	~list2_node()
	{
		next = nullptr;
		prev = nullptr;
	}
	list2_node &operator=(const list2_node &x) = delete;
	friend class list2;
	friend class name_avl_tree;
	friend class v_vector_rec;
	friend class search_structs;
	friend class v_vector_rec_g;
	friend class search_structs_g;
	friend class data_base;
	enum ERRORS
	{
		SUCCESS,
		NODE_LOST
	};
};
class list2
{
private:
	list2_node *head = nullptr;
public:
	list2() = default;
	~list2()
	{
		this->delete_list();
	}
	read_status read_list(const char *filename);
	void print(const ordering order[] = nullptr, FILE *fp = stdout);
	void delete_list();
	void merge_search_name(list2 &x, command &com);
	void Neumann_search_name_sort(command &com);
	void merge_search_phone(list2 &x, command &com);
	void Neumann_search_phone_sort(command &com);
	void merge_search_group(list2 &x, command &com);
	void Neumann_search_group_sort(command &com);
	void merge_search(list2 &x, command &com);
	void Neumann_search_sort(command &com);
	int merge_name_phone(list2 &name_list, list2 &phone_list);
	int merge_name_phone_group(list2 &name_phone_list, list2 &group_list);
	int command_apply(command &x, int fd);
	//int command_apply(command &x, FILE *fp = stdout);
	int print_search(int fd, const ordering order[] = nullptr);
	//void print_search_phone(const ordering order[] = nullptr, FILE *fp = stdout);
	//void print_search_name(const ordering order[] = nullptr, FILE *fp = stdout);
	friend class name_avl_tree;
	friend class v_vector_rec;
	friend class search_structs;
	friend class v_vector_rec_g;
	friend class search_structs_g;
	friend class data_base;
};
#endif //record_Dlist_H