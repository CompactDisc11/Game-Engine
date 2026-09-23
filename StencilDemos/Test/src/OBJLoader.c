#include "OBJLoader.h"
#include "string.h"
#include "EventLoader.h"
char  **SplitString(char *In, char Splitter, char Terminator)
{

    static char ReturnData[99][99];
    
  

    char SplitBuffer[99];
    int SplitCount = 0;
    int CurrentSplitIndex = 0;
    int i = 0;

    char c = In[i];

    
    while ( c != Terminator)
    {

        c = In[i];
    //    printf("%c", c);


        SplitBuffer[CurrentSplitIndex] = c;
        if (c == Splitter){
            SplitBuffer[CurrentSplitIndex] = '\0';
            memcpy(ReturnData[SplitCount], SplitBuffer, 99);
          
            
            SplitCount += 1;
            CurrentSplitIndex = 0;
            
        }
        else{
            CurrentSplitIndex += 1;

        }


        i += 1;
    }
    memcpy(ReturnData[SplitCount], SplitBuffer, 99*sizeof(char));

    
  //  free(SplitBuffer);

    return (char**)ReturnData;



}









int searchForMaterialTexture(Material *materials, char *materialName, int length)
{
    for (int i = 0; i< length; i++)
    {   
        // printf("%s v %s\n", materialName, materials[i].namel);
        if (strcmp(materialName, materials[i].namel) == 0)
        {

            return materials[i].textureIndex;
        }
    }
    printf("Not Found Key %s\n", materialName);
    return -1;
}
void LoadOBJFromFile(char *FilePath, int Size, Vertex *verts, Material *mats)
{
    FILE *FileIN = fopen(FilePath, "r");
    if (FileIN == NULL){
        printf("Error reading file: %d\n");
    }
    fseek(FileIN, 0, SEEK_END);
    int FileLength = ftell(FileIN);
    rewind(FileIN);
    
    printf("Loading OBJ\n");
    char Data[1024];
    
    vec3 Position[Size];
    int PositionIndex = 0;
    vec3 Normal[Size];
    int NormalIndex = 0;

    vec3 Texture[Size];
    int TextureIndex = 0;

    ivec3 Faces[Size];
    int FaceIndex = 0;


    Vertex vertex[Size];
    int VertexIndex = 0;
    int textureID = -1;
    printf("Starting Read of file\n");
    while (fgets(Data, 1024, FileIN) != NULL){
     

        char SplitStringData[99][99];
       // printf("%d, Finished Split\n", sizeof(Data));
        memcpy(SplitStringData, SplitString(Data, ' ', '\0'), 99*99);

        Vertex point;
        // printf("%s\n", SplitStringData[0]);
        if (SplitStringData[0][0] == 'v'){


            vec3 Vector3Data;
            Vector3Data[0] = atof(SplitStringData[1]);
            Vector3Data[1] = atof(SplitStringData[2]);
            Vector3Data[2] = atof(SplitStringData[3]);


            if (SplitStringData[0][1]=='n'){
                // printf("Normal \n");
                memcpy(Normal[NormalIndex], Vector3Data, sizeof(vec3));
                NormalIndex += 1;
            }
            else if(SplitStringData[0][1]=='t'){
                // printf("Texture: \n");
                
                memcpy(Texture[TextureIndex], (vec3){Vector3Data[0], Vector3Data[1], textureID}, sizeof(vec3));
                TextureIndex += 1;
            }
            else{
                // printf("Vertex: \n");

                memcpy(Position[PositionIndex], Vector3Data, sizeof(vec3));
                PositionIndex += 1;
            }
          
            
            
        }      
        
        else if (Data[0]== 'f'){
            // printf("Face: ");
            char V1[99][99];
            char V2[99][99];
            char V3[99][99];

            
            //No space its end of file
            memcpy(V1, SplitString(SplitStringData[1], '/', '\0'), 99*99);
            ivec3 F1Data;
            F1Data[0] = atoi(V1[0])-1;
            F1Data[1] = atoi(V1[1])-1;
            F1Data[2] = atoi(V1[2])-1;
            
            Texture[F1Data[1]][2] = textureID;

            
      
            memcpy(vertex[VertexIndex].Position, Position[F1Data[0]], sizeof(vec3));
            memcpy(vertex[VertexIndex].TextureCoords, Texture[F1Data[1]], sizeof(vec3)); 
            memcpy(vertex[VertexIndex].Normal, Normal[F1Data[2]], sizeof(vec3));
            VertexIndex +=  1;

            //We have Normal and tex coords swapped
            //No i am wrong. its probalby something to do with the transformation

            memcpy(V2, SplitString(SplitStringData[2], '/', '\0'), 99*99);
            F1Data[0] = atoi(V2[0])-1 ;
            F1Data[1] = atoi(V2[1]) -1;
            F1Data[2] = atoi(V2[2])-1;
            
            Texture[F1Data[1]][2] = textureID;

            
      
            memcpy(vertex[VertexIndex].Position, Position[F1Data[0]], sizeof(vec3));
            memcpy(vertex[VertexIndex].TextureCoords, Texture[F1Data[1]], sizeof(vec3)); 
            memcpy(vertex[VertexIndex].Normal, Normal[F1Data[2]], sizeof(vec3));
            VertexIndex +=  1;
            
            memcpy(V3, SplitString(SplitStringData[3], '/', '\0'), 99*99);
            F1Data[0] = atoi(V3[0])-1;
            F1Data[1] = atoi(V3[1])-1;
            F1Data[2] = atoi(V3[2])-1;

            Texture[F1Data[1]][2] = textureID;

            
            memcpy(vertex[VertexIndex].Position, Position[F1Data[0]], sizeof(vec3));
            memcpy(vertex[VertexIndex].TextureCoords, Texture[F1Data[1]], sizeof(vec3)); 
            memcpy(vertex[VertexIndex].Normal, Normal[F1Data[2]], sizeof(vec3));
            VertexIndex +=  1;

            // memcpy(V2, SplitString(SplitStringData[2], '/'), 99*99);
            // memcpy(V2, SplitString(SplitStringData[3], '/'), 99*99);






        }
        else if (strcmp(SplitStringData[0], "usemtl") == 0){
            
            // printf(" Use Material %s\n", SplitStringData[1]);
            SplitStringData[1][ strlen(SplitStringData[1])-1] = '\0';
            printf("KEY: %s\n", SplitStringData[1]);

            textureID = (searchForMaterialTexture(mats, SplitStringData[1], 22));
            // printf("Texture ID is now %d\n", textureID);

        }
    }


    glm_vec3_print(vertex[0].Position, stdout);
    // *Size = VertexIndex;
    printf("Vertex Index: %d\n", VertexIndex);
    memcpy(verts, vertex, sizeof(Vertex)*Size);
}

