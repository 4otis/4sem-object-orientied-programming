#include "geometry.h"
#include <QDebug>
#include <cmath>

#define EPS 10e-8;

point_t *alloc_points(size_t &alen) {
    point_t *tmp = (point_t *)malloc(alen * sizeof(point_t));
    if (!tmp)
        return nullptr;
    return tmp;
}

void destroy_points(point_t *arr) { free(arr); }

error_t scan_points_amount(FILE *f, size_t &amount) {
    if (fscan_uint(f, amount))
        return WRONG_DATA_ERROR;
    return SUCCESS;
}

int scan_point(FILE *f, point_t &p) {
    char coords_str[MAX_STR_LEN + 2];

    if (!fscan_str(f, coords_str))
        if (sscanf(coords_str, "%lf,%lf,%lf", &p.x, &p.y, &p.z) == 3)
            return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

error_t scan_all_points(FILE *f, point_t *arr, size_t &alen) {
    for (size_t i = 0; i < alen; i++)
        if (scan_point(f, arr[i]))
            return WRONG_DATA_ERROR;
    return SUCCESS;
}

error_t write_points_amount(FILE *f, size_t &amount) {
    qDebug() << "step1";
    if (fprintf(f, "%zu\n", amount) < 0)
        return DATA_SAVING_ERROR;
    return SUCCESS;
}

int write_point(FILE *f, point_t &p) {
    if (fprintf(f, "%lf,%lf,%lf\n", p.x, p.y, p.z) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

error_t write_all_points(FILE *f, point_t *arr, size_t &alen) {
    qDebug() << "step2";
    for (size_t i = 0; i < alen; i++)
        if (write_point(f, arr[i]))
            return DATA_SAVING_ERROR;
    return SUCCESS;
}

void move_point(move_t &params, point_t &p) {
    p.x = p.x + params.x;
    p.y = p.y + params.y;
    p.z = p.z + params.z;
}

error_t move_all_points(move_t &params, point_t *arr, size_t &alen) {
    if (!arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < alen; i++)
        move_point(params, arr[i]);

    return SUCCESS;
}

double degrees_to_radians(double &degrees) { return degrees * M_PI / 180; }

void rotate_OX_point(double angle, point_t &p) {
    double tmp_z = p.z * cos(angle) - p.y * sin(angle);
    double tmp_y = p.z * sin(angle) + p.y * cos(angle);

    p.z = tmp_z;
    p.y = tmp_y;
}

void rotate_OY_point(double angle, point_t &p) {
    double tmp_x = p.x * cos(angle) - p.z * sin(angle);
    double tmp_z = p.x * sin(angle) + p.z * cos(angle);

    p.x = tmp_x;
    p.z = tmp_z;
}

void rotate_OZ_point(double angle, point_t &p) {
    double tmp_x = p.x * cos(angle) - p.y * sin(angle);
    double tmp_y = p.x * sin(angle) + p.y * cos(angle);

    p.x = tmp_x;
    p.y = tmp_y;
}

void rotate_point(rotate_t &params, point_t &p) {
    move_t default_transformation;
    default_transformation.x = -params.center_p.x;
    default_transformation.y = -params.center_p.y;
    default_transformation.z = -params.center_p.z;

    move_point(default_transformation, p);

    double angle_OX_in_radians = degrees_to_radians(params.angle_OX_in_degrees);
    double angle_OY_in_radians = degrees_to_radians(params.angle_OY_in_degrees);
    double angle_OZ_in_radians = degrees_to_radians(params.angle_OZ_in_degrees);

    rotate_OX_point(angle_OX_in_radians, p);
    rotate_OY_point(angle_OY_in_radians, p);
    rotate_OZ_point(angle_OZ_in_radians, p);

    default_transformation.x = -default_transformation.x;
    default_transformation.y = -default_transformation.y;
    default_transformation.z = -default_transformation.z;

    move_point(default_transformation, p);
}

error_t rotate_all_points(rotate_t &params, point_t *arr, size_t &alen) {
    if (!arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < alen; i++)
        rotate_point(params, arr[i]);

    return SUCCESS;
}

void scale_point(scale_t &params, point_t &p) {
    p.x = (p.x - params.center_p.x) * params.x_scale + params.center_p.x;
    p.y = (p.y - params.center_p.y) * params.y_scale + params.center_p.y;
    p.z = (p.z - params.center_p.z) * params.z_scale + params.center_p.z;
}

error_t scale_all_points(scale_t &params, point_t *arr, size_t &alen) {
    if (!arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < alen; i++)
        scale_point(params, arr[i]);

    return SUCCESS;
}

edge_t *alloc_edges(size_t &alen) {
    edge_t *tmp = (edge_t *)malloc(alen * sizeof(edge_t));
    if (!tmp)
        return nullptr;
    return tmp;
}

void destroy_edges(edge_t *arr) { free(arr); }

error_t scan_edges_amount(FILE *f, size_t &amount) {
    if (fscan_uint(f, amount))
        return WRONG_DATA_ERROR;
    return SUCCESS;
}

int scan_edge(FILE *f, edge_t &e, size_t &points_amount) {
    char points_indecies_str[MAX_STR_LEN + 2];

    if (!fscan_str(f, points_indecies_str))
        if (sscanf(points_indecies_str, "%zu %zu", &e.p1_ind, &e.p2_ind) == 2 && e.p1_ind < points_amount && e.p2_ind < points_amount)
            return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

error_t scan_all_edges(FILE *f, edge_t *arr, size_t &alen, size_t &points_amount) {
    for (size_t i = 0; i < alen; i++)
        if (scan_edge(f, arr[i], points_amount))
            return WRONG_DATA_ERROR;
    return SUCCESS;
}

error_t write_edges_amount(FILE *f, size_t &amount) {
    qDebug() << "step3";
    if (fprintf(f, "%zu\n", amount) < 0)
        return DATA_SAVING_ERROR;
    return SUCCESS;
}

int write_edge(FILE *f, edge_t &e) {
    if (fprintf(f, "%zu %zu\n", e.p1_ind, e.p2_ind) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

error_t write_all_edges(FILE *f, edge_t *arr, size_t &alen) {
    qDebug() << "step4";
    for (size_t i = 0; i < alen; i++)
        if (write_edge(f, arr[i]))
            return DATA_SAVING_ERROR;
    return SUCCESS;
}
