#include "MainWindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    a.setFont(QFont("Helvetica", 10));

    MainWindow workplace(nullptr);
    workplace.show();
    return a.exec();
}
