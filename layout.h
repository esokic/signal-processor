#ifndef LAYOUT_H
#define LAYOUT_H

#include <QObject>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "nlohmann/json.hpp" // JSON library: https://github.com/nlohmann/json
using json = nlohmann::json;

#include <prikaz.h>
#include <ansamblsignala.h>


class Layout : public QObject
{
    Q_OBJECT
public:
    explicit Layout(QObject *parent = nullptr);

public:

    Prikaz* prikaz;
    AnsamblSignala* pAnsamblSignala;

    void setPrtPrikaz(Prikaz* _prikaz){prikaz = _prikaz;}
    void setPtrAnsamblSignala(AnsamblSignala* _pAnsamlbSignala){pAnsamblSignala = _pAnsamlbSignala;}

    //Podaci za snimanje (kod Prikaza):
    //double initTime = 0.0;
    //double durationTime = 400.0;
   // double koncanica_1 = 0.0;
   // double koncanica_2 = 0.0;


    json to_json()  {
        //Pohrani i podesenja signala
        json signals_json = json::array();

        for (int i=0; i<pAnsamblSignala->dajVektorSignalaSize(); i++)
        {
          json signal_json = (pAnsamblSignala->dajSignal(i))->to_json();
          //  std::cout << "Signal JSON: " << signal_json.dump(4) << std::endl;
          signals_json.push_back(signal_json);
        }
        return {{"initTime", prikaz->get_initTime()},
                {"durationTime", prikaz->get_durationTime()},
                {"koncanica_1", prikaz->get_koncanica_1()},
                {"koncanica_2", prikaz->get_koncanica_2()},
                {"koncanica_1_color", prikaz->get_koncanica_1_color().name().toStdString()},
                {"koncanica_2_color", prikaz->get_koncanica_2_color().name().toStdString()},
                {"signalLayouts", signals_json}};}


    template <typename T>
    void safe_get(const json& j, const std::string& key, T& value, const T& default_value) {
        if (j.contains(key)) {
            j.at(key).get_to(value);
        } else {
            value = default_value;
        }
    }

    void from_json(const json& j) {
        double pom;
        safe_get(j, "initTime", pom, 0.0); prikaz->set_initTime(pom);
        safe_get(j, "durationTime", pom, 400.0); prikaz->set_durationTime(pom);
        safe_get(j, "koncanica_1", pom, 0.0); prikaz->set_koncanica_1(pom);
        safe_get(j, "koncanica_2", pom, 0.0); prikaz->set_koncanica_2(pom);
        std::string pom_str;
        safe_get(j, "koncanica_1_color", pom_str, QString("#00ff00").toStdString()); prikaz->set_koncanica_1_color(QColor(QString::fromStdString(pom_str)));
        safe_get(j, "koncanica_2_color", pom_str, QString("#0000ff").toStdString()); prikaz->set_koncanica_2_color(QColor(QString::fromStdString(pom_str)));

        if (j.contains("signalLayouts")) {
            for (const auto& s : j.at("signalLayouts")) {
                if (s.contains("imeSignala")) {
                        std::string imeSignala = s.at("imeSignala").get<std::string>();
                        Signall* trazenisignal = pAnsamblSignala->dajSignalPoImenu(QString::fromStdString(imeSignala));
                        if (trazenisignal!=nullptr)
                        {
                            trazenisignal->from_json(s);
                        }
                    } else {
                        //std::cout << "Signal nema imeSignala!" << std::endl;
                    }
            }

        }
        //safe_get(j, "signalLayouts", signalLayout, NULL);
    }


/*
    void export_to_file(const std::string& filename)  {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << to_json().dump(4);
            file.close();
        }
    }



    void import_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            json j;
            file >> j;
            from_json(j);
            file.close();
        }
    }
    */

    void export_to_file(const std::string& filename) {
        QString qFilename = QString::fromStdString(filename);

        QFile file(qFilename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for writing:" << file.errorString();
            return;
        }

        QTextStream out(&file);
        out << QString::fromStdString(to_json().dump(4));
        file.close();
    }

    void import_from_file(const std::string& filename) {
        QString qFilename = QString::fromStdString(filename);

        QFile file(qFilename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file for reading:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        try {
            json j = json::parse(content.toStdString());
            from_json(j);
        } catch (const std::exception& e) {
            qDebug() << "Error parsing JSON:" << e.what();
        }
    }

signals:

};

#endif // LAYOUT_H
