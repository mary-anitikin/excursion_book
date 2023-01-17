#include "widget.h"
#include "ui_widget.h"
#include "tour.h"
#include "workdb.h"
#include "car.h"
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

    mdel = new MyDelegate();

    tV_excursion_fill();
    tV_vehicle_fill();
    CB_fill(ui->CB_transport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport, "typeTrID", "Name", "niTransportType");
    CB_fill(ui->CB_availableTransport_2, "typeTrID", "Name", "niTransportType");

    on_CB_transport_activated(0);

    ui->PB_AddExcursion->setFocus();
    on_PB_AddExcursion_clicked();

    ui->LE_disttimenew->setValidator(new QDoubleValidator(0.0, 10000.0, 2, this) );
    ui->LE_fuelQuantityTr->setValidator(new QIntValidator(0,10000,this));
    //ui->PB_save->setEnabled(false);
    ui->PB_save->hide(); ////скрыла, хочу сделать более осознанную реализацию

    BGroup = new QButtonGroup();
    BGroup->addButton(ui->PB_ChoiceExcursion);
    BGroup->addButton(ui->PB_AddExcursion);
    BGroup->addButton(ui->PB_AddTransport);
    BGroup->exclusive();

    ui->PB_ChoiceExcursion->setCheckable(true);
    ui->PB_AddExcursion->setCheckable(true);
    ui->PB_AddTransport->setCheckable(true);

    ui->PB_ChoiceExcursion->setChecked(true);
    ui->PB_ChoiceExcursion->setFocus();
    on_PB_ChoiceExcursion_clicked();
    flag = -1;


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
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_PB_AddTransport_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
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

    QSqlQuery q1(db);
    QString s1 = "SELECT unitsOfMeasurement FROM niTransportType WHERE typeTrID = " + QString::number(type);
    q1.exec(s1);
    q1.next();
    int unit = q1.value(0).toInt();

    QString s_name = ui->LE_nameTour->text();
    QString s_firstP = ui->LE_firstPoint->text();
    QString s_endP = ui->LE_EndPoint->text();
    QString disttime_text = ui->LE_disttimenew->text();
    if(disttime_text.contains(",")) disttime_text.replace(",",".");
    double disttime = disttime_text.toDouble();

    int newId = myWorkDB->selectMaxFromTable("ExcursionID", "ListExcursion");

    QSqlQuery q(db);
    if(unit==1){
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
    else if(unit==2)
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
    ui->tV_excursion->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
            item1->setEditable(false);
            modelTour->setItem(i, 0, item1);
            QStandardItem* item2 = new QStandardItem(q.value(2).toString());
            item2->setEditable(false);
            modelTour->setItem(i, 1, item2);
            QStandardItem* item3 = new QStandardItem(q.value(3).toString());
            item3->setEditable(false);
            modelTour->setItem(i, 2, item3);
            QStandardItem* item4 = new QStandardItem(q.value(4).toString());
            item4->setEditable(false);
            modelTour->setItem(i, 3, item4);
            QStandardItem* item5 = new QStandardItem(q.value(5).toString());
            item5->setEditable(false);
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

void Widget::tV_vehicle_fill()
{
    modelVehicle = new QStandardItemModel;
    modelVehicle->setColumnCount(3);
    modelVehicle->setHeaderData(0,Qt::Horizontal, "Название", Qt::DisplayRole);
    modelVehicle->setHeaderData(1,Qt::Horizontal, "Номер", Qt::DisplayRole);
    modelVehicle->setHeaderData(2,Qt::Horizontal, "Количество топлива", Qt::DisplayRole);
    ui->tV_vehicle->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


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
            item1->setEditable(false);
            modelVehicle->setItem(j, 0, item1);

            QStandardItem* item2 = new QStandardItem(q.value(1).toString());
            item2->setEditable(false);
            modelVehicle->setItem(j, 1, item2);

            QStandardItem* item3 = new QStandardItem(q.value(2).toString());
            modelVehicle->setItem(j, 2, item3);
            ui->tV_vehicle->setItemDelegate(mdel) ; ///куда засунуть
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
    if(!ui->tV_excursion->currentIndex().isValid() || !ui->tV_vehicle->currentIndex().isValid()){
        QMessageBox::information(this,"Внимание!","Выберите экскурсию и транспорт.","Да");
        return;
    }

    int CurRow = ui->tV_excursion->currentIndex().row();
    QModelIndex index = modelTour->index(CurRow, 0, QModelIndex());
    int ExID = modelTour->data(index,Qt::UserRole).toInt();

    int CurRowTr = ui->tV_vehicle->currentIndex().row();
    if(ui->CB_transport->currentIndex()==0 ) {
        if(flag==0) { /// вместо flag хорошо бы найти более изящное и рабочее решение для случая, когда классы будут добавляться
            testObjectTour = new Tour(ExID, modelTour->item(CurRow, 3)->text().toInt() );
            c->setfuelQuantity(ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(CurRowTr,2)).toInt());
            ui->label_res->setText(testObjectTour->slotRate(c));
        }
        else {
            QMessageBox::information(this,"Внимание!","Выберите автомобиль.","Да");
            return;
        }

    }
    else if(ui->CB_transport->currentIndex()==1 ){
        if(flag==1) {
            testObjectTour = new Tour(ExID, modelTour->item(CurRow, 4)->text().toDouble());
            p->setfuelQuantity(ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(CurRowTr,2)).toInt());
            ui->label_res->setText(testObjectTour->slotRate(p));
        }
        else {
            QMessageBox::information(this,"Внимание!","Выберите самолёт","Да");
            return;
        }
    }

    ui->PB_save->setEnabled(true);
}

