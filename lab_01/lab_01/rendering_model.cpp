#include "rendering_model.h"

error_t render_model(model_t &model, render_t &params) {
    if (!model.edges_arr)
        return NO_MODEL_ERROR;

    clear_scene(params);
    render_all_edges(params, model);

    return SUCCESS;
}

void render_all_edges(render_t &params, model_t &model) {
    for (size_t i = 0; i < model.edges_alen; i++) {
        point_t p1 = model.points_arr[model.edges_arr[i].p1_ind];
        point_t p2 = model.points_arr[model.edges_arr[i].p2_ind];
        if (p1.x != p2.x || p1.y != p2.y)
            render_edge(params, p1, p2);
    }
}

void render_edge(render_t &params, point_t &p1, point_t &p2) { params.scene->addLine(p1.x, p1.y, p2.x, p2.y); }

void clear_scene(render_t &params) { params.scene->clear(); }
