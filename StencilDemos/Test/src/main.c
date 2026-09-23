#include "stb_image.h"
#include "LoadShader.h"
#include "cglm/cglm.h"
#include "Model.h"
#include "OBJLoader.h"
#include "time.h"
#include "FontData.h"
#include "Font.h"
#include "EventLoader.h"
#include "Entity.h"
#include "camera.h"
#include "Scene.h"
// #include "LinkedList.h"
vec4 TextPositions[95];
char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
int keysPressed[500];
void DrawModelIndicies(Model m){

	glBindVertexArray(m.VAO);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	
	
}


void DrawModel(Model m){

		
	glUseProgram(m.ShaderData.ProgramID);
	glUniformMatrix4fv(m.ShaderData.modelMatrixLoc, 1, GL_FALSE, (float*)m.modelMatrix);
	glUniformMatrix4fv(m.ShaderData.transformMatrixLoc, 1, GL_FALSE, (float*)m.transformMatrix);
		
	glBindVertexArray(m.VAO);

	// glDrawArrays(GL_TRIANGLES, 0, m.Size);
	glDrawElements(GL_TRIANGLES_ADJACENCY, m.Size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void drawModelCustomShader(Model m, int pID, int meshType){
	glUseProgram(pID);
	glUniformMatrix4fv(glGetUniformLocation(pID, "Model"), 1, GL_FALSE, (float*)m.modelMatrix);
	glUniformMatrix4fv(glGetUniformLocation(pID, "transform"), 1, GL_FALSE, (float*)m.transformMatrix);
		
	glBindVertexArray(m.VAO);

	// glDrawArrays(GL_TRIANGLES, 0, m.Size);
	glDrawElements(GL_TRIANGLES_ADJACENCY, m.Size, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void DrawTextOntoUV(Model N, int tex, int fbo){
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, 2100, 2100);
	glUseProgram(N.ShaderData.ProgramID);
	glBindTexture(GL_TEXTURE_2D, tex);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	int UniformSlices = glGetUniformLocation(N.ShaderData.ProgramID, "SliceArray");
	glUniform4fv(UniformSlices, 95, TextPositions[0]);
	DrawModelIndicies(N);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DrawBackground(Model m, float t){
		
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);		
		//Need to bind the shader, send relevent data THEN draw
		glUseProgram(m.ShaderData.ProgramID);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		int UniformT = glGetUniformLocation(m.ShaderData.ProgramID, "time");
		glUniform1f(UniformT, t);
		
		DrawModelIndicies(m);
		glUseProgram(0);
}

unsigned int GenerateTextureForFramebuffer(int width, int height){
	int texForBuffer;
	glGenTextures(1, &texForBuffer);
	glBindTexture(GL_TEXTURE_2D, texForBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	return texForBuffer;
}
unsigned int CreateRenderBufferObject(int width, int height){
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return rbo;
}

unsigned int CreateFramebuffer(unsigned int TexFrameBuffer){
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexFrameBuffer, 0);
	CreateRenderBufferObject(2100, 2100);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  
	return FBO;
}

void HandleInput(){
		SDL_Event event;
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_KEYDOWN){
				if (event.key.keysym.sym == 'q'){
					SDL_Quit();
					exit(0);
				}
				// printf("Key %d\n", event.key.keysym.scancode);
				keysPressed[event.key.keysym.scancode] = 1;
				
				
			}
			if (event.type == SDL_KEYUP)
			{
				keysPressed[event.key.keysym.scancode] = 0;

			}
			if (event.type == SDL_QUIT)
			{
					SDL_Quit();
					exit(0);
			}
		}
}




int main(){

	
	// NodeManager m;
	// m.count = 0;
	// NodeVertex v;
	// NodeVertex v1;
	// NodeVertex v2;
	// NodeVertex v3;
	// NodeVertex v4;

	// v.ID = 0;
	// m.start = &v;
	// m.end = &v; 
	// addNode(&m, &v1);
	// addNode(&m, &v2);
	// addNode(&m, &v3);
	// addNode(&m, &v4);
	// iterateThroughNodes(&m);

	
	float vertices[] = {
		0.5f,  0.5f, -0.f,  1.0, 1.0, //top right TexCoords
		0.5f, -0.5f, -0.f,  1.0, 0.0, //bottom right
		-0.5f, -0.5f, -0.f,  0.0, 0.0, //bottom left
		-0.5f,  0.5f, -0.f,  0.0, 1.0 //top left 
	};
	float VertBackground[] = {
		1.0,  1.0, 0,  1.0, 1.0, //top right TexCoords
		1.0, -1.0, 0,  1.0, 0.0, //bottom right
		-1.0, -1.0, 0,  0.0, 0.0, //bottom left
		-1.0,  1.0, 0,  0.0, 1.0 //top left 
	};
	unsigned int indices[] = {   //note that we start from 0!
		0, 1, 3,    //first triangle
		1, 2, 3     //second triangle
	}; 
	SDL_Window *mainWindow;
	SDL_GLContext mainContext;
	if (SDL_Init(SDL_INIT_VIDEO)<0){
		printf("Error initilizing SDL\n");	
		printf("%s\n", SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
 	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 1 );
	mainWindow = SDL_CreateWindow("Calander", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	mainContext = SDL_GL_CreateContext(mainWindow);
	  
	//  // // //Needs to be called after the opengl context is created
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)){
			printf("Failed to initialize opengl\n");
			return -1;
		}
	
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);





	vec2 screenSize;
	screenSize[0] = (float)SCREEN_WIDTH/100.0;
	screenSize[1] = (float)SCREEN_HEIGHT/100.0;
	

    int numberOfMaterias = countMaterials("Assets/town.mtl");
	int textureIDS[numberOfMaterias];
    printf("Number of materials %d\n", numberOfMaterias);
    Material materials[numberOfMaterias];
	for (int i = 0; i < numberOfMaterias; i++)
	{
		textureIDS[i] = i;
	}
	loadTextures("Assets/town.mtl", materials);
	printf("Creating Shaders\n");
	int programID = CreateProgram("Shaders/Model/Vertex.vert", "Shaders/Model/Fragment.frag", "");
	ShaderData sd;
	CreateShaderData(&sd, programID);

	int programEdgeID = CreateProgram("Shaders/Model/Vertex.vert", "Shaders/Model/edgeFragment.frag", "");
	ShaderData edgeShader;
	CreateShaderData(&edgeShader, programEdgeID);

	Player p;
	createEntity(&p.body, (vec3){0, 0, 0}, (vec3){0.98, 0.98, 0.98}, (vec3){1.0, 1.0, 1.0});
	Model playerModel;
	// // playerModel.materials = malloc(sizeof(Material)*numberOfMaterias);
	// // memcpy(playerModel.materials, materials, sizeof(Material)*numberOfMaterias);
	printf("Loading player model\n");
	LoadModelFromFile("Assets/Sphere.obj", &playerModel, programID, p.body.position, p.body.scale, materials);
	Model playerModelScaled;
	LoadModelFromFile("Assets/Sphere.obj", &playerModelScaled, programEdgeID, p.body.position, (vec3){1.5, 1.5, 1.5}, materials);



	
	Camera cam;
	constructCamera((vec3){0, 0, 5}, (vec3){0, 0, 0}, (vec3){0, 1, 0}, &cam);
	glm_lookat(cam.position, cam.target, cam.up, cam.viewMatrix);
	glm_perspective(45.0, screenSize[0]/screenSize[1], 0.01, 100.0, cam.projectionMatrix);
	float t = 0.0;
	printf("Start\n");





	while (1){
		t += 1.0/60.0;
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);	
		// 	   //  	// Draw Calander	
		// glUseProgram(level.levelModel.ShaderData.ProgramID);
		glEnable(GL_DEPTH_TEST);
	    glEnable(GL_STENCIL_TEST);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
		glClearColor(0.5, 1.0, 0.0, 1.0);
		HandleInput();	
		

		glStencilFunc(GL_ALWAYS, 1, 0xFF); 
		glStencilMask(0xFF);


		glm_mat4_identity(playerModel.modelMatrix);
		glm_translate(playerModel.modelMatrix, (vec3){0, 0, 0});
		constractMVPMatrix(cam, playerModel, playerModel.transformMatrix);
		DrawModel(playerModel);
		
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		glm_mat4_identity(playerModelScaled.modelMatrix);
		glm_scale(playerModelScaled.modelMatrix, (vec3){1.2, 1.2, 1.2});
		glm_translate(playerModelScaled.modelMatrix, (vec3){0, 0, 0});
		constractMVPMatrix(cam, playerModelScaled, playerModelScaled.transformMatrix);
		DrawModel(playerModelScaled);
		
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);   
		glEnable(GL_DEPTH_TEST); 


		
	  

	// //   	//  for (int i = 0; i<24; i++){	
	//    	//  	char EventLine[] = "%d: ";
	// //   	//  	sprintf(EventLine, "%d: ", i+1);
			
	// //   	//  	RenderText(defaultshader, EventLine, 2000, -400.0-(40*(i+1))*1.5, 1.5, Textvbo, Textvao, textureFontSDF);
	//    	//  }
    	SDL_GL_SwapWindow(mainWindow);
	}
	return 0;
}

//Okay lets think. We are either going to render the text onto the numbers texture using a font file 
//Or sign distance fields... let us think on it while we sleep. then we add the numbers in, add in the two pannels 
//the ability to add events and show events then we call it quits
//top panel should show upcoming events
//side panel should show events by day
