#include "widget.h"
#include "ui_widget.h"
#include "spline.h"

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPropertyAnimation>

#include <iostream>



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    npoints = 0;
    tension = 0.0;
    grain = 0;
    flag = false;
    ui->setupUi(this);


    resize(800,600);//窗口大小
    pix = QPixmap(800,600);//画布大小
    pix.fill(Qt::white);//画布颜色
    QPixmap objpic = QPixmap(":/umi.png").scaled(60,60);
    obj = new QLabel(this);
    obj->setPixmap(objpic);
    anim1 = new QPropertyAnimation(obj,"pos");
    obj->hide();

    b1 = new QPushButton("获取控制曲线",this);
    b1->setGeometry(QRect(250,530,115,32));
    connect(b1,SIGNAL(clicked()),this,SLOT(get_curve()));

    b2 = new QPushButton("开始移动",this);
    b2->setGeometry(QRect(420,530,115,32));
    connect(b2,SIGNAL(clicked()),this,SLOT(move()));

    b3 = new QPushButton("清除",this);
    b3->setGeometry(QRect(590,530,115,32));
    connect(b3,SIGNAL(clicked()),this,SLOT(reset()));


}

void Widget::get_curve()
{
    update();
    tension = ui->tension->value();
    grain = ui->grain->value();
    GetCardinalMatrix(tension);
    Spline(x,y,npoints,grain);
    ControlPoint(npoints+2,grain);


    flag = true;
    this->update();
}

void Widget::reset()
{
    flag = false;
    npoints = 0;
    obj->hide();

    pix.fill(Qt::white);
    update();
}

void Widget::move()
{
    obj->show();
    anim1->setDuration(5000);
    double u = 1.0f / grain;
    for(int i=0; i < grain; i++)
    {
        anim1->setKeyValueAt(u*i, QPoint(pointdraw[i].x-30, pointdraw[i].y-30));
    }

    anim1->setKeyValueAt(1, QPoint(pointdraw[grain].x-30, pointdraw[grain].y-30));
    anim1->start();
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter pp(&pix);
    if(flag == false)
    {
        pp.setPen(QPen(Qt::black,1));
        pp.drawLine(lastPoint, endPoint);
        lastPoint = endPoint;
        QPainter painter(this);
        painter.drawPixmap(0,0,pix);
    }
    else
    {
        pp.setPen(QPen(Qt::red,1.8));

        for(int j = 0; j < ((npoints-1)*grain+1); j++)
        {
            pp.drawLine(draw[j].x,draw[j].y,
                        draw[j+1].x,draw[j+1].y);
        }
        //QPainter painter(this);
        //painter.drawPixmap(0,0,pix);

        //control points
        pp.setPen(QPen(Qt::blue,2));
        for(int i=0; i < grain; i++)
        {
            pp.drawEllipse(pointdraw[i].x-3, pointdraw[i].y-3,3,3);
        }
        pp.drawEllipse(pointdraw[grain-1].x-3, pointdraw[grain-1].y-3,3,3);

        QPainter painter(this);
        painter.drawPixmap(0,0,pix);
    }

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if(npoints == 0)
        {
            lastPoint = event->pos();
            endPoint = lastPoint;
            x[0] = lastPoint.x();
            y[0] = lastPoint.y();
            npoints++;
        }
        else if(npoints < 100)
        {
            endPoint = event->pos();
            x[npoints] = endPoint.x();
            y[npoints] = endPoint.y();
            npoints++;
        }
        else if(npoints >= 100)
        {
        QLabel *labelerror = new QLabel("TOO MANY!");
        labelerror->show();
        }
    }
    update();
}


Widget::~Widget()
{
    delete ui;
}


