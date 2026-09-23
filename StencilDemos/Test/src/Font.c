#include "Font.h"





void SetupText(int *VAO, int *VBO){
    int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float)*2));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    *VAO = vao;
    *VBO = vbo;
}

void RenderText(unsigned int shader, char text[], float x, float y, float Scale, int TEXTVBO, int TEXTVAO, int SDFtex){
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(TEXTVAO);

    int i = 0;  
    while (text[i]  != '\0'){
     
        Character c = characters_Arial[text[i]-' '];

        float xPos = (x)/SCREEN_WIDTH;
        float yPos = (y)/SCREEN_HEIGHT;
        float w = (c.width*Scale)/SCREEN_WIDTH;
        float h = (c.height*Scale)/SCREEN_HEIGHT;

        float vertices[6][4] = {

            {xPos, yPos+h, 0.0, 0.0},
            {xPos, yPos, 0.0, 1.0},
            {xPos+w, yPos, 1.0, 1.0},

            {xPos, yPos+h, 0.0, 0.0},
            {xPos+w, yPos, 1.0, 1.0},
            {xPos+w, yPos+h, 1.0, 0.0}
          };

        int UniformTextData = glGetUniformLocation(shader, "TextData");
        glUniform4fv(UniformTextData, 1, (vec4){c.x, c.y, c.width, c.height});


        
        glBindTexture(GL_TEXTURE_2D, SDFtex);
        glBindBuffer(GL_ARRAY_BUFFER, TEXTVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += w*(SCREEN_WIDTH*0.75);
        i++;
    }
}