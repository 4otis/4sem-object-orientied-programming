#include "model.h"
#include <cstdbool>

model_t init_model() {
    model_t tmp;

    init_points(tmp.points);
    init_edges(tmp.edges);

    return tmp;
}

bool is_model_emty(model_t &model) {
    if (is_points_empty(model.points) && is_edges_empty(model.edges))
        return true;
    return false;
}

void free_model(model_t &model) {
    destroy_points(model.points);
    destroy_edges(model.edges);
}

error_t copy_model(model_t &dst, model_t &src) {
    error_t rc = SUCCESS;

    points_t tmp_points;
    set_points_arr(tmp_points, alloc_points(get_points_amount(src.points)));

    edges_t tmp_edges;
    set_edges_arr(tmp_edges, alloc_edges(get_edges_amount(src.edges)));

    if (is_points_empty(dst.points) || is_edges_empty(dst.edges)) {
        rc = MEMORY_ALLOCATION_ERROR;
        destroy_points(tmp_points);
        destroy_edges(tmp_edges);
    } else {
        free_model(dst);

        dst.points = tmp_points;
        dst.edges = tmp_edges;

        copy_points(dst.points, src.points);
        copy_edges(dst.edges, src.edges);
    }

    return rc;
}

error_t read_model(model_t &model, FILE *f) {
    error_t rc = read_points(model.points, f);
    if (rc == SUCCESS) {
        rc = read_edges(model.edges, f);
        if (rc != SUCCESS)
            free_model(model);
    }
    return rc;
}

error_t load_model(model_t &model, load_t &data) {
    error_t rc = SUCCESS;

    FILE *f = fopen(data.filename, "r");
    if (!f)
        rc = FILENAME_ERROR;
    else {
        model_t tmp_model = init_model();

        rc = read_model(tmp_model, f);

        fclose(f);

        if (rc == SUCCESS) {
            free_model(model);
            rc = copy_model(model, tmp_model);
            free_model(tmp_model);
        }
    }
    return rc;
}

error_t save_model(model_t &model, save_t &data) {
    error_t rc = SUCCESS;

    if (is_model_emty(model))
        rc = NO_MODEL_ERROR;

    FILE *f = fopen(data.filename, "w");
    if (!f)
        rc = FILENAME_ERROR;
    else {
        rc = write_points(model.points, f);

        if (rc == SUCCESS)
            rc = write_edges(model.edges, f);

        fclose(f);
    }

    return SUCCESS;
}

error_t move_model(model_t &model, move_t &data) {
    error_t rc = move_all_points(model.points, data);
    return rc;
}

error_t rotate_model(model_t &model, rotate_t &data) {
    error_t rc = rotate_all_points(model.points, data);
    return rc;
}

error_t scale_model(model_t &model, scale_t &data) {
    error_t rc = scale_all_points(model.points, data);
    return rc;
}
