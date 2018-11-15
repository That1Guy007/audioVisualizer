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

#include "GraphicsEngine.h"
#include "UI.h"

/**
\mainpage Cameras & Basic 3-D Setup

\tableofcontents

\section intro Introduction

This program is a demo program for the basic setup for 3-D OpenGL rendering.  It
incorporates classes for axes and a simple multi-colored cube.  It also has classes
for a spherical camera and yaw-pitch-roll camera.

The initial screen will render the coordinate axes and a grid of multi-colored cubes.  The
options discussed below can be used to alter the position of the camera and scene objects.
The difference between this example and the previous one is that the projection, view, and
model matrices are loaded to the shader and the matrix multiplication is done at the shader
level.  In the previous exercise the matrix multiplication was done on the CPU and the result
was loaded to the shader.

\subsection options User Options

- Escape:  Ends the program.
- M: Toggles between fill mode and line mode to draw the triangles.
- F1: Sets the flag to draw a single box.
- F2: Sets the flag to draw a grid of boxes.
- F3: Sets the flag to draw boxes.
- F4: Sets the flag to hide boxes.
- F10: Saves a screen shot of the graphics window to a png file.
- F11 or O: Turns on the spherical camera.
- F12 or P: Turns on the yaw-pitch-roll camera.

If the spherical camera is currently selected,

If no modifier keys are pressed:

- Left: Increases the camera's theta value.
- Right: Decreases the camera's theta value.
- Up: Increases the camera's psi value.
- Down: Decreases the camera's psi value.

If the control or Z key is down:

- Up: Decreases the camera's radius.
- Down: Increases the camera's radius.

If the yaw-pitch-roll camera is currently selected,

If no modifier keys are pressed:

- Left: Increases the yaw.
- Right: Decreases the yaw.
- Up: Increases the pitch.
- Down: Decreases the pitch.

If the control or Z key is down:

- Left: Increases the roll.
- Right: Decreases the roll.
- Up: Moves the camera forward.
- Down: Moves the camera backward.

If the shift or S key is down:

- Left: Moves the camera left.
- Right: Moves the camera right.
- Up: Moves the camera up.
- Down: Moves the camera down.

If the spherical camera is currently selected, a click and drag with the left mouse
button will alter the theta and psi angles of the spherical camera to give the impression
of the mouse grabbing and moving the coordinate system.

\note Note that the shader programs "VertexShaderBasic3D.glsl" and "PassThroughFrag.glsl"
are expected to be in the same folder as the executable.  Your graphics card must also be
able to support OpenGL version 3.3 to run this program.


---

\subsection copyright Copyright

\author    Don Spickler
\version   1.2
\date      1/14/2018
\copyright 2018


---

\subsection license License

GNU Public License

This software is provided as-is, without warranty of ANY KIND, either expressed or implied,
including but not limited to the implied warranties of merchant ability and/or fitness for a
particular purpose. The authors shall NOT be held liable for ANY damage to you, your computer,
or to anyone or anything else, that may result from its use, or misuse.
All trademarks and other registered names contained in this package are the property
of their respective owners.  USE OF THIS SOFTWARE INDICATES THAT YOU AGREE TO THE ABOVE CONDITIONS.

*/

/**
\file main.cpp
\brief Main driver for the program.

This is the main program driver that sets up the graphics engine and links it to
the user interface processor.

\author    Don Spickler, Carlos Hernandez
\version   1
\date      05/8/2018

*/


/**
\brief The Main function, program entry point.

\return Standard EXIT_SUCCESS return on successful run.

The main function, responsible for initializing OpenGL and setting up
the SFML interface for OpenGL.

*/

int main()
{
    //  Program setup variables.
    std::string programTitle = "Cameras & Basic 3D";
    GLint MinMajor = 3;
    GLint MinMinor = 3;
    GLint WindowWidth = 700;
    GLint WindowHeight = 500;
    bool DisplayInfo = true;

    //  Other variables
    GLint major;
    GLint minor;
    sf::Clock clock;
    sf::Time time = clock.restart();
    long framecount = 0;

    // Try core context of 10.10 (too advanced) and see what card will resort to.
    // For setting up OpenGL, GLEW, and check versions.
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "OpenGL Setup", sf::Style::Default,
                            sf::ContextSettings(24, 8, 4, 10, 10, sf::ContextSettings::Core));

    window.setVisible(false);

#ifndef __APPLE__
    // Turn on GLEW for Windows and Linux.
    glewExperimental = true;
    if (glewInit())
    {
        std::cerr << "\nUnable to initialize GLEW ... exiting. \n";
        exit(EXIT_FAILURE);
    }
#endif // __APPLE__

    //  Get major and minor OpenGL version from graphics card.
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    //  Check card version to minimum required version.
    bool versionOK = true;
    if (major < MinMajor)
        versionOK = false;
    else if (major == MinMajor && minor < MinMinor)
        versionOK = false;

    if (!versionOK)
    {
        std::cerr << "\nGraphics card OpenGL version is " << major << "." << minor << std::endl;
        std::cerr << "Program required OpenGL version is " << MinMajor << "." << MinMinor << std::endl;
        std::cerr << "Exiting\n";
        exit(EXIT_FAILURE);
    }

    //  Output OpenGL version to the console window.
    if (DisplayInfo)
    {
        std::cout << "\n";
        std::cout << "Version  = " << glGetString(GL_VERSION) << "\n";
        std::cout << "Major    = " << major << "\n";
        std::cout << "Minor    = " << minor << "\n";
        std::cout << "Vendor   = " << glGetString(GL_VENDOR) << "\n";
        std::cout << "Renderer = " << glGetString(GL_RENDERER) << "\n";
        std::cout << "\n";
    }
    //  Close setup window and context.
    window.close();

    //  Create graphics engine.
    GraphicsEngine ge(programTitle, major, minor, WindowWidth, WindowHeight);
    UI ui(&ge);
    ge.startAudio();
    // Start the Game/GUI loop
    while (ge.isOpen())
    {
        // Call the display function to do the OpenGL rendering.
        ge.display();

        // Process any events.
        ui.processEvents();

        //  Increment frame counts
        framecount++;

        //  Get Elapsed Time
        float timesec = clock.getElapsedTime().asSeconds();
        char titlebar[1000];

        //  If another second has elapsed, display the FPS and total FPS.
        if (timesec > 1.0)
        {
            float fps = framecount / timesec;
            sprintf(titlebar, "%s     FPS: %.2f", programTitle.c_str(), fps);
            ge.setTitle(titlebar);
            time = clock.restart();
            framecount = 0;
        }
    }

    return EXIT_SUCCESS;
}
