//
//  GLViewer.cpp
//  TextureTry
//
//  Created by Mykola Farion on 4/17/13.
//  Copyright (c) 2013 Mykola Farion. All rights reserved.
//
#include "GLViewer.h"
#include <OpenGLES/ES2/glext.h>

#define TEXTURE_MAX_SIZE 2048

void genTestSubTex(int width, int height, unsigned char* mySubTex);
// used just for rect texture generation for test
void genTestSubTex(int width, int height, unsigned char* mySubTex)
{
    int color;
    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if((i+1)%2==0) {
                color = 0;
            } else {
                if((j+1)%2){
                    color = 125;
                } else {
                    color = 255;
                }
            }
            mySubTex[(i*width+j)*4] = (GLubyte) color;
            mySubTex[(i*width+j)*4+1] = (GLubyte) color;
            mySubTex[(i*width+j)*4+2]= (GLubyte) color;
            mySubTex[(i*width+j)*4+3] = (GLubyte) 255;
        }
    }
}

void GLViewer::grabSubTex(const unsigned char *buffer, const fs::Rect &rect, unsigned char* grabbedImage)
{
    unsigned int position;
    for (int i=0;i<rect.height;i++)
    {
        position = ((rect.y+i)*texture_width+rect.x)*4;
        for(int j=0;j<rect.width;j++)
        {
            for (int k=0; k<4; k++)
            {
                grabbedImage[(i*rect.width+j)*4+k] = buffer[position];
                position++;
            }
        }
    }
}

bool GLViewer::didSizeChanged(int width, int height)
{
    if((width!=texture_width)||(height!=texture_height))
    {
        return true;
    }
    else {
        return false;
    }
}

void GLViewer::updateImage(const  unsigned char *buffer, int width, int height)
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

void GLViewer::reshape(int x, int y, int window_width, int window_height)
{
    glViewport(x, y, window_width, window_height);
}

void GLViewer::removeTexture(GLsizei n, const GLuint *textureNames)
{
    glDeleteTextures(n, textureNames);
}

void GLViewer::addTexImage(const unsigned char *texImage, GLint width, GLint height)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImage);
}

void GLViewer::updateImage(const unsigned char *buffer, const std::vector <fs::Rect> &rects)
{
    
    std::vector<fs::Rect>::const_iterator it;
    for(it = rects.begin();it!=rects.end();it++)
    {
        const fs::Rect &texRect = *it;
        unsigned char subImage[texRect.width*texRect.height*4];
        
        // for check replace grabSubTex(buffer, texRect); string with
        genTestSubTex(texRect.width, texRect.height, subImage);
        //grabSubTex(buffer, texRect, subImage);
        
        addSubImage(subImage, texRect);
    }
}

void GLViewer::addSubImage(const unsigned char *subImage, const fs::Rect &rect)
{
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexSubImage2D(GL_TEXTURE_2D, 0, rect.x, rect.y, rect.width , rect.height, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
}
    
bool GLViewer::initTexture(int width, int height)
{
    setTexSize(width,height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (texName == 0) {
        glGenTextures(1, &texName);
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        return false;
    }
    return true;
}

void GLViewer::setTexSize(int width, int height)
{
    texture_width = width;
    texture_height = height;
}

void GLViewer::setupVBOs(void)
{
    float texCoordMaxW = (float)texture_width/TEXTURE_MAX_SIZE;
    float texCoordMaxH = (float)texture_height/TEXTURE_MAX_SIZE; 
    const Vertex Vertices[] = {
        // texture coordinate flipped couse openGL start draw
        // from bottom left corner
        {{-1, -1},  {0, texCoordMaxH}},
        {{-1, 1},  {0, 0}},
        {{1, 1},  {texCoordMaxW, 0}},
        {{1, -1},  {texCoordMaxW, texCoordMaxH}},
    };
    
    const GLubyte Indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,
    };
    
    //for render drawElements
    indicesCnt = sizeof(Indices)/sizeof(Indices[0]);
    
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
    
}

#pragma mark - zoom & offset

void GLViewer::setOffset(float x, float y)
{
    offsetX = x;
    offsetY = y;
}

void GLViewer::setScale(float x, float y)
{
    scaleX = x;
    scaleY = y;
}

#pragma mark - Shaders

const GLchar *vertex_shader[] = {
    
    "attribute vec2 Position;\n",
    "attribute vec4 SourceColor;\n",
    "attribute vec2 TexCoordIn;\n",
    "uniform mat4 Modelview;\n",
    "varying vec2 TexCoordOut;\n",
    "void main(void) {\n",
    "    gl_Position = Modelview * vec4 (Position.x,Position.y, 0, 1);\n",
    "    TexCoordOut = TexCoordIn;\n",
    "}"
};

const GLchar *color_shader[] = {
    
    "varying lowp vec4 DestinationColor;",
    "varying lowp vec2 TexCoordOut;",
    "uniform sampler2D Texture;",
    "void main(void) {\n",
    "    gl_FragColor = texture2D(Texture, TexCoordOut).zyxw;\n",
    "}"
};

void GLViewer::compileShaders(void)
{
    static GLShaderProg prog(vertex_shader,color_shader);
    prog();
    modelViewUniform = glGetUniformLocation(prog, "Modelview");
    
    positionSlot = glGetAttribLocation(prog, "Position");
    glEnableVertexAttribArray(positionSlot);
    
    texCoordSlot = glGetAttribLocation(prog, "TexCoordIn");
    glEnableVertexAttribArray(texCoordSlot);
}

#pragma mark - render

void GLViewer::render(void)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texName);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    
    Matrix4f tmp;
    Matrix4f scaleMatrix;
    Matrix4f translationMatrix;
    scaleMatrix = tmp.genScaleMatrix(scaleX, scaleY);
    translationMatrix = tmp.genTranslationMatrix(offsetX, offsetY);
    tmp = scaleMatrix * translationMatrix;
    float* modelView;
    modelView = tmp.pointer1D();
    glUniformMatrix4fv(modelViewUniform, 1, 0, modelView);
    
    glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(texCoordSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 2)); // 2 for position
    
    glDrawElements(GL_TRIANGLES, indicesCnt, GL_UNSIGNED_BYTE, 0);
  
}