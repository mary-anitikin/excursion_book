#include "widget.h"
#include "ui_widget.h"
#include "tour.h"
#include "workdb.h"
#include <QDebug>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    myWorkDB = new WorkDB(this);

    db = myWorkDB->getDB();
    myWorkDB->createTable();
    myWorkDB->fillTableNormative();
    myWorkDB->fillTableStart();

    tV_excursion_fill();
    tV_typeTr_fill();
    tV_vehicle_fill();
    CB_fill(ui->CB_transport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableFuel, "FuelID", "Name", "niFuelType" );
    CB_fill(ui->CB_availableTravel, "idMethod", "nameMethod", "niTravelMethod");
    CB_fill(ui->CB_availableUnit, "UnitID", "UnitShortName", "niUnitOfMeasurementType");
    CB_fill(ui->CB_availableTransport_2, "typeTrID", "Name", "niTransportType");

    on_CB_transport_activated(0);

    ui->PB_AddExcursion->setFocus();
    on_PB_AddExcursion_clicked();

    ui->LE_disttimenew->setValidator(new QDoubleValidator(0.0, 10000.0, 2, this) );
    ui->LE_fuelQuantityTr->setValidator(new QIntValidator(0,10000,this));;
    ui->PB_save->hide(); ///временно скрыла, не придумала ещё, как именно сохранять
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_3_clicked()
{
    close();
}

void Widget::on_PB_AddExcursion_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Widget::on_PB_AddTransport_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_PBAddTypeTr_clicked()
{
    if(ui->CB_availableFuel->currentData(Qt::UserRole).isNull() ||
            ui->CB_availableTravel->currentData(Qt::UserRole).isNull() ||
            ui->CB_availableUnit->currentData(Qt::UserRole).isNull() ||
            ui->LE_typeTrName->text().isEmpty()) {
        QMessageBox::information(this, "Внимание!", "Заполните, пожалуйста, поля с информацией о новом типе транспорта", "Да");
        return;
    }
    QString nameTr = ui->LE_typeTrName->text();
    int fuel = ui->CB_availableFuel->currentData(Qt::UserRole).toInt();
    int methodTravel = ui->CB_availableTravel->currentData(Qt::UserRole).toInt();
    int unit = ui->CB_availableUnit->currentData(Qt::UserRole).toInt();

    int newId = myWorkDB->selectMaxFromTable("typeTrID", "niTransportType");

    QSqlQuery q(db);

    q.prepare("INSERT INTO niTransportType "
              " (typeTrID,"
              " Name,"
              " fuel,"
              " unitsOfMeasurement,"
              " idMethod) "
              "values (:typeTrID,"
              " :Name, "
              " :fuel, "
              " :unitsOfMeasurement, "
              " :idMethod) ");
    q.bindValue(":typeTrID", newId);
    q.bindValue(":Name", nameTr);
    q.bindValue(":fuel", fuel);
    q.bindValue(":unitsOfMeasurement", unit);
    q.bindValue(":idMethod", methodTravel);


    if(q.exec()) {
        QMessageBox::information(this, "Новый вид транспорта", "Новый вид транспорта успешно добавлен. Вы можете видеть его на экране", "Да");
        tV_typeTr_fill();
    }
    else {
        QMessageBox::critical(this, "Новый вид транспорта", "Не удалось добавить новый вид транспорта", "Да");
    }

    CB_fill(ui->CB_transport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport_2, "typeTrID", "Name", "niTransportType");
    ui->LE_typeTrName->clear();

}

