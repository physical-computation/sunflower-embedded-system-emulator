#ifndef RADIOWIDGET_INCLUDED
#define RADIOWIDGET_INCLUDED

#include <qbuttongroup.h>
#include <vector.h>

class RadioWidget:public QButtonGroup{
  Q_OBJECT
public:
  RadioWidget(const char* title, QWidget* parent);
  
  int addValue(QString value);
  void setValue(int id);
  void setValue(QString);

  void setDefault(int id);
  
  void reset();
    

  QString currentValue();
  QString currentLabel();

private:
  QString title;
  std::vector< QString > values;
  int dfault;

};

#endif
