#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ansamblsignala.h"
#include "manipulacijaprocesorima.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QString executableDir = QCoreApplication::applicationDirPath();
    QString relativeFilePath = "../60.MAT";
    QString absoluteFilePath = QDir::cleanPath(executableDir + QDir::separator() + relativeFilePath);
    QString filePath = absoluteFilePath;// "putanja/do/vaše/datoteke.dat";

    AnsamblSignala ansamblSignala;
    citajIzMatFajla(filePath, ansamblSignala);

    ansamblSignala.ispisiSveSignale();
    ui->label_nazivFajla->setText(filePath);

    //Popunjavanje tabele signala
    //QListWidget *listWidget = new QListWidget(this);
    listWidget = ui->listWidget_Signali;

    for (auto &signal : ansamblSignala.dajVektorSignala()) {
        QListWidgetItem *item = new QListWidgetItem(signal->ime());
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(signal)));
        listWidget->addItem(item);
    }

    // Povezivanje signala sa slotom
    connect(listWidget, &QListWidget::currentItemChanged, this, &MainWindow::onListWidgetItemChanged);



    prikaz1.setPointerQPlot(ui->mojCustomPlot1);
    prikaz2.setPointerQPlot(ui->mojCustomPlot2);


    manipulatorProc.show();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::citajIzMatFajla(const QString& filePath, AnsamblSignala& ansamblSignala)
{


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

        ansamblSignala.dodajUAnsambl(pSignal);

        // Ne zaboravite osloboditi varijablu nakon što završite s njom
        Mat_VarFree(matvar);
    }

    //Po zavrsetku napraviti dodjelu markerValue na sve signale iz tog seta
    ansamblSignala.dodijeliMarkerValueSvimSignalima();

    // Zatvaranje .mat datoteke
    Mat_Close(mat);

}

void MainWindow::on_pushButton_Refresh_clicked()
{

    //Postavi odabrani procesor kao procesor za odabrani signal
    signalUnderAnalysis->setPointerNaProcesor(manipulatorProc.getPointerNaOdabraniProcesor());

    //Sada procesiraj
    signalUnderAnalysis->procesirajSignal();

    //Osvjezavanje bi trebalo ici automatski ali nema veze
    prikaz2.osvjeziPrikaz();
}



void MainWindow::onListWidgetItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    if (current) {
        Signal *selectedSignal = static_cast<Signal*>(current->data(Qt::UserRole).value<void*>());
        // Sada možeš koristiti selectedSignal kako ti je potrebno

        Signal* ptest = selectedSignal;
        signalUnderAnalysis = ptest;

        prikaz1.setTipPrikaza("ul");
        prikaz1.ocistiPrikaz();
        prikaz1.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
        prikaz1.osvjeziPrikaz();



        prikaz2.setTipPrikaza("izl");
        prikaz2.ocistiPrikaz();
        prikaz2.dodajSignaluGrupuZaPrikaz(signalUnderAnalysis);
        prikaz2.osvjeziPrikaz();

        /*
        if (manipulatorProc.getPointerNaOdabraniProcesor()!=nullptr)
        {
            manipulatorProc.getPointerNaOdabraniProcesor()->setPointerSignalUlazni1(test);
            manipulatorProc.getPointerNaOdabraniProcesor()->setPointerSignalIzlazni1(testout);
        }*/
        // OVO TREBA SREDITIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII - DA SE MOGU PRIDRUZIVATI POKAZIVACI I SL.
    }
}


