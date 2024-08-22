#include <memory>
#include <string.h>
#include <stdio.h>
#include "record.h"
#include "two_int_stack.h"
#include "consts.h"
using namespace std;
int record::init(const char *n, int p, int g)
{
	phone = p;
	group = g;
	if(n)
	{
		name = std::make_unique<char[]>(strlen(n) + 1);
		if(!name) 
		{
			return -1;
		}
		strcpy(name.get(), n);
	}
	else
	{
		name = nullptr;
	}
	return 0;
}
read_status record::read(FILE *fp)
{
	char buf[LEN];
	name = nullptr;
	if(fscanf(fp, "%s%d%d", buf, &phone, &group) != 3)
	{
        if(feof(fp)) 
		{
			return read_status::eof;
		}
		return read_status::format;
	}
	if(init(buf, phone, group))
	{
		return read_status::memory;
	}
	return read_status::success;
}
int record::print (int fd, const ordering order[])
{
    char buf[LEN] = "";
    char p_g[100] = "";
    int len;
    int len_num;
    char num[INT_LEN];
    int i, nbytes;
    const int max_items = 3;
    const ordering default_ordering[max_items] = {ordering::name, ordering::phone, ordering::group};
    const ordering *p = (order ? order : default_ordering);
    for(int i = 0; i < max_items; i++)
    {
        switch (p[i])
        {
            case ordering::name:
            strcat(buf, name.get()); break;
            case ordering::phone:
            sprintf (p_g, " %d", phone);
            strcat(buf, p_g); break;
            case ordering::group:
            sprintf (p_g, " %d", group);
            strcat(buf, p_g); break;
            case ordering::none:
            continue;
        }
    }
    len = strlen(buf);
    //Учитываем в длине нулевой байт
    len++;
    //Формируем длину в байтовой записи
    for(i = 0; i < INT_LEN; i++)
    {
        num[i] = (len >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
    }
    // Пересылаем длину сообщения
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
    // Пересылаем len байт
    for(i = 0; len > 0; i += nbytes, len -= nbytes)
    {
        nbytes = write (fd, buf + i, len);
        if (nbytes < 0)
        {
            perror("write");
            return -1;
        }
        else 
        {
            if (nbytes == 0)
            {
                perror("write truncated");
                return -1;
            }
        }
    }
    return 0;
}
// Check condition ’x’ for field ’phone’ for ’this’ and ’y’
bool record::compare_phone(condition x, const record& y) const
{
	switch(x)
	{
		case condition::none: // not specified
			return true; // unspecified opeation is true
		case condition::eq: // equal
			return phone == y.phone;
		case condition::ne: // not equal
			return phone != y.phone;
		case condition::lt: // less than
			return phone < y.phone;
		case condition::gt: // less than
			return phone > y.phone;
		case condition::le: // less equal
			return phone <= y.phone;
		case condition::ge: // great equal
			return phone >= y.phone;
		case condition::like: // strings only: match pattern
			return false; // cannot be used for phone
        case condition::nlike: // strings only: not match pattern
            return false; // cannot be used for phone
	}
	return false;
}
bool record::compare_group(condition x, const record& y) const
{
	switch(x)
	{
		case condition::none: // not specified
			return true; // unspecified opeation is true
		case condition::eq: // equal
			return group == y.group;
		case condition::ne: // not equal
			return group != y.group;
		case condition::lt: // less than
			return group < y.group;
		case condition::gt: // less than
			return group > y.group;
		case condition::le: // less equal
			return group <= y.group;
		case condition::ge: // great equal
			return group >= y.group;
        case condition::like: // strings only: match pattern
            return false; // cannot be used for group
		case condition::nlike: // strings only: not match pattern
            return false; // cannot be used for group
	}
	return false;
}
bool record::compare_name(condition x, const record& y) const
{
	switch(x)
	{
		case condition::none: // not specified
			return true; // unspecified opeation is true
		case condition::eq: // equal
			return !strcmp(name.get(),y.name.get());
		case condition::ne: // not equal
			return strcmp(name.get(),y.name.get());
		case condition::lt: // less than
			return strcmp(name.get(),y.name.get()) < 0;
		case condition::gt: // less than
			return strcmp(name.get(),y.name.get()) > 0;
		case condition::le: // less equal
			return strcmp(name.get(),y.name.get()) <= 0;
		case condition::ge: // great equal
			return strcmp(name.get(),y.name.get()) >= 0;
		case condition::like: // strings only: match pattern
			return pattern_check(*this, y);
        case condition::nlike: // strings only: not match pattern
            return !pattern_check(*this, y);
	}
	return false;
}
bool pattern_check(const record &x, const record &y)
{
    int buf_end = 0;
    int ind1, ind2;
    int i;
    int mark = 0;
    char buf[LEN];
    two_int_stack A;
    strcpy(buf, y.name.get());
    if(buf[0])
    {
        if(buf[0] == '\\')
        {
            buf[0] = 0;
        }
        for(buf_end = 1; buf[buf_end]; buf_end++)
        {
            if(buf[buf_end] == '\\' && buf[buf_end - 1])
            {
                buf[buf_end] = 0;
            }
        }
    }
    else
    {
        buf_end = 0;
    }
    for(ind1 = ind2 = 0; x.name[ind1] && ind2 < buf_end; ind2++)
    {
        if(buf[ind2])
        {
            if(!ind2 || buf[ind2 - 1])
            {
                switch(buf[ind2])
                {
                    case '_':
                    ind1++;
                    break;
                    case '%':
                    for(i = ind2 + 1; i < buf_end && buf[i] == '%'; i++);
                    if(i == buf_end)
                    {
                        return true;
                    }
                    mark = i - 1;
                    if(!buf[i])
                    {
                        if(i < buf_end - 1)
                        {
                            ind2 = i + 1;
                        }
                        else
                        {
                            return true;
                        }
                        for(i = ind1; x.name[i] && x.name[i]!= buf[ind2]; i++);
                        if(x.name[i])
                        {
                            A.push(i, mark);
                            ind1 = i + 1;
                        }
                        else
                        {
                            if(A.pop(ind1, ind2) >= 0)
                            {
                                ind1++;
                                ind2--;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        ind2 = i;
                        switch(buf[ind2])
                        {
                            case '_':
                            A.push(ind1, mark);
                            ind1++;
                            break;
                            case '[':
                            if(buf[ind2 + 1] != '^')
                            {
                                if(ind2 > buf_end - 4)
                                {
                                    return false;
                                }
                                for(i = ind1; x.name[i] && (x.name[i] < buf[ind2 + 1] || x.name[i] > buf[ind2 + 3]); i++);
                                if(x.name[i])
                                {
                                    A.push(i, mark);
                                    ind1 = i + 1;
                                    ind2 += 4;
                                }
                                else
                                {
                                    if(A.pop(ind1, ind2) >= 0)
                                    {
                                        ind1++;
                                        ind2--;
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                }
                            }
                            else
                            {
                                if(ind2 > buf_end - 5)
                                {
                                    return false;
                                }
                                for(i = ind1; x.name[i] && x.name[i] >= buf[ind2 + 1] && x.name[i] <= buf[ind2 + 3]; i++);
                                if(x.name[i])
                                {
                                    A.push(i, mark);
                                    ind1 = i + 1;
                                    ind2 += 4;
                                }
                                else
                                {
                                    if(A.pop(ind1, ind2) >= 0)
                                    {
                                        ind1++;
                                        ind2--;
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                }
                            }
                            break;
                            default:
                            for(i = ind1; x.name[i] && x.name[i]!= buf[ind2]; i++);
                            if(x.name[i])
                            {
                                A.push(i, mark);
                                ind1 = i + 1;
                            }
                            else
                            {
                                if(A.pop(ind1, ind2) >= 0)
                                {
                                    ind1++;
                                    ind2--;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            break;
                        }
                    }
                    break;
                    case '[':
                    if(buf[ind2 + 1] != '^')
                    {
                        if(ind2 > buf_end - 4)
                        {
                            return false;
                        }
                        if(x.name[ind1] >= buf[ind2 + 1] && x.name[ind1] <= buf[ind2 + 3])
                        {
                            ind1++;
                            ind2 += 4;
                        }
                        else
                        {
                            if(A.pop(ind1, ind2) >= 0)
                            {
                                ind1++;
                                ind2--;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        if(ind2 > buf_end - 5)
                        {
                            return false;
                        }
                        if(x.name[ind1] < buf[ind2 + 2] || x.name[ind1] > buf[ind2 + 4])
                        {
                            ind1++;
                            ind2 += 5;
                        }
                        else
                        {
                            if(A.pop(ind1, ind2) >= 0)
                            {
                                ind1++;
                                ind2--;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                    break;
                    default:
                    if(x.name[ind1] != buf[ind2])
                    {
                        if(A.pop(ind1, ind2) >= 0)
                        {
                            ind1++;
                            ind2--;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        ind1++;
                    }
                    break;
                }
            }
            else
            {
                if(x.name[ind1] != buf[ind2])
                {
                    if(A.pop(ind1, ind2) >= 0)
                    {
                        ind1++;
                        ind2--;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    ind1++;
                }
            }
        }
        if(!(x.name[ind1] && ind2 + 1 < buf_end))
        {
            if(ind2 + 1 < buf_end)
            {
                for(i = ind2 + 1; i < buf_end && buf[i] == '%'; i++);
                ind2 = i;
            }
            else
            {
                ind2++;
            }
            if(!x.name[ind1] && ind2 >= buf_end)
            {
                return true;
            }
            if(A.pop(ind1, ind2) >= 0)
            {
                ind1++;
            }
            ind2--;
        }
    }
    if(!x.name[ind1] && ind2 >= buf_end)
    {
        return true;
    }
    return false;
}
int record::pgn_compare(const record &y) const
{
    int diff;
    diff = phone - y.phone;
    if(diff)
    {
        return diff;
    }
    diff = group - y.group;
    if(diff)
    {
        return diff;
    }
    if(name.get() == nullptr)
    {
        if(y.name.get() == nullptr)
        {
            diff = 0;
        }
        else
        {
            diff = -1;
        }
    }
    else
    {
        if(y.name.get() == nullptr)
        {
            diff = 1;
        }
        else
        {
            diff = strcmp(name.get(),y.name.get());
        }
    }
    return diff;
}
int record::phone_hash(int pow_k)
{
    //int i;
    /*int pow = 1;
    for(i = 0; i < k; i++)
    {
        pow *= TEN;
    }*/
    return phone % pow_k;
}