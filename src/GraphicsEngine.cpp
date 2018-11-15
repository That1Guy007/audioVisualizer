#include "GraphicsEngine.h"

/**
\file GraphicsEngine.cpp
\brief Main graphics driver for the program.

This class extends sf::RenderWindow.

\author    Don Spickler, Carlos Hernandez
\version   1
\date      05/8/2018

*/

/**
\brief Constructor

\param title --- Title to be placed in the titlebar of the graphics window.
\param MajorVer --- The OpenGL major version that is requested.
\param MinorVer --- The OpenGL minor version that is requested.
\param width --- The width (in pixels) of the graphics window.
\param height --- The height (in pixels) of the graphics window.

Creates rendering window, loads the shaders, and sets some initial data settings.

*/

GraphicsEngine::GraphicsEngine(std::string title, GLint MajorVer, GLint MinorVer, int width, int height) :
    sf::RenderWindow(sf::VideoMode(width, height), title, sf::Style::Default,
                     sf::ContextSettings(24, 8, 4, MajorVer, MinorVer, sf::ContextSettings::Core))
{
    //  Load the shaders
    GLuint program = LoadShadersFromFile("VertexShaderBasic3D.glsl", "PassThroughFrag.glsl");

    if (!program)
    {
        std::cerr << "Could not load Shader programs." << std::endl;
        exit(EXIT_FAILURE);
    }

    fftData = (double *)malloc( (((audioObj.getNumSamples()/fftBuffer) * 5) +1) * sizeof(double));

    // Turn on the shader & get location of transformation matrix.
    glUseProgram(program);
    ProjLoc = glGetUniformLocation(program, "Proj");
    ViewLoc = glGetUniformLocation(program, "View");
    ModelLoc = glGetUniformLocation(program, "Model");

    // Initialize some data.
    mode = GL_FILL;
    sscount = 1;
    CameraNumber = 1;
    drawAxes = GL_TRUE;
    drawManyBoxes = GL_TRUE;
    drawBoxes = GL_TRUE;
    counter2 = 0;
    audioTimer2 = 0.0;

    /////////////////////////////////
    audioObj.performFFT();
    timePerVisual = audioObj.getTimePerVisual();
    audioObj.getPeakMag(fftData);
    audioObj.getMaxMag(maxMags);

    // Set position of spherical camera
    sphcamera.setPosition(30, 30, 20);
    track.getLocation(locationArr, 0);
    yprcamera.setPosition(locationArr[0], locationArr[1], locationArr[2]);

    // Enable depth and cull face.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (SetVS)
    {
        setVerticalSyncEnabled(true);
        setFramerateLimit(60);
    }
    else
    {
        setVerticalSyncEnabled(false);
        setFramerateLimit(0);
    }

    // Make it the active window for OpenGL calls, resize to set projection matrix.
    setActive();
    glClearColor(0, 0, 0, 1);

    resize();
}

/**
\brief Destructor

Currently empty, no allocated memory to clear.

*/

GraphicsEngine::~GraphicsEngine()
{
//delete stuff
}

/**
\brief The function responsible for drawing to the OpenGL frame buffer.

This function clears the screen and calls the draw functions of the box. Also keeps track of what visual to display based on the offset of audio.


*/

void GraphicsEngine::display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    audioTimer = audioObj.grabPlayingOffset(); //this method will precalculate the amount of time for each visual representation of data

    sf::Time t = sf::seconds(timePerVisual);//turn it into a time variable

    if(audioObj.isPlaying() == sf::SoundSource::Playing) // if the audio is playing
    {
        if( t.asSeconds() < ((audioTimer) - audioTimer2))//if the timer per visual (around a tenth of a second) is less than amount of time audio has played
        {

            visuals[0] = fftData[counter2];
            visuals[1] = fftData[counter2 +1];
            visuals[2] = fftData[counter2 +2];
            visuals[3] = fftData[counter2 +3];
            visuals[4] = fftData[counter2 + 4];
            audioClock.restart();
            counter2 = counter2 +5;
            audioTimer2 = audioTimer;
            if( counter2+5 >= (audioObj.getNumSamples()/fftBuffer)*5 )//if we are reaching the end of the audio, reset vidual counter
            {
                counter2 = 0;
                audioTimer2 = 0.0;
                //std::cout<<"idc";
            }
            //std::cout<<t.asSeconds()<<"\n";
        }
        else //reset audio timer
        {
            //std::cout<<audioTimer2<<"\n";
            audioTimer2 = 0;

        }
    }
    else//set visuals to 0 to show no audio
    {
        visuals[0] = 0;
        visuals[1] = 0;
        visuals[2] = 0;
        visuals[3] = 0;
        visuals[4] = 0;
    }

    if(CameraNumber == 2)
    {
        if(counter >2094)
            counter = 0;
        yprcamera.setView((-9.8 * sin((  (GLfloat)(2* PI)/2095) * counter)), (3 * cos( 3* (((GLfloat)(2* PI)/2095) * counter)) +
                          4* sin(2*(((GLfloat)(2* PI)/2095) * counter + 0.2)) + 14* cos(7*(((GLfloat)(2* PI)/2095) * counter))),
                          (9.8 * cos(((GLfloat)(2* PI)/2095) * counter)) );
        track.getLocation(locationArr, counter++);
        yprcamera.setPosition( locationArr[0], locationArr[1] + 0.1, locationArr[2]);

    }
    // Set view matrix via current camera.
    glm::mat4 view(1.0);
    if (CameraNumber == 1)
        view = sphcamera.lookAt();
    else if (CameraNumber == 2)
        view = yprcamera.lookAt();

    // Load view matrix to shader.
    glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Set axes scaling.
    glm::mat4 axesscale = glm::scale(glm::mat4(1.0), glm::vec3(10, 10, 10));

    // Load matrix product to shader.
    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(axesscale));

