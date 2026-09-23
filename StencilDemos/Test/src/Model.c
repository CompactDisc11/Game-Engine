#include "Model.h"
#include "OBJLoader.h"
#include "LinkedList.h"
#include <unistd.h>

void CreateModelTransofrmData(ModelTransformData *data, vec3 Position, vec3 Scale)
{
    data->Position[0] = Position[0];
    data->Position[1] = Position[1];
    data->Position[2] = Position[2];

    data->Scale[0] = Scale[0];
    data->Scale[1] = Scale[1];
    data->Scale[2] = Scale[2];


}

void CreateShaderData(ShaderData *data, int ShaderProgramID)
{
    data->ProgramID = ShaderProgramID;
}




void readModelDataFromFile(char *name, ModelSaveData *data){
	FILE *f = fopen(name, "rb");


	data->numberOfVerticies = 0;
	data->numberOfIndicies = 0;
	fread(&data->numberOfVerticies, sizeof(int), 1, f);
	fread(&data->numberOfIndicies, sizeof(int), 1, f);
	data->verticies = malloc(sizeof(Vertex)*data->numberOfVerticies);
	data->indicies = malloc(sizeof(int)*data->numberOfIndicies);

	fread(data->verticies, sizeof(Vertex), data->numberOfVerticies, f);
	fread(data->indicies, sizeof(int), data->numberOfIndicies, f);


	printf("Number of verticies %d, Number of indicies, %d\n", data->numberOfVerticies, data->numberOfIndicies);


}

void saveModelDataToFile(char *name, int *indiciesAdjacencies, Vertex *verts, int indiciesCount, int vertexCount){
	FILE *f= fopen(name, "wb");
	int data[2] = {vertexCount, indiciesCount};
	fwrite(data, sizeof(int), 2, f);
	fwrite(verts, sizeof(Vertex), vertexCount, f);
	fwrite(indiciesAdjacencies, sizeof(int), indiciesCount, f);


	fclose(f);

}
void LoadModelFromFile(char *FilePath, Model *m, unsigned int ModelProgram, vec3 position, vec3 scale, Material *mats)
{

	ModelSaveData data;
	char binPath[1024];
	strcpy(binPath, FilePath);
	strcat(binPath, ".bin");
	printf("File: %s\n", binPath);
	if (access(binPath, F_OK)==0){
		printf("File Exists!\n");
		readModelDataFromFile(binPath, &data);

		createModelWithIndicies(data.verticies, data.indicies, data.numberOfVerticies, data.numberOfIndicies, ModelProgram, position, scale, m);
		
		
		// CreateModelNoIndeciesFromFileData(Verts, numberOfVerts, ModelProgram, position, scale, m);

		m->ShaderData.modelMatrixLoc = glGetUniformLocation(m->ShaderData.ProgramID, "Model");
		m->ShaderData.transformMatrixLoc = glGetUniformLocation(m->ShaderData.ProgramID, "transform");
	}
	else{
		int Size = 0;
		int faces = numberOfFacesInModel(FilePath);
		printf("Number of faces %d\n", faces);
		int numberOfVerts = faces*3;
		printf("Number of verticies %d\n", numberOfVerts);

		Vertex Verts[numberOfVerts];
		unsigned int indicies[numberOfVerts];
		unsigned int indiciesAdjacencies[numberOfVerts*2];

		LoadOBJFromFile(FilePath, numberOfVerts,  Verts, mats);
		NodeManager manager;
		populateIndicies(&manager, Verts, indicies, numberOfVerts);
		Vertex vertsFromIndicies[manager.count];
		populateArray(&manager, vertsFromIndicies);
		
		// for (int i = 0; i < manager.count; i++){
		// 	printf("Vertex\n");
		// 	glm_vec3_print(vertsFromIndicies[i].Position, stdout);
		// 	glm_vec3_print(vertsFromIndicies[i].Normal, stdout);
		// 	glm_vec3_print(vertsFromIndicies[i].TextureCoords, stdout);

		// }
		// generateNodeVertexList(&manager, Verts, nodes, numberOfVerts);
		// iterateThroughNodes(&manager);
		// CreateModel(vertsFromIndicies, indicies, ModelProgram, position, scale, m);
		findAdjacencies(indicies, vertsFromIndicies, numberOfVerts, indiciesAdjacencies);

		saveModelDataToFile(binPath, indiciesAdjacencies,  vertsFromIndicies, numberOfVerts*2, manager.count);
	//	exit(0);
		createModelWithIndicies(vertsFromIndicies, indiciesAdjacencies, manager.count, numberOfVerts*2, ModelProgram, position, scale, m);
		
		
		// CreateModelNoIndeciesFromFileData(Verts, numberOfVerts, ModelProgram, position, scale, m);

		m->ShaderData.modelMatrixLoc = glGetUniformLocation(m->ShaderData.ProgramID, "Model");
		m->ShaderData.transformMatrixLoc = glGetUniformLocation(m->ShaderData.ProgramID, "transform");
	}

	

}



int compareEdge(Edge *e1, Edge *e2){
	int result = 0;

	if (compareVec3(*e1->p1, *e2->p1) == 1 && compareVec3(*e1->p2, *e2->p2) == 1){
		result = 1;
	}


	return result;
}

