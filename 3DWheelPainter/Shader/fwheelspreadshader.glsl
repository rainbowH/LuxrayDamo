#version 330 core
out vec4 FragColor;

in vec3 Normals;  
in vec3 FragPos;  
in vec3 FragPosLoc;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float pickLength;
uniform float pickWidth;
uniform float unitLength;
uniform float unitWidth;
uniform float widthMax;

uniform float originHeight;
uniform float originMaxHeight;
uniform float originMinHeight;
uniform vec2 standardTreadMsg[1000];  //���ٲŹ���
uniform int standardTreadMsgSize;
uniform float maxGap;
vec3 GetColor(float colorChange)
{
  if(colorChange<-1)
  {
    colorChange=-1;
  }
  if(colorChange>1)
  {
    colorChange=1;
  }
  
  vec3 color=vec3(0.0,1.0,0.0);
  if(colorChange<0)
  {
    color=vec3(0.0,1.0+colorChange,0.0-colorChange);
  }
  else if(colorChange>0)
  {
    color=vec3(0.0+colorChange,1.0-colorChange,0.0);
  }
  return color;
}
int FindID(float width)
{ 
  int mid, left = 0, right = standardTreadMsgSize - 1;//������[0, n - 1]
	while (left < right)
	{
		mid = left + ((right + 1 - left) >> 1);//����ȡ��
		if (standardTreadMsg[mid].x < width) left = mid;
		else right = mid - 1;
	}

	if (standardTreadMsg[left].x <= width&&width<=standardTreadMsg[standardTreadMsgSize - 1].x)
	{
		return left;
	}
	else
	{
		return -1;
	}
}
void main()
{
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result;
    vec3 color;
    
    float originY=FragPosLoc.y+originHeight;
    //float heightRange=originMaxHeight-originMinHeight;
    //float colorChange=2.0*(originY-originMinHeight)/heightRange;
    
    float width=FragPosLoc.z;
    int id=FindID(width);
    float dy=originY-standardTreadMsg[id].y;
    
    if(abs(FragPosLoc.z-pickWidth)<=0.1)
    {
        color=vec3(0.0,0.0,0.0);
    }
    else if(abs(FragPosLoc.x-pickLength)<=0.1)
    {
        color=vec3(0.0,0.0,0.0);
    }
    else
    {
        //color=objectColor;
        if(id==-1)
        {
          color=vec3(0.0,0.0,0.0);
        }
        else if(width>widthMax)
        {
          color=vec3(0.0,1.0,0.0);
        }
        else
          color=GetColor(dy/maxGap);
        
    }
    
    //vec3 testColor=GetColor(colorChange);
    //if(abs(originHeight-0.0)<0.1)
    //{
    // color=vec3(1.0,0.0,0.0);
    //}
    
    //color=vec3(0.0,1.0,0.0);
    
    result = (ambient + diffuse + specular) * color;

    FragColor = vec4(result, 1.0);

}