#include "plane.h"

Plane::Plane(QObject *parent, Transport *tr) : QObject(parent)
{

}

Plane::~Plane()
{

}

double Plane::calcDistanceTime()
{
    double distcar = (double)getfuelQuantity()/200;
    return distcar;
}
