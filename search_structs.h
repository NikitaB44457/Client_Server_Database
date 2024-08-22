#ifndef SEARCH_STRUCTS_H
#define SEARCH_STRUCTS_H
#include <memory>
#include "record_Dlist.h"
enum class search_returns
{
	success = 0,
	memory = -1,
	balance = -2,
	repeated_addition = -3,
	unknown = -4
};
class name_avl_tree;
class name_avl_tree_node
{
private:
	list2_node *rec;
	name_avl_tree_node *left = nullptr;
	name_avl_tree_node *right = nullptr;
	name_avl_tree_node *next_name = nullptr;
	int balance = 0;
public:
	name_avl_tree_node() = default;
	~name_avl_tree_node()
	{
		left = nullptr;
		right = nullptr;
		next_name = nullptr;
		balance = 0;
	}
	name_avl_tree_node(const name_avl_tree_node &x) = delete;
	name_avl_tree_node& operator=(const name_avl_tree_node&) = delete;
	friend class name_avl_tree;
};
class v_vector_rec;
class v_vector_rec_g;
class name_avl_tree
{
private:
	name_avl_tree_node *root = nullptr;
	//static int hash_pow;
public:
	name_avl_tree() = default;
	~name_avl_tree()
	{
		this->delete_tree();
	}
	/*static void set_hash_pow(int pow_k)
	{
		name_avl_tree::hash_pow = pow_k;
	}*/
	void delete_tree(name_avl_tree_node *p = nullptr);
	int add_node(list2_node *x);
	int add_in_subtree(list2_node *x, name_avl_tree_node *p, name_avl_tree_node *&new_offspring, int &grow);
	int insert_record(command &x, list2_node *&new_node);
	int insert_in_subtree(command &x, name_avl_tree_node *p, name_avl_tree_node *&new_offspring, int &grow, list2_node *&new_node);
	int remove_record(record &x);
	int remove_from_subtree(record &x, name_avl_tree_node *p, name_avl_tree_node *&new_offspring, int &grow);
	int search(command &x, list2 &search_list, list2_node *&tail);
	int search(command &x, name_avl_tree_node *p, list2 &search_list, list2_node *&tail);
	int search_g(command &x, list2 &search_list, list2_node *&tail);
	int search_g(command &x, name_avl_tree_node *p, list2 &search_list, list2_node *&tail);
	int full_walk_g(list2 &search_list, list2_node *&tail);
	int walk_in_subtree_g(name_avl_tree_node *p, list2 &search_list, list2_node *&tail);
	int read(const char *filename);
	//void print_node(int max_level, int level, name_avl_tree_node *p);
	//void print(int r);
	enum ERRORS{
	SUCCESS = 0,
	ERROR_READ,
	ERROR_OPEN,
	ERROR_FILE,
	ERROR_MEMORY,
	WRONG_BALANCE,
	SECOND_INIT,
	UNKNOWN
	};
};
class vector_rec
{
private:
	int size = 0;
	std::unique_ptr<list2_node*[]> arr = nullptr;
	vector_rec *next = nullptr;
	int init(int m)
	{
		int i;
		size = 0;
		next = nullptr;
		arr = std::make_unique<list2_node*[]>(m);
		if(!arr)
		{
			return -1;
		}
		for(i = 0; i < m; i++)
		{
			arr[i] = nullptr;
		}
		return 0;
	}
public:
	vector_rec() = default;
	~vector_rec() = default;
	vector_rec(const vector_rec &x) = delete;
	vector_rec& operator=(const vector_rec&) = delete;
	int bin_search_left(record &x);
	int bin_search_right(record &x);
	int bin_search_left(command &x);
	int bin_search_right(command &x);
	friend class v_vector_rec;
	friend class v_vector_rec_g;
};
class v_vector_rec
{
private:
	vector_rec *head = nullptr;
	static int max_size;
public:
	v_vector_rec() = default;
	~v_vector_rec()
	{
		this->delete_v_vector_rec();
	}
	static void set_max_size(int m)
	{
		v_vector_rec::max_size = m;
	}
	int add_record(list2_node &x);
	void remove_record(record &x);
	void delete_v_vector_rec();
	int search(command &x, list2 &search_list, list2_node *&tail);
	int search_g(command &x, list2 &search_list, list2_node *&tail);
	//int insert_record(command &x);
};
class v_vector_rec_g
{
private:
	vector_rec *head = nullptr;
	static int max_size;
public:
	v_vector_rec_g() = default;
	~v_vector_rec_g()
	{
		this->delete_v_vector_rec_g();
	}
	static void set_max_size(int m)
	{
		v_vector_rec_g::max_size = m;
	}
	int add_record(list2_node &x);
	void remove_record(record &x);
	void delete_v_vector_rec_g();
	int search(command &x, list2 &search_list, list2_node *&tail);
	int search_g(command &x, list2 &search_list, list2_node *&tail);
	//int insert_record(command &x);
};
class search_structs
{
private:
	name_avl_tree name_tree;
	v_vector_rec *phone_hash_arr = nullptr;
	static int pow_k;
	static int m;
public:
	search_structs() = default;
	~search_structs()
	{
		this->delete_search_structs();
	}
	void delete_search_structs()
	{
		name_tree.delete_tree();
		if(phone_hash_arr)
		{
			delete[] phone_hash_arr;
		}
		phone_hash_arr = nullptr;
	}
	static void set_pow_k(int ten_in_k)
	{
		pow_k = ten_in_k;
	}
	static void set_m(int max_size)
	{
		m = max_size;
	}
	search_returns init()
	{
		if(!phone_hash_arr)
		{
			phone_hash_arr = new v_vector_rec[pow_k];
		}
		else
		{
			return search_returns::success;
		}
		if(!phone_hash_arr)
		{
			return search_returns::memory;
		}
		return search_returns::success;
	}
	search_returns add_node(list2_node *x);
	search_returns insert_record(command &x, list2_node *&new_node);
	int search_group(condition c_name, condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group);
	int search_name(condition c_name, command &x, list2 &search_list_name, list2_node *&tail_name);
	int search_phone(condition c_phone, command &x, list2 &search_list_phone, list2_node *&tail_phone);
	int search_phone_g(condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group);
	//search_returns remove_record(condition c_name, condition c_phone, command &x, list2 &search_list_phone, list2_node *&tail_phone, list2 &data_list);
	search_returns remove_record(record &x);
};
class search_structs_g
{
private:
	name_avl_tree name_tree;
	v_vector_rec_g *phone_hash_arr = nullptr;
	static int pow_g;
	static int m_g;
public:
	search_structs_g() = default;
	~search_structs_g()
	{
		this->delete_search_structs_g();
	}
	void delete_search_structs_g()
	{
		name_tree.delete_tree();
		if(phone_hash_arr)
		{
			delete[] phone_hash_arr;
		}
		phone_hash_arr = nullptr;
	}
	static void set_pow_g(int ten_in_g)
	{
		pow_g = ten_in_g;
	}
	static void set_m_g(int max_size)
	{
		m_g = max_size;
	}
	search_returns init()
	{
		if(!phone_hash_arr)
		{
			phone_hash_arr = new v_vector_rec_g[pow_g];
		}
		else
		{
			return search_returns::success;
		}
		if(!phone_hash_arr)
		{
			return search_returns::memory;
		}
		return search_returns::success;
	}
	search_returns add_node(list2_node *x);
	search_returns insert_record(command &x, list2_node *&new_node);
	int search_group(condition c_name, condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group);
	int search_name(condition c_name, command &x, list2 &search_list_name, list2_node *&tail_name);
	int search_phone(condition c_phone, command &x, list2 &search_list_phone, list2_node *&tail_phone);
	int search_phone_g(condition c_phone, command &x, list2 &search_list_group, list2_node *&tail_group);
	//search_returns remove_record(condition c_name, condition c_phone, command &x, list2 &search_list_phone, list2_node *&tail_phone, list2 &data_list);
	search_returns remove_record(record &x);
};
#endif //SEARCH_STRUCTS_H