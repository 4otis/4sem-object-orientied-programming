#ifndef RENDERING_MODEL_H
#define RENDERING_MODEL_H

#include <QGraphicsScene>

#include "geometry.h"
#include "model.h"

struct render {
    QGraphicsScene *scene;
    size_t width;
    size_t height;
};
using render_t = struct render;

error_t render_model(render_t &params, model_t &model);
void render_all_edges(render_t &params, model_t &model);
void render_edge(render_t &params, point_t &p1, point_t &p2);
void clear_scene(render_t &params);

#endif // RENDERING_MODEL_H
