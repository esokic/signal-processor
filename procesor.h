#ifndef PROCESOR_H
#define PROCESOR_H

#include <QWidget>
#include <QWidget>
#include "signal.h"
#include <armadillo>

#include <QWidget>

namespace Ui {
class Procesor;
}

class Procesor : public QWidget
{
    Q_OBJECT

public:
    explicit Procesor(QWidget *parent = nullptr);
    ~Procesor();

private:
    Ui::Procesor *ui;
    void updejtVrijednostiIzEditBoxova();

public:
    //Stare funkcije
    void promjena_startTime(double startTime);
    void promjena_durationTime(double durationTime);
    //Stara funkcija
    //void promjena_vremenaSempliranja(double samplingTime);
    void promjena_resempliraj(double samplingTime, bool resempliraj,
                                        double tmin1, double tmax1, bool cut1,
                                        double tmin2, double tmax2, bool cut2,
                                        double tmin3, double tmax3, bool cut3,
                                        double k, double n, bool scale);

    void osvjezi();
    void prikazi();

    //Setteri

    //Setteri
    void setIme(QString _ime);
    void setOpis(QString _description);
    void setStartTime(double _time);
    void setPromjenaStartTime(bool _promjena_start_time);
    void setPromjenaDurationTime(bool _promjena_duration_time);
    void setPromjenaOffsetTime(bool _promjena_offset_time);
    void setDurationTime(double _time);
    void setSamplingTime(double _time);
    void setOffsetTime(double _time);
    void setResempliraj(bool _resempliraj);
    void setTmin1(double _tmin1);
    void setTmax1(double _tmax1);
    void setCut1(bool _cut1);
    void setTmin2(double _tmin2);
    void setTmax2(double _tmax2);
    void setCut2(bool _cut2);
    void setTmin3(double _tmin3);
    void setTmax3(double _tmax3);
    void setCut3(bool _cut3);
    void setK(double _k);
    void setN(double _n);
    void setScale(bool _scale);
    void setNewSignalName(const QString &_newSignalName);
    void setChangeSignalName(bool _changeSignalName);

    // Getters
    QString getIme() const;
    QString getOpis() const;
    double getSamplingTime() const;
    double getStartTime() const;
    double getDurationTime() const;
    double getOffsetTime() const;
    bool getPromjenaDurationTime() const;
    bool getPromjenaStartTime() const;
    bool getPromjenaOffsetTime() const;
    bool getResempliraj() const;
    double getTmin1() const;
    double getTmax1() const;
    bool getCut1() const;
    double getTmin2() const;
    double getTmax2() const;
    bool getCut2() const;
    double getTmin3() const;
    double getTmax3() const;
    bool getCut3() const;
    double getK() const;
    double getN() const;
    bool getScale() const;
    QString getNewSignalName() const;
    bool getChangeSignalName() const;


    void setPointerSignalUlazni1(Signal* psig){pSignal_ulazni_1 = psig;}
    void setPointerSignalIzlazni1(Signal* psig){pSignal_izlazni_1 = psig;}

    //Getteri
    double getStartTimeFromMarkerValue(){return pSignal_ulazni_1->getMarkerValue();}
signals:
    void signalOsvjezi();

private slots:
    void on_pushButton_Update_clicked();

    void on_pushButton_AutoReadMarker_clicked();

private:

    QString ime = "(unnamed)";
    QString opis = "";
    double startTime = -5.0;
    bool promjena_start_time = false;
    double durationTime = 5.0;
    bool promjena_duration_time = false;
    double samplingTime = 0.1;
    double offsetTime = 0.0;
    bool promjena_offset_time = false;

    bool resempliraj = false;
    double tmin1 = 0.0;
    double tmax1 = 0.0;
    bool cut1 = false;
    double tmin2 = 0.0;
    double tmax2 = 0.0;
    bool cut2 = false;
    double tmin3  = 0.0;
    double tmax3  = 0.0;
    bool cut3 = false;
    double k = 1.0;
    double n = 0.0;
    bool scale = false;
    QString newSignalName = "new name";
    bool changeSignalName = false;







    Signal* pSignal_ulazni_1;
    Signal* pSignal_izlazni_1;

    arma::vec QVectorToArmadilloVec(const QVector<double> &qvec);
    QVector<double> ArmadilloVecToQVector(const arma::vec &avec);
};

#endif // PROCESOR_H
