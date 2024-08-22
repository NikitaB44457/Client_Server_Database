#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "data_base.h"
#include "read_config.h"
#include "consts.h"
int search_structs::m = 0;
int search_structs::pow_k = 0;
int v_vector_rec::max_size = 0;
int search_structs_g::m_g = 128;
int search_structs_g::pow_g = 100;
int v_vector_rec_g::max_size = 128;
int readFromClient(int fd, char *buf);
int writeToClient(int fd, char *buf, data_base &all_rec);
int readFromClient (int fd, char *buf)
{
	int nbytes, len, i;
	int len_num = INT_LEN;
	char num[INT_LEN];
	//Получаем длину в байтовой записи
	for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
	{
		nbytes = read(fd, num + i, len_num);
		if(nbytes < 0)
		{
			perror("read length");
			exit (EXIT_FAILURE);
		}
		else
		{ 
			if(nbytes == 0)
			{
				perror("read truncated");
				exit(EXIT_FAILURE);
			}
		}
	}
	// Формируем длину в виде целого числа
	len = 0;
	for(i = 0; i < INT_LEN; i++)
	{
		len += ((num[i]&B_MAX) << BYTE_LEN*(INT_LEN - i - 1));
	}
	// Проверяем размер буфера
	if(len > LEN)
	{
		perror("too big length");
		return -1;
	}
	// Получаем len байт
	for(i = 0; len > 0; i += nbytes, len -= nbytes)
	{
		nbytes = read(fd, buf + i, len);
		if(nbytes < 0)
		{
			perror("read");
			return -1;
		}
		else 
		{
			if(nbytes == 0)
			{
				perror("read truncated");
				return -1;
			}
		}
	}
	// Длина находится в i
	if(i == 0)
	{
		// нет данных
		fprintf(stderr, "Server: no message\n");
		return -1;
	}
	else
	{
		// есть данные
		fprintf(stdout, "Server got %d bytes of message: %s\n", i, buf);
		return 0;
	}
}
int writeToClient(int fd, char *buf, data_base &all_rec)
{
	int res, i, nbytes;
	char zer[10];
	command com;
	char num[INT_LEN];
	int len_num = INT_LEN;
	if(com.parse(buf, zer))
	{
		res = all_rec.command_apply(com, fd);
		if(res < 0)
		{
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
			switch(res)
			{
				case -1:
				printf ("Server: Disconnected from client.\n");
				break;
				case -2:
				printf ("Server: End of work.\n");
				break;
				case -3:
				printf ("Not enough memory!\n");
				break;
				case -4:
				fprintf (stdout, "Server: Problems with balances in tree!\n");
				break;
				default:
				fprintf (stdout, "Server: Unknown error! Error code: %d\n", res);
				break;
			}
			return res;
		}
	}
	else
	{
		res = -6;
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
	}
	return 0;
}
int main(int argc, char *argv[])
{
	int i, err, opt = 1;
	int k, m;
	int res;
	int pow_k = 1;
	int port;
	int sock, new_sock;
	read_status er_code;
	data_base all_rec;
	fd_set active_set, read_set;
	struct sockaddr_in addr;
	struct sockaddr_in client;
	char buf[LEN];
	char way[LEN];
	socklen_t size;
	if(!(argc == 3 && sscanf(argv[2], "%d", &port) == 1))
	{
		fprintf(stderr, "Usage: %s filename port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if(cat_way(way, argv[0]))
	{
		printf("Can't read way to config.txt\n");
		exit(EXIT_FAILURE);
	}
	er_code = read_config(way, k, m);
	if(er_code != read_status::success)
	{
		switch(er_code)
		{
			case read_status::format:
			printf("Can't read file %s\n", way);
			break;
			case read_status::memory:
			printf("Not enough memory!\n");
			break;
			case read_status::open:
			printf("Can't open file %s\n", way);
			break;
			case read_status::eof:
			printf("EOF error in file %s\n", way);
			break;
			case read_status::balance:
			printf("Problems with balances in tree!\n");
			break;
			case read_status::group:
			printf("Wrong group number detected!\n");
			break;
			case read_status::unknown:
			printf("Unknown error in file %s\n", way);
			break;
			case read_status::success:
			printf("Everything is OK, why are you here?\n");
			break;
		}
		exit(EXIT_FAILURE);
	}
	if(k <= 0 || m <= 0)
	{
		printf("Wrong parametrs in config.txt!\n");
		exit(EXIT_FAILURE);
	}
	for(i = 0; i < k; i++)
	{
		pow_k *= TEN;
	}
	search_structs::set_m(m);
	search_structs::set_pow_k(pow_k);
	v_vector_rec::set_max_size(m);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Server: cannot create socket");
		exit(EXIT_FAILURE);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	err = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
	if(err < 0)
	{
		perror("Server: cannot bind socket");
		exit(EXIT_FAILURE);
	}
	err = listen(sock, 20);
	if(err < 0)
	{
		perror("Server: listen queue failure");
		exit(EXIT_FAILURE);
	}
	FD_ZERO(&active_set);
	FD_SET(sock, &active_set);
	er_code = all_rec.read_base(argv[1]);
	if(er_code != read_status::success)
	{
		switch(er_code)
		{
			case read_status::format:
			printf("Can't read file %s\n", argv[1]);
			break;
			case read_status::memory:
			printf("Not enough memory!\n");
			break;
			case read_status::open:
			printf("Can't open file %s\n", argv[1]);
			break;
			case read_status::eof:
			printf("EOF error in file %s\n", argv[1]);
			break;
			case read_status::balance:
			printf("Problems with balances in tree!\n");
			break;
			case read_status::group:
			printf("Wrong group number detected!\n");
			break;
			case read_status::unknown:
			printf("Unknown error in file %s\n", way);
			break;
			case read_status::success:
			printf("Everything is OK, why are you here?\n");
			break;
		}
		exit(EXIT_FAILURE);
	}
	//printf("Server: ready\n");
	while(1)
	{
		read_set = active_set;
		if(select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0)
		{
			perror("Server: select failure");
			exit(EXIT_FAILURE);
		}
		for(i = 0; i < FD_SETSIZE; i++)
		{
			if(FD_ISSET(i, &read_set))
			{
				if(i == sock)
				{
					size = sizeof(client);
					new_sock = accept(sock, (struct sockaddr *) &client, &size);
					if(new_sock < 0)
					{
						perror("accept");
						exit(EXIT_FAILURE);
					}
					fprintf(stdout, "Server: connect from host %s, port %d.\n", inet_ntoa(client.sin_addr), (unsigned int) ntohs(client.sin_port));
					FD_SET(new_sock, &active_set);
				}
				else
				{
					err = readFromClient(i, buf);
					if(err < 0)
					{
						close(i);
						FD_CLR(i, &active_set);
					}
					else
					{
						res = writeToClient(i, buf, all_rec);
						if(res < 0)
						{
							switch(res)
							{
								case -1:
								close(i);
								FD_CLR(i, &active_set);
								break;
								case -2:
								close(i);
								FD_CLR(i, &active_set);
								close (sock);
								return 0;
								default:
								fprintf(stdout, "Server: problems with data_base\nError #%d\n", i);
								close(i);
								FD_CLR(i, &active_set);
								close (sock);
								return -1;
							}
						}
					}
				}
			}
		}
	}
}