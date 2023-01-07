#include "widget.h"
#include "ui_widget.h"
#include "tour.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../db/excursion_db.db");
    if(db.open()) {
        ui->l_status->setText("Успешно подключились к базе данных: " + db.databaseName());
        tV_excursion_fill();
        tV_typeTr_fill();
        CB_transport_fill(ui->CB_transport);
        CB_transport_fill(ui->CB_availableTransport);
        on_CB_transport_activated(0);
    }
    else {
        ui->l_status->setText("При подключении к базе данных произошла ошибка: " + db.lastError().databaseText() );
    }
    ui->PB_AddExcursion->setFocus();
    on_PB_AddExcursion_clicked();

    ui->LE_disttimenew->setValidator(new QDoubleValidator(0.0, 10000.0, 2, this) );
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
    ///Добавление нового вида транспорта пока не работает
    model->insertRow(model->rowCount());
    CB_transport_fill(ui->CB_transport);
    CB_transport_fill(ui->CB_availableTransport);

}

void Widget::on_PB_AddTour_clicked()
{

    int type = ui->CB_availableTransport->currentData(Qt::UserRole).toInt();

    QString s_name = ui->LE_nameTour->text();
    QString s_firstP = ui->LE_firstPoint->text();
    QString s_endP = ui->LE_EndPoint->text();
    int disttime = ui->LE_disttimenew->text().toInt();
    int newId = 0;

    QSqlQuery q1(db);
    if(q1.exec("Select MAX(ExcursionID ) from ListExcursion")) {
        q1.next();
        newId = q1.value(0).toInt()+1;
    }
    else newId = 1;

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
    }
    else {
        QMessageBox::critical(this, "Новая экскурсия", "Не удалось добавить экскурсию", "Да");
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
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal, "Транспорт", Qt::DisplayRole);
    model->setHeaderData(1,Qt::Horizontal, "Топливо", Qt::DisplayRole);
    model->setHeaderData(2,Qt::Horizontal, "Единицы измерения", Qt::DisplayRole);
    ui->tV_typeTr->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QSqlQuery q(db);

    if(q.exec("Select niTransportType.Name, "
              " niFuelType.Name,"
              " niUnitOfMeasurementType.UnitShortName"
              " from niTransportType, niFuelType, niUnitOfMeasurementType"
              " where niTransportType.fuel = niFuelType.FuelID "
              " AND niTransportType.unitsOfMeasurement = niUnitOfMeasurementType.UnitID")) {

        int j = 0;
        while(q.next()) {
            QStandardItem* item1 = new QStandardItem(q.value(0).toString());
            model->setItem(j, 0, item1);

            QStandardItem* item2 = new QStandardItem(q.value(1).toString());
            model->setItem(j, 1, item2);

            QStandardItem* item3 = new QStandardItem(q.value(2).toString());
            model->setItem(j, 2, item3);
            j++;
        }
    }
    else {
        QMessageBox::critical(this,"Запрос не удался", "Запрос транспорта не удался","Да");
    }
    ui->tV_typeTr->setModel(model);
}

void Widget::CB_transport_fill(QComboBox *combo)
{
    combo->clear();
    QSqlQuery q2(db);
    q2.exec("SELECT typeTrID, Name FROM niTransportType");
    int k = 0;
    while(q2.next()) {
        combo->insertItem(k,q2.value(1).toString(), q2.value(0));
        k++;
    }
}

void Widget::on_CB_transport_activated(int index)
{
    modelVehicle = new QStandardItemModel;
    modelVehicle->setColumnCount(3);
    modelVehicle->setHeaderData(0,Qt::Horizontal, "Название", Qt::DisplayRole);
    modelVehicle->setHeaderData(1,Qt::Horizontal, "Номер", Qt::DisplayRole);
    modelVehicle->setHeaderData(2,Qt::Horizontal, "Количество топлива", Qt::DisplayRole);
    ui->tV_vehicle->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int type;
    type = ui->CB_transport->itemData(index).toInt();

    QSqlQuery q(db);
    q.prepare("Select Brand, "
              " Number, "
              " FuelQuantity "
              " from ListTransport "
              " where typeTrID = :typeTrID");
    q.bindValue(":typeTrID", type);

    if(q.exec()) {
        int j = 0;
        while(q.next()) {
            QStandardItem* item1 = new QStandardItem(q.value(0).toString());
            modelVehicle->setItem(j, 0, item1);

            QStandardItem* item2 = new QStandardItem(q.value(1).toString());
            modelVehicle->setItem(j, 1, item2);

            QStandardItem* item3 = new QStandardItem(q.value(2).toString());
            modelVehicle->setItem(j, 2, item3);
            j++;
        }
    }
    ui->tV_vehicle->setModel(modelVehicle);
    showTourForTransport(type, false);
}

void Widget::on_PB_CalcTour_clicked()
{
    int CurRow = ui->tV_excursion->currentIndex().row() ;
    Tour *testObjectTour = new Tour();

    QModelIndex index = modelTour->index(CurRow, 0, QModelIndex());
    QModelIndex index1 = modelTour->index(CurRow, 3, QModelIndex());

    testObjectTour->Name=ui->tV_excursion->model()->data(index).toString();
    testObjectTour->typeTrID = ui->tV_excursion->model()->data(index,Qt::UserRole+1).toInt();
    ui->tV_excursion->indexAt(QPoint(0,CurRow)).data().toString();

    if(testObjectTour->typeTrID == 1){
        QModelIndex index1 = modelTour->index(CurRow, 3, QModelIndex());
    }
    else if(testObjectTour->typeTrID == 2) {
        QModelIndex index1 = modelTour->index(CurRow, 4, QModelIndex());
    }
    testObjectTour->dist_time=ui->tV_excursion->model()->data(index1).toInt();

    int CurRowTr = ui->tV_vehicle->currentIndex().row();
    testObjectTour->FuelQuantity = ui->tV_vehicle->model()->data(ui->tV_vehicle->model()->index(CurRowTr,2)).toInt();


    ui->label_res->setText(testObjectTour->slotRate(testObjectTour->FuelQuantity,testObjectTour->dist_time,testObjectTour->typeTrID));

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
    if(type==1){
        ui->label_disttimenew->setText("Расстояние");
    }
    else if(type==2){
        ui->label_disttimenew->setText("Время в пути");
    }
}
