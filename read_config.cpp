#include "read_config.h"
int cat_way(char *way, const char *s)
{
	int i;
	int last_slash = 0;
	if(s == nullptr)
	{
		return -1;
	}
	for(i = 0; s[i]; i++)
	{
		if(s[i] == '/')
		{
			last_slash = i;
		}
	}
	strncpy(way, s, last_slash + 1);
	way[last_slash + 1] = 0;
	strcat(way, "config.txt");
	return 0;
}
read_status read_config(const char *way, int &k, int &m)
{
	FILE *fp;
	int i;
	char buf[LEN];
	if(!(fp = fopen(way, "r")))
	{
		return read_status::open;
	}
	m = k = 0;
	while(fgets(buf, LEN, fp))
	{
		for(i = 0; (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') && buf[i]; i++);
		if(buf[i] && buf[i] != '#')
		{
			if(sscanf(buf,"%d", &k) != 1)
			{
				fclose(fp);
				return read_status::format;
			}
			else
			{
				for(; buf[i] >= '0' && buf[i] <=  '9'; i++);
				for(; (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') && buf[i]; i++);
				if(buf[i])
				{
					if(sscanf(buf + i,"%d", &m) != 1)
					{
						fclose(fp);
						return read_status::format;
					}
					else
					{
						fclose(fp);
						return read_status::success;
					}
				}
				break;
			}
		}
	}
	if(feof(fp))
	{
		fclose(fp);
		return read_status::format;
	}
	while(fgets(buf, LEN, fp))
	{
		for(i = 0; (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\n') && buf[i]; i++);
		if(buf[i] && buf[i] != '#')
		{
			if(sscanf(buf,"%d", &m) != 1)
			{
				fclose(fp);
				return read_status::format;
			}
			else
			{
				fclose(fp);
				return read_status::success;
			}
		}
	}
	if(!m || !k)
	{
		fclose(fp);
		return read_status::format;
	}
	fclose(fp);
	return read_status::success;
}