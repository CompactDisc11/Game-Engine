#version 430 core

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 18) out;



in VS_OUT {
      vec3 Normal;
      vec3 FragPos;
      vec3 TexCoord;
} gs_in[];

out vec3 Normal;
out vec3 FragPos;
out vec3 TexCoord;

uniform mat4 transform;
uniform vec4 gLightPos;



void emitQuad(int StartIndex, int EndIndex)
{
    vec3 p1 = gs_in[StartIndex].FragPos;
    vec3 p2 = gs_in[EndIndex].FragPos;

    gl_Position = transform* gl_in[StartIndex].gl_Position;
    Normal = vec3(0.0);
    FragPos = vec3(0.0);
    TexCoord = vec3(0.0);

    EmitVertex();

    gl_Position = transform* gl_in[EndIndex].gl_Position;
    Normal = vec3(0.0);
    FragPos = vec3(0.0);
    TexCoord = vec3(0.0);
    EmitVertex();


    vec3 lightDir = normalize(p1.xyz-gLightPos.xyz);
    gl_Position = transform* vec4(lightDir, 0.0);
    Normal = vec3(0.0);
    FragPos = vec3(0.0);
    TexCoord = vec3(0.0);
    EmitVertex();

    lightDir = normalize(p2.xyz-gLightPos.xyz);
    gl_Position = transform* vec4(lightDir, 0.0);
    Normal = vec3(0.0);
    FragPos = vec3(0.0);
    TexCoord = vec3(0.0);
    EmitVertex();

    
    EndPrimitive();
}



//https://ogldev.org/www/tutorial39/tutorial39.html

void main()
{                                                 //                 e4
    vec3 e1 = gs_in[2].FragPos - gs_in[0].FragPos;//       1------2------3
    vec3 e2 = gs_in[4].FragPos - gs_in[0].FragPos;//        \     /\     /
    vec3 e3 = gs_in[1].FragPos - gs_in[0].FragPos;//       e3\e1 /  \e5 /
    vec3 e4 = gs_in[3].FragPos - gs_in[2].FragPos;//          \ /    \ /
    vec3 e5 = gs_in[4].FragPos - gs_in[2].FragPos;//           0------4
    vec3 e6 = gs_in[5].FragPos - gs_in[0].FragPos;//            \ e2 /
                                                  //           e6\  /
                                                  //              \/
                                                  //              5

    vec3 n = cross(e1,e2);                              //center triangle
    vec3 LightDir = gLightPos.xyz - gs_in[0].FragPos;

                                                        //dot less than zero, face is facing away from light
                                                        //dot greather than zero, face is facing the light

    if (dot(n, LightDir) > 0.00001) {

        n = cross(e3,e1);                               //top left triangle

        if (dot(n, LightDir) <= 0) {                    //facing away from the light
            emitQuad(0, 2);
        }

        n = cross(e4,e5);                               //top right triangle
        LightDir = gLightPos.xyz - gs_in[2].FragPos;    
    
        if (dot(Normal, LightDir) <=0) {                //facing away from the light
            emitQuad(2, 4);
        }

        n = cross(e2,e6);                               //bottom triangle
        LightDir = gLightPos.xyz - gs_in[4].FragPos;

        if (dot(n, LightDir) <= 0) {                    //facing away from the light
            emitQuad(4, 0);
        }

        gl_Position = transform* gl_in[0].gl_Position;
        Normal = vec3(0.0);
        FragPos = vec3(0.0);
        TexCoord = vec3(0.0);
        EmitVertex();

        gl_Position = transform* gl_in[2].gl_Position;
        Normal = vec3(0.0);
        FragPos = vec3(0.0);
        TexCoord = vec3(0.0);
        EmitVertex();

        gl_Position = transform* gl_in[4].gl_Position;
        Normal = vec3(0.0);
        FragPos = vec3(0.0);
        TexCoord = vec3(0.0);
        EmitVertex();
        EndPrimitive();

        
    }

} 