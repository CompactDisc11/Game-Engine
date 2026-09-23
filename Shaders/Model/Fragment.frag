#version 430 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gShadows;


in vec3 Normal;
in vec3 FragPos;
in vec3 TexCoord;

uniform sampler2D textures[32];
uniform sampler2D tex;



void main()
    {

      //Fix UV
    gNormal.rgb = normalize(Normal);
    gNormal.a = 1.0;
    gPosition.rgb = FragPos;
    gPosition.a = 1.0;
    gAlbedoSpec.rgb = vec3(TexCoord.z/22.0);
      gAlbedoSpec.a = 1.0;
     if (TexCoord.z > -1.0){
      gAlbedoSpec.rgba = texture(textures[int(TexCoord.z)], TexCoord.xy).rgba;
      if (gAlbedoSpec.a <= 0.0)
      {
        discard;
      }
 
     }
     
    }
  