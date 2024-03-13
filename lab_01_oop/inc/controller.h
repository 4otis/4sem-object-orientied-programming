#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "errorcodes.h"
#include "model.h"
#include "rendering_model.h"

enum action { LOAD, SAVE, MOVE, ROTATE, SCALE, RENDER };
using action_t = enum action;

struct request {
    action_t action;
    union {
        load_t load_data;
        save_t save_data;
        move_t move_data;
        rotate_t rotate_data;
        scale_t scale_data;
        render_t render_data;
    };
};
using request_t = struct request;

error_t action_handler(request_t &req);

#endif
