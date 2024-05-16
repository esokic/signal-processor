#include "procesor.h"

Procesor::Procesor(QObject *parent) : QObject(parent)
{

}

void Procesor::promjena_startTime(double startTime)
{
    QVector<double> xData_ul = pSignal_ulazni_1->get_xData();
    QVector<double> yData_ul = pSignal_ulazni_1->get_yData();

    QVector<double> xData_izl;
    QVector<double> yData_izl;

    for (int j=0; j < xData_ul.size(); j++)
    {
        if (xData_ul[j]>=startTime)
        {
            xData_izl.push_back(xData_ul[j]);
            yData_izl.push_back(yData_ul[j]);
        }
    }

    pSignal_izlazni_1->set_xData(xData_izl);
    pSignal_izlazni_1->set_yData(yData_izl);
}
