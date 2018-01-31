#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include "spline.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    QPixmap pix;
    QPoint lastPoint;
    QPoint endPoint;
    int n;
    int grain;
    bool mode;
    double tension;
    double x[100];
    double y [100];
    CSpline *cspline;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