int compareEdgeWithEdges(Edge *e1, Edge *e2, Edge *e3, Edge *e4){
	int result = -1;
	if (compareEdge(e1, e2) == 1){
		result = 1;
	}
	if (compareEdge(e1, e3) == 1){
		result = 2;
	}
	if (compareEdge(e1, e4) == 1){
		result = 3;
	}
	return result;

	
}
void findAdjacencies(int *indicies, Vertex *verts, int numberOfIndicies, int *newIndicies){
	//Loop 1
	int newIndiciesIndex = 0;
	printf("Length of indicies: %d", numberOfIndicies);
	for (int i = 0; i < numberOfIndicies/3; i++){

			int index_I =  i *3;
			int v1 = indicies[index_I];
			int v2 = indicies[index_I+1];
			int v3 = indicies[index_I+2];

			Edge edge_1_1 = {&verts[v1].Position, &verts[v2].Position, v1, v2};
			Edge edge_1_2 = {&verts[v2].Position, &verts[v3].Position, v2, v3};
			Edge edge_1_3 = {&verts[v3].Position, &verts[v1].Position, v3, v1};

			Edge edges[] = {edge_1_1, edge_1_2, edge_1_3};

		
			int vertexIndexForEachSide[3] = {0, 0, 0};
			for (int side = 0; side < 3; side++){
				for (int j = 0; j < numberOfIndicies/3; j++){
					//Loop 2
					int index_J =  j *3;


					int v1_2 = indicies[index_J];
					int v2_2 = indicies[index_J+1];
					int v3_2 = indicies[index_J+2];
					
					if (v1 != v1_2 && v2 != v2_2 && v3 != v3_2){
						Edge edge_2_1 = {&verts[v1_2].Position, &verts[v2_2].Position, v1_2, v2_2};
						Edge edge_2_2 = {&verts[v2_2].Position, &verts[v3_2].Position, v2_2, v3_2};
						Edge edge_2_3 = {&verts[v3_2].Position, &verts[v1_2].Position, v3_2, v1_2};
						
						
						int result = compareEdgeWithEdges(&edges[side], &edge_2_1, &edge_2_2, &edge_2_3);
						if (result != -1){
							if (result == 1){
								vertexIndexForEachSide[side] = v3_2;
							}
							else if (result == 2){
								vertexIndexForEachSide[side] = v1_2;
							}
							else if (result == 3)
							{
								vertexIndexForEachSide[side] = v2_2;
							}	
						
						}
					
					}
				}
			}	
			int triange_adjacent[6] = {v1, vertexIndexForEachSide[0], v2, vertexIndexForEachSide[1], v3, vertexIndexForEachSide[2]};
			newIndicies[newIndiciesIndex] = v1;
			newIndicies[newIndiciesIndex+1] = vertexIndexForEachSide[0];
			newIndicies[newIndiciesIndex+2] = v2;
			newIndicies[newIndiciesIndex+3] = vertexIndexForEachSide[1];
			newIndicies[newIndiciesIndex+4] = v3;
			newIndicies[newIndiciesIndex+5] = vertexIndexForEachSide[2];
			newIndiciesIndex += 6;

		
		}
}




void populateIndicies(NodeManager *manager, Vertex *verticies, unsigned int *indicies, int numberOfVerticies){

	printf("Starting population\n");
	manager->count = 0;
	NodeVertex *start = malloc(sizeof(NodeVertex));
	manager->start = start;
	manager->end = start;
	start->ID = 0;
	start->data = &verticies[0];
	start->next = NULL;
	indicies[0] = 0;
	printf("Number of verticies %d\n", numberOfVerticies);
	for (int i = 1; i < numberOfVerticies; i++){
		
		int result = hasNodeVertex(manager, &verticies[i]);
		if (result == -1){
			NodeVertex *temp = malloc(sizeof(NodeVertex));
			temp->data = &verticies[i];
			addNode(manager, temp);
			indicies[i] = hasNodeVertex(manager, &verticies[i]);

		}
		else{

			indicies[i] = result;
		}

	}
	manager->count += 1;
	for (int i = 0; i < numberOfVerticies; i++){
	
	}
	printf("\n");
	printf("Manager count: %d\n", manager->count);

}
void CreateModelData(int VAO, vec3 Position, vec3 Scale, int ShaderProgramID, Model *m)
{
    CreateModelTransofrmData(&m->transformData, Position, Scale);
    CreateShaderData(&m->ShaderData, ShaderProgramID);
    m->VAO = VAO;
    
}



void CreateModelNoIndeciesFromFileData(Vertex *vert, int VCount, int ShaderProgramID, vec3 Position, vec3 Scale, Model *m){
    unsigned int VBO;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	
	//Bind the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//write data to the vertex buffer object

    //Issues with size of pointer versus size of array in total
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*VCount, &vert[0], GL_STATIC_DRAW);
	m->Size = VCount;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);

    CreateModelData(VAO, Position,  Scale, ShaderProgramID, m);



}


void createModelWithIndicies(Vertex *vert, int *indicies, int vertexCount, int indexCount, int ShaderProgramID, vec3 Position, vec3 Scale, Model *m){
    unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertexCount, &vert[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(int), &indicies[0], GL_STATIC_DRAW);
	m->Size = indexCount;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
    CreateModelData(VAO, Position,  Scale, ShaderProgramID, m);


	

}
void CreateModel(float *verticies, int *indices, int ShaderProgramID, vec3 Position, vec3 Scale, Model *m)
{
    unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(int), indices, GL_STATIC_DRAW);
	//Bind the vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//write data to the vertex buffer object

    //Issues with size of pointer versus size of array in total
	glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), verticies, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

    CreateModelData(VAO, Position,  Scale, ShaderProgramID, m);
}