int numberOfFacesInModel(char *filePath)
{
    FILE *FileIN = fopen(filePath, "r");
    if (FileIN == NULL){
        printf("Error reading file: %d\n");
    }
    fseek(FileIN, 0, SEEK_END);
    int FileLength = ftell(FileIN);
    rewind(FileIN);
    char Data[1024];

    int startCountingFaces = 0;
    int numberOfFaces = 0;
    while (fgets(Data, 1024, FileIN) != NULL){
        if (Data[0]== 'f'){
            startCountingFaces = 1;
        }
        if (startCountingFaces == 1){
            numberOfFaces += 1;
        }

    }
    return numberOfFaces;
}

int countMaterials(char *filePath){
    printf("Counting Materials\n");
    FILE *FileIN = fopen(filePath, "r");
    if (FileIN == NULL){
        printf("Error reading file: %d\n");
    }
    fseek(FileIN, 0, SEEK_END);
    int FileLength = ftell(FileIN);
    rewind(FileIN);
    char Data[1024];
    int numberOMaterials  = 0;
    while (fgets(Data, 1024, FileIN) != NULL){
        char split[99][99];
        memcpy(split, SplitString(Data, ' ', '\n'), 99*99);

        printf("%s\n", split[0]);
        if (strlen(Data) > 0){
        if (strcmp(split[0], "newmtl") == 0){
            numberOMaterials += 1;
            printf("Materials %d\n", numberOMaterials);
        }}


    }
    return numberOMaterials;
}
unsigned int LoadTexture(char *FilePath, int filter)
{
    printf("Loadng texture %s\n", FilePath);
    
	unsigned int texture;
	int width, height, nrChanels;
    stbi_set_flip_vertically_on_load(1);
	unsigned char *data = stbi_load(FilePath, &width, &height, &nrChanels, 0);
	printf("Texture Loaded number of chanels: %d\n", nrChanels);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	printf("Textture generated\n");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	printf("opengl texture generated\n");

	stbi_image_free(data);  
	return texture;
}

