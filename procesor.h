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
    //Stara funkcija
    //void promjena_vremenaSempliranja(double samplingTime);
    void promjena_resempliraj(double samplingTime, bool resempliraj,
                                        double tmin1, double tmax1, bool cut1,
                                        double tmin2, double tmax2, bool cut2,
                                        double tmin3, double tmax3, bool cut3);
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
