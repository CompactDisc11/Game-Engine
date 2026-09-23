#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Norm;

layout (location = 2) in vec3 aTexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 TexCoord;

uniform mat4 transform;
uniform mat4 Model;
uniform int Index;

void main()
    {
       gl_Position = transform*(vec4(aPos, 1.0));
       TexCoord = aTexCoord;
       Normal = mat3(transpose(inverse(Model)))*Norm ;
       FragPos = vec3(Model*vec4(aPos, 1.0));
    }
    