#include "camera.h"

void constractMVPMatrix(Camera cam, Model m, mat4 destination)
{   
    mat4 dest;
    glm_mul(cam.projectionMatrix, cam.viewMatrix, dest);
    glm_mul(dest, m.modelMatrix, destination);
}

void constructCamera(vec3 position, vec3 target, vec3 up, Camera *cam)
{
    memcpy(cam->position, position, sizeof(vec3));
    memcpy(cam->target, target, sizeof(vec3));
    memcpy(cam->up, up, sizeof(vec3));

}
