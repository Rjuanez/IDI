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
    
    vec3 L_cam_magenta = posCamara - fvertex;
      L_cam_magenta = normalize(L_cam_magenta);

      vec3 L_foco_escena = posFoco1 - fvertex;
      L_foco_escena = normalize(L_foco_escena);


    vec3 L_foco_pat = posFoco2 - fvertex;
      L_foco_pat = normalize(L_foco_pat);



    vec3 NormSCO = normalize(fnormal);

      //Phong1
      vec3 Phong1 =  Difus(NormSCO,L_cam_magenta,colCamara ) + Especular (NormSCO,L_cam_magenta,fvertex,colCamara);

    //Phong2
      vec3 Phong2 =  Difus(NormSCO,L_foco_escena,colFoco1 ) + Especular (NormSCO,L_foco_escena,fvertex,colFoco1);

    //Phong3
      vec3 Phong3 =  Difus(NormSCO,L_foco_pat,colFoco2 ) + Especular (NormSCO,L_foco_pat,fvertex,colFoco2);

      //Phong final:
      vec3 Phong = Ambient() + Phong1 + Phong2 + Phong3;

      FragColor = vec4(Phong,1);
}
