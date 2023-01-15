#ifndef CAR_H
#define CAR_H

#include <QObject>
#include "transport.h"

class Car : public QObject, public Transport
{
    Q_OBJECT
public:
    explicit Car(QObject *parent = nullptr, Transport *tr = nullptr);
    ~Car() override;
    double calcDistanceTime() override;

signals:

public slots:
};

#endif // CAR_H
