#ifndef IO_TXT_H
#define IO_TXT_H

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_STR_LEN 100

int fscan_str(FILE *f, char *str);

int fscan_int(FILE *f, int &num);

int fscan_uint(FILE *f, size_t &num);

#endif // IO_TXT_H
