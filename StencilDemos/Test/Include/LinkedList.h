#pragma once
#include "OBJLoader.h"



typedef struct NodeVertex{
    struct NodeVertex *next;
    Vertex *data;
    int ID;
} NodeVertex;
typedef struct
{
   NodeVertex *start;
   NodeVertex *end;
   int count;
} NodeManager;



void createNodeManager(NodeManager *n, NodeVertex *first);

void addNode(NodeManager *manager, NodeVertex *node);
int hasNodeVertex(NodeManager *manager, Vertex *vertex);
void iterateThroughNodes(NodeManager *manager);

void generateNodeVertexList(NodeManager *manager, Vertex *verts, NodeVertex *nodes, int numberOfVerticies);
void populateArray(NodeManager *manager, Vertex *verticies);
Vertex getNodeFromIndex(NodeManager *manager, int index);

