#include "car.h"

Car::Car(QObject *parent, Transport *tr) : QObject(parent)
{

}

Car::~Car()
{

}

double Car::calcDistanceTime()
{
    double distcar = (double)getfuelQuantity()*100/12; //8.33 - 12л/100км
    return distcar;
}