//    if (drawAxes)
//        coords.draw();

    glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
    //track.draw();

    if (drawBoxes)
    {
        if (drawManyBoxes)
        {
            int x = -4;
            int y = 0;
            int z = 0;
            for (int i = 0; i <5; i++)
            {
                /*
                We shall create the boxes next to each other in space around the origin in order to
                visualize the audio data
                */

                glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(x + (i * 2), y, z));
                model = glm::scale(model, glm::vec3(1,(visuals[i] / maxMags[i]) * 10,1));
                glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(model));
                box.draw();
            }


        }
        else
        {
            // Load model matrix to shader.
            glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));
            box.draw();
        }
    }



    sf::RenderWindow::display();
    printOpenGLErrors();
}

/**
\brief Changes the fill and line mode being used.

*/

void GraphicsEngine::changeMode()
{
    if (mode == GL_FILL)
        mode = GL_LINE;
    else
        mode = GL_FILL;

    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

/**
\brief Saves a screenshot of the current display to a file, ScreenShot###.png.

*/

void GraphicsEngine::screenshot()
{
    char ssfilename[100];
    sprintf(ssfilename, "ScreenShot%d.png", sscount);
    sf::Vector2u windowSize = getSize();
    sf::Texture texture;
    texture.create(windowSize.x, windowSize.y);
    texture.update(*this);
    sf::Image img = texture.copyToImage();
    img.saveToFile(ssfilename);
    sscount++;
}

/**
\brief Handles the resizing events of the window.

Sets the viewport to the entire screen and recalculates the projection matrix.
*/

void GraphicsEngine::resize()
{
    glViewport(0, 0, getSize().x, getSize().y);
    projection = glm::perspective(75.0f*degf, (float)getSize().x/getSize().y, 0.01f, 500.0f);//field of view and other stuff

    // Load projection matrix to shader.
    glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

/**
\brief Sets the size of the rendering window.

\param width --- the width in pixels of the new size.

\param height --- the height in pixels of the new size.

*/

void GraphicsEngine::setSize(unsigned int width, unsigned int height)
{
    sf::RenderWindow::setSize(sf::Vector2u(width, height));
}

/**
\brief Returns a pointer to the box object.

*/

Cube* GraphicsEngine::getBox()
{
    return &box;
}

/**
\brief Returns a pointer to the spherical camera.

*/

SphericalCamera* GraphicsEngine::getSphericalCamera()
{
    return &sphcamera;
}

/**
\brief Returns a pointer to the yaw-pitch-roll camera.

*/

YPRCamera* GraphicsEngine::getYPRCamera()
{
    return &yprcamera;
}

/**
\brief Prints all OpenGL errors to stderr.

*/

void GraphicsEngine::printOpenGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;

    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}

/**
\brief Prints the glm matrix to the console window.  Remember that glm
matrices are column major.

\param m --- the glm matrix to be displayed.

*/

void GraphicsEngine::print_GLM_Matrix(glm::mat4 m)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
            printf("%7.2f", m[c][r]);

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/**
\brief Sets the boolean to draw a grid of boxes if true and a single box if false.

\param b --- Draws a grid of boxes if true and a single box if false.

*/

void GraphicsEngine::setDrawManyBoxes(GLboolean b)
{
    drawManyBoxes = b;
}

/**
\brief Sets the boolean to draw any boxes or not.

\param b --- Draws boxes if true and not if false.

*/

void GraphicsEngine::setDrawBoxes(GLboolean b)
{
    drawBoxes = b;
}

/**
\brief Returns true if the spherical camera is currently in use.

\return True if the spherical camera is currently in use and false otherwise.

*/

GLboolean GraphicsEngine::isSphericalCameraOn()
{
    return CameraNumber == 1;
}

/**
\brief Turns the spherical camera on.

*/

void GraphicsEngine::setSphericalCameraOn()
{
    CameraNumber = 1;
}

/**
\brief Returns true if the yaw-pitch-roll camera is currently in use.

\return True if the yaw-pitch-roll camera is currently in use and false otherwise.

*/

GLboolean GraphicsEngine::isYPRCameraOn()
{
    return CameraNumber == 2;
}

/**
\brief Turns the YPR camera on.

*/

void GraphicsEngine::setYPRCameraOn()
{
    CameraNumber = 2;
}

/**
\brief Sets the boolean to draw the axes or not.

\param b --- Draws the axes if true and not if false.

*/

void GraphicsEngine::setDrawAxes(GLboolean b)
{
    drawAxes = b;
}
/**
\brief Starts the audio

*/

void GraphicsEngine::startAudio()
{
    audioObj.soundStart();
    audioClock.restart();
}
/**
\brief Pauses the audio

*/
void GraphicsEngine::pauseAudio()
{
    audioObj.soundPause();
}
/**
\brief plays the audio, unpauses

*/
void GraphicsEngine::playAudio()
{
    audioObj.soundStart();
}
/**
\brief Checks the status of the audio object
*/
sf::SoundSource::Status GraphicsEngine::isPlaying()
{
    return audioObj.isPlaying();
}
