#include "io_txt.h"

int fscan_str(FILE *f, char *str) {
    if (fgets(str, MAX_STR_LEN + 2, f) == NULL)
        return EXIT_FAILURE;

    size_t str_len = strlen(str);
    if (str[str_len - 1] == '\n')
        str[--str_len] = '\0';

    if (str_len > MAX_STR_LEN || !str)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int fscan_int(FILE *f, int &num) {
    char str[MAX_STR_LEN + 2] = {"\0"};

    fscan_str(f, str);

    char *pend;
    long int tmp = strtol(str, &pend, 10);
    if (tmp >= INT_MAX || tmp <= INT_MIN || (tmp == 0 && pend == str))
        return EXIT_FAILURE;

    num = tmp;

    return EXIT_SUCCESS;
}

int fscan_uint(FILE *f, size_t &num) {
    char str[MAX_STR_LEN] = {"\0"};

    fscan_str(f, str);

    char *pend;
    long int tmp = strtol(str, &pend, 10);
    if (tmp >= INT_MAX || tmp <= INT_MIN || (tmp == 0 && pend == str) || tmp < 0)
        return EXIT_FAILURE;

    num = tmp;

    return EXIT_SUCCESS;
}
