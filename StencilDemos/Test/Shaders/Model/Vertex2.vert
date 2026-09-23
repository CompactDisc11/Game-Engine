#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Norm;

layout (location = 2) in vec3 aTexCoord;

out VS_OUT {
      vec3 Normal;
      vec3 FragPos;
      vec3 TexCoord;
} vs_out;

uniform mat4 transform;
uniform mat4 Model;
uniform int Index;





void main()
    {
       gl_Position = transform*(vec4(aPos, 1.0));
       vs_out.TexCoord = aTexCoord;
       vs_out.Normal = mat3(transpose(inverse(Model)))*Norm ;
       vs_out.FragPos = vec3(Model*vec4(aPos, 1.0));
    }
