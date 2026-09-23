#include "cglm/cglm.h"

typedef struct 
{
    vec3 position;
    vec2 size;
} HitBox;

typedef struct 
{
    vec3 position;
    vec3 accelaration;
    vec3 velocity;
    vec3 friction;
    vec3 scale;
} Entity;

typedef struct {
    Entity body;
    HitBox hurtBox;
} Player;



void createEntity(Entity *e, vec3 position, vec3 friction, vec3 scale);
void updateEntityPhysics(Entity *e, float delta);

void updatePhysics(Entity *enteties, int length, float delta);
void updatePlayer(Player *p, int keys[500]);