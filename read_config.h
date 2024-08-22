#ifndef READ_CONFIG_H
#define READ_CONFIG_H
#include <stdio.h>
#include <string.h>
#include "record.h"
int cat_way(char *way, const char *s);
read_status read_config(const char *way, int &k, int &m);
#endif //READ_CONFIG_H