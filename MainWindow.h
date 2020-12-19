#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RepairPlan.h"

class QTableWidget;

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent);

private:
    void setTableSettings(QTableWidget *table, bool timing = false);
    static QSize tableWidgetSize(QTableWidget *t);

    RepairPlan mPlan;
};

#endif // MAINWINDOW_H