void Widget::on_tV_excursion_clicked(const QModelIndex &index)
{
    QVariant type = modelTour->data(ui->tV_excursion->model()->index(index.row(),0),Qt::UserRole+1);
    int ind = ui->CB_transport->findData(type,Qt::UserRole);
    ui->CB_transport->setCurrentIndex(ind);
    on_CB_transport_activated(ind);
    ui->label_res->clear();
}

void Widget::on_PB_ShowAllTour_clicked()
{
    tV_excursion_fill();
    flagAllTour = true;
    ui->label_res->clear();
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
        ui->label_disttimenew->setText("Расстояние, км");
    }
    else if(q.value(0).toInt()==2){
        ui->label_disttimenew->setText("Время в пути, ч");
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
        QMessageBox::information(this,"Внимание!","Выберите экскурсию, которую хотите удалить","Да");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удалить","Вы уверены, что хотите удалить выбранную экскурсию?", QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int id;
        int row = ui->tV_excursion->currentIndex().row();
        id = modelTour->item(row,0)->data(Qt::UserRole).toInt();
        myWorkDB->deleteRowFromTable(id,"ExcursionID", "ListExcursion");
        tV_excursion_fill();
        ui->label_res->clear();
    }
    else return;
}

void Widget::on_PB_DeleteTr_clicked()
{
    if(!ui->tV_vehicle->currentIndex().isValid()){
        QMessageBox::information(this,"Внимание!","Выберите транспорт, который хотите удалить","Да");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удалить","Вы уверены, что хотите удалить выбранное транспортное средство?", QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        int id;
        int row = ui->tV_vehicle->currentIndex().row();
        id = modelVehicle->item(row,0)->data(Qt::UserRole).toInt();
        myWorkDB->deleteRowFromTable(id,"IDtr", "ListTransport");
        tV_vehicle_fill();
        ui->label_res->clear();
    }
    else return;
}

