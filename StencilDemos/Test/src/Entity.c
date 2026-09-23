#include "Entity.h"
#include "SDL2/SDL.h"

void createEntity(Entity *e, vec3 position, vec3 friction, vec3 scale)
{
    memcpy(e->position, position, sizeof(vec3));
    memcpy(e->friction, friction, sizeof(vec3));
    memcpy(e->scale, scale, sizeof(vec3));
    memcpy(e->velocity, (vec3){0, 0, 0}, sizeof(vec3));
    memcpy(e->accelaration, (vec3){0, 0, 0}, sizeof(vec3));
    memcpy(e->friction, (vec3){.1, .1, .1}, sizeof(vec3));



}
void updateEntityPhysics(Entity *e, float delta)
{

    glm_vec3_add(e->velocity, e->accelaration, e->velocity);
    glm_vec3_add(e->position, e->velocity, e->position);
    glm_vec3_mul(e->velocity, e->friction, e->velocity);
    
    
}

void updatePhysics(Entity *entities, int length, float delta)
{
    for (int i= 0; i < length; i++)
    {
        updateEntityPhysics(&entities[i], delta);
    }
}



void updatePlayer(Player *p, int keys[500])
{
    p->body.accelaration[2] = 0.0;
    p->body.accelaration[0] = 0.0;

    if (keys[SDL_SCANCODE_W] == 1)
    {
        
      
        p->body.accelaration[2] = -1.0*0.16;
    }
    if (keys[SDL_SCANCODE_A] == 1)
    {
        
      
        p->body.accelaration[0] = -1.0*0.16;
    }
    if (keys[SDL_SCANCODE_D] == 1)
    {
        
      
        p->body.accelaration[0] = 1.0*0.16;
    }
    if (keys[SDL_SCANCODE_S] == 1)
    {
        
      
        p->body.accelaration[2] = 1.0*0.16;
    }
}