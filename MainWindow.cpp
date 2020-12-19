#include "MainWindow.h"

#include <iostream>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
    auto widget = new QWidget();
    auto mainLayout = new QVBoxLayout();
    auto hlayout = new QHBoxLayout();
    auto vlayout1 = new QVBoxLayout();
    auto vlayout2 = new QVBoxLayout();
    auto costsTable = new QTableWidget(5, 3);
    costsTable->setHorizontalHeaderLabels({"Week 1", "Week 2", "Week 3"});
    costsTable->setVerticalHeaderLabels({"1", "2", "3", "4", "5"});
    setTableSettings(costsTable);

    auto timingTable = new QTableWidget(5, 2);
    timingTable->setHorizontalHeaderLabels({"Early start", "Late start"});
    timingTable->setVerticalHeaderLabels({"1", "2", "3", "4", "5"});
    setTableSettings(timingTable, true);

    auto planTable = new QTableWidget(6, 8);
    planTable->horizontalHeader()->hide();
    planTable->verticalHeader()->hide();
    planTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    planTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    planTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    planTable->resizeRowsToContents();
    planTable->resizeColumnsToContents();

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(2);
    line->setStyleSheet(QString("background-color: #c0c0c0;"));

    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    line2->setLineWidth(2);
    line2->setStyleSheet(QString("background-color: #c0c0c0;"));

    auto resultValue = new QLabel();
    auto results = new QLabel();
    auto getMinMax = new QPushButton("Minimize maximum weekly");
    getMinMax->setFixedHeight(50);
    auto getMinHesitation = new QPushButton("Minimize hesitation");
    getMinHesitation->setFixedHeight(50);

    auto calculateResult = [=](bool hesitation){
        const QString &postfix = hesitation ? "hesitation:" : "maximum weekly:";
        results->setText("Results of minimize " + postfix);
        resultValue->setText("Objective function value: " + QString::number(mPlan.process(hesitation)));
        const auto &plan = mPlan.getPlan();
        const auto &weekyCost = mPlan.weeklyCost();
        for (size_t row = 0; row < plan.size(); row++) {
            for (size_t col = 0; col < weekyCost.size(); col++) {
                QTableWidgetItem *item = planTable->item(row, col);
                if(!item) {
                    item = new QTableWidgetItem();
                    planTable->setItem(row, col, item);
                    item->setTextAlignment(Qt::AlignCenter);
                }
                if (col - plan[row] < 3 && col - plan[row] >= 0) {
                    item->setText(costsTable->item(row, col - plan[row])->text());
                    item->setBackground(QColor("#6EC167"));
                } else {
                    item->setText("");
                    item->setBackground(QColor("#B4B1B1"));
                }
            }
        }
        for (size_t col = 0; col < weekyCost.size(); col++) {
            QTableWidgetItem *item = planTable->item(5, col);
            if(!item) {
                item = new QTableWidgetItem();
                planTable->setItem(5, col, item);
                item->setTextAlignment(Qt::AlignCenter);
                item->setBackground(QColor("#D0D0D0"));
            }
            item->setText(QString::number(weekyCost[col]));
        }
    };

    connect(getMinMax, &QPushButton::clicked, this, [=](){
        calculateResult(false);
    });
    connect(getMinHesitation, &QPushButton::clicked, this, [=](){
        calculateResult(true);
    });

    vlayout1->addWidget(costsTable);
    vlayout1->addWidget(getMinMax);
    vlayout2->addWidget(timingTable);
    vlayout2->addWidget(getMinHesitation);
    hlayout->addItem(vlayout1);
    hlayout->addItem(vlayout2);
    mainLayout->addWidget(new QLabel("Data:"));
    mainLayout->addWidget(line);
    mainLayout->addItem(hlayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(results);
    mainLayout->addWidget(line2);
    mainLayout->addWidget(resultValue);
    mainLayout->addWidget(planTable);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
    getMinMax->click();
}

void MainWindow::setTableSettings(QTableWidget *table, bool timing)
{
    table->horizontalHeader()->setStyleSheet("::section {margin: 1px;}");

    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (!timing) {
        for (const auto &cost: mPlan.getExpenses()) {
            int row = cost.first;
            for (size_t col = 0; col < cost.second.size(); col++) {
                QTableWidgetItem *item = table->item(row, col);
                if(!item) {
                    item = new QTableWidgetItem();
                    table->setItem(row, col, item);
                    item->setTextAlignment(Qt::AlignCenter);
                }
                item->setText(QString::number(cost.second.at(col)));
            }
        }
    } else {
        auto timeRange = mPlan.getTiming();
        for (size_t row = 0; row < timeRange.size(); row++) {
            for (size_t col = 0; col < 2; col++) {
                QTableWidgetItem *item = table->item(row, col);
                if(!item) {
                    item = new QTableWidgetItem();
                    table->setItem(row, col, item);
                    item->setTextAlignment(Qt::AlignCenter);
                }
                int num = col == 0 ? timeRange.at(row).first : timeRange.at(row).second;
                item->setText(QString::number(num));
            }
        }
    }
    table->setMaximumSize(tableWidgetSize(table));
    table->setMinimumSize(table->maximumSize());
    table->setAlternatingRowColors(true);
    connect(table, &QTableWidget::cellChanged, this, [=](int row, int col){
        QTableWidgetItem *item = table->item(row, col);
        if(!item) {
            return;
        }
        bool ok = true;
        int value = item->text().toInt(&ok);
        if (!ok) {
            table->blockSignals(true);
            item->setText("1");
            table->blockSignals(false);
            return;
        }
        if (!timing) {
            mPlan.setExpenses(row, col, value);
        } else {
            int previous = mPlan.setTiming(row, col, value);
            if (previous != -1) {
                table->blockSignals(true);
                item->setText(QString::number(previous));
                table->blockSignals(false);
            }
        }
    });
}

QSize MainWindow::tableWidgetSize(QTableWidget *t) {
   int w = t->verticalHeader()->width() + 4;
   for (int i = 0; i < t->columnCount(); i++) {
      w += t->columnWidth(i);
   }
   int h = t->horizontalHeader()->height() + 4;
   for (int i = 0; i < t->rowCount(); i++) {
      h += t->rowHeight(i);
   }
   return QSize(w, h);
}
