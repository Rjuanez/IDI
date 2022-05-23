// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    
    virtual void initializeGL();
    virtual void modelTransformPatricio();
    
    void changeCamara();
    void changeFocoE();
    void changeFocoP();
    
    float giro;
    bool ctrlC, ctrlP, ctrlE;
    glm::vec3 centroGiro;
    glm::vec3 colCamara, colFocoE,colFocoP;
    glm::vec3 posCamara, posFocoE,posFocoP;
    GLuint posCamaraMagentaLoc, posFocoEscenaLoc, posFocoPatricioLoc;
    GLuint colCamaraMagentaLoc, colFocoEscenaLoc, colFocoPatricioLoc;
    
  private:
    int printOglError(const char file[], int line, const char func[]);
};
