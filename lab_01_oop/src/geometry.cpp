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

error_t scan_all_points(point_t *arr, size_t &alen, FILE *f) {
    for (size_t i = 0; i < alen; i++)
        if (scan_point(arr[i], f))
            return WRONG_DATA_ERROR;
    return SUCCESS;
}

error_t read_points(points_t &points, FILE *f) {
    error_t rc = SUCCESS;

    rc = scan_points_amount(model.points_alen, f);

    if (rc == SUCCESS) {
        model.points_arr = alloc_points(model.points_alen);
        if (!model.points_arr)
            rc = MEMORY_ALLOCATION_ERROR;
    }

    if (rc == SUCCESS)
        rc = scan_all_points(model.points_arr, model.points_alen, f);
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

error_t write_all_points(point_t *arr, size_t &alen, FILE *f) {
    for (size_t i = 0; i < alen; i++)
        if (write_point(arr[i], f))
            return DATA_SAVING_ERROR;
    return SUCCESS;
}

error_t write_points(points_t &points, FILE *f);

void move_point(point_t &p, move_t &params) {
    p.x = p.x + params.x;
    p.y = p.y + params.y;
    p.z = p.z + params.z;
}

error_t move_all_points(point_t *arr, size_t &alen, move_t &params) {
    if (!arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < alen; i++)
        move_point(arr[i], params);

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

void rotate_point(point_t &p, rotate_t &params) {
    move_t default_transformation;
    default_transformation.x = -params.center_p.x;
    default_transformation.y = -params.center_p.y;
    default_transformation.z = -params.center_p.z;

    move_point(p, default_transformation);

    double angle_OX_in_radians = degrees_to_radians(params.angle_OX_in_degrees);
    double angle_OY_in_radians = degrees_to_radians(params.angle_OY_in_degrees);
    double angle_OZ_in_radians = degrees_to_radians(params.angle_OZ_in_degrees);

    rotate_OX_point(p, angle_OX_in_radians);
    rotate_OY_point(p, angle_OY_in_radians);
    rotate_OZ_point(p, angle_OZ_in_radians);

    default_transformation.x = -default_transformation.x;
    default_transformation.y = -default_transformation.y;
    default_transformation.z = -default_transformation.z;

    move_point(p, default_transformation);
}

error_t rotate_all_points(point_t *arr, size_t &alen, rotate_t &params) {
    if (!arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < alen; i++)
        rotate_point(arr[i], params);

    return SUCCESS;
}

void scale_point(point_t &p, scale_t &params) {
    p.x = (p.x - params.center_p.x) * params.x_scale + params.center_p.x;
    p.y = (p.y - params.center_p.y) * params.y_scale + params.center_p.y;
    p.z = (p.z - params.center_p.z) * params.z_scale + params.center_p.z;
}

error_t scale_all_points(point_t *arr, size_t &alen, scale_t &params) {
    if (!arr)
        return NO_MODEL_ERROR;

    for (size_t i = 0; i < alen; i++)
        scale_point(arr[i], params);

    return SUCCESS;
}

edge_t *alloc_edges(size_t &alen) {
    edge_t *tmp = (edge_t *)malloc(alen * sizeof(edge_t));
    if (!tmp)
        return nullptr;
    return tmp;
}

void destroy_edges(edge_t *arr) { free(arr); }

error_t scan_edges_amount(size_t &amount, FILE *f) {
    if (fscan_uint(f, amount))
        return WRONG_DATA_ERROR;
    return SUCCESS;
}

int scan_edge(edge_t &e, size_t &points_amount, FILE *f) {
    char points_indecies_str[MAX_STR_LEN + 2];

    if (!fscan_str(f, points_indecies_str))
        if (sscanf(points_indecies_str, "%zu %zu", &e.p1_ind, &e.p2_ind) == 2 && e.p1_ind < points_amount && e.p2_ind < points_amount)
            return EXIT_SUCCESS;

    return EXIT_FAILURE;
}

error_t scan_all_edges(edge_t *arr, size_t &alen, size_t &points_amount, FILE *f) {
    for (size_t i = 0; i < alen; i++)
        if (scan_edge(arr[i], points_amount, f))
            return WRONG_DATA_ERROR;
    return SUCCESS;
}

error_t read_edges(model_t &model, FILE *f) {
    rc = scan_edges_amount(model.edges_alen, f);

    if (rc == SUCCESS) {
        model.edges_arr = alloc_edges(model.edges_alen);
        if (!model.edges_arr) {
            destroy_points(model.points_arr);
            rc = MEMORY_ALLOCATION_ERROR;
        }
    }

    if (rc == SUCCESS)
        rc = scan_all_edges(model.edges_arr, model.edges_alen, model.points_alen, f);
}

error_t write_edges_amount(size_t &amount, FILE *f) {
    qDebug() << "step3";
    if (fprintf(f, "%zu\n", amount) < 0)
        return DATA_SAVING_ERROR;
    return SUCCESS;
}

int write_edge(edge_t &e, FILE *f) {
    if (fprintf(f, "%zu %zu\n", e.p1_ind, e.p2_ind) < 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

error_t write_all_edges(edge_t *arr, size_t &alen, FILE *f) {
    qDebug() << "step4";
    for (size_t i = 0; i < alen; i++)
        if (write_edge(arr[i], f))
            return DATA_SAVING_ERROR;
    return SUCCESS;
}
