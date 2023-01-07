#include "tour.h"

Tour::Tour(QObject *parent) : QObject(parent)
{

}

QString Tour::slotRate(double fuel, double distime, int typeTr)
{
    if(typeTr==1) { //автомобиль
        //расход 12л/100км

    double maxDist = fuel*CarFuelRate;
        if(maxDist>distime){
            return("Топлива хватит");
        }
        else if (maxDist==distime) {
            return("Топлива впритык в одну сторону");
        }
        else if (maxDist<distime) {
            return("Топлива не хватит");
        }
    }
    if(typeTr==2){ //самолёт
        int maxTime = fuel*FlyFuelRate;
            if(maxTime>distime){
                return("Топлива хватит");
            }
            else if (maxTime==distime) {
                return("Топлива впритык");
            }
            else if (maxTime<distime) {
                return("Топлива не хватит");
            }
    }
}
