OBJS = src/main.c src/glad.c src/LoadShader.c  src/stb_image.c src/Model.c src/OBJLoader.c src/Font.c src/EventLoader.c src/camera.c src/Entity.c src/LinkedList.c

OBJ_Name = main


CC = gcc
INCLUDE_PATHS = -I/usr/local/include/json-c -I/usr/include/libxml2 -I/usr/include/SDL2/ -I/home/dimitris/Documents/ZeldaClone/StencilDemos/Test/Include/
LINKER_FLAGS = -lSDL2  -lGL -lcurl -lxml2 -ljson-c -L/usr/local/lib -lm 
all: $(OBJS)	
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LINKER_FLAGS) -o $(OBJ_Name)
