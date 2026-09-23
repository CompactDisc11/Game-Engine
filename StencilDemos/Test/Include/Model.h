#pragma once
#include "OBJLoader.h"
#include "LinkedList.h"

typedef struct {
    vec3 Position;
    vec3 Scale;
    vec3 Rotation;
} ModelTransformData;

typedef struct {
    int ProgramID;
    int NumberOfTextures;
    int *TextureIDs;

    int modelMatrixLoc;
    int transformMatrixLoc;
    
} ShaderData;
typedef struct{

    int VAO;
    ModelTransformData transformData;
    mat4 modelMatrix;
    mat4 transformMatrix;
    ShaderData ShaderData;  
    int Size;

    int Texture1;
    int Texture2;
    Material *materials;
} Model;


typedef struct{
    int numberOfVerticies;
    int numberOfIndicies;
    int *indicies;
    Vertex *verticies;

} ModelSaveData;



void CreateModelTransofrmData(ModelTransformData *data, vec3 Position, vec3 Scale);
void CreateModelNoIndeciesFromFileData(Vertex *vert, int VCount, int ShaderProgramID, vec3 Position, vec3 Scale, Model *m);
void CreateShaderData(ShaderData *data, int ShaderProgramID);




void createModelWithIndicies(Vertex *vert, int *indicies, int vertexCount, int indexCount, int ShaderProgramID, vec3 Position, vec3 Scale, Model *m);
void LoadModelFromFile(char *FilePath, Model *m, unsigned int ModelProgram, vec3 position, vec3 scale, Material *mats);
void populateIndicies(NodeManager *manager, Vertex *verticies, unsigned int *indicies, int numberOfVerticies);
void CreateModelData(int VAO, vec3 Position, vec3 Scale, int ShaderProgramID, Model *m);
void CreateModel(float *verticies, int *indiices, int ShaderProgramID, vec3 Position, vec3 Scale, Model*m);

void findAdjacencies(int *indicies, Vertex *verts, int numberOfIndicies, int *newIndicies);

