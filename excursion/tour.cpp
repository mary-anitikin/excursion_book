#include "tour.h"

Tour::Tour(QObject *parent) : QObject(parent)
{

}

QString Tour::slotRate()
{
//    if(typeTr==1) { //автомобиль
//        //расход 12л/100км

//    double maxDist = fuel*CarFuelRate;
//        if(maxDist>distime){
//            return("Топлива хватит");
//        }
//        else if (maxDist==distime) {
//            return("Топлива впритык в одну сторону");
//        }
//        else if (maxDist<distime) {
//            return("Топлива не хватит");
//        }
//    }
//    if(typeTr==2){ //самолёт
//        int maxTime = fuel*FlyFuelRate;
//            if(maxTime>distime){
//                return("Топлива хватит");
//            }
//            else if (maxTime==distime) {
//                return("Топлива впритык");
//            }
//            else if (maxTime<distime) {
//                return("Топлива не хватит");
//            }
//    }

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
