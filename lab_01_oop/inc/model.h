#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include <errorcodes.h>

struct my_model {
    point_t *points_arr;
    size_t points_alen;

    edge_t *edges_arr;
    size_t edges_alen;
};
using model_t = struct my_model;

struct load {
    const char *filename;
};
using load_t = struct load;

struct save {
    const char *filename;
};
using save_t = struct save;

model_t &init_model();
error_t load_model(model_t &model, load_t &data);
error_t save_model(model_t &model, save_t &data);
error_t move_model(model_t &model, move_t &data);
error_t rotate_model(model_t &model, rotate_t &data);
error_t scale_model(model_t &model, scale_t &data);

#endif
