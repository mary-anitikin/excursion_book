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

    void CB_transport_fill(QComboBox *combo);

    void on_CB_transport_activated(int index);

    void on_PB_CalcTour_clicked();

    void on_tV_excursion_clicked(const QModelIndex &index);

    void on_PB_ShowAllTour_clicked();

    void showTourForTransport(int type, bool flag);

    void on_CB_availableTransport_activated(int index);

private:
    Ui::Widget *ui;
    QSqlDatabase db;
    QStandardItemModel *model;
    QStandardItemModel *modelTour;
    QStandardItemModel *modelVehicle;
    int currentRow;
    bool flagAllTour=false;

};

#endif // WIDGET_H
