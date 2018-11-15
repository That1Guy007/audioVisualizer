#ifndef GRAPHICSENGINE_H_INCLUDED
#define GRAPHICSENGINE_H_INCLUDED

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <stdio.h>

#include "LoadShaders.h"
#include "Cube.h"
#include "Track.h"
#include "ProgramDefines.h"
#include "SphericalCamera.h"
#include "YPRCamera.h"
#include "Axes.h"
#include "fft_SFML.h"

/**
\file GraphicsEngine.h
\brief Header file for GraphicsEngine.cpp

\author    Don Spickler, Carlos Hernandez
\version   1
\date      05/8/2018

*/

/**
\class GraphicsEngine

\brief The GraphicsEngine class is an extension of sf::RenderWindow which
handles all of the graphics rendering in the program.

*/

class GraphicsEngine : public sf::RenderWindow
{
private:
    Track track;
    Cube box;
    GLenum mode;    ///< Mode, either point, line or fill.
    int sscount;    ///< Screenshot count to be appended to the screenshot filename.
    Axes coords;    ///< Axes Object
    GLfloat locationArr[3]; ///<location array
    int counter;    ///<counter
    double maxMags[5]; ///<keeps max mags
    float audioTimer, audioTimer2; ///<audio timers to help with audio and visual synch
    double visuals[5]; ///<the visuals displayed

    GLuint ProjLoc;      ///< Location ID of the Projection matrix in the shader.
    GLuint ViewLoc;      ///< Location ID of the View matrix in the shader.
    GLuint ModelLoc;     ///< Location ID of the Model matrix in the shader.

    SphericalCamera sphcamera;  ///< Spherical Camera
    YPRCamera yprcamera;        ///< Yaw-Pitch-Roll Camera
    int CameraNumber;           ///< Camera number 1 = spherical, 2 = yaw-pitch-roll.

    glm::mat4 projection;       ///< Projection Matrix

    GLboolean drawAxes;        ///< Boolean for axes being drawn.
    GLboolean drawManyBoxes;   ///< Boolean for many boxes verses one box being drawn.
    GLboolean drawBoxes;       ///< Boolean for boxes being drawn.

    fft_SFML audioObj;  ///<audio object
    sf::Clock audioClock;   ///<sfml clock
    float timePerVisual; ///< time calculate per visual screen time
    double *fftData; ///< pointer to the fft data
    int counter2; ///<general second counter


    void printOpenGLErrors();
    void print_GLM_Matrix(glm::mat4 m);

public:
    GraphicsEngine(std::string title = "OpenGL Window", GLint MajorVer = 3, GLint MinorVer = 3,
                   int width = 600, int height = 600);
    ~GraphicsEngine();

    void startAudio();
    void pauseAudio();
    void playAudio();
    void display();
    void changeMode();
    void screenshot();
    void resize();
    void setSize(unsigned int, unsigned int);
    GLfloat* getScreenBounds();
    Cube* getBox();

    void setDrawManyBoxes(GLboolean b);
    void setDrawBoxes(GLboolean b);
    void setDrawAxes(GLboolean b);
    sf::SoundSource::Status isPlaying();

    GLboolean isSphericalCameraOn();
    void setSphericalCameraOn();
    GLboolean isYPRCameraOn();
    void setYPRCameraOn();

    SphericalCamera* getSphericalCamera();
    YPRCamera* getYPRCamera();
};

#endif // GRAPHICSENGINE_H_INCLUDED
