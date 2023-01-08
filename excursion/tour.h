#ifndef TOUR_H
#define TOUR_H

#include <QObject>

class Tour : public QObject
{
    Q_OBJECT
public:
    explicit Tour(QObject *parent = nullptr);

    const double CarFuelRate = 8.33; // 12 л на 100 км
    const double FlyFuelRate = 0.005; //200 кг на час полёта

    int id; //идентификатор экскурсия-транспорт
    int ExcursionID; //id экскурсии
    int IDtr; //id конкретного транспортного средства
    double FuelQuantity; //количество топлива в л или в кг в зависимости от ТС
    QString Name; // Название

    double dist_time; //расстояние или время в пути в зависимости от типа транспорта
    int typeTrID; //тип транспортного средства

    enum modeOfTravel
    {
        Drive = 0, ///< ехать
        Fly, ///< лететь
        Sail,     ///< плыть на корабле
        UnknownMethod  ///запасной вариант, если пользователь добавил что-то новенькое (а вдруг!?)
    };

signals:

public slots:
     QString slotRate();
};

#endif // TOUR_H
