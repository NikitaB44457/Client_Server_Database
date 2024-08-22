#include "consts.h"
#include "record_Dlist.h"
read_status list2::read_list(const char *filename)
{
    FILE *fp;
    list2_node *curr;
    list2_node *tail = nullptr;
    read_status res;
    if(head != nullptr)
    {
        this->delete_list();
    }
    if(!(fp = fopen(filename, "r")))
    {
        return read_status::open;
    }
    curr = new list2_node;
    if(curr == nullptr)
    {
        fclose(fp);
        return read_status::memory;
    }
    res = curr->read(fp);
    if(res != read_status::success)
    {
        if(res == read_status::eof)
        {
            fclose(fp);
            delete curr;
            return read_status::success;
        }
        else
        {
            fclose(fp);
            delete curr;
            return res;
        }
    }
    head = tail = curr;
    curr = new list2_node;
    if(curr == nullptr)
    {
        fclose(fp);
        this->delete_list();
        return read_status::memory;
    }
    while((res = curr->read(fp)) == read_status::success)
    {
        curr->prev = tail;
        tail->next = curr;
        tail = curr;
        curr = new list2_node;
        if(curr == nullptr)
        {
            fclose(fp);
            this->delete_list();
            return read_status::memory;
        }
    }
    delete curr;
    if(!feof(fp))
    {
        fclose(fp);
        this->delete_list();
        return read_status::format;
    }
    fclose(fp);
    return read_status::success;
}
void list2::delete_list()
{
    list2_node *curr = head;
    list2_node *curr_n;
    while(curr != nullptr)
    {
        curr_n = curr->next;
        delete curr;
        curr = curr_n;
    }
    head = nullptr;
}
void list2::merge_search_name(list2 &x, command &com)
{
    list2_node *curr1 = head;
    list2_node *curr2 = x.head;
    list2_node *new_head;
    list2_node *tail;
    if(curr1 == nullptr)
    {
        head = x.head;
        x.head = nullptr;
        return;
    }
    else
    {
        if(curr2 == nullptr)
        {
            return;
        }
        else
        {
            if(com.record_order_cmp(*curr1, *curr2) <= 0)
            {
                tail = new_head = curr1;
                curr1 = curr1->next_search_name;
            }
            else
            {
                tail = new_head = curr2;
                curr2 = curr2->next_search_name;   
            }
        }
    }
    while(curr1 != nullptr && curr2 != nullptr)
    {
        if(com.record_order_cmp(*curr1, *curr2) <= 0)
        {
            tail->next_search_name = curr1;
            tail = curr1;
            curr1 = curr1->next_search_name;
        }
        else
        {
            tail->next_search_name = curr2;
            tail = curr2;
            curr2 = curr2->next_search_name;
        }
    }
    if(curr1 == nullptr)
    {
        tail->next_search_name = curr2;
    }
    else
    {
        tail->next_search_name = curr1;
    }
    head = new_head;
    x.head = nullptr;
}
void list2::Neumann_search_name_sort(command &com)
{
    list2 A;
    list2 B;
    list2 C;
    list2_node *curr;
    list2_node *tail_c = nullptr;
    int k;
    int i;
    for(k = 1; ; k <<= 1)
    {
        while(head != nullptr)
        {
            for(i = 1, curr = head; i < k && curr != nullptr; curr = curr->next_search_name, i++);
            A.head = head;
            if(curr != nullptr)
            {
                B.head = curr->next_search_name;
                curr->next_search_name = nullptr;
            }
            else
            {
                if(C.head == nullptr)
                {
                    A.head = B.head = nullptr;
                    return;
                }
                else
                {
                    B.head = nullptr;
                }
            }
            for(i = 1, curr = B.head; i < k && curr != nullptr; curr = curr->next_search_name, i++);
            if(curr != nullptr)
            {
                head = curr->next_search_name;
                curr->next_search_name = nullptr;
            }
            else
            {
                head = nullptr;
            }
            A.merge_search_name(B, com);
            if(tail_c == nullptr)
            {
                C.head = A.head;
                for(tail_c = A.head; tail_c->next_search_name != nullptr; tail_c = tail_c->next_search_name);
            }
            else
            {
                tail_c->next_search_name = A.head;
                for(; tail_c->next_search_name != nullptr; tail_c = tail_c->next_search_name);
            }
        }
        if(C.head != nullptr)
        {
            head = C.head;
            C.head = nullptr;
            tail_c = nullptr;
        }
        else
        {
            break;
        }
    }
    A.head = B.head = C.head = nullptr;
}
void list2::merge_search_phone(list2 &x, command &com)
{
    list2_node *curr1 = head;
    list2_node *curr2 = x.head;
    list2_node *new_head;
    list2_node *tail;
    if(curr1 == nullptr)
    {
        head = x.head;
        x.head = nullptr;
        return;
    }
    else
    {
        if(curr2 == nullptr)
        {
            return;
        }
        else
        {
            if(com.record_order_cmp(*curr1, *curr2) <= 0)
            {
                tail = new_head = curr1;
                curr1 = curr1->next_search_phone;
            }
            else
            {
                tail = new_head = curr2;
                curr2 = curr2->next_search_phone;   
            }
        }
    }
    while(curr1 != nullptr && curr2 != nullptr)
    {
        if(com.record_order_cmp(*curr1, *curr2) <= 0)
        {
            tail->next_search_phone = curr1;
            tail = curr1;
            curr1 = curr1->next_search_phone;
        }
        else
        {
            tail->next_search_phone = curr2;
            tail = curr2;
            curr2 = curr2->next_search_phone;
        }
    }
    if(curr1 == nullptr)
    {
        tail->next_search_phone = curr2;
    }
    else
    {
        tail->next_search_phone = curr1;
    }
    head = new_head;
    x.head = nullptr;
}
void list2::Neumann_search_phone_sort(command &com)
{
    list2 A;
    list2 B;
    list2 C;
    list2_node *curr;
    list2_node *tail_c = nullptr;
    int k;
    int i;
    for(k = 1; ; k <<= 1)
    {
        while(head != nullptr)
        {
            for(i = 1, curr = head; i < k && curr != nullptr; curr = curr->next_search_phone, i++);
            A.head = head;
            if(curr != nullptr)
            {
                B.head = curr->next_search_phone;
                curr->next_search_phone = nullptr;
            }
            else
            {
                if(C.head == nullptr)
                {
                    A.head = B.head = nullptr;
                    return;
                }
                else
                {
                    B.head = nullptr;
                }
            }
            for(i = 1, curr = B.head; i < k && curr != nullptr; curr = curr->next_search_phone, i++);
            if(curr != nullptr)
            {
                head = curr->next_search_phone;
                curr->next_search_phone = nullptr;
            }
            else
            {
                head = nullptr;
            }
            A.merge_search_phone(B, com);
            if(tail_c == nullptr)
            {
                C.head = A.head;
                for(tail_c = A.head; tail_c->next_search_phone != nullptr; tail_c = tail_c->next_search_phone);
            }
            else
            {
                tail_c->next_search_phone = A.head;
                for(; tail_c->next_search_phone != nullptr; tail_c = tail_c->next_search_phone);
            }
        }
        if(C.head != nullptr)
        {
            head = C.head;
            C.head = nullptr;
            tail_c = nullptr;
        }
        else
        {
            break;
        }
    }
    A.head = B.head = C.head = nullptr;
}
void list2::merge_search_group(list2 &x, command &com)
{
    list2_node *curr1 = head;
    list2_node *curr2 = x.head;
    list2_node *new_head;
    list2_node *tail;
    if(curr1 == nullptr)
    {
        head = x.head;
        x.head = nullptr;
        return;
    }
    else
    {
        if(curr2 == nullptr)
        {
            return;
        }
        else
        {
            if(com.record_order_cmp(*curr1, *curr2) <= 0)
            {
                tail = new_head = curr1;
                curr1 = curr1->next_search_group;
            }
            else
            {
                tail = new_head = curr2;
                curr2 = curr2->next_search_group;   
            }
        }
    }
    while(curr1 != nullptr && curr2 != nullptr)
    {
        if(com.record_order_cmp(*curr1, *curr2) <= 0)
        {
            tail->next_search_group = curr1;
            tail = curr1;
            curr1 = curr1->next_search_group;
        }
        else
        {
            tail->next_search_group = curr2;
            tail = curr2;
            curr2 = curr2->next_search_group;
        }
    }
    if(curr1 == nullptr)
    {
        tail->next_search_group = curr2;
    }
    else
    {
        tail->next_search_group = curr1;
    }
    head = new_head;
    x.head = nullptr;
}
void list2::Neumann_search_group_sort(command &com)
{
    list2 A;
    list2 B;
    list2 C;
    list2_node *curr;
    list2_node *tail_c = nullptr;
    int k;
    int i;
    for(k = 1; ; k <<= 1)
    {
        while(head != nullptr)
        {
            for(i = 1, curr = head; i < k && curr != nullptr; curr = curr->next_search_group, i++);
            A.head = head;
            if(curr != nullptr)
            {
                B.head = curr->next_search_group;
                curr->next_search_group = nullptr;
            }
            else
            {
                if(C.head == nullptr)
                {
                    A.head = B.head = nullptr;
                    return;
                }
                else
                {
                    B.head = nullptr;
                }
            }
            for(i = 1, curr = B.head; i < k && curr != nullptr; curr = curr->next_search_group, i++);
            if(curr != nullptr)
            {
                head = curr->next_search_group;
                curr->next_search_group = nullptr;
            }
            else
            {
                head = nullptr;
            }
            A.merge_search_group(B, com);
            if(tail_c == nullptr)
            {
                C.head = A.head;
                for(tail_c = A.head; tail_c->next_search_group != nullptr; tail_c = tail_c->next_search_group);
            }
            else
            {
                tail_c->next_search_group = A.head;
                for(; tail_c->next_search_group != nullptr; tail_c = tail_c->next_search_group);
            }
        }
        if(C.head != nullptr)
        {
            head = C.head;
            C.head = nullptr;
            tail_c = nullptr;
        }
        else
        {
            break;
        }
    }
    A.head = B.head = C.head = nullptr;
}
void list2::merge_search(list2 &x, command &com)
{
    list2_node *curr1 = head;
    list2_node *curr2 = x.head;
    list2_node *new_head;
    list2_node *tail;
    if(curr1 == nullptr)
    {
        head = x.head;
        x.head = nullptr;
        return;
    }
    else
    {
        if(curr2 == nullptr)
        {
            return;
        }
        else
        {
            if(com.record_order_cmp(*curr1, *curr2) <= 0)
            {
                tail = new_head = curr1;
                curr1 = curr1->next_search;
            }
            else
            {
                tail = new_head = curr2;
                curr2 = curr2->next_search;   
            }
        }
    }
    while(curr1 != nullptr && curr2 != nullptr)
    {
        if(com.record_order_cmp(*curr1, *curr2) <= 0)
        {
            tail->next_search = curr1;
            tail = curr1;
            curr1 = curr1->next_search;
        }
        else
        {
            tail->next_search = curr2;
            tail = curr2;
            curr2 = curr2->next_search;
        }
    }
    if(curr1 == nullptr)
    {
        tail->next_search = curr2;
    }
    else
    {
        tail->next_search = curr1;
    }
    head = new_head;
    x.head = nullptr;
}
void list2::Neumann_search_sort(command &com)
{
    list2 A;
    list2 B;
    list2 C;
    list2_node *curr;
    list2_node *tail_c = nullptr;
    int k;
    int i;
    for(k = 1; ; k <<= 1)
    {
        while(head != nullptr)
        {
            for(i = 1, curr = head; i < k && curr != nullptr; curr = curr->next_search, i++);
            A.head = head;
            if(curr != nullptr)
            {
                B.head = curr->next_search;
                curr->next_search = nullptr;
            }
            else
            {
                if(C.head == nullptr)
                {
                    A.head = B.head = nullptr;
                    return;
                }
                else
                {
                    B.head = nullptr;
                }
            }
            for(i = 1, curr = B.head; i < k && curr != nullptr; curr = curr->next_search, i++);
            if(curr != nullptr)
            {
                head = curr->next_search;
                curr->next_search = nullptr;
            }
            else
            {
                head = nullptr;
            }
            A.merge_search(B, com);
            if(tail_c == nullptr)
            {
                C.head = A.head;
                for(tail_c = A.head; tail_c->next_search != nullptr; tail_c = tail_c->next_search);
            }
            else
            {
                tail_c->next_search = A.head;
                for(; tail_c->next_search != nullptr; tail_c = tail_c->next_search);
            }
        }
        if(C.head != nullptr)
        {
            head = C.head;
            C.head = nullptr;
            tail_c = nullptr;
        }
        else
        {
            break;
        }
    }
    A.head = B.head = C.head = nullptr;
}
int list2::merge_name_phone(list2 &name_list, list2 &phone_list)
{
    list2_node *curr_n;
    list2_node *curr_p;
    list2_node *tail = nullptr;
    int diff = 0;
    int res = 0;
    if(head != nullptr)
    {
        head = nullptr;
    }
    curr_n = name_list.head, curr_p = phone_list.head;
    while(curr_n != nullptr && curr_p != nullptr)
    {
        diff = curr_n->pgn_compare(*curr_p);
        if(diff < 0)
        {
            if(head)
            {
                tail->next_search = curr_n;
                tail = curr_n;
            }
            else
            {
                head = tail = curr_n;
            }
            curr_n = curr_n->next_search_name;
        }
        else
        {
            if(diff > 0)
            {
                if(head)
                {
                    tail->next_search = curr_p;
                    tail = curr_p;
                }
                else
                {
                    head = tail = curr_p;
                }
                curr_p = curr_p->next_search_phone;
            }
            else
            {
                if(head)
                {
                    tail->next_search = curr_n;
                    tail = curr_n;
                }
                else
                {
                    head = tail = curr_n;
                }
                curr_n = curr_n->next_search_name;
                curr_p = curr_p->next_search_phone;
            }
        }
        res++;
    }
    if(curr_n == nullptr)
    {
        while(curr_p)
        {
            if(head)
            {
                tail->next_search = curr_p;
                tail = curr_p;
            }
            else
            {
                head = tail = curr_p;
            }
            curr_p = curr_p->next_search_phone;
            res++;
        }
    }
    else
    {
        while(curr_n)
        {
            if(head)
            {
                tail->next_search = curr_n;
                tail = curr_n;
            }
            else
            {
                head = tail = curr_n;
            }
            curr_n = curr_n->next_search_name;
            res++;
        }
    }
    if(tail)
    {
        tail->next_search = nullptr;
    }
    return res;
}
int list2::merge_name_phone_group(list2 &name_phone_list, list2 &group_list)
{
    list2_node *curr;
    list2_node *curr_g;
    list2_node *tail = nullptr;
    int diff = 0;
    int res = 0;
    if(head != nullptr)
    {
        head = nullptr;
    }
    curr = name_phone_list.head;
    curr_g = group_list.head;
    while(curr != nullptr && curr_g != nullptr)
    {
        diff = curr->pgn_compare(*curr_g);
        if(diff < 0)
        {
            if(head)
            {
                tail->next_search = curr;
                tail = curr;
            }
            else
            {
                head = tail = curr;
            }
            curr = curr->next_search;
        }
        else
        {
            if(diff > 0)
            {
                if(head)
                {
                    tail->next_search = curr_g;
                    tail = curr_g;
                }
                else
                {
                    head = tail = curr_g;
                }
                curr_g = curr_g->next_search_group;
            }
            else
            {
                if(head)
                {
                    tail->next_search = curr;
                    tail = curr;
                }
                else
                {
                    head = tail = curr;
                }
                curr = curr->next_search;
                curr_g = curr_g->next_search_group;
            }
        }
        res++;
    }
    if(curr == nullptr)
    {
        while(curr_g)
        {
            if(head)
            {
                tail->next_search = curr_g;
                tail = curr_g;
            }
            else
            {
                head = tail = curr_g;
            }
            curr_g = curr_g->next_search_group;
            res++;
        }
    }
    else
    {
        while(curr)
        {
            if(head)
            {
                tail->next_search = curr;
                tail = curr;
            }
            else
            {
                head = tail = curr;
            }
            curr = curr->next_search;
            res++;
        }
    }
    if(tail)
    {
        tail->next_search = nullptr;
    }
    return res;
}
/*void list2::print(const ordering order[], FILE *fp)
{
    int i;
    list2_node *curr = head;
    for(i = 0; curr != nullptr && i < MAX_PRINT; i++)
    {
        curr->print(order, fp);
        curr = curr->next;
    }
}*/
int list2::print_search(int fd, const ordering order[])
{
    list2_node *curr_search;
    for(curr_search = head; curr_search != nullptr; curr_search = curr_search->next_search)
    {
        if(curr_search->print(fd, order))
        {
            return -1;
        }
    }
    return 0;
}
/*
void list2::print_search_name(const ordering order[], FILE *fp)
{
    int i;
    list2_node *curr = head;
    for(i = 0; curr != nullptr && i < MAX_PRINT; i++)
    {
        curr->print(order, fp);
        curr = curr->next_search_name;
    }
}
void list2::print_search_phone(const ordering order[], FILE *fp)
{
    int i;
    list2_node *curr = head;
    for(i = 0; curr != nullptr && i < MAX_PRINT; i++)
    {
        curr->print(order, fp);
        curr = curr->next_search_phone;
    }
}*/
int list2::command_apply(command &x, int fd)
{
    int res = 0;
    list2_node *curr = nullptr;
    list2_node *curr_n = nullptr;
    list2 search_list;
    list2_node *curr_search = nullptr;
    int i, nbytes, len_num;
    char num[INT_LEN];
    switch(x.get_type())
    {
        case command_type::insert:
        for(curr = head; curr != nullptr; curr = curr->next)
        {
            if(curr->compare_name(condition::eq, x) && curr->compare_phone(condition::eq, x) && curr->compare_group(condition::eq, x))
            {
                return 0;
            }
        }
        curr = new list2_node;
        if(curr == nullptr)
        {
            return -2;
        }
        curr->init(x.get_name(), x.get_phone(), x.get_group());
        if(head)
        {
            head->prev = curr;
            curr->next = head;
        }
        head = curr;
        // Формируем ответ в байтовой записи
        for(i = 0; i < INT_LEN; i++)
        {
            num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
        }
        // Пересылаем ответ
        len_num = INT_LEN;
        for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
        {
            nbytes = write(fd, num + i, len_num);
            if(nbytes < 0)
            {
                perror("write length");
                exit(EXIT_FAILURE);
            }
            else 
            {
                if(nbytes == 0)
                {
                    perror ("write truncated");
                    exit (EXIT_FAILURE);
                }
            }
        }
        return res;
        case command_type::select:
        for(curr = head; curr != nullptr; curr = curr->next)
        {
            if(x.apply(*curr))
            {
                res++;
                if(!search_list.head)
                {
                    curr_search = search_list.head = curr;
                }
                else
                {
                    curr_search->next_search = curr;
                    curr_search = curr;
                }
            }
        }
        if(curr_search)
        {
            curr_search->next_search = nullptr;
        }
        search_list.Neumann_search_sort(x);
        // Формируем ответ в байтовой записи
        for(i = 0; i < INT_LEN; i++)
        {
            num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
        }
        // Пересылаем ответ
        len_num = INT_LEN;
        for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
        {
            nbytes = write(fd, num + i, len_num);
            if(nbytes < 0)
            {
                perror("write length");
                exit(EXIT_FAILURE);
            }
            else 
            {
                if(nbytes == 0)
                {
                    perror ("write truncated");
                    exit (EXIT_FAILURE);
                }
            }
        }
        for(curr_search = search_list.head; curr_search != nullptr; curr_search = curr_search->next_search)
        {
            if(curr_search->print(fd, x.get_order()))
            {
                return -1;
            }
        }
        search_list.head = nullptr;
        return res;
        case command_type::del:
        curr = head;
        while(curr != nullptr)
        {
            if(x.apply(*curr))
            {
                //res++;
                curr_n = curr->next;
                if(curr_n)
                {
                    curr_n->prev = curr->prev;
                }
                if(curr != head)
                {
                    curr->prev->next = curr_n;
                }
                else
                {
                    head = curr_n;
                }
                delete curr;
                curr = curr_n;
            }
            else
            {
                curr = curr->next;
            }
        }
        // Формируем ответ в байтовой записи
        for(i = 0; i < INT_LEN; i++)
        {
            num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
        }
        // Пересылаем ответ
        len_num = INT_LEN;
        for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
        {
            nbytes = write(fd, num + i, len_num);
            if(nbytes < 0)
            {
                perror("write length");
                exit(EXIT_FAILURE);
            }
            else 
            {
                if(nbytes == 0)
                {
                    perror ("write truncated");
                    exit (EXIT_FAILURE);
                }
            }
        }
        return res;
        case command_type::quit:
        res = -1;
        // Формируем ответ в байтовой записи
        for(i = 0; i < INT_LEN; i++)
        {
            num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
        }
        // Пересылаем ответ
        len_num = INT_LEN;
        for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
        {
            nbytes = write(fd, num + i, len_num);
            if(nbytes < 0)
            {
                perror("write length");
                exit(EXIT_FAILURE);
            }
            else 
            {
                if(nbytes == 0)
                {
                    perror ("write truncated");
                    exit (EXIT_FAILURE);
                }
            }
        }
        return res;
        case command_type::stop:
        res = -2;
        // Формируем ответ в байтовой записи
        for(i = 0; i < INT_LEN; i++)
        {
            num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
        }
        // Пересылаем ответ
        len_num = INT_LEN;
        for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
        {
            nbytes = write(fd, num + i, len_num);
            if(nbytes < 0)
            {
                perror("write length");
                exit(EXIT_FAILURE);
            }
            else 
            {
                if(nbytes == 0)
                {
                    perror ("write truncated");
                    exit (EXIT_FAILURE);
                }
            }
        }
        return res;
        case command_type::none:
        res = 0;
        // Формируем ответ в байтовой записи
        for(i = 0; i < INT_LEN; i++)
        {
            num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
        }
        // Пересылаем ответ
        len_num = INT_LEN;
        for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
        {
            nbytes = write(fd, num + i, len_num);
            if(nbytes < 0)
            {
                perror("write length");
                exit(EXIT_FAILURE);
            }
            else 
            {
                if(nbytes == 0)
                {
                    perror ("write truncated");
                    exit (EXIT_FAILURE);
                }
            }
        }
        return res;
    }
    // Формируем ответ в байтовой записи
    for(i = 0; i < INT_LEN; i++)
    {
        num[i] = (res >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
    }
    // Пересылаем ответ
    len_num = INT_LEN;
    for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
    {
        nbytes = write(fd, num + i, len_num);
        if(nbytes < 0)
        {
            perror("write length");
            exit(EXIT_FAILURE);
        }
        else 
        {
            if(nbytes == 0)
            {
                perror ("write truncated");
                exit (EXIT_FAILURE);
            }
        }
    }
    return res;
}