#include "ansamblsignala.h"
#include <QDebug>


AnsamblSignala::AnsamblSignala(QObject *parent) : QObject(parent)
{

}

void AnsamblSignala::dodajUAnsambl(Signal *pSignal)
{
    vektor_pSignala.push_back(pSignal);
    std::cout << "Dodan signal:" << pSignal->ime().toStdString();
}

void AnsamblSignala::ispisiSveSignale()
{
    for (Signal *pSignal : vektor_pSignala) {
            qDebug() << pSignal->ime() ;//<< pSignal->ispisi_dimenzije();
    }
}

void AnsamblSignala::dodijeliMarkerValueSvimSignalima()
{
    //Prvo odredi marker value
    double MarkerValue = 0.0;
    for (Signal *pSignal : vektor_pSignala) {
        if (pSignal->isMarkerValueAssigned())
        {
            MarkerValue = pSignal->getMarkerValue();
            for (Signal *pSignal2 : vektor_pSignala) {
                //Sada svima dodijeli
                if (!(pSignal2->isMarkerValueAssigned())) {pSignal2->setMarkerValue(MarkerValue);}
            }
            emit changedMarkerValue(MarkerValue);
            break;
        }
    }

}

void AnsamblSignala::presloziVektorSignalaPoAbecedi()
{
    //Preslozi
    // Popunjavanje tabele signala
    auto vektorSignala = vektor_pSignala;

    // Poredajte vektor signala po abecednom redu prema imenu
    std::sort(vektorSignala.begin(), vektorSignala.end(), [](Signal* a, Signal* b) {
        return a->ime() < b->ime();
    });

    vektor_pSignala = vektorSignala;
}

void AnsamblSignala::citajIzMatFajla(const QString& filePath)
{
/*
    // Pretvorite QString u char* jer MatIO koristi char*
    const char* filename = filePath.toLocal8Bit().constData();

    // Otvaranje .mat datoteke za čitanje
    mat_t *mat = Mat_Open(filename, MAT_ACC_RDONLY);
//    if (mat == NULL) {
//        qDebug() << "Nije moguće otvoriti datoteku";
//        return;
//    }

    matvar_t *matvar;
    while ((matvar = Mat_VarReadNext(mat)) != nullptr) {

        Signal* pSignal = new Signal;
        pSignal->ucitajSignalIzMatlabVarijable(matvar);

        pSignal->setPointerNaProcesor(&defaultniProcesor);

        this->dodajUAnsambl(pSignal);

        // Ne zaboravite osloboditi varijablu nakon što završite s njom
        Mat_VarFree(matvar);
    }

    //Ovo je malo varanje ali nemam druge varijante
     connect(this, &AnsamblSignala::changedMarkerValue, &manipulatorProc, &ManipulacijaProcesorima::onChangedMarkerValue);

    //Po zavrsetku napraviti dodjelu markerValue na sve signale iz tog seta
    this->dodijeliMarkerValueSvimSignalima();

    this->presloziVektorSignalaPoAbecedi();

    // Zatvaranje .mat datoteke
    Mat_Close(mat);
*/
}
