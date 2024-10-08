#ifndef record_H
#define record_H
#include <memory>
#include <stdio.h>
#include <unistd.h>
#include "condition.h"
#include "ordering.h"
#define LEN 1234
#define TEN 10
enum class read_status
{
	success = 0,
	eof = -1,
	format = -2,
	memory = -3,
	open = -4,
	balance = -5,
	group = -6,
	unknown = -7
};
class name_avl_tree;
class record
{
	private:
		std::unique_ptr<char[]> name = nullptr;
		int phone = 0;
		int group = 0;
	public:
		record() = default;
		~record() = default;
		char* get_name() const
		{
			return name.get();
		}
		int get_phone() const
		{
			return phone;
		}
		int get_group() const
		{
			return group;
		}
		int init(const char *n, int p, int g); // Allow as return value for functions
		record(record &&x) = default; // move constructor
		// Assignement move operator
		record& operator=(record&& x) = default;
		// Prohoibit pass by value
		// (it is default when move constructor is declared)
		record(const record &x) = delete;
		// Prohoibit assignement operator
		// (it is default when move constructor is declared)
		record& operator=(const record&) = delete;
		bool compare_name(condition x, const record &y) const; // Check condition ’x’ for field ’name’ for ’this’ and ’y’
		bool compare_phone(condition x, const record &y) const; // Check condition ’x’ for field ’phone’ for ’this’ and ’y’
		bool compare_group(condition x, const record &y) const; // Check condition ’x’ for field ’group’ for ’this’ and ’y’
		int pgn_compare(const record &y) const;
		int print (int fd, const ordering order[] = nullptr);
		read_status read(FILE *fp = stdin);
		int phone_hash(int k);
		friend bool pattern_check(const record &x, const record &y);
		friend class name_avl_tree;
};
# endif //record_H