#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QComboBox>
#include "workdb.h"
#include "tour.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_3_clicked();

    void on_PB_AddExcursion_clicked();

    void on_PB_AddTransport_clicked();

    void on_PBAddTypeTr_clicked();

    void on_PB_AddTour_clicked();

    void tV_excursion_fill();

    void tV_typeTr_fill();

    void tV_vehicle_fill();

    void CB_fill(QComboBox *combo, QString id, QString name, QString table);

    void on_CB_transport_activated(int index);

    void on_PB_CalcTour_clicked();

    void on_tV_excursion_clicked(const QModelIndex &index);

    void on_PB_ShowAllTour_clicked();

    void showTourForTransport(int type, bool flag);

    void on_CB_availableTransport_activated(int index);

    void on_PB_AddTrVehicle_clicked();

    void on_PB_DeleteTour_clicked();

    void on_PB_DeleteTr_clicked();

    void on_PB_DeleteTypeTr_clicked();

    void on_PB_save_clicked();

private:
    Ui::Widget *ui;
    WorkDB *myWorkDB;
    QSqlDatabase db;
    QStandardItemModel *model;
    QStandardItemModel *modelTour;
    QStandardItemModel *modelVehicle;
    int currentRow;
    bool flagAllTour=false;
    Tour *testObjectTour;

};

#endif // WIDGET_H
