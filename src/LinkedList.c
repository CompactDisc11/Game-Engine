#include "LinkedList.h"

void createNodeManager(NodeManager *n, NodeVertex *first){
    n->start = first;
    n->end = first;
    n->count = 0;
    first->ID = n->count;
}


void addNode(NodeManager *manager, NodeVertex *node){

    

    // glm_vec3_print(node->data.Position, stdout);
    manager->count += 1;
    node->ID = manager->count;
    
    manager->end->next = node;
    manager->end = node;
    manager->end->next = NULL;
    
    
}


int hasNodeVertex(NodeManager *manager, Vertex *vertex){
    

    NodeVertex *current = manager->start;
    while (current != NULL)
    {   
        // printf("ID: %d\n", current->ID);
        if (compareVertex(current->data, vertex) == 1){
           
            return current->ID;
        }
        current = current->next;
        
    }
   
    return -1;
}


void iterateThroughNodes(NodeManager *manager)
{      
    printf("Starting\n");

    NodeVertex *current = manager->start;
    while (current != NULL)
    {   
        printf("ID: %d\n", current->ID);
        glm_vec3_print(current->data->Position, stdout);
        glm_vec3_print(current->data->Normal, stdout);
        glm_vec3_print(current->data->TextureCoords, stdout);

        current = current->next;
        
    }
    
}

void generateNodeVertexList(NodeManager *manager, Vertex *verts, NodeVertex *nodes, int numberOfVerticies)
{
    printf("Starting node vertex list\n");
    manager->count = 0;

   
    nodes[0].ID = 0;
    // 
    nodes[0].data = &verts[0];
    manager->start = &nodes[0];
    manager->end = &nodes[0];
    

  
    for (int i = 1; i < numberOfVerticies; i++){
        nodes[i].ID = i;
       nodes[i].data = &verts[0];
        addNode(manager, &nodes[i]);
    }

   
    printf("Finish\n");

}


Vertex getNodeFromIndex(NodeManager *manager, int index){
  
    NodeVertex *current = manager->start;
    while (current != NULL)
    {   
        // printf("ID: %d\n", current->ID);
        if (current->ID == index){
      
            return *current->data;
        }
        current = current->next;
        
    }
    printf("Empty\n");
    Vertex empty;
    return empty;
}
void populateArray(NodeManager *manager, Vertex *verticies)
{
    for (int i = 0; i < manager->count; i++){
        verticies[i] = getNodeFromIndex(manager, i);
    }
}
