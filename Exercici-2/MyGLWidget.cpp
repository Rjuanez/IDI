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
  if (not gol) {
      glBindVertexArray (VAO_Pil);
      pilTransform();
      glDrawArrays(GL_TRIANGLES, 0, pil.faces().size()*3);
  }

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
  TG = glm::scale(TG, glm::vec3(altPorter*escalaPatr, altPorter*escalaPatr, altPorter*escalaPatr));
  TG = glm::rotate(TG, -float(M_PI)/2, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBasePatr);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniCamera(){
  
  obs = glm::vec3(0, distancia, 0);
  vrp = glm::vec3(0, 0, 0);
  up = glm::vec3(-1, 0, 0);
  fov = 2.0*asin(radiEscena/distancia);
  ra  = 1.0;
  znear =  distancia - radiEscena;
  zfar  = distancia + radiEscena;;

  viewTransform();
  projectTransform();
}

void MyGLWidget::iniEscena()
{
  presp = true;
  gol = false;
  escenaMinima = glm::vec3(-10,0,-7);
  escenaMaxima = glm::vec3(10,altPorter,7);
  radiEscena = distance(escenaMinima,escenaMaxima)/2.0;
  centreEscena = glm::vec3(0,0,0);
  distancia = radiEscena*2.0;

  altPorter = 4;
  posPorter = glm::vec3(9.0, 0.0, 0.0);  // posició inicial del porter
  posPilota = glm::vec3(7.0, 0.0, 0.0);  // posició inicial de la pilota
  dirInicialPilota();    // direcció inicial de la pilota
}

void MyGLWidget::viewTransform () {
   glm::mat4 View(1.0f);
    if (presp) {
        View = glm::translate(View, glm::vec3(0.0, 0.0, -(distancia)));
        View = glm::rotate(View, float(M_PI)/4+factorAngleY, glm::vec3(1, 0, 0));
        View = glm::rotate(View, 0.0f+factorAngleX, glm::vec3(0, 1, 0));
        View = glm::translate(View, -centreEscena);
    }
    else {
        View = glm::lookAt (obs, vrp, up);
    }
   glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
    
  factorAngleX += float(e->x() - xClick)/150.0;
  factorAngleY += float(e->y() - yClick)/150.0;

  xClick = e->x();
  yClick = e->y();
  viewTransform();

  update ();
}
void MyGLWidget::rebotaParets()
{
    //pared -6.9, 6.9
    if (posPilota[2]-0.5 <=-6.5) {
        dirPilota[2] *= -1;
        posPilota[2] = -6.3;
    }else if (posPilota[2]+0.5 >=6.5) {
        dirPilota[2] *= -1;
        posPilota[2] = 6.3;
    }
    if (posPilota[0]-0.5 <= -9.5){
        dirPilota[0] *= -1;
        posPilota[0] = -9.3;
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { // moviment de la pilota
      if (posPilota[0] == 7.0)
        mourePilota ();
      break;
    }
    case Qt::Key_I: { // reinicia posició pilota
      iniciPilota ();
      gol = false;
      break;
    }
    case Qt::Key_Right: { //
      if (posPorter[2] > -(7-(altPorter/2))) posPorter[2] -= 0.7;
      break;
    }
    case Qt::Key_Left: { //
      if (posPorter[2] < (7-(altPorter/2))) posPorter[2] += 0.7;
      break;
    }
    case Qt::Key_C: { //
        presp = not presp;
        projectTransform();
        viewTransform();
      break;
    }
    case Qt::Key_W: { //
        if (altPorter < 6) ++altPorter;
      break;
    }
    case Qt::Key_S: { //
        if (altPorter > 2) --altPorter;
      break;
    }
    case Qt::Key_R: { //
        initializeGL();
        iniciPilota();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::tractamentGol()
{
    gol = true;
  timer.stop();
}
void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if (presp) {
      if (ra < 1){
          fov = 2.0*asin(radiEscena/distancia);
          fov = 2.0*atan(tan(fov/2.0)/ra);
      }
      Proj = glm::perspective (fov, ra, znear, zfar);
  }
  else {
      if (ra < 1) Proj = glm::ortho(-radiEscena, radiEscena, -radiEscena/ra, radiEscena/ra, znear, zfar);
      else Proj = glm::ortho(-radiEscena*ra, radiEscena*ra, -radiEscena, radiEscena, znear, zfar);
  }
   
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}
