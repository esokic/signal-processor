#include "procesor.h"
#include "ui_procesor.h"

#include "procesor.h"
#include <armadillo>

Procesor::Procesor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Procesor)
{
    ui->setupUi(this);
}

Procesor::~Procesor()
{
    delete ui;
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

//Stara
/*
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
*/

void Procesor::promjena_resempliraj(double samplingTime, bool resempliraj,
                                    double tmin1, double tmax1, bool cut1,
                                    double tmin2, double tmax2, bool cut2,
                                    double tmin3, double tmax3, bool cut3,
                                    double k, double n, bool scale)
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
        bool proslijedi_uzorak = true;
        if (xData_ul[j]<start_time) proslijedi_uzorak = false;
        if (xData_ul[j]>end_time) proslijedi_uzorak = false;
        if ((xData_ul[j]>=tmin1)&&(xData_ul[j]<tmax1)&&(cut1==true)) proslijedi_uzorak = false;
        if ((xData_ul[j]>=tmin2)&&(xData_ul[j]<tmax2)&&(cut2==true)) proslijedi_uzorak = false;
        if ((xData_ul[j]>=tmin3)&&(xData_ul[j]<tmax3)&&(cut3==true)) proslijedi_uzorak = false;

        if (proslijedi_uzorak == true)
        {
            xData_izl.push_back(xData_ul[j]);
            yData_izl.push_back(yData_ul[j]);
        }
    }

    //Ako treba skalirati
    if (scale == true)
    {
        for (int j=0; j < xData_izl.size(); j++)
        {
            yData_izl[j] = yData_izl[j]*k+n;
        }
    }

    //Ako se treba resemplirati:
    if (resempliraj == true)
    {
        //ULJEPSAJ FUNKCIJU
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
    } else
    {
        pSignal_izlazni_1->set_xData(xData_izl);
        pSignal_izlazni_1->set_yData(yData_izl);
    }

}


void Procesor::osvjezi()
{
    double start_time = getStartTime();
    if (getPromjenaOffsetTime()) start_time =startTime + getOffsetTime();
    double end_time = start_time + getDurationTime();

    //Onda izdvojiti ulazni signal na tome dijelu
    QVector<double> xData_ul = pSignal_ulazni_1->get_xData();
    QVector<double> yData_ul = pSignal_ulazni_1->get_yData();
    QVector<double> xData_izl;
    QVector<double> yData_izl;

    for (int j=0; j < xData_ul.size(); j++)
    {
        bool proslijedi_uzorak = true;

        if ((xData_ul[j]<start_time)&&(getPromjenaStartTime())) proslijedi_uzorak = false;
        if ((xData_ul[j]>end_time) &&(getPromjenaDurationTime())) proslijedi_uzorak = false;
        if ((xData_ul[j]>=tmin1)&&(xData_ul[j]<tmax1)&&(cut1==true)) proslijedi_uzorak = false;
        if ((xData_ul[j]>=tmin2)&&(xData_ul[j]<tmax2)&&(cut2==true)) proslijedi_uzorak = false;
        if ((xData_ul[j]>=tmin3)&&(xData_ul[j]<tmax3)&&(cut3==true)) proslijedi_uzorak = false;

        if (proslijedi_uzorak == true)
        {
            xData_izl.push_back(xData_ul[j]);
            yData_izl.push_back(yData_ul[j]);
        }
    }

    //Ako treba skalirati
    if (scale == true)
    {
        for (int j=0; j < xData_izl.size(); j++)
        {
            yData_izl[j] = yData_izl[j]*k+n;
        }
    }

    //Ako se treba resemplirati:
    if (resempliraj == true)
    {
        //ULJEPSAJ FUNKCIJU
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
    } else
    {
        pSignal_izlazni_1->set_xData(xData_izl);
        pSignal_izlazni_1->set_yData(yData_izl);
    }





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





//Setteri
void Procesor::setIme(QString _ime) {ime =_ime;};
void Procesor::setStartTime(double _time){startTime = _time;};
void Procesor::setDurationTime(double _time){durationTime = _time;};
void Procesor::setPromjenaStartTime(bool _promjena_start_time){promjena_start_time = _promjena_start_time;};
void Procesor::setPromjenaOffsetTime(bool _promjena_offset_time){promjena_offset_time = _promjena_offset_time;};
void Procesor::setPromjenaDurationTime(bool _promjena_duration_time){promjena_duration_time = _promjena_duration_time;};
void Procesor::setOffsetTime(double _time){offsetTime = _time;};
void Procesor::setSamplingTime(double _time) { samplingTime = _time; }
void Procesor::setResempliraj(bool _resempliraj) { resempliraj = _resempliraj; }
void Procesor::setTmin1(double _tmin1) { tmin1 = _tmin1; }
void Procesor::setTmax1(double _tmax1) { tmax1 = _tmax1; }
void Procesor::setCut1(bool _cut1) { cut1 = _cut1; }
void Procesor::setTmin2(double _tmin2) { tmin2 = _tmin2; }
void Procesor::setTmax2(double _tmax2) { tmax2 = _tmax2; }
void Procesor::setCut2(bool _cut2) { cut2 = _cut2; }
void Procesor::setTmin3(double _tmin3) { tmin3 = _tmin3; }
void Procesor::setTmax3(double _tmax3) { tmax3 = _tmax3; }
void Procesor::setCut3(bool _cut3) { cut3 = _cut3; }
void Procesor::setK(double _k) { k = _k; }
void Procesor::setN(double _n) { n = _n; }
void Procesor::setScale(bool _scale) { scale = _scale; }
void Procesor::setNewSignalName(const QString &_newSignalName) { newSignalName = _newSignalName; }
void Procesor::setChangeSignalName(bool _changeSignalName) { changeSignalName = _changeSignalName; }

// Getters
double Procesor::getSamplingTime() const { return samplingTime; }
double Procesor::getOffsetTime() const {return offsetTime;}
double Procesor::getStartTime() const {return startTime;}
bool Procesor::getPromjenaDurationTime() const {return promjena_duration_time;}
bool Procesor::getPromjenaStartTime() const {return promjena_start_time;};
bool Procesor::getPromjenaOffsetTime() const {return promjena_offset_time;};

double Procesor::getDurationTime() const {return durationTime;}
bool Procesor::getResempliraj() const { return resempliraj; }
double Procesor::getTmin1() const { return tmin1; }
double Procesor::getTmax1() const { return tmax1; }
bool Procesor::getCut1() const { return cut1; }
double Procesor::getTmin2() const { return tmin2; }
double Procesor::getTmax2() const { return tmax2; }
bool Procesor::getCut2() const { return cut2; }
double Procesor::getTmin3() const { return tmin3; }
double Procesor::getTmax3() const { return tmax3; }
bool Procesor::getCut3() const { return cut3; }
double Procesor::getK() const { return k; }
double Procesor::getN() const { return n; }
bool Procesor::getScale() const { return scale; }
QString Procesor::getNewSignalName() const { return newSignalName; }
bool Procesor::getChangeSignalName() const { return changeSignalName; }
