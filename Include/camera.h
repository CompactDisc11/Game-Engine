#pragma once

#include  "Model.h"
#include "cglm/cglm.h"
typedef struct
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
    
    vec3 position;
    vec3 target;
    vec3 up;

} Camera;


void constractMVPMatrix(Camera cam, Model m, mat4 destination);
void constructCamera(vec3 position, vec3 target, vec3 up, Camera *cam);