void Widget::on_PB_AddTour_clicked()
{
    if(ui->CB_availableTransport->currentData(Qt::UserRole).isNull() || ui->LE_nameTour->text().isEmpty()
            || ui->LE_firstPoint->text().isEmpty() || ui->LE_EndPoint->text().isEmpty()
            || ui->LE_disttimenew->text().isEmpty()) {
        QMessageBox::information(this, "Внимание!", "Заполните, пожалуйста, поля с информацией о новой экскурсии", "Да");
        return;
    }
    int type = ui->CB_availableTransport->currentData(Qt::UserRole).toInt();
    QString s_name = ui->LE_nameTour->text();
    QString s_firstP = ui->LE_firstPoint->text();
    QString s_endP = ui->LE_EndPoint->text();
    QString disttime_text = ui->LE_disttimenew->text();
    if(disttime_text.contains(",")) disttime_text.replace(",",".");
    double disttime = disttime_text.toDouble();

    int newId = myWorkDB->selectMaxFromTable("ExcursionID", "ListExcursion");

    QSqlQuery q(db);
    if(type==1){
        q.prepare("INSERT INTO ListExcursion "
                  " (ExcursionID,"
                  " NameExcursion,"
                  " StartPoint,"
                  " EndPoint,"
                  " Distance,"
                  " typeTrID) values (:ExcursionID,"
                  " :NameExcursion, "
                  " :StartPoint, "
                  " :EndPoint, "
                  " :Distance, "
                  " :typeTrID)");
        q.bindValue(":ExcursionID", newId);
        q.bindValue(":NameExcursion", s_name);
        q.bindValue(":StartPoint", s_firstP);
        q.bindValue(":EndPoint", s_endP);
        q.bindValue(":Distance", disttime);
        q.bindValue(":typeTrID", type);
    }
    else if(type==2)
    {
        q.prepare("INSERT INTO ListExcursion "
                  " (ExcursionID,"
                  " NameExcursion,"
                  " StartPoint,"
                  " EndPoint,"
                  " FlightTime,"
                  " typeTrID) values (:ExcursionID,"
                  " :NameExcursion, "
                  " :StartPoint, "
                  " :EndPoint, "
                  " :FlightTime, "
                  " :typeTrID)");
        q.bindValue(":ExcursionID", newId);
        q.bindValue(":NameExcursion", s_name);
        q.bindValue(":StartPoint", s_firstP);
        q.bindValue(":EndPoint", s_endP);
        q.bindValue(":FlightTime",disttime );
        q.bindValue(":typeTrID", type);
    }
    if(q.exec()) {
        QMessageBox::information(this, "Новая экскурсия", "Экскурсия успешно добавлена. Вы можете видеть её в списке экскурсий", "Да");
        tV_excursion_fill();
        ui->LE_nameTour->clear();
        ui->LE_firstPoint->clear();
        ui->LE_EndPoint->clear();
        ui->LE_disttimenew->clear();
    }
    else {
        QMessageBox::critical(this, "Новая экскурсия", "Не удалось добавить экскурсию. Попробуйте перезапустить программу.", "Да");
    }


}

void Widget::tV_excursion_fill() {
    modelTour = new QStandardItemModel;
    modelTour->setColumnCount(5);
    modelTour->setHeaderData(0,Qt::Horizontal, "Название\nэкскурсии", Qt::DisplayRole);
    modelTour->setHeaderData(1,Qt::Horizontal, "Пункт\nотправления", Qt::DisplayRole);
    modelTour->setHeaderData(2,Qt::Horizontal, "Пункт\nзавершения", Qt::DisplayRole);
    modelTour->setHeaderData(3,Qt::Horizontal, "Расстояние,\nкм", Qt::DisplayRole);
    modelTour->setHeaderData(4,Qt::Horizontal, "Время\nполёта, ч", Qt::DisplayRole);
    ui->tV_excursion->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlQuery q(db);

    if(q.exec("SELECT ExcursionID,"
              " NameExcursion,"
              " StartPoint,"
              " EndPoint,"
              " Distance,"
              " FlightTime,"
              " typeTrID"
              " FROM ListExcursion ")) {
        int i = 0;
        while(q.next()) {
            QStandardItem* item1 = new QStandardItem(q.value(1).toString());
            item1->setData(q.value(0),Qt::UserRole);
            item1->setData(q.value(6),Qt::UserRole+1);
            modelTour->setItem(i, 0, item1);
            QStandardItem* item2 = new QStandardItem(q.value(2).toString());
            modelTour->setItem(i, 1, item2);
            QStandardItem* item3 = new QStandardItem(q.value(3).toString());
            modelTour->setItem(i, 2, item3);
            QStandardItem* item4 = new QStandardItem(q.value(4).toString());
            modelTour->setItem(i, 3, item4);
            QStandardItem* item5 = new QStandardItem(q.value(5).toString());
            modelTour->setItem(i, 4, item5);
            i++;
        }
    }
    else {
        QMessageBox::critical(this,"Запрос не удался", "Запрос экскурсий не удался","Да");
    }
    ui->tV_excursion->setModel(modelTour);
    showTourForTransport(q.value(6).toInt(), true);
}

