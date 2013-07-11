//
//  GLShaderProg.h
//  HelloOpenGL
//
//  Created by Mykola Farion on 4/22/13.
//
//

#ifndef __HelloOpenGL__GLShaderProg__
#define __HelloOpenGL__GLShaderProg__

#include <iostream>
#include <OpenGLES/ES2/gl.h>
#endif /* defined(__HelloOpenGL__GLShaderProg__) */


class GLShaderProg
{
    
public:
    
    template <int N, int M>
    GLShaderProg(GLchar const *(&v_source)[N], GLchar const *(&f_source)[M])
    {
        vertex_shader = compile(GL_VERTEX_SHADER, v_source);
        fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);
        prog = glCreateProgram();
        glAttachShader(prog, vertex_shader);
        glAttachShader(prog, fragment_shader);
        glLinkProgram(prog);
    }
    
    operator GLuint() { return prog; }
    void operator()() { glUseProgram(prog); }
    
    ~GLShaderProg()
    {
        glDeleteProgram(prog);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    
private:
    
    GLuint vertex_shader, fragment_shader, prog;
    
    template <int N>
    GLuint compile(GLuint type, char const *(&source)[N])
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, N, source, NULL);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string log(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &log[0]);
            return false;
        }
        return shader;
    }

};