void Widget::on_PB_save_clicked()
{
    if(!ui->tV_excursion->currentIndex().isValid() || !ui->tV_vehicle->currentIndex().isValid()) {
        QMessageBox::critical(this, "Сохранение", "Не удалось сохранить пару экскурсия-транспортное средство. Выберете экскурсию и ТС", "Да");
        return;
    }
    int CurRowEx = ui->tV_excursion->currentIndex().row();
    int ExcursionID = modelTour->item(CurRowEx,0)->data(Qt::UserRole).toInt();

    int CurRowTr = ui->tV_vehicle->currentIndex().row();
    int IDtr = modelVehicle->item(CurRowTr,0)->data(Qt::UserRole).toInt();

    int newId = myWorkDB->selectMaxFromTable("ID", "TourWithTransport");

    testObjectTour->id = newId;

    int EnoughFuel = 0; // 0 - достаточно топлива
    // 1 - недостаточно
    /// 2 - топлива ровно на дистанцию/время поездки НЕ ИСПОЛЬЗУЮ ТЕПЕРЬ

    QSqlQuery q(db);

    q.prepare("INSERT INTO TourWithTransport "
              " (ID,"
              " ExcursionID,"
              " IDtr,"
              " EnoughFuel) "
              "values (:ID,"
              " :ExcursionID, "
              " :IDtr, "
              " :EnoughFuel) ");
    q.bindValue(":IDtr", newId);
    q.bindValue(":ExcursionID", ExcursionID);
    q.bindValue(":IDtr", IDtr);
    q.bindValue(":EnoughFuel", EnoughFuel);

    if(q.exec()) QMessageBox::information(this, "Сохранено", " Для экскурсии успешно назначен транспорт. ", "Да");

    ui->PB_save->setEnabled(false);
}

void Widget::on_tV_vehicle_clicked(const QModelIndex &index)
{
    ui->label_res->clear();
    int CurRowTr = ui->tV_vehicle->currentIndex().row();
    if(ui->CB_transport->currentIndex()==0) {
        c = new Car() ;
        c->setfuelQuantity(ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(CurRowTr,2)).toInt());
        c->setName(modelVehicle->item(CurRowTr,0)->text());
        c->setRegNumber(modelVehicle->item(CurRowTr,1)->text());
        ui->label_dt->setText("Автомобиль  " + c->getName() + ", " + c->getRegNumber() + " \nможет проехать\n" + QString::number(c->calcDistanceTime()) + " км");
        flag = 0;
    }
    else if(ui->CB_transport->currentIndex()==1){
        p = new Plane();
        p->setfuelQuantity(ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(CurRowTr,2)).toInt());
        p->setName(modelVehicle->item(CurRowTr,0)->text());
        p->setRegNumber(modelVehicle->item(CurRowTr,1)->text());
        ui->label_dt->setText("Самолёт " +p->getName() + ", " + p->getRegNumber() + " \nможет пролететь\n" + QString::number(p->calcDistanceTime()) + " ч");
        flag = 1;
    }
}

void Widget::on_CB_transport_currentIndexChanged(int index)
{
    ui->label_res->clear();
}

void Widget::on_PB_ChoiceExcursion_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

}

void Widget::on_PB_saveFuel_clicked()
{
    /// !!!!!!!!!!! Как узнать номера ячеек, в которых количество топлива поменялось???
    /// !!!!!!!!!!! Решить вопрос с вводом букв и символов в ячейку

    ///// определяю количество строк в модели. rowCount не подходит. Решила запросом в БД ...
    //int rowC = ui->tV_vehicle->model()->rowCount(index);
    QSqlQuery q1(db);
    q1.exec("SELECT count(*) FROM ListTransport ");
    if(q1.next()){
        int test = q1.value(0).toInt();/// количество строк в modelVehicle
        for(int i=0; i<test; i++) {
                QSqlQuery q(db);
                int fc = modelVehicle->item(i,2)->text().toInt();
                int idtr = modelVehicle->item(i,0)->data(Qt::UserRole).toInt();
                q.prepare("UPDATE ListTransport "
                          " set FuelQuantity = "
                          " :fuelquantity"
                          " WHERE IDtr = :IDTr ");
                q.bindValue(":fuelquantity", fc);
                q.bindValue(":IDTr", idtr);

                if(!q.exec()) {
                    qDebug() << "не удалось обновить";
                }
        }
    }
}

void Widget::on_CB_availableTransport_2_currentIndexChanged(int index)
{
    ui->LE_modelTr->clear();
    ui->LE_numberTr->clear();
    ui->LE_fuelQuantityTr->clear();
}
