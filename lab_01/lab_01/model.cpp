#include "model.h"
#include <cstdbool>

model_t &init_model() {
    static model_t tmp;
    tmp.points_arr = nullptr;
    tmp.points_alen = 0;
    tmp.edges_arr = nullptr;
    tmp.edges_alen = 0;
    return tmp;
}

bool is_model_emty(model_t &model) {
    if (!model.points_arr || !model.edges_arr)
        return true;
    return false;
}

void free_model(model_t &model) {
    destroy_points(model.points_arr);
    destroy_edges(model.edges_arr);
}

error_t load_model(model_t &model, load_t &data) {
    error_t rc = SUCCESS;

    if (is_model_emty(model))
        free_model(model);

    FILE *f = fopen(data.filename, "r");
    if (!f)
        rc = FILENAME_ERROR;

    if (rc == SUCCESS)
        rc = scan_points_amount(f, model.points_alen);

    if (rc == SUCCESS) {
        model.points_arr = alloc_points(model.points_alen);
        if (!model.points_arr)
            rc = MEMORY_ALLOCATION_ERROR;
    }

    if (rc == SUCCESS)
        rc = scan_all_points(f, model.points_arr, model.points_alen);

    if (rc == SUCCESS)
        rc = scan_edges_amount(f, model.edges_alen);

    if (rc == SUCCESS) {
        model.edges_arr = alloc_edges(model.edges_alen);
        if (!model.edges_arr) {
            destroy_points(model.points_arr);
            rc = MEMORY_ALLOCATION_ERROR;
        }
    }

    if (rc == SUCCESS)
        rc = scan_all_edges(f, model.edges_arr, model.edges_alen, model.points_alen);

    fclose(f);
    return rc;
}

error_t save_model(model_t &model, save_t &data) {
    error_t rc = SUCCESS;

    if (is_model_emty(model))
        rc = NO_MODEL_ERROR;

    FILE *f = fopen(data.filename, "w");
    if (!f)
        rc = FILENAME_ERROR;

    if (rc == SUCCESS)
        rc = write_points_amount(f, model.points_alen);

    if (rc == SUCCESS)
        write_all_points(f, model.points_arr, model.points_alen);

    if (rc == SUCCESS)
        write_edges_amount(f, model.edges_alen);

    if (rc == SUCCESS)
        write_all_edges(f, model.edges_arr, model.edges_alen);

    fclose(f);

    return SUCCESS;
}

error_t move_model(model_t &model, move_t &data) {
    error_t rc = move_all_points(data, model.points_arr, model.points_alen);
    return rc;
}

error_t rotate_model(model_t &model, rotate_t &data) {
    error_t rc = rotate_all_points(data, model.points_arr, model.points_alen);
    return rc;
}

error_t scale_model(model_t &model, scale_t &data) {
    error_t rc = scale_all_points(data, model.points_arr, model.points_alen);
    return rc;
}
