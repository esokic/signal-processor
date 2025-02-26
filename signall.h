#ifndef SIGNAL_H
#define SIGNAL_H

#include <QObject>
#include <matio.h>
#include "procesor.h"
#include <iostream>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>


class Signall : public QObject
{
    Q_OBJECT
public:
    explicit Signall(QObject *parent = nullptr);



    void ucitajSignalIzMatlabVarijable(matvar_t*);
    void ucitajSignalIzDrugogSignala(Signall*& signal); //prakticno kopija
    void kopirajPodesenjaIzDrugogSignala(const Signall& drugiSignal);

    //Kljucna funkcija
    void procesirajSignal(){if (pProcesor!=nullptr) {pProcesor->procesiraj(xData_ul,yData_ul,xData_izl,yData_izl);}}

   // void promijeni_startTime(double t);


    //getteri
    QString ime(){return imeSignala;}

    /*
    QString ispisi_dimenzije(){return QString::number(dimenzijePodatka[0])+"x"+
                QString::number(dimenzijePodatka[1]);}
*/

    QVector<double> get_xData_ul(){return xData_ul;}
    QVector<double> get_yData_ul(){return yData_ul;}
    QVector<double> get_xData_izl(){return xData_izl;}
    double get_xData_izl(int row){if (row < xData_izl.size()) return xData_izl[row]; else return -5000.0;}
    QVector<double> get_yData_izl(){return yData_izl;}
    double get_yData_izl(int row){if (row < xData_izl.size()) return xData_izl[row]; else return -5000.0;}


    #define br_samplova_za_prikaz 500
    QVector<double> get_xData_ul_resampled() {return resampleData(xData_ul, br_samplova_za_prikaz);}
    QVector<double> get_yData_ul_resampled() {return resampleData(yData_ul, br_samplova_za_prikaz);}
    QVector<double> get_xData_izl_resampled() {return resampleData(xData_izl, br_samplova_za_prikaz);}
    QVector<double> get_yData_izl_resampled() {return resampleData(yData_izl, br_samplova_za_prikaz);}

    void get_xData_yData_from_to(QVector<double>& xData_cutted, QVector<double>& yData_cutted, double start_time, double end_time);
    double vratiVrijednostSignalaUtrenutku(double time_x);


    QVector<double> resampleData(const QVector<double>& data, int N);


    double getMarkerValue(){return MarkerValue;}
    bool isMarkerValueAssigned(){return markerValueAssigned;}

    QString getNewName(){return novoImeSignala;}
    bool isMarkedForExport(){return oznacen_za_export;}
    bool isMarkedForPrikaz(){return oznacen_za_prikaz;}

    int get_signal_position(){return signal_position;}
    double get_signal_size(){return signal_size;}
    double get_signal_vshift(){return signal_vshift;}

    //setteri
    //Sluzi za podesavanje prikaza ovog signala
    //void podesiQCPgraph(QCPGraph*, QString tip_grafika, QColor boja); //moze biti "ul" i "izl"
    //void dodajSignalULegendu(QCPLegend*);

    //Ovdje ima problem jer se mora paziti da se sve mijenja
    void set_xData_ul(QVector<double> xd){xData_ul = xd;}
    void set_yData_ul(QVector<double> yd){yData_ul = yd;}
    void set_xData_izl(QVector<double> xd){xData_izl = xd;}
    void set_yData_izl(QVector<double> yd){yData_izl = yd;}
    void setMarkerValue(double _mv){MarkerValue = _mv; markerValueAssigned = true;}
    void set_novoIme(QString _ime){novoImeSignala = _ime;}
    void setMarkedForExport(bool _smfe){oznacen_za_export = _smfe;}
    void setMarkedForPrikaz(bool _smfe){oznacen_za_prikaz = _smfe;}

    void set_signal_position(int _pos){
        //if ((_pos>=0)&&(_pos<=100)) signal_position = _pos;
        signal_position = _pos;
    }
    void set_signal_size(double _percent){if ((_percent>0)&&(_percent<=10000)) signal_size = _percent;}
    void set_signal_vshift(double _vshift){signal_vshift = _vshift;}

