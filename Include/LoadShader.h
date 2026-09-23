#pragma once
#include "stb_image.h"
#include "stdio.h"
#include "SDL2/SDL.h"
#include "glad/glad.h"

int LoadAndCompileShader(char *Filepath, int type);
int CreateProgram(char *VertexShader, char *FragmentShader, char *GeometryShader);