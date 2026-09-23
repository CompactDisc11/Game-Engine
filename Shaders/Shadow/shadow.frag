#version 430 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gShadows;

in vec3 Normal;
in vec3 FragPos;
in vec3 TexCoord;




void main()
    {

      //Fix UV

  
    gShadows.rgba = vec4(Normal, 1.0);

     
     
    }
  