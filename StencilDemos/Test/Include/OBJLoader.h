#pragma once
#include "cglm/cglm.h"

#include "LoadShader.h"
typedef struct
{
    vec3 Position;
    vec3 Normal;
    vec3 TextureCoords;
} Vertex;


typedef struct {
    vec3 *p1;
    vec3 *p2;
    int index1;
    int index2;
} Edge;

typedef struct {
    int p1;
    int p2;
} EdgeFromIndex;
typedef struct {
    Vertex vertices[3];
} Face;
typedef struct
{
    char *path;
    char namel[1024];
    int textureID;
    int textureIndex;


} Material;

char **SplitString(char* In, char Splitter,  char Terminator);

void LoadOBJFromFile(char *FilePath, int Size, Vertex *verts, Material *mats);
int numberOfFacesInModel(char* filePath);
int countMaterials(char *filePath);
void loadTextures(char *filePath, Material *materials);
unsigned int LoadTexture(char *FilePath, int filter);
int compareVec3(vec3 v1, vec3 v2);
int compareVertex(Vertex *v1, Vertex *v2);
int getVertexPosition(Vertex *verticies, Vertex *vertex, int totalNumberOfVerticies);