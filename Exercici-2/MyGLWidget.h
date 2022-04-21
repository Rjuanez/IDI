#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();
    
  protected:
    virtual void paintGL ( );
    
    virtual void patrTransform();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void viewTransform ();
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void rebotaParets ();
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void tractamentGol ();
    virtual void projectTransform ();
    
    

  private:
    int printOglError(const char file[], int line, const char func[]);
    void transformParet(int i);
    void transformParet2();
    glm::vec3 escenaMaxima, escenaMinima;
    float distancia;
    bool gol, presp;
};
