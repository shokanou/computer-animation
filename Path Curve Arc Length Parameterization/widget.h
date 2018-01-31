#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <spline.h>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>

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

    int npoints;
    bool flag;
    bool flag1;
    double tension;
    int grain;
    double x[100];
    double y[100];
    QPushButton *b1,*b2,*b3;

private slots:
   void get_curve();
   void move();
   void reset();


   //void on_pushButton_clicked();

private:
    Ui::Widget *ui;
    QLabel *obj;
    QPropertyAnimation *anim1;
};

#endif // WIDGET_H
