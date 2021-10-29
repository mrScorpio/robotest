#include "robotest.h"
#include "ui_robotest.h"

#include <QGenericMatrix>

double a,b,c,e1,e2,e3,l1,l2,l3,m1,m2,m3;

QGenericMatrix<4,4,double> matr1, matr2, moz, mox, moy;
QGenericMatrix<1,4,double> v1,v1b,v1x,v1y,v2,v2b,v3,v3b,v3n;

robotest::robotest(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::robotest)
{
    ui->setupUi(this);

    e1 = 0;
    e2 = 45;
    e3 = 0;

    ui->e1Spin->setValue(e1);
    ui->e2Spin->setValue(e2);
    ui->e3Spin->setValue(e3);

    l1 = 6;
    l2 = 6;
    l3 = 6;

    v1.fill(0);
    v1(2,0) = l1;
    v1(3,0) = 1;

    v2.fill(0);
    v2(1,0) = l2;
    v2(2,0) = l1;
    v2(3,0) = 1;

    v3.fill(0);
    v3(1,0) = l3;
    v3(3,0) = 1;

    connect(ui->xSpin,SIGNAL(valueChanged(double)),this,SLOT(curVal()));
    connect(ui->ySpin,SIGNAL(valueChanged(double)),this,SLOT(curVal()));
    connect(ui->zSpin,SIGNAL(valueChanged(double)),this,SLOT(curVal()));
    connect(ui->e1Spin,SIGNAL(valueChanged(double)),this,SLOT(curVal()));
    connect(ui->e2Spin,SIGNAL(valueChanged(double)),this,SLOT(curVal()));
    connect(ui->e3Spin,SIGNAL(valueChanged(double)),this,SLOT(curVal()));
}

void robotest::curVal()
{
    a = ui->xSpin->value();     //задание угла поворота в базовой точке вокруг OX
    b = ui->ySpin->value();     //задание угла поворота в базовой точке вокруг OY
    c = ui->zSpin->value();     //задание угла поворота в базовой точке вокруг OZ

    e1 = ui->e1Spin->value();   //задание начального положение сервопривода M1
    e2 = ui->e2Spin->value();   //задание начального положение сервопривода M2
    e3 = ui->e3Spin->value();   //задание начального положение сервопривода M3

    m1 = b+90*c/(e2+c);         //угол корректировки положения сервопривода
    m2 = a+c;
    m3 = 90*c/(e2+c);

    ui->m1Gr->setNum(m1);
    ui->m2Gr->setNum(m2);
    ui->m3Gr->setNum(m3);

    matr1.fill(0);  //матрица поворота сегмента l3 вокруг OX с помощью m2
    matr1(0,0) = 1;
    matr1(3,3) = 1;
    matr1(1,3) = l1;
    matr1(2,3) = l2;
    matr1(1,1) = cos(qDegreesToRadians(e2 - m2));
    matr1(1,2) = -sin(qDegreesToRadians(e2 - m2));
    matr1(2,1) = sin(qDegreesToRadians(e2 - m2));
    matr1(2,2) = cos(qDegreesToRadians(e2 - m2));

    v3n.fill(0);
    v3n = matr1*v3; //поворот

    v3n(1,0) = v3n(1,0)-l2;
    v3n(2,0) = v3n(2,0)-l1;

    matr2.fill(0);  //матрица поворота сегмента l3 вокруг OY с помощью m1
    matr2(1,1) = 1;
    matr2(3,3) = 1;
    matr2(1,3) = l1;
    matr2(2,3) = l2;
    matr2(0,0) = cos(qDegreesToRadians(e1 - m1));
    matr2(2,0) = -sin(qDegreesToRadians(e1 - m1));
    matr2(0,2) = sin(qDegreesToRadians(e1 - m1));
    matr2(2,2) = cos(qDegreesToRadians(e1 - m1));

    v3n = matr2*v3n;    //исходные координаты точки контакта сегмента l3 с фиолетовой осью


    moz.fill(0);  //матрица поворота манипулятора вокруг OZ
    moz(3,3) = 1;
    moz(2,2) = 1;
    moz(0,0) = cos(qDegreesToRadians(c));
    moz(0,1) = -sin(qDegreesToRadians(c));
    moz(1,0) = sin(qDegreesToRadians(c));
    moz(1,1) = cos(qDegreesToRadians(c));

    v2b = moz*v2;   //новые координаты точки контакта сегментов l2 и l3
    v3b = moz*v3n;  //новые координаты точки контакта сегмента l3 с фиолетовой осью

    mox.fill(0);  //матрица поворота манипулятора вокруг OX
    mox(0,0) = 1;
    mox(3,3) = 1;
    mox(1,1) = cos(qDegreesToRadians(a));
    mox(1,2) = -sin(qDegreesToRadians(a));
    mox(2,1) = sin(qDegreesToRadians(a));
    mox(2,2) = cos(qDegreesToRadians(a));

    v2b = mox*v2b;
    v3b = mox*v3b;

    moy.fill(0);  //матрица поворота манипулятора вокруг OY
    moy(1,1) = 1;
    moy(3,3) = 1;
    moy(0,0) = cos(qDegreesToRadians(b));
    moy(2,0) = -sin(qDegreesToRadians(b));
    moy(0,2) = sin(qDegreesToRadians(b));
    moy(2,2) = cos(qDegreesToRadians(b));

    v2b = moy*v2b;
    v3b = moy*v3b;

    ui->v3x2->setNum(v3b(0,0));     //отображение текущих значений в окне
    ui->v3y2->setNum(v3b(1,0));
    ui->v3z2->setNum(v3b(2,0));

    ui->v3x1->setNum(v2b(0,0));
    ui->v3y1->setNum(v2b(1,0));
    ui->v3z1->setNum(v2b(2,0));

    if (a==0.0 && b==0.0 && c==0.0)
    {
        ui->v3x2_2->setNum(v3b(0,0));   //отображение исходного положения в окне
        ui->v3y2_2->setNum(v3b(1,0));
        ui->v3z2_2->setNum(v3b(2,0));

        ui->v3x1_2->setNum(v2b(0,0));
        ui->v3y1_2->setNum(v2b(1,0));
        ui->v3z1_2->setNum(v2b(2,0));
    }

}

robotest::~robotest()
{
    delete ui;
}
