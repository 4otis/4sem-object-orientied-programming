#include "rendering_model.h"

error_t render_model(render_t &params, model_t &model) {
    if (is_edges_empty(model.edges))
        return NO_MODEL_ERROR;

    clear_scene(params);
    render_all_edges(params, model);

    return SUCCESS;
}

void render_all_edges(render_t &params, model_t &model) {
    size_t p1_ind, p2_ind;
    point_t p1, p2;

    for (size_t i = 0; i < model.edges.alen; i++) {
        p1_ind = get_first_point_ind_by_ind(model.edges, i);
        p2_ind = get_second_point_ind_by_ind(model.edges, i);

        p1 = get_point_by_ind(model.points, p1_ind);
        p2 = get_point_by_ind(model.points, p2_ind);

        if (p1.x != p2.x || p1.y != p2.y)
            render_edge(params, p1, p2);
    }
}

void render_edge(render_t &params, point_t &p1, point_t &p2) { params.scene->addLine(p1.x, p1.y, p2.x, p2.y); }

void clear_scene(render_t &params) { params.scene->clear(); }
