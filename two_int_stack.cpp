#include "two_int_stack.h"
void two_int_stack::delete_stack()
{
    list_node *curr = top;
    list_node *curr_n;
    while(curr != nullptr)
    {
        curr_n = curr->next;
        delete curr;
        curr = curr_n;
    }
}
void two_int_stack::push(int i1, int i2)
{
    list_node *new_node = new list_node(i1, i2);
    new_node->next = top;
    top = new_node;
}
int two_int_stack::pop(int &i1, int &i2)
{
    list_node *del = top;
    if(top != nullptr)
    {
        i1 = del->ind1;
        i2 = del->ind2;
        top = del->next;
        delete del;
        return 0;
    }
    return -1;
}
void two_int_stack::del()
{
    list_node *del = top;
    if(top != nullptr)
    {
        top = del->next;
        delete del;
    }
}
/*int two_int_stack::top()
{
    if(top != nullptr)
    {
        return 1;
    }
    return 0;
}*/