void Widget::tV_typeTr_fill() {

    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal, "Транспорт", Qt::DisplayRole);
    model->setHeaderData(1,Qt::Horizontal, "Топливо", Qt::DisplayRole);
    model->setHeaderData(2,Qt::Horizontal, "Единицы\nизмерения", Qt::DisplayRole);
    model->setHeaderData(3,Qt::Horizontal, "Способ\nпередвижения", Qt::DisplayRole);
    ui->tV_typeTr->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlQuery q(db);

    if(q.exec("Select niTransportType.Name, "
              " niFuelType.Name,"
              " niUnitOfMeasurementType.UnitShortName,"
              " niTravelMethod.nameMethod ,"
              " niTransportType.typeTrID "
              " from niTransportType, niFuelType, niUnitOfMeasurementType, niTravelMethod"
              " where niTransportType.fuel = niFuelType.FuelID "
              " AND niTransportType.unitsOfMeasurement = niUnitOfMeasurementType.UnitID"
              " AND niTransportType.idMethod = niTravelMethod.idMethod")) {

        int j = 0;
        while(q.next()) {
            QStandardItem* item1 = new QStandardItem(q.value(0).toString());
            item1->setData(q.value(4), Qt::UserRole+1);
            model->setItem(j, 0, item1);

            QStandardItem* item2 = new QStandardItem(q.value(1).toString());
            model->setItem(j, 1, item2);

            QStandardItem* item3 = new QStandardItem(q.value(2).toString());
            model->setItem(j, 2, item3);

            QStandardItem* item4 = new QStandardItem(q.value(3).toString());
            model->setItem(j, 3, item4);
            j++;
        }
    }
    else {
        QMessageBox::critical(this,"Запрос не удался", "Запрос транспорта не удался","Да");
    }
    ui->tV_typeTr->setModel(model);
}

void Widget::tV_vehicle_fill()
{
    modelVehicle = new QStandardItemModel;
    modelVehicle->setColumnCount(3);
    modelVehicle->setHeaderData(0,Qt::Horizontal, "Название", Qt::DisplayRole);
    modelVehicle->setHeaderData(1,Qt::Horizontal, "Номер", Qt::DisplayRole);
    modelVehicle->setHeaderData(2,Qt::Horizontal, "Количество топлива", Qt::DisplayRole);
    ui->tV_vehicle->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlQuery q(db);
    q.prepare("Select Brand, "
              " Number, "
              " FuelQuantity,"
              " typeTrID,"
              " IDtr "
              " from ListTransport ");

    if(q.exec()) {
        int j = 0;
        while(q.next()) {
            QStandardItem* item1 = new QStandardItem(q.value(0).toString());
            item1->setData(q.value(4),Qt::UserRole);
            item1->setData(q.value(3),Qt::UserRole+1);
            modelVehicle->setItem(j, 0, item1);

            QStandardItem* item2 = new QStandardItem(q.value(1).toString());
            modelVehicle->setItem(j, 1, item2);

            QStandardItem* item3 = new QStandardItem(q.value(2).toString());
            modelVehicle->setItem(j, 2, item3);
            j++;
        }
    }
    ui->tV_vehicle->setModel(modelVehicle);
}

