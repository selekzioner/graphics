#include "cubewidget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QColorDialog>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto* color_dialog = new QColorDialog;
    color_dialog->setOptions(QColorDialog::NoButtons);

    auto cube_widget = new CubeWidget();
    QObject::connect(color_dialog, &QColorDialog::currentColorChanged, cube_widget, &CubeWidget::setColor);

    auto* grid_slider = new QSlider;
    grid_slider->setRange(1, 10);
    QObject::connect(grid_slider, &QSlider::valueChanged, cube_widget, &CubeWidget::setGrid);

    auto* morph_slider = new QSlider;
    morph_slider->setRange(0, 10);
    QObject::connect(morph_slider, &QSlider::valueChanged, cube_widget, &CubeWidget::setMorph);

    auto layout = std::make_unique<QHBoxLayout>();
    layout->addWidget(cube_widget);
    layout->addWidget(color_dialog);
    layout->addWidget(grid_slider);
    layout->addWidget(morph_slider);

    QWidget window;
    window.setFixedSize(1300, 560);
    window.setLayout(layout.release());
    window.show();

    return app.exec();
}
