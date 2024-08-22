#ifndef command_H
#define command_H
#include <stdio.h>
#include "record.h"
#include "operation.h"
#include "ordering.h"
#include "command_type.h"
enum class field_type
{
	none,
	name,
	phone,
	group
};
class command : public record
{
	private:
		static const int max_items = 3;
		command_type type = command_type::none;
		condition c_name = condition::none;
		condition c_phone = condition::none;
		condition c_group = condition::none;
		operation op = operation::none;
		ordering order[max_items] = { };
		ordering order_by[max_items] = { };
		bool insert_parse(const char *string, char *remainder);
		bool select_parse(const char *string, char *remainder);
		bool delete_parse(const char *string, char *remainder);
		bool order_parse(const char* &cond, char *word, char *remainder);
		bool order_by_parse(const char* &cond, char *word, char *remainder);
		void rem_write(const char *string, char *remainder);
		bool condition_parse(const char* &cond, char *word, field_type fld);
		bool field_read(const char* &cond, char *n, int &p, int &g, field_type fld);
		bool first_condition_parse(const char* &cond, char *word, char *n, int &p, int &g, field_type &first_field_type, int &ind);
		bool second_condition_parse(const char* &cond, char *word, char *n, int &p, int &g, field_type &first_field_type, field_type &second_field_type, int &i);
		bool third_condition_parse(const char* &cond, char *word, char *n, int &p, int &g, field_type &first_field_type, field_type &second_field_type, field_type &third_field_type,int &i);
	public:
		command() = default;
		~command() = default;
		//Convert string command to data structure
		//Example: "select name, group where phone = 1234567 and name
		//like St% and group <> 208 order by group, name;"
		//parsed to
		//command::type = command_type::select,
		//command::name = "St%", command::c_name = condition::like,
		//command::phone = 1234567, command::c_phone = condition::eq,
		//command::group = 208, command::c_group = condition::ne,
		//command::op = operation::land,
		//command::order={ordering::name,ordering::group,ordering::none},
		//command::order_by={ordering::group,ordering::name,ordering::none}
		//other fields are unspecified
		const ordering* get_order() const
		{
			return order;
		}
		const ordering* get_order_by() const
		{
			return order_by;
		}
		command_type get_type() const
		{
			return type;
		}
		operation get_op() const
		{
			return op;
		}
		condition get_c_name() const
		{
			return c_name;
		}
		condition get_c_phone() const
		{
			return c_phone;
		}
		condition get_c_group() const
		{
			return c_group;
		}
		bool parse(const char *string, char *remainder);
		void print(FILE *fp = stdout) const; // Print parsed structure
		bool apply(const record& x) const; // Apply command, return comparision result for record ’x’
		int record_order_cmp(const record& x, const record& y) const;
};
#endif //command_H