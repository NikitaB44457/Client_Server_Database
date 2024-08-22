#ifndef two_int_stack_H
#define two_int_stack_H
class two_int_stack;
class list_node
{
private:
	int ind1 = 0;
	int ind2 = 0;
	list_node *next = nullptr;
	void init(int i1, int i2)
	{
		ind1 = i1;
		ind2 = i2;
	}
public:
    list_node *get_next() const
	{
		return this->next;
	}
	int set_next(list_node *lst_nd)
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
	list_node() = default;
	list_node(const list_node &x) = delete;
	list_node(int i1, int i2)
	{
		init(i1, i2);
	}
	/*list2_node(list2_node &&x):student(x)
	{
		next = x.next;
		x.next = nullptr;
		prev = x.prev;
		x.prev = nullptr;
	}*/
	~list_node()
	{
		ind1 = ind2 = 0;
		next = nullptr;
	}
	list_node &operator=(const list_node &x) = delete;
	/*list2_node &operator=(list2_node &&x)
	{
		*(student*)(this) = x;
		next = x.next;
		x.next = nullptr;
		prev = x.prev;
		x.prev = nullptr;
		return *this;
	}*/
	friend class two_int_stack;
	enum ERRORS
	{
		SUCCESS,
		NODE_LOST
	};
};
class two_int_stack
{
private:
	list_node *top = nullptr;
public:
	two_int_stack() = default;
	~two_int_stack()
	{
		this->delete_stack();
	}
	//read_status read(const char *filename);
	//void print(FILE *fp = stdout);
	void push(int i1, int i2);
	int pop(int &i1, int &i2);
	void del();
	//int not_empty();
	void delete_stack();
	//int solve(command &x, FILE *fp = stdout);
};
#endif //two_int_stack