void Widget::CB_fill(QComboBox *combo, QString id, QString name, QString table)
{
    combo->clear();
    QString str;
    str = "SELECT " + id + " , " + name + " FROM " + table;

    QSqlQuery q2(db);
    q2.exec(str);
    int k = 0;
    while(q2.next()) {
        combo->insertItem(k,q2.value(1).toString(), q2.value(0));
        k++;
    }
}

void Widget::on_CB_transport_activated(int index)
{
    int type;
    type = ui->CB_transport->itemData(index).toInt();
    for(int i=0; i<ui->tV_vehicle->model()->rowCount(); i++) {
        if(ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(i,0), Qt::UserRole+1).toInt() != type) {
            ui->tV_vehicle->hideRow(i);
        }
        else {
            ui->tV_vehicle->showRow(i);
        }
    }
    showTourForTransport(type, false);
}

void Widget::on_PB_CalcTour_clicked()
{
    if(!ui->tV_excursion->currentIndex().isValid() || !ui->tV_vehicle->currentIndex().isValid()) {
        QMessageBox::information(this, "Внимание!", "Выберете, пожалуйста, поездку и транспорт", "Да");
        return;
    }
    int CurRow = ui->tV_excursion->currentIndex().row() ;
    Tour *testObjectTour = new Tour();

    QModelIndex index = modelTour->index(CurRow, 0, QModelIndex());
    QModelIndex index1 = modelTour->index(CurRow, 3, QModelIndex());

    testObjectTour->Name=ui->tV_excursion->model()->data(index).toString();
    testObjectTour->typeTrID = ui->tV_excursion->model()->data(index,Qt::UserRole+1).toInt();
    ui->tV_excursion->indexAt(QPoint(0,CurRow)).data().toString();

    int Column;
    if(testObjectTour->typeTrID == 1){
        Column = 3;
    }
    else if(testObjectTour->typeTrID == 2) {
        Column = 4;
    }
    testObjectTour->dist_time=ui->tV_excursion->model()->data(modelTour->index(CurRow, Column, QModelIndex())).toDouble();

    int CurRowTr = ui->tV_vehicle->currentIndex().row();
    testObjectTour->FuelQuantity = ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(CurRowTr,2)).toInt();


    ui->label_res->setText(testObjectTour->slotRate());

}

void Widget::on_tV_excursion_clicked(const QModelIndex &index)
{

    QVariant type = modelTour->data(ui->tV_excursion->model()->index(index.row(),0),Qt::UserRole+1);

    int ind = ui->CB_transport->findData(type,Qt::UserRole);

    ui->CB_transport->setCurrentIndex(ind);
    on_CB_transport_activated(ind);
}

void Widget::on_PB_ShowAllTour_clicked()
{
    tV_excursion_fill();
    flagAllTour = true;
}

void Widget::showTourForTransport(int type, bool flag)
{
    if(!flag) {
        for(int i=0; i<ui->tV_excursion->model()->rowCount(); i++) {
            if(ui->tV_excursion->model()->data(ui->tV_excursion->model()->index(i,0), Qt::UserRole+1).toInt() != type) {
                ui->tV_excursion->hideRow(i);
            }
            else {
                ui->tV_excursion->showRow(i);
            }
        }
    }
    else {
        for(int i=0; i<ui->tV_excursion->model()->rowCount(); i++) {
            ui->tV_excursion->showRow(i);
        }
    }
}

void Widget::on_CB_availableTransport_activated(int index)
{
    int type = ui->CB_availableTransport->currentData(Qt::UserRole).toInt();
    QString s;
    QSqlQuery q(db);
    s = "SELECT unitsOfMeasurement FROM niTransportType WHERE typeTrID = " + QString::number(type);
    q.exec(s);
    q.next();
    if(q.value(0).toInt()==1){
        ui->label_disttimenew->setText("Расстояние");
    }
    else if(q.value(0).toInt()==2){
        ui->label_disttimenew->setText("Время в пути");
    }
}



