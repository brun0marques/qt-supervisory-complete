#include "plotter.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>

Plotter::Plotter(QWidget *parent) :
    QWidget(parent)
{

}

void Plotter::setDados(std::vector<Data> dados)
{
    dataOutput = dados;
    repaint();
}

void Plotter::paintEvent(QPaintEvent *e){

  int x1, y1, x2, y2;

  // cria um objeto pintor
  QPainter p(this);

  // faz com que a linha seja desenhada suave
  p.setRenderHint(QPainter::Antialiasing);

  // cria um objeto de caneta
  QPen pen;
  // cria um objeto de pincel
  QBrush brush;
  // define a cor da caneta
  pen.setColor(QColor(0,0,0));
  pen.setWidth(2);

  // informa ao painter qual a caneta
  p.setPen(pen);

  // define a cor do pincel
  brush.setColor(QColor(204, 230, 255));
  brush.setStyle(Qt::SolidPattern);

  // informa oa painter qual o pincel
  p.setBrush(brush);
  p.drawRect(0,0,width(),height());
  pen.setColor(QColor(0,0,255));
  p.setPen(pen);

   if(dataOutput.size()>=2){
      int tam = dataOutput.size();
      qint64 tempoTotal = (*(dataOutput.end()-1)).tempo - (*dataOutput.begin()).tempo;
      tempoTotal /= 1000;
      float escalaX = width()/(tempoTotal);
      escalaX = (width()/(escalaX*tempoTotal)) * escalaX;

      float escalaY = height()/100;
      escalaY = (height()/(escalaY*100)) * escalaY;

      std::vector<Data>::iterator it;

      qint64 t1 = 0, t2;
      int v1=(*dataOutput.begin()).valor, v2;

      for(it = dataOutput.begin()+1; it!=dataOutput.end(); it++){
        t2 = ((*it).tempo - (*dataOutput.begin()).tempo)/1000;
        v2 = (*it).valor;

        p.drawLine(t1*escalaX, height()-(v1*escalaY), t2*escalaX, height() - (v2*escalaY));

        t1 = t2;
        v1 = v2;
      }
   }
}
