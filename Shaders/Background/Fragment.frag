#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gShadows;



struct LightData {
  vec4 position;
  vec4 color;
  float constant;
  float linear;
  float quadratic;
};





uniform LightData lights[32];
uniform int numberOfLights = 1;
void main()
    {
      //FragColor = texture(screenTexture, TexCoord);
      vec3 FragPos = texture(gPosition, TexCoord).rgb;
      vec3 Normal = texture(gNormal, TexCoord).rgb;
      vec3 Albedo = texture(gAlbedoSpec, TexCoord).rgb;
      
      vec3 lighting = vec3(0.0);//max(dot(Normal, vec3(0.0, 1.0, 1.0)), 0.0)*Albedo*0.15;
      for (int i = 0; i < numberOfLights; i++){
        vec3 lightDir = normalize(lights[i].position.xyz-FragPos);
        float distance    = length(lights[i].position.xyz - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0)*Albedo*lights[i].color.rgb;

        float attenuation = 1.0/(lights[i].constant+lights[i].linear*distance+lights[i].quadratic*(distance*distance));
        lighting += diffuse*attenuation;

      }
      FragColor = (texture(gShadows, TexCoord));
      FragColor.a = 1.0;
    }