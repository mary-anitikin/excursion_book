#ifndef TOUR_H
#define TOUR_H

#include <QObject>
#include "transport.h"

class Tour : public QObject
{
    Q_OBJECT
public:
    explicit Tour(int ExID, double t,  QObject *parent = nullptr);
    explicit Tour(int ExID, int d,  QObject *parent = nullptr);

    int id; //идентификатор экскурсия-транспорт
    int ExcursionID; //id экскурсии
    QString Name; // Название    
    int typeTrID; //тип транспортного средства


signals:

public slots:
     QString slotRate(Transport *tr);
private:
     double dist; //расстояние в пути в зависимости от типа транспорта
     double time; //время в пути в зависимости от типа транспорта

};

#endif // TOUR_H
