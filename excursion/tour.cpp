#include "tour.h"

Tour::Tour(QObject *parent) : QObject(parent)
{

}

QString Tour::slotRate()
{
    if(typeTrID==1) { //автомобиль
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
    if(typeTrID==2){ //самолёт
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
}
