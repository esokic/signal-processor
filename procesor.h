#ifndef PROCESOR_H
#define PROCESOR_H

#include <QObject>
#include "signal.h"
#include <armadillo>

class Procesor : public QObject
{
    Q_OBJECT
public:
    explicit Procesor(QObject *parent = nullptr);

    void promjena_startTime(double startTime);
    void promjena_durationTime(double durationTime);
    void promjena_vremenaSempliranja(double samplingTime);
    //Setteri
    void setPointerSignalUlazni1(Signal* psig){pSignal_ulazni_1 = psig;}
    void setPointerSignalIzlazni1(Signal* psig){pSignal_izlazni_1 = psig;}

    //Getteri
    double getStartTimeFromMarkerValue(){return pSignal_ulazni_1->getMarkerValue();}
signals:

private:
    Signal* pSignal_ulazni_1;
    Signal* pSignal_izlazni_1;

    arma::vec QVectorToArmadilloVec(const QVector<double> &qvec);
    QVector<double> ArmadilloVecToQVector(const arma::vec &avec);

};

#endif // PROCESOR_H
