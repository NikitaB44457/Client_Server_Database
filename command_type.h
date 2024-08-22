#ifndef command_type_H
#define command_type_H
enum class command_type
{
	none, //uninitialized
	stop, //end of server work
	quit, //end of input stream
	insert, //add record
	select, //find by conditions specified
	del //delete record
};
#endif //command_type_H