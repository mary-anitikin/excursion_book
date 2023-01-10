#include "workdb.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

WorkDB::WorkDB(QWidget *parent) : QWidget(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../excursion_db.db");
    if(db.open()) {
        QMessageBox::information(this,"Подключение к БД","Успешно подключились к базе данных: " + db.databaseName(), "Да");
    }
    else {
        QMessageBox::critical(this,"Подключение к БД","Не удалось подключиться к базе данных: " + db.databaseName(), "Да");

    }
}

WorkDB::~WorkDB()
{

}

QSqlDatabase WorkDB::getDB()
{
    return db;

}

void WorkDB::createTable()
{
    QSqlQuery q(db);

    QString s1 = "CREATE TABLE IF NOT EXISTS ListExcursion ("
                 "                ExcursionID   INTEGER PRIMARY KEY"
                 "                                      UNIQUE"
                 "                                      NOT NULL,"
                 "                NameExcursion TEXT,"
                 "                StartPoint    TEXT,"
                 "                EndPoint      TEXT,"
                 "                Distance      DOUBLE,"
                 "                FlightTime    DOUBLE,"
                 "                typeTrID      INTEGER REFERENCES niTransportType (typeTrID) "
                 "            )";
    q.exec(s1);

    QString s2 = "CREATE TABLE IF NOT EXISTS niUnitOfMeasurementType ("
                 "            UnitID        INTEGER PRIMARY KEY"
                 "                                  UNIQUE"
                 "                                  NOT NULL,"
                 "            UnitName      TEXT    UNIQUE,"
                 "            UnitShortName TEXT    UNIQUE"
                 "        )";
    q.exec(s2);

    QString s3 = "CREATE TABLE IF NOT EXISTS niTravelMethod ("
                 "            idMethod   INTEGER PRIMARY KEY,"
                 "            nameMethod TEXT"
                 "        )";
    q.exec(s3);

    QString s4 = "CREATE TABLE IF NOT EXISTS niFuelType ("
                 "            FuelID INTEGER PRIMARY KEY"
                 "                           UNIQUE"
                 "                           NOT NULL,"
                 "            Name   TEXT    NOT NULL"
                 "                           UNIQUE"
                 "        )";

    q.exec(s4);

    QString s5 = "CREATE TABLE IF NOT EXISTS niTransportType ("
                 "            typeTrID           INTEGER UNIQUE"
                 "                                       NOT NULL"
                 "                                       PRIMARY KEY,"
                 "            Name               TEXT,"
                 "            fuel               INTEGER REFERENCES niFuelType (FuelID) ON DELETE CASCADE"
                 "                                                                    ON UPDATE CASCADE,"
                 "            unitsOfMeasurement INTEGER REFERENCES niUnitOfMeasurementType (UnitID),"
                 "            idMethod           INTEGER REFERENCES niTravelMethod (idMethod) "
                 "        )";
    q.exec(s5);

    QString s6 = "CREATE TABLE IF NOT EXISTS ListTransport ("
                 "            IDtr         INTEGER PRIMARY KEY,"
                 "            typeTrID     INTEGER REFERENCES niTransportType (typeTrID) ON DELETE CASCADE,"
                 "            Brand        TEXT,"
                 "            Number       TEXT UNIQUE,"
                 "            FuelQuantity INTEGER"
                 "        )";
    q.exec(s6);

    QString s7 = "CREATE TABLE TourWithTransport ("
                 "            ID          INTEGER PRIMARY KEY,"
                 "            ExcursionID INTEGER REFERENCES ListExcursion (ExcursionID) ON DELETE CASCADE,"
                 "            IDtr        INTEGER REFERENCES ListTransport (IDtr) ON DELETE CASCADE,"
                 "            EnoughFuel  INTEGER"
                 "        );";
    q.exec(s7);

}

void WorkDB::fillTableNormative()
{
    QSqlQuery q(db);

    QString s1 = "INSERT INTO niFuelType (FuelID, Name) values (1, 'бензин')";
    q.exec(s1);

    QString s2 = "INSERT INTO niTravelMethod (idMethod, nameMethod) values (1, 'по земле')";
    q.exec(s2);
    QString s3 = "INSERT INTO niTravelMethod (idMethod, nameMethod) values (2, 'по воздуху')";
    q.exec(s3);
    QString s4 = "INSERT INTO niTravelMethod (idMethod, nameMethod) values (3, 'по воде')";
    q.exec(s4);

    QString s5 = "INSERT INTO niUnitOfMeasurementType (UnitID, UnitName, UnitShortName) values (1, 'литр', 'л')";
    q.exec(s5);
    QString s6 = "INSERT INTO niUnitOfMeasurementType (UnitID, UnitName, UnitShortName) values (2, 'килограмм', 'кг')";
    q.exec(s6);
}

