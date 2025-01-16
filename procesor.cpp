#include "procesor.h"
#include "ui_procesor.h"

#include "procesor.h"
#include <armadillo>

Procesor::Procesor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Procesor)
{
    ui->setupUi(this);
    //Defaultna vrijednost:
    ime = "---";

}

Procesor::~Procesor()
{
    delete ui;
}

void Procesor::procesiraj(QVector<double>& xData_ul, QVector<double>& yData_ul, QVector<double>& xData_izl, QVector<double>& yData_izl)
{
    xData_izl.clear();
    yData_izl.clear();

    double start_time = getStartTime();
    if (getPromjenaOffsetTime()) start_time =startTime + getOffsetTime();
    double end_time = start_time + getDurationTime();


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
/*
        arma::vec a = QVectorToArmadilloVec(xData_izl);
        arma::vec b  = QVectorToArmadilloVec(yData_izl);
        arma::vec c = QVectorToArmadilloVec(vektor_vremena);
        arma::vec d = QVectorToArmadilloVec(yData_izl_interpolirano);

        arma::interp1(a,b ,c , d);


        //Onda prelijepiti na izlaz

        xData_izl = vektor_vremena;
        yData_izl = ArmadilloVecToQVector(d);
        */
    } else
    {

    }


}
/*
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
*/
void Procesor::prikaziGUI()
{
    ui->doubleSpinBox_startTime->setValue(getStartTime());
    ui->doubleSpinBox_offsetTime->setValue(getOffsetTime());
    ui->doubleSpinBox_timeStep->setValue(getSamplingTime() * 1000.0);  // Pretpostavljamo da je vrijeme u milisekundama
    ui->doubleSpinBox_duration->setValue(getDurationTime());

    ui->checkBox_changeStartTime->setChecked(getPromjenaStartTime());
    ui->checkBox_changeOffset->setChecked(getPromjenaOffsetTime());
    ui->checkBox_changeDuration->setChecked(getPromjenaDurationTime());

    ui->checkBox_changeTimeStep->setChecked(getResempliraj());
    ui->doubleSpinBox_tmin1->setValue(getTmin1());
    ui->doubleSpinBox_tmax1->setValue(getTmax1());
    ui->checkBox_cutPart_1->setChecked(getCut1());

    ui->doubleSpinBox_tmin2->setValue(getTmin2());
    ui->doubleSpinBox_tmax2->setValue(getTmax2());
    ui->checkBox_cutPart_2->setChecked(getCut2());

    ui->doubleSpinBox_tmin3->setValue(getTmin3());
    ui->doubleSpinBox_tmax3->setValue(getTmax3());
    ui->checkBox_cutPart_3->setChecked(getCut3());

    ui->doubleSpinBox_scale_k->setValue(getK());
    ui->doubleSpinBox_scale_n->setValue(getN());
    ui->checkBox_scale->setChecked(getScale());

    ui->plainTextEdit_ProcesorName->setPlainText(getIme());
    ui->plainTextEdit_ProcesorDescription->setPlainText(getOpis());


    this->show();
}


//Setteri
void Procesor::setIme(QString _ime) {ime =_ime;};
void Procesor::setOpis(QString _opis) {opis =_opis;};
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
QString Procesor::getIme() const {return ime;}
QString Procesor::getOpis() const {return opis;};
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


void Procesor::updejtVrijednostiIzEditBoxova()
{
    // Logika za osvježavanje
    double startTime = ui->doubleSpinBox_startTime->value(); setStartTime(startTime);
    double offsetTime = ui->doubleSpinBox_offsetTime->value(); setOffsetTime(offsetTime);
    double samplingTime = ui->doubleSpinBox_timeStep->value()/1000.0; setSamplingTime(samplingTime);
    double durationTime = ui->doubleSpinBox_duration->value(); setDurationTime(durationTime);

    bool changeStartTime = ui->checkBox_changeStartTime->isChecked(); setPromjenaStartTime(changeStartTime);
    bool changeOffsetTime = ui->checkBox_changeOffset->isChecked(); setPromjenaOffsetTime(changeOffsetTime);
    bool changeDurationTime = ui->checkBox_changeDuration->isChecked(); setPromjenaDurationTime(changeDurationTime);

    bool resemplirati = ui->checkBox_changeTimeStep->isChecked(); setResempliraj(resemplirati);
    double tmin1= ui->doubleSpinBox_tmin1->value(); setTmin1(tmin1);
    double tmax1 = ui->doubleSpinBox_tmax1->value(); setTmax1(tmax1);
    bool cut1 =  ui->checkBox_cutPart_1->isChecked(); setCut1(cut1);
    double tmin2= ui->doubleSpinBox_tmin2->value(); setTmin2(tmin2);
    double tmax2= ui->doubleSpinBox_tmax2->value(); setTmax2(tmax2);
    bool cut2 = ui->checkBox_cutPart_2->isChecked(); setCut2(cut2);
    double tmin3 = ui->doubleSpinBox_tmin3->value(); setTmin3(tmin3);
    double tmax3 = ui->doubleSpinBox_tmax3->value(); setTmax3(tmax3);
    bool cut3 = ui->checkBox_cutPart_3->isChecked(); setCut3(cut3);
    double k = ui->doubleSpinBox_scale_k->value(); setK(k);
    double n = ui->doubleSpinBox_scale_n->value(); setN(n);
    bool scale = ui->checkBox_scale->isChecked(); setScale(scale);

    QString ime = ui->plainTextEdit_ProcesorName->toPlainText(); setIme(ime);
    QString opis = ui->plainTextEdit_ProcesorDescription->toPlainText(); setOpis(opis);

    // Emitiranje signala
    emit signalOsvjezi();
}

void Procesor::on_pushButton_Update_clicked()
{
    this->updejtVrijednostiIzEditBoxova();
    lastUpdateTime = QDateTime::currentDateTime();
}

void Procesor::on_pushButton_AutoReadMarker_clicked()
{
    //Ovo se prenijelo iz ansambla signala u manipulator procesima - nije pametno ali kasno je
    ui->doubleSpinBox_startTime->setValue(getStartTimeFromMarkerValue());
}
