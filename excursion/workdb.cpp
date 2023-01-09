#include "workdb.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

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
                 "            fuel               INTEGER REFERENCES FuelType (FuelID) ON DELETE CASCADE"
                 "                                                                    ON UPDATE CASCADE,"
                 "            unitsOfMeasurement INTEGER REFERENCES UnitOfMeasurementType (UnitID),"
                 "            idMethod           INTEGER REFERENCES niTravelMethod (idMethod) "
                 "        )";
    q.exec(s5);

    QString s6 = "CREATE TABLE IF NOT EXISTS ListTransport ("
                 "            IDtr         INTEGER PRIMARY KEY,"
                 "            typeTrID     INTEGER REFERENCES niTransportType (typeTrID) ON DELETE CASCADE,"
                 "            Brand        TEXT,"
                 "            Number       TEXT,"
                 "            FuelQuantity INTEGER"
                 "        )";
    q.exec(s6);

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
