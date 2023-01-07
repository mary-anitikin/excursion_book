#ifndef TOUR_H
#define TOUR_H

#include <QObject>

class Tour : public QObject
{
    Q_OBJECT
public:
    explicit Tour(QObject *parent = nullptr);

    const int CarFuelRate = 100/12; // 12 л на 100 км
    const int FlyFuelRate = 1/200; //200 кг на час полёта

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


    /// Экскурсия и транспорт связаны по способу передвижения modeOfTravel
    /// В таблицах и классах есть это enum
    /// Если выбрали экскурсию "Лететь", то в выборе транспорта доступны только самолёты (и потом может быть вертолёты)
    /// Если сначала выбрали "Автомобиль", то доступны только экскурсии "Ехать"
    ///

signals:

public slots:
     void testslot(QString str);
     QString slotRate(double fuel, double distime, int typeTr);
};

#endif // TOUR_H
