#include "tour.h"

Tour::Tour(int ExID, int TrID, QObject *parent) : QObject(parent)
{
    ExcursionID = ExID;
    IDtr = TrID;
}

QString Tour::slotRate()
{
    if(UnitID==1) { //литры
        //расход 12л/100км

    double maxDist = FuelQuantity*CarFuelRate;
        if(maxDist>dist_time){
            return("Топлива достаточно");
        }
        else if (maxDist==dist_time) {
            return("Для данного маршрута топливо на пределе.\nРекомендуется дозаправка.");
        }
        else if (maxDist<dist_time) {
            return("Топлива не достаточно.\nНа оставшемся топливе можно продолжать движение " + QString::number(maxDist) + " км");
        }
    }
    if(UnitID==2){ //килограммы
        double maxTime = FuelQuantity*FlyFuelRate;
            if(maxTime>dist_time){
                return("Топлива достаточно");
            }
            else if (maxTime==dist_time) {
                return("Для данного маршрута топливо на пределе.\nТребуется дозаправка.");
            }
            else if (maxTime<dist_time) {
                return("Топлива не достаточно.\nНа оставшемся топливе можно продолжать движение " + QString::number(maxTime) + " ч");
            }
    }
    else {
        return("Для данного вида транспорта расчёт ещё не реализован.");
    }
}
