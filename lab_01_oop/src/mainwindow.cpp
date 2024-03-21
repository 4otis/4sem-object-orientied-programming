#include "QDebug"
#include "QElapsedTimer"
#include "QFileDialog"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

QGraphicsScene *scene;

float **points_coordinates;

void delay(int ms) {
    QElapsedTimer et;
    et.start();
    while (true) {
        qApp->processEvents();
        if (et.elapsed() > ms)
            break;
    }
}

error_t MainWindow::render() {
    request_t new_req;
    new_req.action = RENDER;
    new_req.render_data = {.scene = ui->graphicsView->scene(), .width = 1000, .height = 1000};

    return action_handler(new_req);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setStyleSheet("QGraphicsView {background-color: white}");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_MainWindow_destroyed() { delete scene; }

void MainWindow::on_btnLoad_clicked() {
    QString path = QFileDialog::getOpenFileName();

    request_t new_req;
    new_req.action = LOAD;
    new_req.load_data.filename = path.toUtf8().data();

    error_t rc = action_handler(new_req);
    if (rc)
        show_error(rc);
    else {
        rc = render();
        if (rc)
            show_error(rc);
    }
}

void MainWindow::on_btnSave_clicked() {
    QString path = QFileDialog::getSaveFileName();
    request_t new_req;

    new_req.action = SAVE;
    new_req.save_data.filename = path.toUtf8().data();

    error_t rc = action_handler(new_req);
    if (rc)
        show_error(rc);
    else {
        rc = render();
        if (rc)
            show_error(rc);
    }
}

void MainWindow::on_btnMove_clicked() {
    request_t new_req;

    new_req.action = MOVE;

    new_req.move_data.x = ui->dspMoveX->value();
    new_req.move_data.y = ui->dspMoveY->value();
    new_req.move_data.z = ui->dspMoveZ->value();

    error_t rc = action_handler(new_req);
    if (rc)
        show_error(rc);
    else {
        rc = render();
        if (rc)
            show_error(rc);
    }
}

void MainWindow::on_btnRotate_clicked() {
    request_t new_req;

    new_req.action = ROTATE;

    new_req.rotate_data.angle_OX_in_degrees = ui->dspRotX->value();
    new_req.rotate_data.angle_OY_in_degrees = ui->dspRotY->value();
    new_req.rotate_data.angle_OZ_in_degrees = ui->dspRotZ->value();

    new_req.rotate_data.center_p.x = ui->dspRotXcenter->value();
    new_req.rotate_data.center_p.y = ui->dspRotYcenter->value();
    new_req.rotate_data.center_p.z = ui->dspRotZcenter->value();

    do {
        error_t rc = action_handler(new_req);
        if (rc) {
            show_error(rc);
            break;
        } else {
            rc = render();
            if (rc) {
                show_error(rc);
                break;
            }
        }
        delay(ui->horizontalSlider->value());
    } while (ui->checkBoxAutoRotation->isChecked());
}

void MainWindow::on_btnScale_clicked() {
    request_t new_req;

    new_req.action = SCALE;

    new_req.scale_data.x_scale = ui->dspScaleX->value();
    new_req.scale_data.y_scale = ui->dspScaleY->value();
    new_req.scale_data.z_scale = ui->dspScaleZ->value();

    new_req.scale_data.center_p.x = ui->dspScaleXcenter->value();
    new_req.scale_data.center_p.y = ui->dspScaleYcenter->value();
    new_req.scale_data.center_p.z = ui->dspScaleZcenter->value();

    error_t rc = action_handler(new_req);
    if (rc)
        show_error(rc);
    else {
        rc = render();
        if (rc)
            show_error(rc);
    }
}
