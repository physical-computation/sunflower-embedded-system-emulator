#include "radiowidget.h"

#include <qradiobutton.h>

RadioWidget::RadioWidget(const char* title, QWidget* parent)
  :QButtonGroup(title,parent),title(title){
  setExclusive(true);
  //setAutoResize(true);

}
  
int max(int x, int y){ return x > y ? x : y; }

int RadioWidget::addValue(QString value){
  QButton* button = new QRadioButton(value,this);
  button->setAutoResize(true);
  values.push_back(value);
  insert(button,values.size()-1);

  int max_height = 0;
  int max_width = 0;
  for(int i = 0; i < values.size(); i++){
    button = find(i);
    max_height = max(button->height(),max_height);
    max_width = max(button->width(),max_width);
  }

  max_width += 10; // pad the layout

  // Layout buttons in two columns
  for(int i = 0; i < values.size(); i++){
    button = find(i);
    button->move(10+max_width*(i%4),20+max_height*(i/4));
  }
  
  resize(20 + max_width * (values.size() > 4 ? 4 : values.size()), 30 + max_height*(values.size()/4 + (0 < values.size()%4)));

  return values.size()-1;
}


QString RadioWidget::currentValue(){
  for(int i = 0; i < values.size(); i++){
    if(find(i)->isOn()) return values[i];
  }
  return "BAD_VALUE";
}

void RadioWidget::setValue(QString value){
  for(int i = 0; i < values.size(); i++){
    if(values[i] == value){
      setValue(i);
      return;
    }
  }
  reset();
}

void RadioWidget::setValue(int id){
  setButton(id);
}
  

void RadioWidget::setDefault(int id){
  dfault = id;
  setValue(id);
}
  
void RadioWidget::reset(){
  setValue(dfault);
}
  

QString RadioWidget::currentLabel(){ return title; }
