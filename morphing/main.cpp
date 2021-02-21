#include "mainwidget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QSlider>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto* color_dialog = new QColorDialog;
    color_dialog->setOptions(QColorDialog::NoButtons);

    auto cube_widget = std::make_unique<MainWidget>(*color_dialog);

    auto* slider = new QSlider;

    auto layout = std::make_unique<QHBoxLayout>();
    layout->addWidget(cube_widget.release());
    layout->addWidget(color_dialog);
    layout->addWidget(slider);

    QWidget window;
    window.setFixedSize(1200, 480);
    window.setLayout(layout.release());
    window.show();

    return app.exec();
}
