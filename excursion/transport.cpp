#include "transport.h"

Transport::Transport()
{

}

Transport::~Transport()
{

}

void Transport::setfuelQuantity(int valueFuel)
{
    fuelQuantity = valueFuel;
}

void Transport::setName(QString textName)
{
    Name = textName;
}

void Transport::setRegNumber(QString textRegNum)
{
    RegNumber = textRegNum;
}

int Transport::getfuelQuantity()
{
    return fuelQuantity;
}

QString Transport::getName()
{
    return Name;
}

QString Transport::getRegNumber()
{
    return RegNumber;
}

double Transport::calcDistanceTime(){}
