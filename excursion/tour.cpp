#include "tour.h"
#include "car.h"
#include "plane.h"

Tour::Tour(int ExID, double t, QObject *parent) : QObject(parent)
{
    ExcursionID = ExID;
    time = t;
    dist = -1;
}

Tour::Tour(int ExID, int d, QObject *parent)
{
    ExcursionID = ExID;
    dist = d;
    time = -1;
}

QString Tour::slotRate(Transport *tr)
{
    double maxDistTime = tr->calcDistanceTime();
    QString s;

    if(dist > -1) {
        if(maxDistTime > dist) {
            s = "Топлива достаточно\nдля совершения поездки";
        }
        else{
            s = "Топлива недостаточно\nдля совершения поездки";
        }
    }
    else if(time > 0) {
        if(maxDistTime > time) {
            s = "Топлива достаточно\nдля совершения поездки";
        }
        else{
            s = "Топлива недостаточно\nдля совершения поездки";
        }
    }
    else s = "Недостаточно\nисходных данных";

    return s;
}


