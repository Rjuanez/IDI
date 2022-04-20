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

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Patricio
  glBindVertexArray (VAO_Patr);
  patrTransform();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pilota
  glBindVertexArray (VAO_Pil);
  pilTransform();
  glDrawArrays(GL_TRIANGLES, 0, pil.faces().size()*3);

  // Cub
  glBindVertexArray (VAO_Cub);
  transformParet(0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  transformParet(1);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  transformParet2();
  glDrawArrays(GL_TRIANGLES, 0, 36);
    
  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::transformParet(int i){
    glm::mat4 TG(1.0f);
    if ( i == 0)TG = glm::translate(TG, glm::vec3(0, 0, -6.9));
    else TG = glm::translate(TG, glm::vec3(0, 0, 6.9));
    
    TG = glm::scale(TG, glm::vec3(20, 2, 0.2));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::transformParet2(){
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(-9.9, 0, 0));
    TG = glm::scale(TG, glm::vec3(0.2, 2, 14));
    glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::patrTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posPorter);
  //    TG = glm::scale(TG, glm::vec3(1/escalaPatr, 1/escalaPatr, 1/escalaPatr));
  TG = glm::translate(TG, -centreBasePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::calcularAlcadaPatr () {
    
}
void MyGLWidget::iniCamera(){

  obs = glm::vec3(14, 4, 14);
  vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(0, 1, 0);
  fov = float(M_PI)/4.0f;
  ra  = 1.0;
  znear =  10;
  zfar  = 30;

  viewTransform();
  projectTransform();
}

void MyGLWidget::viewTransform () {
   glm::mat4 View(1.0f);
   //   View = glm::lookAt (obs, vrp, up);
    View = glm::translate(View, glm::vec3(0.0, 0.0, -30));
    View = glm::rotate(View, 45.0f, glm::vec3(1, 0, 0));
    View = glm::rotate(View, 0.0f, glm::vec3(0, 1, 0));
    View = glm::translate(View, -vrp);
    
   glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}
