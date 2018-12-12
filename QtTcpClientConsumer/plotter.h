#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include "data.h"

class Plotter : public QWidget
{
    Q_OBJECT
private:

    std::vector<Data> dataOutput;
public:
    explicit Plotter(QWidget *parent = 0);

    void setDados(std::vector<Data> dados);

    void paintEvent(QPaintEvent *e);

signals:

public slots:

};

#endif // PLOTTER_H
