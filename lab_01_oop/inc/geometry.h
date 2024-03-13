#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdio>
#include <cstdlib>
#include <errorcodes.h>
#include <stdlib.h>

#include "io_txt.h"

struct point {
    double x;
    double y;
    double z;
};
using point_t = struct point;

struct edge {
    size_t p1_ind;
    size_t p2_ind;
};
using edge_t = struct edge;

struct move {
    double x;
    double y;
    double z;
};
using move_t = struct move;

struct rotate {
    double angle_OX_in_degrees;
    double angle_OY_in_degrees;
    double angle_OZ_in_degrees;
    point_t center_p;
};
using rotate_t = struct rotate;

struct scale {
    double x_scale;
    double y_scale;
    double z_scale;
    point_t center_p;
};
using scale_t = struct scale;

point_t *alloc_points(size_t &alen);
void destroy_points(point_t *arr);

error_t scan_points_amount(FILE *f, size_t &amount);
error_t scan_all_points(FILE *f, point_t *arr, size_t &alen);
error_t write_points_amount(FILE *f, size_t &amount);
error_t write_all_points(FILE *f, point_t *arr, size_t &alen);

error_t move_all_points(move_t &params, point_t *arr, size_t &alen);
error_t rotate_all_points(rotate_t &params, point_t *arr, size_t &alen);
error_t scale_all_points(scale_t &params, point_t *arr, size_t &alen);

edge_t *alloc_edges(size_t &alen);
void destroy_edges(edge_t *arr);

error_t scan_edges_amount(FILE *f, size_t &amount);
error_t scan_all_edges(FILE *f, edge_t *arr, size_t &alen, size_t &points_amount);
error_t write_edges_amount(FILE *f, size_t &amount);
error_t write_all_edges(FILE *f, edge_t *arr, size_t &alen);

#endif // GEOMETRY_H
