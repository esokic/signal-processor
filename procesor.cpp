#include "procesor.h"
#include <armadillo>

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

void Procesor::promjena_durationTime(double durationTime)
{
    //Prvo provjeravamo trenutnu pocetnu vrijednost izlaznog signala
    double start_time = pSignal_izlazni_1->get_xData()[0];

    //Onda uradimo promjenu pocetnog vremena ali se zaustavimo na duration tima
    QVector<double> xData_ul = pSignal_ulazni_1->get_xData();
    QVector<double> yData_ul = pSignal_ulazni_1->get_yData();

    QVector<double> xData_izl;
    QVector<double> yData_izl;

    for (int j=0; j < xData_ul.size(); j++)
    {
        if ((xData_ul[j]>=start_time) && (xData_ul[j]<start_time+durationTime))
        {
            xData_izl.push_back(xData_ul[j]);
            yData_izl.push_back(yData_ul[j]);
        }
    }

    pSignal_izlazni_1->set_xData(xData_izl);
    pSignal_izlazni_1->set_yData(yData_izl);
}

void Procesor::promjena_vremenaSempliranja(double samplingTime)
{
    //Prvo je potrebno vidjeti prvu i zadnju vrijednost u izlaznom signalu
    double start_time = pSignal_izlazni_1->get_xData()[0];
    double end_time = pSignal_izlazni_1->get_xData()[pSignal_izlazni_1->get_xData().size()-1]; //NEEFIKASNO!


    //Onda izdvojiti ulazni signal na tome dijelu
    QVector<double> xData_ul = pSignal_ulazni_1->get_xData();
    QVector<double> yData_ul = pSignal_ulazni_1->get_yData();
    QVector<double> xData_izl;
    QVector<double> yData_izl;

    for (int j=0; j < xData_ul.size(); j++)
    {
        if ((xData_ul[j]>=start_time) && (xData_ul[j]<end_time))
        {
            xData_izl.push_back(xData_ul[j]);
            yData_izl.push_back(yData_ul[j]);
        }
    }

    //Potom ga resemplirati
    QVector<double> vektor_vremena;
    double vrijeme = start_time;
    while (vrijeme<=end_time)
    {
        vektor_vremena.push_back(vrijeme);
        vrijeme = vrijeme + samplingTime;
    }

    QVector<double> yData_izl_interpolirano;

    arma::vec a = QVectorToArmadilloVec(xData_izl);
    arma::vec b  = QVectorToArmadilloVec(yData_izl);
    arma::vec c = QVectorToArmadilloVec(vektor_vremena);
    arma::vec d = QVectorToArmadilloVec(yData_izl_interpolirano);

    arma::interp1(a,b ,c , d);


    //Onda prelijepiti na izlaz

    pSignal_izlazni_1->set_xData(vektor_vremena);
    pSignal_izlazni_1->set_yData(ArmadilloVecToQVector(d));

}

arma::vec Procesor::QVectorToArmadilloVec(const QVector<double> &qvec) {
    arma::vec avec(qvec.size());
    for (int i = 0; i < qvec.size(); ++i) {
        avec(i) = qvec[i];
    }
    return avec;
}

QVector<double> Procesor::ArmadilloVecToQVector(const arma::vec &avec) {
    QVector<double> qvec(avec.n_elem);
    for (int i = 0; i < avec.n_elem; ++i) {
        qvec[i] = avec(i);
    }
    return qvec;
}
