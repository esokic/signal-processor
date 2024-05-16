#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ansamblsignala.h"

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

    prikaz1.setPointerQPlot(ui->mojCustomPlot);
    Signal* test = ansamblSignala.dajSignal(19);
    prikaz1.dodajSignaluGrupuZaPrikaz(test);
    prikaz1.osvjeziPrikaz();




    /*

    prikaz1.qplot = _plt1;
        prikaz2.qplot = _plt2;
        prikaz3.qplot = _plt3;
        prikaz4.qplot = _plt4;

        QCPLegend* legend1 = new QCPLegend(); prikaz1.legend = legend1;
        */
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

    // Zatvaranje .mat datoteke
    Mat_Close(mat);

}
