#include "widget.h"
#include "ui_widget.h"
#include "spline.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QWidget>
#include <iostream>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    n =0;
    tension =0.0;
    grain = 0;//initialize
    mode = false;
    ui->setupUi(this);
    pix = QPixmap(800,600);//canvas size
    pix.fill(Qt::white);//fill color of canvas
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    grain = ui->spinBox->value();//get the value of grain
    tension = ui->doubleSpinBox->value(); //get the value of tension
    cspline = new CSpline(x, y, grain, tension, n);//use cspline to get the curve
    mode = true;
    this->update();
}
void Widget::on_pushButton_2_clicked()
{
    mode = false;
    while(n>0)
    {
        x[n]=0;
        y[n]=0;
        n--;
        //std::cout<<n<<std::endl;
    }
    delete cspline;
    ui->setupUi(this);
    pix.fill(Qt::white);
    update();

}

void Widget::paintEvent(QPaintEvent *)
{
      QPainter pp(&pix);
    if( mode == false)
    {
        pp.setPen(Qt::black);
        pp.drawLine(lastPoint,endPoint);
        lastPoint = endPoint;
        QPainter painter(this);
        painter.drawPixmap(0,0,pix);
       }
    else
    {
        pp.setPen(Qt::blue);
        for(int i = 0; i < ((n-1)*grain)+1; i++)
        {
            pp.drawLine(cspline->Spline[i].x,cspline->Spline[i].y,
                        cspline->Spline[i+1].x,cspline->Spline[i+1].y);
        }
         QPainter painter(this);
         painter.drawPixmap(0,0,pix);
    }
}

void Widget::mousePressEvent(QMouseEvent *event)
{
   if(event->buttons() & Qt::LeftButton)
    {
       if(n == 0)
       {
           lastPoint = event->pos();
           endPoint = lastPoint;
           x[0] = lastPoint.x();
           y[0] = lastPoint.y();
           n++;
       }
       else if( n < 100)
        {
            endPoint = event->pos();
            x[n] = endPoint.x();
            y[n] = endPoint.y();
            n++;
        }
        else if (n >= 100)
        {
            QLabel *labelerror = new QLabel("TOO MANY CONTROL POINTS!");
            labelerror->show();
       }
    }
   update();
}
