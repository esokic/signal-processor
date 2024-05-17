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


    /*
    Signal* ptest = new Signal;
    test = ptest;
    test = ansamblSignala.dajSignal(19);
    prikaz1.dodajSignaluGrupuZaPrikaz(test);
    prikaz1.osvjeziPrikaz();


    //Signal* testout = new Signal;
    Signal* ptestout = new Signal;
    testout = ptestout;
    testout->ucitajSignalIzDrugogSignala(test);

    prikaz2.dodajSignaluGrupuZaPrikaz(testout);
    prikaz2.osvjeziPrikaz();

    procesor_signala.setPointerSignalUlazni1(test);
    procesor_signala.setPointerSignalIzlazni1(testout);
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

    //Po zavrsetku napraviti dodjelu markerValue na sve signale iz tog seta
    ansamblSignala.dodijeliMarkerValueSvimSignalima();

    // Zatvaranje .mat datoteke
    Mat_Close(mat);

}

void MainWindow::on_pushButton_Refresh_clicked()
{
    double startTime = ui->doubleSpinBox_startTime->value();
    double offsetTime = ui->doubleSpinBox_offsetTime->value();
    procesor_signala.promjena_startTime(startTime + offsetTime);
    procesor_signala.promjena_durationTime(ui->doubleSpinBox_duration->value());

    double samplingTime = ui->doubleSpinBox_timeStep->value()/1000.0;
    bool resemplirati = ui->checkBox_changeTimeStep->isChecked();
    double tmin1= ui->doubleSpinBox_tmin1->value();
    double tmax1 = ui->doubleSpinBox_tmax1->value();
    bool cut1 =  ui->checkBox_cutPart_1->isChecked();
    double tmin2= ui->doubleSpinBox_tmin2->value();
    double tmax2= ui->doubleSpinBox_tmax2->value();
    bool cut2 = ui->checkBox_cutPart_2->isChecked();
    double tmin3 = ui->doubleSpinBox_tmin3->value();
    double tmax3 = ui->doubleSpinBox_tmax3->value();
    bool cut3 = ui->checkBox_cutPart_3->isChecked();


    procesor_signala.promjena_resempliraj(samplingTime, resemplirati,
                                        tmin1, tmax1, cut1,
                                        tmin2, tmax2, cut2,
                                        tmin3, tmax3, cut3);

    //Stara
    //if (ui->checkBox_changeTimeStep->isChecked()) procesor_signala.promjena_vremenaSempliranja(samplingTime);


    prikaz2.osvjeziPrikaz();
}

void MainWindow::on_pushButton_AutoReadMarker_clicked()
{
    ui->doubleSpinBox_startTime->setValue(procesor_signala.getStartTimeFromMarkerValue());
}

void MainWindow::onListWidgetItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    if (current) {
        Signal *selectedSignal = static_cast<Signal*>(current->data(Qt::UserRole).value<void*>());
        // Sada možeš koristiti selectedSignal kako ti je potrebno

        Signal* ptest = selectedSignal;
        test = ptest;
        //test = ansamblSignala.dajSignal(19);
        prikaz1.ocistiPrikaz();
        prikaz1.dodajSignaluGrupuZaPrikaz(test);
        prikaz1.osvjeziPrikaz();


        //Signal* testout = new Signal;
        Signal* ptestout = new Signal;
        testout = ptestout;
        testout->ucitajSignalIzDrugogSignala(test);

        prikaz2.ocistiPrikaz();
        prikaz2.dodajSignaluGrupuZaPrikaz(testout);
        prikaz2.osvjeziPrikaz();

        procesor_signala.setPointerSignalUlazni1(test);
        procesor_signala.setPointerSignalIzlazni1(testout);
    }
}


