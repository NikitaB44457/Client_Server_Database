CPPFLAGS=-O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -std=gnu++14 -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format -Wmissing-declarations
all: client server
%.o: %.cpp
	g++ -c $(CPPFLAGS) $<
record.o: record.cpp record.h condition.h two_int_stack.h ordering.h consts.h
command.o: command.cpp record.h ordering.h operation.h
record_Dlist.o: record_Dlist.cpp record_Dlist.h command.h consts.h
two_int_stack.o: two_int_stack.cpp two_int_stack.h
search_structs.o: search_structs.cpp search_structs.h
read_config.o: read_config.cpp read_config.h record.h
data_base.o: data_base.cpp data_base.h search_structs.h consts.h
server: server.cpp data_base.h read_config.h consts.h record.o command.o record_Dlist.o two_int_stack.o search_structs.o data_base.o read_config.o
	g++ $(CPPFLAGS) server.cpp data_base.h read_config.h record.o command.o record_Dlist.o two_int_stack.o search_structs.o data_base.o read_config.o -o server
client: client.cpp consts.h
	g++ $(CPPFLAGS) client.cpp -o client
clean: 
	rm -f record.o command.o record_Dlist.o two_int_stack.o search_structs.o data_base.o read_config.o
	rm -f client server
#-O3 -fsanitize=address
# -lasan
