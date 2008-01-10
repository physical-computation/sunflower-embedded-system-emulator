#ifndef FORMVIEW_INCLUDED
#define FORMVIEW_INCLUDED

#include <qwidget.h>

#include <vector>

class RadioWidget;
class ImageData;
class PersonData;  

class FormView: public QWidget{
Q_OBJECT
public:

FormView(QWidget* parent);

  void storedata(PersonData*, ImageData*);
  void loaddata(PersonData*, ImageData*);

 
protected:
  void resizeEvent(QResizeEvent*);


private:

  std::vector<RadioWidget*> personform;
  std::vector<RadioWidget*> imageform;
};



#endif
