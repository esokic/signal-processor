#ifndef MYCUSTOMPLOT_H
#define MYCUSTOMPLOT_H

#include <QObject>
#include <QMouseEvent>
#include <qcustomplot.h>


class MyCustomPlot : public QCustomPlot {
    Q_OBJECT
public:
    MyCustomPlot(QWidget* parent = nullptr) : QCustomPlot(parent), cursorLine1(nullptr), cursorLine2(nullptr)  {
        //setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // Omogući interakciju s mišem
    }

    double get_cursor1(){return cursor1;}
    double get_cursor2(){return cursor2;}

    void set_color_cursor1(QColor boja){color_cursor1 = boja;}
    void set_color_cursor2(QColor boja){color_cursor2 = boja;}

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            // Ako postoji linija, obriši je
            if (cursorLine1) {
                removeItem(cursorLine1);
                cursorLine1 = nullptr;
            }

            // Kreiraj novu vertikalnu liniju
            cursorLine1 = new QCPItemStraightLine(this);
            cursorLine1->setPen(QPen(color_cursor1)); // Postavi boju linije

            // Postavi poziciju linije na poziciju miša
            cursor1  = xAxis->pixelToCoord(event->pos().x());
            cursorLine1->point1->setCoords(cursor1 , 0); // Početna točka (x, 0)
            cursorLine1->point2->setCoords(cursor1 , 1); // Krajnja točka (x, 1)

            emit cursorPositionChanged(cursor1,cursor2); // Emitiraj signal s novom pozicijom

            replot(); // Osveži prikaz
        }

        if (event->button() == Qt::RightButton) {
            // Ako postoji linija, obriši je
            if (cursorLine2) {
                removeItem(cursorLine2);
                cursorLine2 = nullptr;
            }

            // Kreiraj novu vertikalnu liniju
            cursorLine2 = new QCPItemStraightLine(this);
            cursorLine2->setPen(QPen(color_cursor2)); // Postavi boju linije

            // Postavi poziciju linije na poziciju miša
            cursor2  = xAxis->pixelToCoord(event->pos().x());
            cursorLine2->point1->setCoords(cursor2 , 0); // Početna točka (x, 0)
            cursorLine2->point2->setCoords(cursor2 , 1); // Krajnja točka (x, 1)

            emit cursorPositionChanged(cursor1,cursor2); // Emitiraj signal s novom pozicijom

            replot(); // Osveži prikaz
        }




        QCustomPlot::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (cursorLine1 && (event->buttons() & Qt::LeftButton)) {
            // Ako korisnik drži lijevo dugme miša, pomjeri liniju
            cursor1  = xAxis->pixelToCoord(event->pos().x());
            cursorLine1->point1->setCoords(cursor1 , 0);
            cursorLine1->point2->setCoords(cursor1 , 1);

            emit cursorPositionChanged(cursor1,cursor2); // Emitiraj signal s novom pozicijom

            replot(); // Osveži prikaz
        }

        if (cursorLine2 && (event->buttons() & Qt::RightButton)) {
            // Ako korisnik drži lijevo dugme miša, pomjeri liniju
            cursor2  = xAxis->pixelToCoord(event->pos().x());
            cursorLine2->point1->setCoords(cursor2 , 0);
            cursorLine2->point2->setCoords(cursor2 , 1);

            emit cursorPositionChanged(cursor1,cursor2); // Emitiraj signal s novom pozicijom

            replot(); // Osveži prikaz
        }

        QCustomPlot::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && cursorLine1) {
            // Kada korisnik pusti dugme miša, linija ostaje fiksirana
            cursor1  = xAxis->pixelToCoord(event->pos().x());

            emit cursorPositionChanged(cursor1,cursor2); // Emitiraj signal s novom pozicijom

            //qDebug() << "Linija fiksirana na x =" << cursor1 ; // Ispiši x poziciju
        }

        if (event->button() == Qt::RightButton && cursorLine2) {
            // Kada korisnik pusti dugme miša, linija ostaje fiksirana
            cursor2  = xAxis->pixelToCoord(event->pos().x());

            emit cursorPositionChanged(cursor1,cursor2); // Emitiraj signal s novom pozicijom

            //qDebug() << "Linija fiksirana na x =" << cursor1 ; // Ispiši x poziciju
        }

        QCustomPlot::mouseReleaseEvent(event);
    }


signals:
    void cursorPositionChanged(double t1, double t2); // Signal za promjenu pozicije linije




private:
    QCPItemStraightLine* cursorLine1; // Pokazivač na vertikalnu liniju
    QCPItemStraightLine* cursorLine2; // Pokazivač na vertikalnu liniju
    double cursor1 = 0.0;  //pozicija kursora 1
    double cursor2 = 0.0;  //pozicija kursora 2

    QColor color_cursor1 = Qt::red;
    QColor color_cursor2 = Qt::blue;
};

#endif // MYCUSTOMPLOT_H
