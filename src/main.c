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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	
	glCullFace(GL_BACK);





	unsigned int gBuffer;
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	unsigned int gPosition, gColor, gShadows, gNormal;
	gPosition = GenerateTextureForFramebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	gNormal = GenerateTextureForFramebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	gColor = GenerateTextureForFramebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColor, 0);
	gShadows = GenerateTextureForFramebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gShadows, 0);
	unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, attachments);

	unsigned int rbo = CreateRenderBufferObject(SCREEN_WIDTH, SCREEN_HEIGHT);






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

	int programEdgeID = CreateProgram("Shaders/Shadow/shadow.vert", "Shaders/Shadow/shadow.frag", "Shaders/Shadow/shadow.geom");
	ShaderData edgeShader;
	CreateShaderData(&edgeShader, programEdgeID);

	Player p;
	createEntity(&p.body, (vec3){0, 0, 0}, (vec3){0.98, 0.98, 0.98}, (vec3){1.0, 1.0, 1.0});
	Model playerModel;
	// // playerModel.materials = malloc(sizeof(Material)*numberOfMaterias);
	// // memcpy(playerModel.materials, materials, sizeof(Material)*numberOfMaterias);
	printf("Loading player model\n");
	LoadModelFromFile("Untitled.obj", &playerModel, programID, p.body.position, p.body.scale, materials);

	// mat4 ortho;




	Camera camera;
	constructCamera((vec3){screenSize[0]/2.0, 15, -screenSize[1]/2.0+15},(vec3){screenSize[0]/2.0, 0, -screenSize[1]/2.0}, (vec3){0.000, 1, 0.000}, &camera);
	// glm_perspective(90.0, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.01, 100, camera.projectionMatrix);
	glm_ortho(0.0, screenSize[0], 0.0, screenSize[1], 0.01, 100.0, camera.projectionMatrix);
	

	
	Scene level;
	LoadModelFromFile("Assets/t.obj", &level.levelModel, programID, (vec3){0, 0, 0}, (vec3){3, 3, 3}, materials);
	glm_mat4_identity(level.levelModel.modelMatrix);
	glm_scale(level.levelModel.modelMatrix, (vec3){1.0, 1.0, 1.0});
	glm_translate(level.levelModel.modelMatrix, level.levelModel.transformData.Position);
	
	

	int lightProgram = CreateProgram("Shaders/Model/Vertex.vert", "Shaders/light.frag", "");
	Model sphere;
	LoadModelFromFile("Assets/Sphere.obj", &sphere, lightProgram,  (vec3){0, 0, 0}, (vec3){1, 1, 1}, materials);
	


	int quadProgram = CreateProgram("Shaders/Background/Vertex.vert", "Shaders/Background/Fragment.frag", "");
	Model quad;
	CreateModel(VertBackground, indices, quadProgram, (vec3){0, 0, 0}, (vec3){1, 1, 1}, &quad);
	
	glm_mat4_identity(quad.modelMatrix);
	glm_mat4_identity(quad.transformMatrix);

	glUseProgram(programID);
	int textureLOC = glGetUniformLocation(programID, "textures");
	glUniform1iv(textureLOC, numberOfMaterias, textureIDS);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	// // //  Number Texture
	float t = 0.0;
	printf("Start\n");



	while (1){
		t += 1.0/60.0;
		// 	   //  	// Draw Calander	
		// glUseProgram(level.levelModel.ShaderData.ProgramID);
		vec4 lightPosition;
		memcpy(lightPosition, (vec4){sin(t)*10.0, 6, 10, 1}, sizeof(vec4));
		glm_mat4_identity(sphere.modelMatrix);
		glm_translated_x(sphere.modelMatrix, lightPosition[0]);
		glm_translated_y(sphere.modelMatrix, lightPosition[1]);
		glm_translated_z(sphere.modelMatrix, lightPosition[2]);
		


		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);	
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			

		HandleInput();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

			
		constractMVPMatrix(camera, level.levelModel, level.levelModel.transformMatrix);






		for (int i = 0; i < numberOfMaterias; i++)
		{
			glActiveTexture(GL_TEXTURE0+i);
				
			glBindTexture(GL_TEXTURE_2D, materials[i].textureID);
		}
		DrawModel(level.levelModel);
		


		updatePlayer(&p, keysPressed);
		updateEntityPhysics(&p.body, 0.16);

		vec3 tempPosition;
		vec3 tempTarget;
		memcpy(tempPosition, p.body.position, sizeof(vec3));
		memcpy(tempTarget, p.body.position, sizeof(vec3));
		glm_vec3_sub(tempTarget, (vec3){screenSize[0]/2, 0, -screenSize[0]/2}, tempTarget);
		glm_vec3_add(tempTarget, (vec3){0, 15, 15}, tempPosition);

		glm_lookat(tempPosition, tempTarget, camera.up, camera.viewMatrix);
		// glm_mat4_identity(playerModel.modelMatrix);
		glm_mat4_identity(playerModel.modelMatrix);
		glm_translate(playerModel.modelMatrix, p.body.position);
				
		constractMVPMatrix(camera, playerModel, playerModel.transformMatrix);
		DrawModel(playerModel);

		
		constractMVPMatrix(camera, sphere, sphere.transformMatrix);
		DrawModel(sphere);

		

		





		glEnable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);   
		glEnable(GL_DEPTH_TEST);

	
		glCullFace(GL_BACK);
		glUseProgram(programEdgeID);
		//Set To increament on depth fail
		glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);


		glUniform4fv(glGetUniformLocation(programEdgeID, "gLightPos"), 1,  lightPosition);
		drawModelCustomShader(level.levelModel, programEdgeID, GL_TRIANGLES_ADJACENCY);
		glCullFace(GL_FRONT);
		glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);

		//decrement on depth fail
		drawModelCustomShader(level.levelModel, programEdgeID, GL_TRIANGLES_ADJACENCY);

	


		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR);
		glStencilMask(0x00);



		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5, 1.0, 0.5, 1.0);
	
	
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);	
		

		


		

	
	

	

		



		glUseProgram(quad.ShaderData.ProgramID);
		int numberOfLights = 2;
		glUniform1i(glGetUniformLocation(quad.ShaderData.ProgramID, "numberOfLights"), numberOfLights);
		vec4 lightPositions[numberOfLights];
		memcpy(lightPositions[0], (vec4){0, 6, 0, 1}, sizeof(vec4));
		memcpy(lightPositions[1], lightPosition, sizeof(vec4));

		for (int i = 0; i < numberOfLights; i++){
			char uniformNAME[512];
			sprintf(uniformNAME, "lights[%d].position", i);
			int uniformLocation = glGetUniformLocation(quad.ShaderData.ProgramID, uniformNAME);
			glUniform4fv(uniformLocation, 1, (float*)lightPositions[i]);
			sprintf(uniformNAME, "lights[%d].color", i);
			uniformLocation = glGetUniformLocation(quad.ShaderData.ProgramID, uniformNAME);
			glUniform4fv(uniformLocation, 1, (float*)(vec4){1.0, 1.0, 1.0, 1});
			sprintf(uniformNAME, "lights[%d].constant", i);
			uniformLocation = glGetUniformLocation(quad.ShaderData.ProgramID, uniformNAME);
			glUniform1f(uniformLocation, 1.0);
			sprintf(uniformNAME, "lights[%d].linear", i);
			uniformLocation = glGetUniformLocation(quad.ShaderData.ProgramID, uniformNAME);
			glUniform1f(uniformLocation, 0.09);
			sprintf(uniformNAME, "lights[%d].quadratic", i);
			uniformLocation = glGetUniformLocation(quad.ShaderData.ProgramID, uniformNAME);
			glUniform1f(uniformLocation, 0.032);
		}
		glUniform1i(glGetUniformLocation(quad.ShaderData.ProgramID, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(quad.ShaderData.ProgramID, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(quad.ShaderData.ProgramID, "gAlbedoSpec"), 2);
		glUniform1i(glGetUniformLocation(quad.ShaderData.ProgramID, "gShadows"), 3);


		glDisable(GL_CULL_FACE);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gColor);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gShadows);



		DrawModelIndicies(quad);


	
	  

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
