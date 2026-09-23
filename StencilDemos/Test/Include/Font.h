#pragma once
#include "cglm/cglm.h"
#include "Model.h"
#include "FontData.h"
#include "LoadShader.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
typedef struct {
    

    char letter;

    unsigned int FontSDFTexture;
    vec3 Offset;
    vec4 Region;
    Model model;
    Character c;


    

} CharModel;

void SetupText(int *VAO, int *VBO);
void RenderText(unsigned int shader, char text[], float x, float y, float Scale, int TEXTVBO, int TEXTVAO, int SDFtex);
