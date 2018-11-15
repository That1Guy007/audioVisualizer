#include "UI.h"

/**
\file UI.cpp
\brief User interface processor for the program.

\author    Don Spickler
\version   1.2
\date      1/14/2018

*/

/**
\brief Constructor

\param graph --- Pointer to the GraphicsEngine that this interface processor is attached.

Simply stores the pointer of the GraphicsEngine.

*/

UI::UI(GraphicsEngine* graph)
{
    ge = graph;
    mouseDown = false;
    playing = true;
}

/**
\brief Destructor

No implementation needed at this point.

*/

UI::~UI() {}

/**
\brief The method handles the SFML event processing and calls the keyboard state processor
method.

This method processes all events in the current SFML event queue and calls the
corresponding processing method.  At the end it calls the keyboard state processor
method, outside the event loop.

*/

void UI::processEvents()
{
    // Process user events
    sf::Event event;
    while (ge->pollEvent(event))
    {
        // Close Window or Escape Key Pressed: exit
        if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            ge->close();

        // Key is pressed.
        if (event.type == sf::Event::KeyPressed)
            keyPressed(event.key);

        // Window is resized.
        if (event.type == sf::Event::Resized)
            ge->resize();

        if (event.type == sf::Event::MouseMoved)
            processMouseMoved(event.mouseMove);

        if (event.type == sf::Event::MouseButtonPressed)
            processMouseButtonPressed(event.mouseButton);

        if (event.type == sf::Event::MouseButtonReleased)
            processMouseButtonReleased(event.mouseButton);

    }

    // Process the state of the keyboard outside of event firing,
    keyboardStateProcessing();

}

/**
\brief The method updates the theta and psi values of the spherical camera
on a click and drag.

\param mouseMoveEvent --- The SFML mouse move event structure.

*/

void UI::processMouseMoved(sf::Event::MouseMoveEvent mouseMoveEvent)
{
    if (ge->isSphericalCameraOn() && mouseDown)
    {
        ge->getSphericalCamera()->addTheta((mouseMoveEvent.x - LastPosition.x)*degf*5);
        ge->getSphericalCamera()->addPsi((mouseMoveEvent.y - LastPosition.y)*degf*5);

        LastPosition.x = mouseMoveEvent.x;
        LastPosition.y = mouseMoveEvent.y;
    }
}

/**
\brief On a left mouse click it will track the mouse down and tag the current position
of the mouse as the last position the mouse was at.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonPressed(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = true;
        LastPosition.x = mouseButtonEvent.x;
        LastPosition.y = mouseButtonEvent.y;
    }
}

/**
\brief If the left mouse button is released this method will track the release and
exit any drag movement.

\param mouseButtonEvent --- The SFML mouse button event structure.

*/

void UI::processMouseButtonReleased(sf::Event::MouseButtonEvent mouseButtonEvent)
{
    if (mouseButtonEvent.button == sf::Mouse::Left)
    {
        mouseDown = false;
    }
}

/**
\brief The function handles the keyboard input events from the user.

\param keyevent --- The SFML key code for the key pressed.

\remark

- M: Toggles between fill mode and line mode to draw the triangles.
- F1: Sets the flag to draw a single box.
- F2: Sets the flag to draw a grid of boxes.
- F3: Sets the flag to draw boxes.
- F4: Sets the flag to hide boxes.
- F10: Saves a screen shot of the graphics window to a png file.
- F11 or O: Turns on the spherical camera.
- F12 or P: Turns on the yaw-pitch-roll camera.

*/

void UI::keyPressed(sf::Event::KeyEvent keyevent)
{
    int key = keyevent.code;

    switch (key)
    {
    case sf::Keyboard::F1:
        ge->setDrawManyBoxes(GL_FALSE);
        break;

    case sf::Keyboard::F2:
        ge->setDrawManyBoxes(GL_TRUE);
        break;

    case sf::Keyboard::F3:
        ge->setDrawBoxes(GL_TRUE);
        break;

    case sf::Keyboard::F4:
        ge->setDrawBoxes(GL_FALSE);
        break;

    case sf::Keyboard::F5:
        ge->setDrawAxes(GL_TRUE);
        break;

    case sf::Keyboard::F6:
        ge->setDrawAxes(GL_FALSE);
        break;

    case sf::Keyboard::F10:
        ge->screenshot();
        break;

    case sf::Keyboard::F11:
    case sf::Keyboard::O:
        ge->setSphericalCameraOn();
        break;

    case sf::Keyboard::F12:
    case sf::Keyboard::P:
        ge->setYPRCameraOn();
        break;

    case sf::Keyboard::M:
        ge->changeMode();
        break;
    case sf::Keyboard::Space:
        if(ge->isPlaying() == sf::SoundSource::Playing){
            ge->pauseAudio();
            //playing = false;
        }
        else{
            ge->playAudio();
            //playing = true;
        }
        break;
    default:
        break;
    }
}

/**
\brief Calls the respective method for key processing depending on
which camera, spherical or yaw-pitch-roll, is currently selected.

*/

void UI::keyboardStateProcessing()
{
    if (ge->isSphericalCameraOn())
        keyboardStateProcessingSphericalCamera();
    else
        keyboardStateProcessingYPRCamera();
}

/**
\brief The method processes the keyboard state if the spherical camera is the one currently
being used.

\remark

If no modifier keys are pressed:

- Left: Increases the camera's theta value.
- Right: Decreases the camera's theta value.
- Up: Increases the camera's psi value.
- Down: Decreases the camera's psi value.

If the control or Z key is down:

- Up: Decreases the camera's radius.
- Down: Increases the camera's radius.

*/

void UI::keyboardStateProcessingSphericalCamera()
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    if (ctrldown || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getSphericalCamera()->addR(-0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getSphericalCamera()->addR(0.1);
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getSphericalCamera()->addTheta(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getSphericalCamera()->addTheta(-1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getSphericalCamera()->addPsi(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getSphericalCamera()->addPsi(-1);
    }
}

/**
\brief The method processes the keyboard state if the yaw-pitch-roll camera is the
one currently being used.

\remark

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

*/

void UI::keyboardStateProcessingYPRCamera()
{
    bool ctrldown = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
    bool shiftdown = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

    if (shiftdown || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getYPRCamera()->moveRight(-0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getYPRCamera()->moveRight(0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getYPRCamera()->moveUp(0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getYPRCamera()->moveUp(-0.1);
    }
    else if (ctrldown || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getYPRCamera()->addRoll(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getYPRCamera()->addRoll(-1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getYPRCamera()->moveForward(0.1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getYPRCamera()->moveForward(-0.1);
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            ge->getYPRCamera()->addYaw(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            ge->getYPRCamera()->addYaw(-1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            ge->getYPRCamera()->addPitch(1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            ge->getYPRCamera()->addPitch(-1);
    }
}
