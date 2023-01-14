#ifndef TRANSPORT_H
#define TRANSPORT_H


class Transport
{
public:
    Transport();
    ~Transport();
    int fuelQuantity;

    virtual double calcDistanceTime();

};

#endif // TRANSPORT_H
