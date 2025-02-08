#ifndef PRIKAZ_H
#define PRIKAZ_H

#include <QObject>
#include <qcustomplot.h>
#include <signall.h>

#define GRANULACIJA_KONCANICE 1000.0

class Prikaz : public QObject
{
    Q_OBJECT
public:
    explicit Prikaz(QObject *parent = nullptr);

    void setPointerQPlot(QCustomPlot* _qplt){qplot = _qplt;}
    void dodajSignaluGrupuZaPrikaz(Signall* signal){vektor_pSignala.push_back(signal);}

    void ocistiPrikaz(){vektor_pSignala.clear();}

    void osvjeziPrikaz();

    QString getTipPrikaza(){return tip_prikaza;}
    void setTipPrikaza(QString _tp){tip_prikaza = _tp;}

    double get_initTime(){return initTime;}
    double get_durationTime(){return durationTime;}

    void set_initTime(double _t){
        initTime = _t;
        min_koncanica = initTime;
        max_koncanica = initTime+durationTime;
        if (koncanica_1<min_koncanica) koncanica_1 = min_koncanica;
        if (koncanica_1>max_koncanica) koncanica_1 = max_koncanica;
        if (koncanica_2<min_koncanica) koncanica_2 = min_koncanica;
        if (koncanica_2>max_koncanica) koncanica_2 = max_koncanica;

        azurirajGraniceKoncanice();
    }

    void set_durationTime(double _t){
        if (_t>0) {durationTime = _t;}
        max_koncanica = initTime+durationTime;
        if (koncanica_1<min_koncanica) koncanica_1 = min_koncanica;
        if (koncanica_1>max_koncanica) koncanica_1 = max_koncanica;
        if (koncanica_2<min_koncanica) koncanica_2 = min_koncanica;
        if (koncanica_2>max_koncanica) koncanica_2 = max_koncanica;
        azurirajGraniceKoncanice();
    }

    QLineEdit* lineEdit_initTime;
    QLineEdit* lineEdit_durationTime;

    double get_koncanica_1(){return koncanica_1;}
    void set_koncanica_1(double _v){koncanica_1 = _v;}
    double get_koncanica_2(){return koncanica_2;}
    void set_koncanica_2(double _v){koncanica_2 = _v;}
    double get_min_koncanica(){return min_koncanica;}
    void set_min_koncanica(double _v){min_koncanica = _v;}
    double get_max_koncanica(){return max_koncanica;}
    void set_max_koncanica(double _v){max_koncanica = _v;}


    QColor get_koncanica_1_color(){return koncanica_1_color;}
    QColor get_koncanica_2_color(){return koncanica_2_color;}
    void set_koncanica_1_color(QColor _boja){koncanica_1_color = _boja;}
    void set_koncanica_2_color(QColor _boja){koncanica_2_color = _boja;}



    void setPointerQDoubleSpinBox(QDoubleSpinBox* _qspinbox){doubleSpinBox_koncanica = _qspinbox;}
    void setPointerQSlider(QSlider* _qhorslid){horizontalSlider_koncanica = _qhorslid;}
    void setPointerQDoubleSpinBox_2(QDoubleSpinBox* _qspinbox){doubleSpinBox_koncanica_2 = _qspinbox;}
    void setPointerQSlider_2(QSlider* _qhorslid){horizontalSlider_koncanica_2 = _qhorslid;}
    void inicijalizirajKoncanicu();
    void inicijalizirajKoncanicu_2();

    void setPointerQPushbutton_koncanica1_color(QPushButton* _pB){pushButton_koncanica1_color = _pB;}
    void setPointerQPushbutton_koncanica2_color(QPushButton* _pB){pushButton_koncanica2_color = _pB;}

    void setPointerQlineEdit_initTime(QLineEdit* _l){lineEdit_initTime = _l;}
    void setPointerQlineEdit_durationTime(QLineEdit* _l){lineEdit_durationTime = _l;}


    void setPointerTableWidget_trenutnaKoncanica(QTableWidget* tab){tabelaTrenutnaKoncanica = tab;}
    //void inicijaliziraj_tabelaTrenutnaKoncanica();

    void osvjeziTabeluKoncanica();
    void PopuniMatricuKoncanica(std::vector<std::vector<QString>> &matricaSadrzajaTabelaKoncanica, std::vector<Signall*> vektor_pSignala, double t1, double t2);
    void popuniTabeluMatricomKoncanica(QTableWidget* tabela, std::vector<std::vector<QString>> matricaSadrzajaTabelaKoncanica);

