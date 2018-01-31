#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QDebug>
#include <QTimer>
#include <QMouseEvent>
#include <QPushButton>

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
    QPoint Point0;
    QPushButton *b1,*b2,*b3;
    QTimer *timer;
    double x0[100], y0[100], xl[100], yl[100], xt[100], yt[100];
    bool mode1,mode2,mode3,color;
    int m;
    int n;
    float t;
    int i,j;
    int count;
    int num;
    

private slots:
   void get_curve();
   void move();
   void slow_move();


private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
