#version 330 core


uniform vec3 posCamara;
uniform vec3 colCamara;

//Patricio
uniform vec3 posFoco2;
uniform vec3 colFoco2;

//Escena
uniform vec3 posFoco1;
uniform vec3 colFoco1;




in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    
    vec3 Lcam = posCamara - fvertex;
      Lcam = normalize(Lcam);

      vec3 Lfoco1 = posFoco1 - fvertex;
      Lfoco1 = normalize(Lfoco1);


    vec3 Lfoco2 = posFoco2 - fvertex;
      Lfoco2 = normalize(Lfoco2);



    vec3 NormSCO = normalize(fnormal);

      //Phong camara
      vec3 PhongC =  Difus(NormSCO,Lcam,colCamara ) + Especular (NormSCO,Lcam,fvertex,colCamara);
    
    //Phong patricio
    vec3 PhongP =  Difus(NormSCO,Lfoco2,colFoco2 ) + Especular (NormSCO,Lfoco2,fvertex,colFoco2);
    
    //Phong escena
    vec3 PhongE =  Difus(NormSCO,Lfoco1,colFoco1 ) + Especular (NormSCO,Lfoco1,fvertex,colFoco1);

    

      //Phong final:
      vec3 Phong = Ambient() + PhongC + PhongP + PhongE;

      FragColor = vec4(Phong,1);
}
