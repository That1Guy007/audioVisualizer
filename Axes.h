#ifndef AXES_H_INCLUDED
#define AXES_H_INCLUDED

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glew.h>
#endif // __APPLE__

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "ProgramDefines.h"

/**
\file Axes.h
\brief Header file for Axes.cpp

\author    Don Spickler
\version   1.2
\date      1/14/2018

*/

/**
\class Axes

\brief Axes for development, allows the programmer to see the positioning of objects
easier.

Draws the xyz axes on the screen, each axis has the coordinate range of [-1, 1]. The
x-axis is in red, with the positive x-axis is bright red and the negative x-axis in
a darker red.  The same is true for the y-axis in green and the z-axis in blue.

*/

class Axes
{
private:
    GLuint vboptr;  ///< ID for the VBO.
    GLuint bufptr;  ///< ID for the array buffer.
    GLuint eboptr;  ///< ID for the index array buffer.

    void LoadDataToGraphicsCard();

public:
    Axes();
    ~Axes();

    void draw();
};

#endif // AXES_H_INCLUDED