    void setPointerNaProcesor(Procesor* _pProcesor){pProcesor = _pProcesor;}
    //Procesor* getPointerNaProcesor(){return pProcesor;}

    QString getTrenutniProcesorIme(){if(pProcesor!=nullptr){return pProcesor->getIme();}else{return "---------";}}
    QDateTime getTrenutniProcesorUpdateTime(){if(pProcesor!=nullptr){return pProcesor->getLastUpdateTime();}else{return QDateTime::currentDateTime();}}


    QColor getBojaSignala(){return bojaSignala;}
    void setBojaSignala(QColor _boja){bojaSignala = _boja;}

    using json = nlohmann::json;
    // Metode za JSON konverziju
    json to_json() const {
        return {{"imeSignala", imeSignala.toStdString()},
                {"oznacen_za_export", oznacen_za_export},
                {"oznacen_za_prikaz", oznacen_za_prikaz},
                {"novoImeSignala", novoImeSignala.toStdString()},
                {"signal_position", signal_position},
                {"signal_size", signal_size},
                {"signal_vshift", signal_vshift},
                {"bojaSignalaR", bojaSignala.red()},
                {"bojaSignalaG", bojaSignala.green()},
                {"bojaSignalaB", bojaSignala.blue()}};}

    // Sigurno dohvaćanje iz JSON-a
    template <typename T>
    void safe_get(const json& j, const std::string& key, T& value, const T& default_value) {
        if (j.contains(key) && !j.at(key).is_null()) {
            try {
                value = j.at(key).get<T>();
            } catch (const json::exception& e) {
                // Ako ne može konvertovati, postavi na podrazumijevanu vrijednost
                value = default_value;
            }
        } else {
            // Ako ključ ne postoji ili je null, koristi podrazumijevanu vrijednost
            value = default_value;
        }
    }

    void from_json(const json& j) {
        bool pom;
        safe_get(j, "oznacen_za_export", pom, false); setMarkedForExport(pom);
        safe_get(j, "oznacen_za_prikaz", pom, false); setMarkedForPrikaz(pom);
        std::string pom_novo_ime;
        safe_get(j, "novoImeSignala", pom_novo_ime, QString("Unnamed").toStdString()); set_novoIme(QString::fromStdString(pom_novo_ime));
        int pommie;
        safe_get(j, "signal_position", pommie, 50); set_signal_position(pommie);
        double pommie2;
        safe_get(j, "signal_size", pommie2, 100.0); set_signal_size(pommie2);
        double pommie3;
        safe_get(j, "signal_vshift", pommie3, 0.0); set_signal_vshift(pommie3);
        int bojaSignalaR(0),bojaSignalaG(0),bojaSignalaB(0);
        safe_get(j, "bojaSignalaR", bojaSignalaR, 0);
        safe_get(j, "bojaSignalaG", bojaSignalaG, 0);
        safe_get(j, "bojaSignalaB", bojaSignalaB, 0);
        setBojaSignala(QColor(bojaSignalaR, bojaSignalaG, bojaSignalaB));

    }



private:
    QString imeSignala;                 //Npr. U1
    QString tipPodatka;                 //Npr. MAT_C_DOUBLE

    bool oznacen_za_export = false;
    bool oznacen_za_prikaz= false;
    QString novoImeSignala;

    QVector<double> xData_ul;
    QVector<double> yData_ul;

    QVector<double> xData_izl;
    QVector<double> yData_izl;
    //Ogranicit cemo se na dvije dimenzije i jednake dimenzije signala

    double MarkerValue = 0.0; //Ovo je za onaj specificni fajl od KEME
    bool markerValueAssigned = false;


    //Parametri za prikaz koji su kompatibilni sa KEMA vieverom
    int signal_position = 50; //Broj koji ide u procentima i odnosi se na polozaj grafa (0 dno, 100 vrh)
    double signal_size = 100; //Skaliranje grafika (u procentima) (100% je normalno)
    double signal_vshift = 0.0;

    Procesor* pProcesor = nullptr;

    QColor bojaSignala = QColor(0, 0, 255);

signals:

};

#endif // SIGNAL_H
