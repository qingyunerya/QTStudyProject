#include "widget.h"
#include "ui_widget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;

}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(255,0,0,255));
    pen.setStyle(Qt::DotLine);
    QBrush brush;
   // brush.setColor(QColor(255,255,0));
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    QRectF rectangle(10.0,20.0,80.0,60.0);
    int startAngle=30*60;
    int spanAngle=120*16;
    painter.drawArc(rectangle,startAngle,spanAngle);
    //painter.drawRect(50,50,100,100);
    QLinearGradient linearGradient(QPointF(40,190),QPointF(70,190));
    linearGradient.setColorAt(0,Qt::yellow);
    linearGradient.setColorAt(0.5,Qt::red);
    linearGradient.setColorAt(1,Qt::green);
    linearGradient.setSpread(QGradient::RepeatSpread);
    painter.setBrush(linearGradient);
    painter.drawRect(100,100,90,90);
    QRadialGradient radialGradient(QPointF(100,190),50,QPointF(275,200));
    radialGradient.setColorAt(0,QColor(255,255,100,150));
    radialGradient.setColorAt(1,QColor(0,0,0,50));
    radialGradient.setSpread(QGradient::RepeatSpread);
    painter.setBrush(radialGradient);
    painter.drawEllipse(QPointF(100,200),50,50);
    QConicalGradient conicalGradient(
                        QPointF(250, 190), 60);
    conicalGradient.setColorAt(0.2, Qt::cyan);
    conicalGradient.setColorAt(0.9, Qt::black);
    painter.setBrush(conicalGradient);
    painter.drawEllipse(QPointF(250, 200), 50, 50);
}
