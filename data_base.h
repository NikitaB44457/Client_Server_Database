#ifndef DATA_BASE_H
#define DATA_BASE_H
#include "search_structs.h"
#define MAX_GROUP 1000
class data_base
{
private:
	list2 data_list;
	search_structs n_p_search;
	search_structs_g g_search[MAX_GROUP];
public:
	data_base() = default;
	~data_base()
	{
		this->delete_data_base();
	}
	read_status read_base(const char *filename);
	void print(const ordering order[] = nullptr, FILE *fp = stdout)
	{
		data_list.print(order, fp);
	}
	void delete_data_base()
	{
		int i;
		n_p_search.delete_search_structs();
		for(i = 0; i < MAX_GROUP; i++)
		{
			g_search[i].delete_search_structs_g();
		}
		data_list.delete_list();
		data_list.head = nullptr;
	}
	int command_apply(command &x, int fd);
};
#endif //DATA_BASE_H