void WorkDB::fillTableStart()
{
    QSqlQuery q(db);

    QString str1 = "INSERT INTO niTransportType "
                 "(typeTrID,"
                 " Name, "
                 " fuel, "
                 " unitsOfMeasurement,  "
                 " idMethod ) values "
                 "(1, "
                 "'Автомобиль',"
                 " 1, "
                 " 1, "
                 " 1)";
    q.exec(str1);

    QString str2 = "INSERT INTO niTransportType "
                 "(typeTrID,"
                 " Name, "
                 " fuel, "
                 " unitsOfMeasurement,  "
                 " idMethod ) values "
                 "(2, "
                 "'Самолёт',"
                 " 1, "
                 " 2, "
                 " 2)";
    q.exec(str2);

    QString s1 = "INSERT INTO ListExcursion "
                 "(ExcursionID,"
                 " NameExcursion, "
                 " StartPoint, "
                 " EndPoint,  "
                 " Distance, "
                 "typeTrID ) values "
                 "(1, "
                 "'Москва Златоглавая',"
                 " 'Ковров', "
                 " 'Москва', "
                 " 750, "
                 "1)";
    q.exec(s1);

    QString s2 = "INSERT INTO ListExcursion "
                 "(ExcursionID,"
                 " NameExcursion, "
                 " StartPoint, "
                 " EndPoint,  "
                 " Distance, "
                 "typeTrID ) values "
                 "(2, "
                 "'К Илье Муромцу',"
                 " 'Ковров', "
                 " 'Муром', "
                 " 223, "
                 "1)";
    q.exec(s2);

    QString s3 = "INSERT INTO ListExcursion "
                 "(ExcursionID,"
                 " NameExcursion, "
                 " StartPoint, "
                 " EndPoint,  "
                 " FlightTime, "
                 "typeTrID ) values "
                 "(3, "
                 "'Янтарная столица России',"
                 " 'Москва', "
                 " 'Калининград', "
                 " 2.7, "
                 "2)";
    q.exec(s3);

    QString st1 = "INSERT INTO ListTransport "
                 "(IDtr,"
                 " typeTrID, "
                 " Brand, "
                 " Number,  "
                 " FuelQuantity ) values "
                 "(1, "
                 " 1 ,"
                 " 'ПОБЕДА', "
                 " 'у234ек', "
                 " 50)";
    q.exec(st1);

    QString st2 = "INSERT INTO ListTransport "
                 "(IDtr,"
                 " typeTrID, "
                 " Brand, "
                 " Number,  "
                 " FuelQuantity ) values "
                 "(2, "
                 " 1 ,"
                 " 'Opel', "
                 " 'м567ол', "
                 " 60)";
    q.exec(st2);

    QString st3 = "INSERT INTO ListTransport "
                 "(IDtr,"
                 " typeTrID, "
                 " Brand, "
                 " Number,  "
                 " FuelQuantity ) values "
                 "(3, "
                 " 2 ,"
                 " 'LightFly', "
                 " 'RA09876', "
                 " 550)";
    q.exec(st3);

}

int WorkDB::selectMaxFromTable(QString nameColumn, QString nameTable)
{
    int newId = 0;

    QString s = "Select MAX(" + nameColumn +") from "+nameTable;
    QSqlQuery q1(db);
    if(q1.exec(s)) {
        q1.next();
        newId = q1.value(0).toInt()+1;
    }
    else newId = 1;
    return newId;
}

void WorkDB::deleteRowFromTable(int idR, QString idName, QString nameTable)
{
    QString s = "DELETE FROM "+nameTable+ " WHERE "+ idName +" = " + QString::number(idR);
    qDebug() << s;
    QSqlQuery q(db);
    if(q.exec(s)) {
        QMessageBox::information(this,"Удаление","Удаление прошло успешно", "Да");
    }
    else {
        QMessageBox::critical(this,"Удаление","Удаление не удалось" + q.lastError().text(), "Да");

    }
}