int compareVec3(vec3 v1, vec3 v2)
{

    int result = 1;
    if (v1[0] != v2[0]){
        result = 0;
    }
    if (v1[1] != v2[1]){
        result = 0;
    }
    if (v1[2] != v2[2]){
        result = 0;
    }
    return result;
}

int compareVertex(Vertex *v1, Vertex *v2)
{	


	int result = 1;
	if (compareVec3(v1->Normal, v2->Normal) != 1){
		result = 0;
	}
	if (compareVec3(v1->Position, v2->Position) != 1){
		result = 0;
	}
	if (compareVec3(v1->TextureCoords, v2->TextureCoords) != 1){
		result = 0;
	}

    return result;
}

void createMaterial(Material *mat, char *texturePath, int textureIndex, char *name)
{
    // mat->path = texturePath;
    mat->textureIndex = textureIndex;
    // mat->namel = name;

    // StringCopy(&name, &mat->namel);
    // strcpy(name, mat->namel);
    // StringCopy(&mat->path, &texturePath);
    // texturePath[ strlen(texturePath) - 1 ] = '\0';
    mat->textureID = LoadTexture(texturePath, GL_NEAREST);
    

}
void loadTextures(char *filePath, Material *materials)
{

    int materialCount = 0;
    FILE *FileIN = fopen(filePath, "r");
        if (FileIN == NULL){
            printf("Error reading file: %d\n");
        }
        fseek(FileIN, 0, SEEK_END);
        int FileLength = ftell(FileIN);
        rewind(FileIN);
        char Data[1024];
        char texturePath[1024];
        char name[1024];
        while (fgets(Data, 1024, FileIN) != NULL){

            char SplitStringData[99][99];
            // printf("%d, Finished Split\n", sizeof(Data));
            memcpy(SplitStringData, SplitString(Data, ' ', '\0'), 99*99);
            if (strcmp(SplitStringData[0], "newmtl") == 0){
              
              
                    SplitStringData[1][strlen(SplitStringData[1])-1] = '\0';
                    // texturePath = SplitStringData[1];
                    strcpy(name, SplitStringData[1]);
                    // printf("Name: %s\n", name);

                
                
            }

            if (strcmp(SplitStringData[0], "map_Kd") == 0)
            {
                SplitStringData[1][ strlen(SplitStringData[1])-1] = '\0';
                // printf("FilePath %s\n", SplitStringData[1]);
                // texturePath = SplitStringData[1];
                // strcpy(texturePath, SplitStringData[1]);
                createMaterial(&materials[materialCount], SplitStringData[1], materialCount, name);
                strcpy(materials[materialCount].namel, name);
                    
                // printf("AHH: %s\n", materials[materialCount].namel);
                materialCount += 1;

            }
          

        }
        printf("Finished Loading Materials\n");



}

int getVertexPosition(Vertex *verticies, Vertex *vertex, int totalNumberOfVerticies){
    
    // printf("Starting get vertex position\n");
    for (int i = 0; i < totalNumberOfVerticies; i++){   
        // printf("ID: %d\n", current->ID);
        if (compareVertex(&verticies[i], vertex) == 1){
            return i;
        }
       
        
    }
    return -1;
}
