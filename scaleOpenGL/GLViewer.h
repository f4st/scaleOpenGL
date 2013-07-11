//
//  GLViewer.h
//  TextureTry
//
//  Created by Mykola Farion on 4/17/13.
//  Copyright (c) 2013 Mykola Farion. All rights reserved.
//

#ifndef __TextureTry__GLViewer__
#define __TextureTry__GLViewer__

#include <vector>
#include <iostream>
#import <OpenGLES/ES2/gl.h>
#include "GLShaderProg.h"
#include "Matrix.h"

// for fs::Rect 
//#include "freesee_codec.h"
#include <iterator>

//#include <GLUT/GLUT.h>
#endif /* defined(__TextureTry__GLViewer__) */

namespace fs {

struct Rect
{
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(int _x, int _y, int _width, int _height) : x(_x), y(_y), width(_width), height(_height) {}
    bool empty() const { return width == 0 || height == 0; }
    int x, y, width, height;
};
    
}

struct Vertex {
    float Position[2];
    float TexCoord[2];
};


class GLViewer{
    
public:
    
    
    // texture scale & offset
    float scaleX;
    float scaleY;
    float offsetX;
    float offsetY;
    
    void render();
    
    // shaders
    void compileShaders();
    
    // scale & offset
    void setScale(float x, float y);
    void setOffset(float x, float y);
    
    // uses texture sizes  
    bool initTexture(int width, int height); //returns true if inited before;
    void updateImage(const unsigned char *buffer, int width, int height);
    void updateImage(const unsigned char *buffer, const std::vector<fs::Rect> &rects);
    
    // uses window sizes
    void reshape(int x, int y, int width, int height);
    
    bool didSizeChanged(int width, int height);
    
    GLViewer(): texName(0), texture_width(0), texture_height(0) ,
                scaleX(1) ,scaleY(1), offsetX(0), offsetY(0)
    {
        
    }
    
    ~GLViewer()
    {
        removeTexture(1, &texName);
    }
    
    GLuint texId(void)
    {
        return texName;
    }
    
    void setupVBOs(void);
    
    
    
private:
    
    
    //unsigned char *subImage;
    
    GLsizei texture_width;
    GLsizei texture_height;
    GLuint texName;

    //for shaders
    GLuint positionSlot;
    GLuint texCoordSlot;
    GLuint modelViewUniform;
    
    //for rendering
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLsizei indicesCnt;
    
       
    // texture functions
    void addTexImage(const unsigned char *texImage, GLint width, GLint height);
    void removeTexture(GLsizei n, const GLuint *textureNames);
    
    // subImage
    void addSubImage(const unsigned char *subImage, const fs::Rect &rect);
    
    // getting subtexture from texture
    void grabSubTex(const unsigned char *buffer, const fs::Rect &rect, unsigned char* grabbedImage);
    
    // size
    void setTexSize(int width, int height);
};