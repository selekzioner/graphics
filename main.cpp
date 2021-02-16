#include "cubewindow.h"
#include <QApplication>
#include <QColorDialog>


QColor selectColor()
{
    QColor color;

    QColorDialog dialog;
    if (dialog.exec()) {
        color = dialog.selectedColor();
    }
    return color;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto color = selectColor();
    if (!color.isValid()){
        return -1;
    }

    QSurfaceFormat format;
    format.setSamples(16);

    CubeWindow window;
    window.setColor(color);
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
