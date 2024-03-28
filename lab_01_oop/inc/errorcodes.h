#ifndef ERRORCODES_H
#define ERRORCODES_H

enum errors {
    SUCCESS,
    MEMORY_ALLOCATION_TMP_MODEL_ERROR,
    MEMORY_ALLOCATION_ERROR,
    FILENAME_ERROR,
    WRONG_DATA_ERROR,
    NO_MODEL_ERROR,
    DATA_SAVING_ERROR,
    INVALID_REQUEST_ERROR
};
using error_t = errors;

void show_error(error_t error_code);

#endif // ERRORCODES_H
