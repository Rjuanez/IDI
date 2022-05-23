// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
    changeCamara();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_C: {
      off1 = !off1;
      changeCamara();
    break;
  }
  case Qt::Key_E :{
      off2 = !off2;
      changeFocoE();
    break;
	}
  case Qt::Key_P: {
      off3 = !off3;
      changeFocoP();
    break;
	}
  
  case Qt::Key_Right: {
      giro -= glm::radians(90.0f);
      modelTransformPatricio();
      changeFocoP();
    break;
	}
  case Qt::Key_Left: {
      giro += glm::radians(90.0f);
      modelTransformPatricio();
      changeFocoP();
    break;
	}
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::modelTransformPatricio(){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(5,0,2));

  // en orden: lo llevo al 0 , escalo , lo muevo segun el radio hacia atras
  // roto, devuelvo al 0,0 y lo colo en su sitio
  TG = glm::translate(TG,glm::vec3(0.0 ,0.0 , 3.0 ));
  TG = glm::rotate(TG, giro ,glm::vec3(0.0, 1.0, 0.0));
  TG = glm::translate (TG, glm::vec3(0.0 , 0.0 , -3.0 ));
  
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::initializeGL(){
  LL4GLWidget::initializeGL();

  posCamaraMagentaLoc = glGetUniformLocation (program->programId(), "posCamaraMagenta");
  colCamaraMagentaLoc = glGetUniformLocation (program->programId(), "colCamaraMagenta");

  posFocoEscenaLoc = glGetUniformLocation (program->programId(), "posFocoEscena");
  colFocoEscenaLoc = glGetUniformLocation (program->programId(), "colFocoEscena");

  posFocoPatricioLoc = glGetUniformLocation (program->programId(), "posFocoPatricio");
  colFocoPatricioLoc = glGetUniformLocation (program->programId(), "colFocoPatricio");


  off1 = off2 = off3 = false;
  giro = 0;

  centroGiro = glm::vec3 (5.0 , 3.0 , 5.0 );
  posCamara = glm::vec3 (0.0 , 0.0 , 0.0);
  posFocoE = glm::vec3 (10.0 , 3.0 , 5.0);
  posFocoP = glm::vec3 (5.0 , 3.0 , 2.0 );

  changeCamara();
  changeFocoE();
  changeFocoP();
  
}


void MyGLWidget::changeCamara(){

  glm::vec3 posCamaraMagenta = posCamara;
  glUniform3fv(posCamaraMagentaLoc, 1, &posCamaraMagenta[0]);

  // si no esta apagado mando el color normal
  if( !off1 ) colCamara  = glm::vec3(0.9 , 0.0 , 0.9);

  else colCamara  = glm::vec3(0.0 , 0.0 , 0.0);
    
  glUniform3fv(colCamaraMagentaLoc, 1, &colCamara[0]);
  
}

void MyGLWidget::changeFocoE(){


  glm::vec3 posFocoEscena = View * glm::vec4(posFocoE,1.0);
  glUniform3fv(posFocoEscenaLoc, 1, &posFocoEscena[0]);

  // si no esta apagado mando el color normal
  if( !off2 ) colFocoE  = glm::vec3(0.9 , 0.9 , 0.9);

  else colFocoE  = glm::vec3(0.0 , 0.0 , 0.0);
    
  glUniform3fv(colFocoEscenaLoc, 1, &colFocoE[0]);
  
}


void MyGLWidget::changeFocoP(){
  // conseguir la pos del foco segun patricio;
  // la posicion y nunca varia
  // la pos x y z se calcula usando el centro (5,3,5):
    //visto desde el centro el angulo 0 corresponderia al fasor 90 de nuestra imagen
    // por tanto se resta al angulo -90 para que el 0 de la imagen corresponda a un fasor 0
    
  posFocoP.x = centroGiro.x - cos(giro-glm::radians(90.0f))*3.0f;
  posFocoP.y = centroGiro.y ;
  posFocoP.z = centroGiro.z + sin(giro-glm::radians(90.0f))*3.0f;

  glm::vec3 posEscenaPatricio =  View * glm::vec4(posFocoP,1.0);
  glUniform3fv(posFocoPatricioLoc, 1, &posEscenaPatricio[0]);

  // si no esta apagado mando el color normal
  if( !off3 ) colFocoP  = glm::vec3(0.9 , 0.9 , 0.2);

  else colFocoP  = glm::vec3(0.0 , 0.0 , 0.0);
    
  glUniform3fv(colFocoPatricioLoc, 1, &colFocoP[0]);
  
}