    QString num2str(double value);

    void napraviSnapshotKoncanice(QString tekuciFajl);
    QString getSnapshotAnnotation(QWidget *parent);

    void osvjeziElementeNaFormi();

    void set_ptrAktivniSignal(Signall* _ptr){pAktivniSignal = _ptr;}
    Signall* get_ptrAktivniSignal(){return pAktivniSignal;}


    QCPItemText *labelica_koncanica1;
    QCPItemText *labelica_koncanica2;

    //Interakcija sa kursorom
    QCPItemRect *selectionRect = nullptr;
    QPointF startPoint; // Početna tačka
    QPointF endPoint;   // Krajnja tačka
    // Funkcija za inicijalizaciju grafa
    void setupPlot_zaInteraktivniPravougaonik();


    // Funkcije za rukovanje događajima miša
    void onMousePress(QMouseEvent *event);
    void onMouseMove(QMouseEvent *event);
    void onMouseRelease(QMouseEvent *event);
    // Funkcija za obradu selekcije
    void handleSelection(QPointF start, QPointF end);
    //Ovo je za implementaciju zoom prozora
    void showSelectedArea(QPointF start, QPointF end);





signals:



private:

       std::vector<Signall*> vektor_pSignala;

       Signall* pAktivniSignal = nullptr;

       QCustomPlot* qplot;
       QCPLegend *legend;

       QDoubleSpinBox *doubleSpinBox_koncanica;
       QSlider *horizontalSlider_koncanica;
       QDoubleSpinBox *doubleSpinBox_koncanica_2;
       QSlider *horizontalSlider_koncanica_2;
       QTableWidget *tabelaTrenutnaKoncanica;
       QPushButton* pushButton_koncanica1_color;
       QPushButton* pushButton_koncanica2_color;

       std::vector<std::vector<QString>> matricaSadrzajaTabelaKoncanica;

       QString tip_prikaza = "ul";

       //QCPAxis::ScaleType skalaXose = QCPAxis::stLogarithmic;

       /*
       double xmin = 0.0;
       double xmax = 1000.0;
       double ymin = 0.0;
       double ymax = 40000.0;
       */

       //Ovo se tice prikaza na drugom dijagramu
       double initTime = 0.0;
       double durationTime = 400.0;

       //Vezano za koncanicu
       double koncanica_1 = 0.0;
       double koncanica_2 = 0.0;
       double min_koncanica = 0.0;
       double max_koncanica = GRANULACIJA_KONCANICE;//100.0;

       QColor koncanica_1_color = Qt::green;
       QColor koncanica_2_color = Qt::blue;

       void azurirajGraniceKoncanice();
       void koncanicaOsvjezena(){
           osvjeziPrikaz();
           osvjeziTabeluKoncanica();
       }  //OVO mozda treba optimizirati


       // Lista boja koje će se koristiti za signale
       QVector<QColor> boje = {
           Qt::blue, Qt::red, Qt::green, Qt::yellow, Qt::cyan, Qt::magenta, Qt::gray,
           QColor(255, 165, 0), QColor(255, 105, 180), QColor(0, 255, 127), QColor(255, 20, 147),
           QColor(240, 128, 128), QColor(138, 43, 226), QColor(0, 255, 255), QColor(255, 0, 255),
           QColor(0, 0, 255), QColor(255, 0, 0), QColor(0, 128, 0), QColor(255, 255, 0),
           QColor(128, 0, 128), QColor(128, 128, 0), QColor(0, 128, 128), QColor(255, 215, 0),
           QColor(105, 105, 105), QColor(255, 69, 0), QColor(139, 0, 0), QColor(75, 0, 130),
           QColor(255, 215, 180), QColor(0, 191, 255), QColor(32, 178, 170), QColor(255, 99, 71),
           QColor(0, 250, 154), QColor(244, 164, 96), QColor(0, 128, 255), QColor(100, 149, 237),
           QColor(205, 92, 92), QColor(255, 140, 0), QColor(255, 182, 193), QColor(46, 139, 87),
           QColor(70, 130, 180), QColor(255, 228, 196), QColor(240, 230, 140), QColor(139, 69, 19),
           QColor(0, 255, 255), QColor(255, 182, 193), QColor(50, 205, 50), QColor(160, 82, 45)
       };

       void podesiQCPgraphZaSignal(QCPGraph*& graph, Signall* pSignal, QString tip_grafika);

};

#endif // PRIKAZ_H
