#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <QString>

class Transport
{
public:
    Transport();
    virtual ~Transport();

    virtual double calcDistanceTime();

    void setfuelQuantity(int valueFuel);
    void setName(QString textName);
    void setRegNumber(QString textRegNum);

    int getfuelQuantity();
    QString getName();
    QString getRegNumber();



private:
    int fuelQuantity;
    QString Name;
    QString RegNumber;
};

#endif // TRANSPORT_H
