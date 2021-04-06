#pragma once
#include "ui_MainWindow.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass _ui;
};
