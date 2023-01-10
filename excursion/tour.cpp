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
            return("Топлива хватит");
        }
        else if (maxDist==dist_time) {
            return("Топлива впритык в одну сторону");
        }
        else if (maxDist<dist_time) {
            return("Топлива не хватит");
        }
    }
    if(UnitID==2){ //килограммы
        double maxTime = FuelQuantity*FlyFuelRate;
            if(maxTime>dist_time){
                return("Топлива хватит");
            }
            else if (maxTime==dist_time) {
                return("Топлива впритык");
            }
            else if (maxTime<dist_time) {
                return("Топлива не хватит");
            }
    }
    else {
        return("Для данного вида транспорта расчёт ещё не реализован.");
    }
}
