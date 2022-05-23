#version 330 core


uniform vec3 posCamaraMagenta;
uniform vec3 colCamaraMagenta;

uniform vec3 posFocoEscena;
uniform vec3 colFocoEscena;

uniform vec3 posFocoPatricio;
uniform vec3 colFocoPatricio;


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
    
    vec3 L_cam_magenta = posCamaraMagenta - fvertex;
      L_cam_magenta = normalize(L_cam_magenta);

      vec3 L_foco_escena = posFocoEscena - fvertex;
      L_foco_escena = normalize(L_foco_escena);


    vec3 L_foco_pat = posFocoPatricio - fvertex;
      L_foco_pat = normalize(L_foco_pat);



    vec3 NormSCO = normalize(fnormal);

      //Phong Camara
      vec3 PhongC =  Difus(NormSCO,L_cam_magenta,colCamaraMagenta ) + Especular (NormSCO,L_cam_magenta,fvertex,colCamaraMagenta);

    //Phong Escena
      vec3 PhongE =  Difus(NormSCO,L_foco_escena,colFocoEscena ) + Especular (NormSCO,L_foco_escena,fvertex,colFocoEscena);

    //Phong Patricio
      vec3 PhongP =  Difus(NormSCO,L_foco_pat,colFocoPatricio ) + Especular (NormSCO,L_foco_pat,fvertex,colFocoPatricio);

      //Phong final:
      vec3 Phong = Ambient() + PhongC + PhongE + PhongP;

      FragColor = vec4(Phong,1);
}