void Widget::on_PB_AddTrVehicle_clicked()
{
    if(ui->CB_availableTransport_2->currentData(Qt::UserRole).isNull() || ui->LE_modelTr->text().isEmpty()
            || ui->LE_numberTr->text().isEmpty() || ui->LE_fuelQuantityTr->text().isEmpty()) {
        QMessageBox::information(this, "Внимание!", "Заполните, пожалуйста, поля с информацией о новом транспортном средстве", "Да");
        return;
    }
    int type = ui->CB_availableTransport_2->currentData(Qt::UserRole).toInt();
    QString modelTr = ui->LE_modelTr->text();
    QString numberTr = ui->LE_numberTr->text();
    int fuelQuantityTr = ui->LE_fuelQuantityTr->text().toInt();

    int newId = myWorkDB->selectMaxFromTable("IDtr", "ListTransport");

    QSqlQuery q(db);

    q.prepare("INSERT INTO ListTransport "
              " (IDtr,"
              " typeTrID,"
              " Brand,"
              " Number,"
              " FuelQuantity) "
              "values (:IDtr,"
              " :typeTrID, "
              " :Brand, "
              " :Number, "
              " :FuelQuantity) ");
    q.bindValue(":IDtr", newId);
    q.bindValue(":typeTrID", type);
    q.bindValue(":Brand", modelTr);
    q.bindValue(":Number", numberTr);
    q.bindValue(":FuelQuantity", fuelQuantityTr);


    if(q.exec()) {
        QMessageBox::information(this, "Новое транспортное средство", "Новое транспортное средство успешно добавлено. Вы можете видеть в списке траспортных средств", "Да");
        tV_vehicle_fill();
        ui->LE_modelTr->clear();
        ui->LE_numberTr->clear();
        ui->LE_fuelQuantityTr->clear();
    }
    else {
        QMessageBox::critical(this, "Новое транспортное средство", "Не удалось добавить новое транспортное средство", "Да");
    }
    on_CB_transport_activated(0);

}

void Widget::on_PB_DeleteTour_clicked()
{
    if(!ui->tV_excursion->currentIndex().isValid()){
        return;
    }
    int id;
    int row = ui->tV_excursion->currentIndex().row();
    id = modelTour->item(row,0)->data(Qt::UserRole).toInt();
    myWorkDB->deleteRowFromTable(id,"ExcursionID", "ListExcursion");
    tV_excursion_fill();
}

void Widget::on_PB_DeleteTr_clicked()
{
    if(!ui->tV_vehicle->currentIndex().isValid()){
        return;
    }
    int id;
    int row = ui->tV_vehicle->currentIndex().row();
    id = modelVehicle->item(row,0)->data(Qt::UserRole).toInt();
    myWorkDB->deleteRowFromTable(id,"IDtr", "ListTransport");
    tV_vehicle_fill();
}

void Widget::on_PB_DeleteTypeTr_clicked()
{
    if(!ui->tV_typeTr->currentIndex().isValid()) {
        return;
    }
    int id;
    int row = ui->tV_typeTr->currentIndex().row();
    id = model->item(row,0)->data(Qt::UserRole+1).toInt();
    myWorkDB->deleteRowFromTable(id,"typeTrID", "niTransportType");
    tV_typeTr_fill();
    CB_fill(ui->CB_transport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport_2, "typeTrID", "Name", "niTransportType");

    QString s = "SELECT IDtr FROM ListTransport WHERE typeTrID = " + QString::number(id);
    QSqlQuery q(db);
    q.exec(s);
    while(q.next()) {
        myWorkDB->deleteRowFromTable(q.value(0).toInt(),"IDtr", "ListTransport");
    }
    tV_vehicle_fill();
}
