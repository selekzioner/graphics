#include "cubewidget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>
#include <QColorDialog>
#include <QLabel>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto* colorDialog = new QColorDialog;
    colorDialog->setOptions(QColorDialog::NoButtons);

    auto* label = new QLabel;
    label->setStyleSheet("QLabel { font-size : 30px}");
    label->setFixedSize(300, 50);

    auto* cubeWidget = new CubeWidget(*label);
    QObject::connect(colorDialog, &QColorDialog::currentColorChanged, cubeWidget, &CubeWidget::setColor);

    auto* gridSlider = new QSlider;
    gridSlider->setRange(1, 10);
    QObject::connect(gridSlider, &QSlider::valueChanged, cubeWidget, &CubeWidget::setGrid);

    auto* morphSlider = new QSlider;
    morphSlider->setRange(0, 10);
    QObject::connect(morphSlider, &QSlider::valueChanged, cubeWidget, &CubeWidget::setMorph);

    auto* numberOfCubesSlider = new QSlider;
    numberOfCubesSlider->setRange(1, 1000);
    QObject::connect(numberOfCubesSlider, &QSlider::valueChanged, cubeWidget, &CubeWidget::setNumberOfCubes);

    auto* VLayout = new QVBoxLayout;
    VLayout->addWidget(label);
    VLayout->addWidget(cubeWidget);

    auto* HLayout = new QHBoxLayout;
    HLayout->addLayout(VLayout);
    HLayout->addWidget(colorDialog);
    HLayout->addWidget(gridSlider);
    HLayout->addWidget(morphSlider);
    HLayout->addWidget(numberOfCubesSlider);

    QWidget window;
    window.setFixedSize(1300, 560);
    window.setLayout(HLayout);
    window.show();

    return app.exec();
}
