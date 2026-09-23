#version 430 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 Normal;
in vec3 FragPos;
in vec3 TexCoord;




void main()
    {

      //Fix UV
    gNormal.rgb = vec3(1, 1, 1);
    gNormal.a = 1.0;
    gPosition.rgb = FragPos;
    gPosition.a = 1.0;
    gAlbedoSpec.rgba = vec4(1.0, 1.0, 1.0, 1.0);
     
     
    }
  