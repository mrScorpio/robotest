#ifndef ROBOTEST_H
#define ROBOTEST_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class robotest; }
QT_END_NAMESPACE

class robotest : public QMainWindow
{
    Q_OBJECT

public:
    robotest(QWidget *parent = nullptr);

    ~robotest();

public slots:
    void curVal();

private:
    Ui::robotest *ui;
};
#endif // ROBOTEST_H
