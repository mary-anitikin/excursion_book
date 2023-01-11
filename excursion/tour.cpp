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
    coefRate = (double)100/FuelRate;

    double maxDist = FuelQuantity*coefRate;
        if(maxDist>dist_time){
            return("Топлива достаточно на "+ QString::number(maxDist) + " км");
        }
        else if (maxDist==dist_time) {
            return("Для данного маршрута топливо на пределе.\nРекомендуется дозаправка. Топлива хватит ровно на " + QString::number(maxDist) + " км");
        }
        else if (maxDist<dist_time) {
            return("Топлива не достаточно.\nНа оставшемся топливе можно продолжать движение " + QString::number(maxDist) + " км");
        }
    }
    if(UnitID==2){ //килограммы
        coefRate = (double)1/FuelRate;
        double maxTime = FuelQuantity*coefRate;
            if(maxTime>dist_time){
                return("Топлива достаточно на " + QString::number(maxTime) + " ч. движения");
            }
            else if (maxTime==dist_time) {
                return("Для данного маршрута топливо на пределе.\nТребуется дозаправка. Топлива хватит ровно на " + QString::number(maxTime) + " ч");
            }
            else if (maxTime<dist_time) {
                return("Топлива не достаточно.\nНа оставшемся топливе можно продолжать движение " + QString::number(maxTime) + " ч");
            }
    }
    else {
        return("Для данного вида транспорта расчёт ещё не реализован.");
    }
}
