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

struct my_points {
    point_t *points_arr;
    size_t points_alen;
};
using points_t = struct my_points;

struct my_edges {
    edge_t *edges_arr;
    size_t edges_alen;
};
using edges_t = struct my_edges;

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

error_t scan_points_amount(size_t &amount, FILE *f);
error_t scan_all_points(point_t *arr, size_t &alen, FILE *f);

error_t read_points(points_t &points, FILE *f);
error_t write_points(points_t &points, FILE *f);

error_t move_all_points(point_t *arr, size_t &alen, move_t &params);
error_t rotate_all_points(point_t *arr, size_t &alen, rotate_t &params);
error_t scale_all_points(point_t *arr, size_t &alen, scale_t &params);

edge_t *alloc_edges(size_t &alen);
void destroy_edges(edge_t *arr);

error_t read_edges(edges_t &edges, FILE *f);
error_t write_edges(edges_t &edges, FILE *f);

#endif // GEOMETRY_H
