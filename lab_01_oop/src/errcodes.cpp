#include "errorcodes.h"

void show_error(error_t error_code) {
    switch (error_code) {
    case MEMORY_ALLOCATION_TMP_MODEL_ERROR:
        QMessageBox::critical(NULL, "Ошибка", "Не удалось выделить память под новую модель. Старая модель не была изменена.");
        break;
    case MEMORY_ALLOCATION_ERROR:
        QMessageBox::critical(NULL, "Ошибка", "Не удалось выделить память.");
        break;
    case FILENAME_ERROR:
        QMessageBox::critical(NULL, "Ошибка", "По указанному пути не удалось открыть текстовый файл.");
        break;
    case WRONG_DATA_ERROR:
        QMessageBox::critical(NULL, "Ошибка", "Некорректный формат данных, описывающих 3D модель.");
        break;
    case NO_MODEL_ERROR:
        QMessageBox::critical(NULL, "Ошибка", "Необходимо сначала загрузить 3D модель.");
        break;
    case INVALID_REQUEST_ERROR:
        QMessageBox::critical(NULL, "Ошибка", "Некорректный запрос.");
        break;
    default:
        QMessageBox::critical(NULL, "Ошибка", "Smth went wrong...");
    }
}
