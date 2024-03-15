#include "controller.h"

error_t action_handler(request_t &req) {
    static model_t model = init_model();
    error_t rc = SUCCESS;

    switch (req.action) {
    case LOAD:
        rc = load_model(model, req.load_data);
        break;
    case SAVE:
        rc = save_model(model, req.save_data);
        break;
    case MOVE:
        rc = move_model(model, req.move_data);
        break;
    case ROTATE:
        rc = rotate_model(model, req.rotate_data);
        break;
    case SCALE:
        rc = scale_model(model, req.scale_data);
        break;
    case RENDER:
        rc = render_model(req.render_data, model);
        break;
    default:
        break;
    }

    return rc;
}
