#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWidget>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    pix = QPixmap(800,600);
    pix.fill(Qt::white);
    b1 = new QPushButton("获取闭合曲线",this);
    b1->setGeometry(QRect(70,20,115,32));
    connect(b1,SIGNAL(clicked()),this,SLOT(get_curve()));

    b2 = new QPushButton("移动过程",this);
    b2->setGeometry(QRect(290,20,115,32));
    connect(b2,SIGNAL(clicked()),this,SLOT(move()));

    b3 = new QPushButton("分步进行",this);
    b3->setGeometry(QRect(490,20,115,32));
    connect(b3,SIGNAL(clicked()),this,SLOT(slow_move()));

    num = 0;
    count = 0;
    mode2 = false;
    j = 1;//initialize
    color = false;

}


void Widget::paintEvent(QPaintEvent *)
{
      QPainter pp(&pix);
      QPainter painter(this);
      if(color)
      {
          pp.setPen(Qt::blue);
      }
      else
          pp.setPen(Qt::red);
      //pen color

      //mark the point
      if(!lastPoint.isNull())
          pp.drawEllipse(lastPoint,2,2);

      if(lastPoint!=endPoint)
      {
          pp.drawLine(lastPoint,endPoint);
          lastPoint = endPoint;
      }

      if(mode1 == true)
      {
          pp.drawLine(endPoint,Point0);
          mode1 = false;
          mode2 = true;
      }

    if(mode3==true)
    {
        pix.fill(Qt::white);
        pp.setPen(Qt::black);//use black line

        for(int nt=0;nt<n-1;nt++)
              {
           pp.drawLine(xl[nt],yl[nt],xl[nt+1],yl[nt+1]);
              }
              pp.drawLine(xl[0],yl[0],xl[n-1],yl[n-1]);

        for(int nt=0;nt<n-1;nt++)
        {
     pp.drawLine(x0[nt],y0[nt],x0[nt+1],y0[nt+1]);
        }
        pp.drawLine(x0[0],y0[0],x0[n-1],y0[n-1]);




        for(int nt = 0;nt<n-1;nt++)
        {
            pp.drawLine(xt[nt],yt[nt],
                        xt[nt+1],yt[nt+1]);
        }
        pp.drawLine(xt[0],yt[0],xt[n-1],yt[n-1]);

    }

   painter.drawPixmap(0,0,pix);
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {

        if(mode2==false)
        {
            endPoint = event->pos();
            xl[num] = endPoint.x();
            yl[num] = endPoint.y();
            num++;
            count++;
            if(lastPoint.isNull())
            {
                lastPoint = endPoint;
                Point0 = endPoint;
            }
            update();
        }
        else if(mode2==true)
        {
            endPoint = event->pos();
            x0[num] = endPoint.x();
            y0[num] = endPoint.y();

            if(count == 0)
            {
                lastPoint = endPoint;
                Point0 = endPoint;
            }
            num++;
            count++;
           update();
        }

    }

}

void Widget::get_curve()
{
    n = count;
    count = 0;
    num = 0;
    mode1 = true;
    color = !color;

    update();

}

/*void Widget::on_pushButton_4_clicked()
{
    //mode = 1;
    while(n>0)
    {
        x0[n]=0;
        y0[n]=0;
        xl[n]=0;
        yl[n]=0;
        xt[n]=0;
        yt[n]=0;
        n--;
    }
    ui->setupUi(this);
    pix.fill(Qt::white);
    update();
}*/


void Widget::move()
{
    m = ui->spinBox->value();
    if(n == count)
        {
            mode1 = true;
            mode2 = false;
            timer = new QTimer;
            connect(timer,SIGNAL(timeout()),this,SLOT(slow_move()));
            timer->start(100);
            update();

        }
    else
        qDebug()<<"point error!";
    //
}


void Widget::slow_move()
{
    m = ui->spinBox->value();
    if(j<=m)
    {
            t=(float)j/m;
            for(i=0;i<n;i++)
            {
                xt[i]=(1-t)*x0[i]+t*xl[i] ;
                yt[i]=(1-t)*y0[i]+t*yl[i] ;
            }
            j++;
            mode3 = true;
            update();
    }
    else
        timer->stop();
}


Widget::~Widget()
{
    delete ui;
}
