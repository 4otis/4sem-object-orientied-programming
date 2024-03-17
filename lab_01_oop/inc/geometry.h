#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdbool>
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
    point_t *arr;
    size_t alen;
};
using points_t = struct my_points;

struct edge {
    size_t p1_ind;
    size_t p2_ind;
};
using edge_t = struct edge;

struct my_edges {
    edge_t *arr;
    size_t alen;
};
using edges_t = struct my_edges;

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

point_t *alloc_points(size_t alen);
void init_points(points_t &points);
void destroy_points(points_t &points);
bool is_points_empty(points_t &points);

error_t scan_points_amount(size_t &amount, FILE *f);
error_t scan_all_points(points_t &points, FILE *f);

error_t read_points(points_t &points, FILE *f);
error_t write_points(points_t &points, FILE *f);

void copy_points(points_t &dst, points_t &src);

point_t get_point_by_ind(points_t &points, size_t ind);
size_t get_points_amount(points_t &points);

error_t move_all_points(points_t &points, move_t &params);
error_t rotate_all_points(points_t &points, rotate_t &params);
error_t scale_all_points(points_t &points, scale_t &params);

edge_t *alloc_edges(size_t alen);
void init_edges(edges_t &edges);
void destroy_edges(edges_t &edges);
bool is_edges_empty(edges_t &edges);

error_t read_edges(edges_t &edges, FILE *f);
error_t write_edges(edges_t &edges, FILE *f);

void copy_edges(edges_t &dst, edges_t &src);

size_t get_first_point_ind_by_ind(edges_t &edges, size_t ind);
size_t get_second_point_ind_by_ind(edges_t &edges, size_t ind);
size_t get_edges_amount(edges_t &edges);

#endif // GEOMETRY_H
