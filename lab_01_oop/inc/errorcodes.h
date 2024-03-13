#ifndef ERRORCODES_H
#define ERRORCODES_H

#include <QDebug>
#include <QMessageBox>

enum errors { SUCCESS, MEMORY_ALLOCATION_ERROR, FILENAME_ERROR, WRONG_DATA_ERROR, NO_MODEL_ERROR, DATA_SAVING_ERROR };
using error_t = errors;

void show_error(error_t error_code);

#endif // ERRORCODES_H