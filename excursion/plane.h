#ifndef PLANE_H
#define PLANE_H

#include <QObject>
#include "transport.h"

class Plane : public QObject, public Transport
{
    Q_OBJECT
public:
    explicit Plane(QObject *parent = nullptr, Transport *tr = nullptr);
    ~Plane() override;
    double calcDistanceTime() override;
signals:

public slots:
};

#endif // PLANE_H
