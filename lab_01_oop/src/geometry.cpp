#include "geometry.h"
#include <QDebug>
#include <cmath>

#define EPS 10e-8;

point_t *alloc_points(size_t alen) {
    point_t *tmp = (point_t *)malloc(alen * sizeof(point_t));
    if (!tmp)
        return nullptr;
    return tmp;
}

void init_points(points_t &points) {
    points.arr = nullptr;
    points.alen = 0;
}

void destroy_points(points_t &points) {
    if (points.arr)
        free(points.arr);
}

bool is_points_empty(points_t &points) {
    if (points.arr)
        return false;
    return true;
}

error_t scan_points_amount(size_t &amount, FILE *f) {
    if (fscan_uint(f, amount))
        return WRONG_DATA_ERROR;
    return SUCCESS;
}

int scan_point(point_t &p, FILE *f) {
    char coords_str[MAX_STR_LEN + 2];

    if (!fscan_str(f, coords_str))
        if (sscanf(coords_str, "%lf,%lf,%lf", &p.x, &p.y, &p.z) == 3)
            return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

error_t scan_all_points(points_t &points, FILE *f) {
    for (size_t i = 0; i < points.alen; i++)
        if (scan_point(points.arr[i], f))
            return WRONG_DATA_ERROR;
    return SUCCESS;
}

error_t read_points(points_t &points, FILE *f) {
    error_t rc = SUCCESS;

    rc = scan_points_amount(points.alen, f);

    if (rc == SUCCESS) {
        points.arr = alloc_points(points.alen);
        if (!points.arr)
            rc = MEMORY_ALLOCATION_ERROR;
        else
            rc = scan_all_points(points, f);
    }

    return rc;
}

error_t write_points_amount(size_t &amount, FILE *f) {
    if (fprintf(f, "%zu\n", amount) < 0)
        return DATA_SAVING_ERROR;
    return SUCCESS;
}

int write_point(point_t &p, FILE *f) {
    if (fprintf(f, "%lf,%lf,%lf\n", p.x, p.y, p.z) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

error_t write_all_points(points_t &points, FILE *f) {
    for (size_t i = 0; i < points.alen; i++)
        if (write_point(points.arr[i], f))
            return DATA_SAVING_ERROR;
    return SUCCESS;
}

error_t write_points(points_t &points, FILE *f) {
    error_t rc = write_points_amount(points.alen, f);

    if (rc == SUCCESS)
        rc = write_all_points(points, f);

    return rc;
}

void copy_points(points_t &dst, points_t &src) {
    dst.alen = src.alen;

    for (size_t i = 0; i < src.alen; i++)
        dst.arr[i] = src.arr[i];
}

point_t get_point_by_ind(points_t &points, size_t ind) { return points.arr[ind]; }

size_t get_points_amount(points_t &points) { return points.alen; }

void move_point(point_t &p, move_t &params) {
    p.x = p.x + params.x;
    p.y = p.y + params.y;
    p.z = p.z + params.z;
}

error_t move_all_points(points_t &points, move_t &params) {
    if (!points.arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < points.alen; i++)
        move_point(points.arr[i], params);

    return SUCCESS;
}

double degrees_to_radians(double &degrees) { return degrees * M_PI / 180; }

void rotate_OX_point(point_t &p, double angle) {
    double tmp_z = p.z * cos(angle) - p.y * sin(angle);
    double tmp_y = p.z * sin(angle) + p.y * cos(angle);

    p.z = tmp_z;
    p.y = tmp_y;
}

void rotate_OY_point(point_t &p, double angle) {
    double tmp_x = p.x * cos(angle) - p.z * sin(angle);
    double tmp_z = p.x * sin(angle) + p.z * cos(angle);

    p.x = tmp_x;
    p.z = tmp_z;
}

void rotate_OZ_point(point_t &p, double angle) {
    double tmp_x = p.x * cos(angle) - p.y * sin(angle);
    double tmp_y = p.x * sin(angle) + p.y * cos(angle);

    p.x = tmp_x;
    p.y = tmp_y;
}

void move_point_to_zero(point_t &p, point_t &center_point) {
    move_t default_transformation;
    default_transformation.x = -center_point.x;
    default_transformation.y = -center_point.y;
    default_transformation.z = -center_point.z;

    move_point(p, default_transformation);
}

void move_point_from_zero(point_t &p, point_t &center_point) {
    move_t default_transformation;
    default_transformation.x = center_point.x;
    default_transformation.y = center_point.y;
    default_transformation.z = center_point.z;

    move_point(p, default_transformation);
}

void rotate_point(point_t &p, rotate_t &params) {
    move_point_to_zero(p, params.center_p);

    double angle_OX_in_radians = degrees_to_radians(params.angle_OX_in_degrees);
    double angle_OY_in_radians = degrees_to_radians(params.angle_OY_in_degrees);
    double angle_OZ_in_radians = degrees_to_radians(params.angle_OZ_in_degrees);

    rotate_OX_point(p, angle_OX_in_radians);
    rotate_OY_point(p, angle_OY_in_radians);
    rotate_OZ_point(p, angle_OZ_in_radians);

    move_point_from_zero(p, params.center_p);
}

error_t rotate_all_points(points_t &points, rotate_t &params) {
    if (!points.arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < points.alen; i++)
        rotate_point(points.arr[i], params);

    return SUCCESS;
}

void scale_point(point_t &p, scale_t &params) {
    p.x = (p.x - params.center_p.x) * params.x_scale + params.center_p.x;
    p.y = (p.y - params.center_p.y) * params.y_scale + params.center_p.y;
    p.z = (p.z - params.center_p.z) * params.z_scale + params.center_p.z;
}

error_t scale_all_points(points_t &points, scale_t &params) {
    if (!points.arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < points.alen; i++)
        scale_point(points.arr[i], params);

    return SUCCESS;
}

edge_t *alloc_edges(size_t alen) {
    edge_t *tmp = (edge_t *)malloc(alen * sizeof(edge_t));
    if (!tmp)
        return nullptr;
    return tmp;
}

void init_edges(edges_t &edges) {
    edges.arr = nullptr;
    edges.alen = 0;
}

void destroy_edges(edges_t &edges) {
    if (edges.arr)
        free(edges.arr);
}

bool is_edges_empty(edges_t &edges) {
    if (edges.arr)
        return false;
    return true;
}

error_t scan_edges_amount(size_t &amount, FILE *f) {
    if (fscan_uint(f, amount))
        return WRONG_DATA_ERROR;
    return SUCCESS;
}

int scan_edge(edge_t &e, FILE *f) {
    char points_indecies_str[MAX_STR_LEN + 2];

    if (!fscan_str(f, points_indecies_str))
        if (sscanf(points_indecies_str, "%zu %zu", &e.p1_ind, &e.p2_ind) == 2)
            return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

error_t scan_all_edges(edges_t &edges, FILE *f) {
    for (size_t i = 0; i < edges.alen; i++)
        if (scan_edge(edges.arr[i], f))
            return WRONG_DATA_ERROR;
    return SUCCESS;
}

error_t read_edges(edges_t &edges, FILE *f) {
    error_t rc = scan_edges_amount(edges.alen, f);

    if (rc == SUCCESS) {
        edges.arr = alloc_edges(edges.alen);
        if (!edges.arr)
            rc = MEMORY_ALLOCATION_ERROR;
        else
            rc = scan_all_edges(edges, f);
    }

    return rc;
}

error_t write_edges_amount(size_t &amount, FILE *f) {
    if (fprintf(f, "%zu\n", amount) < 0)
        return DATA_SAVING_ERROR;
    return SUCCESS;
}

int write_edge(edge_t &e, FILE *f) {
    if (fprintf(f, "%zu %zu\n", e.p1_ind, e.p2_ind) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

error_t write_all_edges(edges_t &edges, FILE *f) {
    for (size_t i = 0; i < edges.alen; i++)
        if (write_edge(edges.arr[i], f))
            return DATA_SAVING_ERROR;
    return SUCCESS;
}

error_t write_edges(edges_t &edges, FILE *f) {
    error_t rc = write_edges_amount(edges.alen, f);

    if (rc == SUCCESS)
        rc = write_all_edges(edges, f);

    return rc;
}

void copy_edges(edges_t &dst, edges_t &src) {
    dst.alen = src.alen;

    for (size_t i = 0; i < src.alen; i++)
        dst.arr[i] = src.arr[i];
}

size_t get_first_point_ind_by_ind(edges_t &edges, size_t ind) { return edges.arr[ind].p1_ind; }

size_t get_second_point_ind_by_ind(edges_t &edges, size_t ind) { return edges.arr[ind].p2_ind; }

size_t get_edges_amount(edges_t &edges) { return edges.alen; }
