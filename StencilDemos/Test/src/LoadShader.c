#include "LoadShader.h"

int LoadAndCompileShader(char *Filepath, int type)
{   
    int success;
    FILE *FileIN = fopen(Filepath, "r");
    if (FileIN == NULL){
        printf("Error reading shader type: %d\n");
    }
   
    fseek(FileIN, 0, SEEK_END);
    int FileLength = ftell(FileIN);
    rewind(FileIN);
    
    const char *Data =NULL;
    
    Data = (const char*)malloc(sizeof(char)*(FileLength+1));
    fread(Data, 1, FileLength, FileIN);
    
    
    
    // printf("%s\n", Data);
    printf("Size of Data: %d\n", FileLength);
    unsigned int shader;
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &Data, &FileLength);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader Error %s\n", infoLog);
    }
    //free(Data);

    printf("Finished\n");
    return shader;
}

int CreateProgram(char *VertexShader, char *FragmentShader, char *GeometryShader)
{   

    printf("Geometry shader is %s\n", GeometryShader);
    int geometryShader = 0;
    int fragmentShader = LoadAndCompileShader(VertexShader, GL_VERTEX_SHADER);
    int vertexShader = LoadAndCompileShader(FragmentShader, GL_FRAGMENT_SHADER);
    if (strcmp(GeometryShader, "") != 0){
        printf("Geometry created\n");
        geometryShader = LoadAndCompileShader(GeometryShader, GL_GEOMETRY_SHADER);
    }
	
    printf("Shaders Loaded\n");
	//Create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//Bind shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
    if (strcmp(GeometryShader, "") != 0){
        printf("Geometry attached\n");

        glAttachShader(shaderProgram, geometryShader);
    }
	glLinkProgram(shaderProgram);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
    if (strcmp(GeometryShader, "") != 0){
        glDeleteShader(geometryShader);
    }
    printf("Finsihed\n");
    return shaderProgram;
}
