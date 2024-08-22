#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include "consts.h"
#define LEN 1234
void writeToServer(int fd, char *string, int len);
int readFromServer(int fd);
void ReadRecord(int fd);
void writeToServer(int fd, char *string, int len)
{
	int nbytes, i;
	int len_num = INT_LEN;
	char num[INT_LEN];
	// Длина сообщения
	len++;
	//Формируем длину в байтовой записи
	for(i = 0; i < INT_LEN; i++)
	{
		num[i] = (len >> BYTE_LEN*(INT_LEN - i - 1)) & B_MAX;
	}
	// Пересылаем длину сообщения
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
		nbytes = write(fd, string + i, len);
		if(nbytes < 0)
		{
			perror("write");
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
int readFromServer(int fd)
{
	int nbytes, i;
	int len_num = INT_LEN;
	char num[INT_LEN];
	int res;
	//Получаем ответ в байтовой записи
	for(i = 0; len_num > 0; i += nbytes, len_num -= nbytes)
	{
		nbytes = read(fd, num + i, len_num);
		if(nbytes < 0)
		{
			perror("read answer");
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
	// Формируем ответ в виде целого числа
	res = 0;
	for(i = 0; i < INT_LEN; i++)
	{
		res += ((num[i]&B_MAX) << BYTE_LEN*(INT_LEN - i - 1));
	}
	//Получаем нужное количество строк
	for(i = 0; i < res; i++)
	{
		ReadRecord(fd);
	}
	return res;
}
void ReadRecord(int fd)
{
	int nbytes, len, i;
	char buf[LEN];
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
	// Получаем len байт
	for(i = 0; len > 0; i += nbytes, len -= nbytes)
	{
		nbytes = read(fd, buf + i, len);
		if(nbytes < 0)
		{
			perror("read");
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
	// Длина находится в i
	if(i != 0)
	{
		// выводим найденную запись
		fprintf(stdout, "%s\n", buf);
	}
}
int main(int argc, char *argv[])
{
	int i;
	int j;
	int ans = 0;
	int err;
	int sock;
	int port;
	double t;
	struct sockaddr_in server_addr;
	struct hostent *hostinfo;
	char buf[LEN];
	char string[LEN] = "";
	char time[LEN] = "Elapsed = ";
	char elapsed[LEN] = "";
	if(!(argc == 3 && sscanf(argv[2], "%d", &port) == 1))
	{
		fprintf(stderr, "Usage: %s host port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	hostinfo = gethostbyname(argv[1]);
	if(hostinfo == NULL)
	{
		fprintf(stderr, "Unknown host %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *(struct in_addr *) hostinfo->h_addr;
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("Client: socket was not created");
		exit(EXIT_FAILURE);
	}
	err = connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(err < 0)
	{
		perror("Client: connect failure");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "Connection is ready\n");
	t = clock();
	while(fgets(buf, LEN, stdin))
	{
		strcat(string, buf);
		for(j = 0; string[j] != ';' && string[j]; j++);
		if(string[j])
		{
			do
			{
				j++;
				strncpy(buf, string, j);
				buf[j] = 0;
				writeToServer(sock, buf, j);
				for(; string[j] == ' ' || string[j] == '\t'|| string[j] == '\n'; j++);
				for(i = 0; string[j]; i++, j++)
				{
					string[i] = string[j];
				}
				string[i] = string[j];
				i = readFromServer(sock);
				if(i < 0)
				{
					switch(i)
					{
						case -1:
						close(sock);
						t = (clock() - t)/CLOCKS_PER_SEC;
						printf ("%s : Result = %d\n", argv[0], ans);
						sprintf (elapsed, "%.2f\n", t);
            			strcat(time, elapsed); 
						perror(time);
						exit(EXIT_SUCCESS);
						case -2:
						close(sock);
						t = (clock() - t)/CLOCKS_PER_SEC;
						printf ("%s : Result = %d\n", argv[0], ans);
						sprintf (elapsed, "%.2f\n", t);
            			strcat(time, elapsed); 
						perror(time);
						exit(EXIT_SUCCESS);
						case -3:
						close(sock);
						t = (clock() - t)/CLOCKS_PER_SEC;
						printf ("Not enough memory!\n%s : Result = %d Elapsed = %.2f\n", argv[0], ans, t);
						sprintf (elapsed, "%.2f\n", t);
            			strcat(time, elapsed); 
						perror(time);
						exit(EXIT_FAILURE);
						case -4:
						close(sock);
						t = (clock() - t)/CLOCKS_PER_SEC;
						printf ("Problems with balances in tree!\n%s : Result = %d Elapsed = %.2f\n", argv[0], ans, t);
						sprintf (elapsed, "%.2f\n", t);
            			strcat(time, elapsed); 
						perror(time);
						exit(EXIT_FAILURE);
						case -6:
						printf ("Wrong command: %s\n", buf);
						break;
						default:
						close(sock);
						t = (clock() - t)/CLOCKS_PER_SEC;
						printf ("Unknown error!\n%s : Result = %d Elapsed = %.2f\n", argv[0], ans, t);
						sprintf (elapsed, "%.2f\n", t);
            			strcat(time, elapsed); 
						perror(time);
						exit(EXIT_FAILURE);
					}
				}
				ans += i;
				for(j = 0; string[j] != ';' && string[j]; j++);
			}while(string[j]);
		}
	}
	t = (clock() - t)/CLOCKS_PER_SEC;
	printf ("%s : Result = %d\n", argv[0], ans);
	sprintf (elapsed, "%.2f\n", t);
    strcat(time, elapsed); 
	perror(time);
	close(sock);
	exit(EXIT_SUCCESS);
}