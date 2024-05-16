#ifndef PROCESOR_H
#define PROCESOR_H

#include <QObject>
#include "signal.h"

class Procesor : public QObject
{
    Q_OBJECT
public:
    explicit Procesor(QObject *parent = nullptr);

    void promjena_startTime(double startTime);
    void promjena_durationTime(double durationTime);
    //Setteri
    void setPointerSignalUlazni1(Signal* psig){pSignal_ulazni_1 = psig;}
    void setPointerSignalIzlazni1(Signal* psig){pSignal_izlazni_1 = psig;}

signals:

private:
    Signal* pSignal_ulazni_1;
    Signal* pSignal_izlazni_1;

};

#endif // PROCESOR_H
