#include "Cube.h"

/**
\file Cube.cpp

\brief Implementation file for the Cube class.

\author    Don Spickler
\version   1.2
\date      1/14/2018

*/

/**
\brief Constructor

Simply stores the pointer of the GraphicsEngine.

*/

Cube::Cube()
{
    Color = glm::vec4(1, 1, 1, 1);
    BoarderColor = glm::vec4(0, 0, 0, 1);

    drawFaces = GL_TRUE;
    drawBorder = GL_FALSE;
    isColorCube = GL_TRUE;

    glGenVertexArrays(1, &vboptr);
    glGenBuffers(1, &eboptr);
    glGenBuffers(1, &bufptr);
    glGenVertexArrays(1, &vboptrborder);
    glGenBuffers(1, &eboptrborder);
    glGenBuffers(1, &bufptrborder);

    isColorCube = GL_TRUE;
    LoadDataToGraphicsCard();
}

/**
\brief Destructor

Removes allocated data from the graphics card.

*/

Cube::~Cube()
{
    glBindVertexArray(vboptr);
    glDeleteBuffers(1, &bufptr);
    glDeleteBuffers(1, &eboptr);
    glBindVertexArray(vboptrborder);
    glDeleteBuffers(1, &eboptrborder);
    glDeleteBuffers(1, &bufptrborder);
}

/**
\brief Turns on and off the drawing of the box border.

\param d --- True to turn the border drawing on and false for off.

*/

void Cube::setDrawBorder(GLboolean d)
{
    drawBorder = d;
}

/**
\brief Turns on and off the drawing of the box faces.

\param d --- True to turn the face drawing on and false for off.

*/

void Cube::setDrawFaces(GLboolean d)
{
    drawFaces = d;
}

/**
\brief Sets the solid box color.

\param r --- Red channel for the box color.
\param g --- Green channel for the box color.
\param b --- Blue channel for the box color.
\param a --- Alpha channel for the box color.

*/

void Cube::setColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    Color = glm::vec4(r, g, b, a);
    LoadDataToGraphicsCard();
}

/**
\brief Sets the border color.

\param r --- Red channel for the border color.
\param g --- Green channel for the border color.
\param b --- Blue channel for the border color.
\param a --- Alpha channel for the border color.

*/

void Cube::setBorderColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    BoarderColor = glm::vec4(r, g, b, a);
    LoadDataToGraphicsCard();
}

/**
\brief Turns on and off the drawing of the multicolored box.

\param b --- True to use the multicolored box and false for a solid color box.

*/

void Cube::setColorCube(GLboolean b)
{
    isColorCube = b;
    LoadDataToGraphicsCard();
}

/**
\brief Loads the vertex and color data to the graphics card.

*/

void Cube::LoadDataToGraphicsCard()
{
    GLuint vPosition = 0;
    GLuint vColor = 1;

    GLushort indices[] = {0, 1, 2,
                          2, 3, 0,
                          6, 5, 4,
                          4, 7, 6,
                          10, 9, 8,
                          8, 11, 10,
                          12, 13, 14,
                          14, 15, 12,
                          16, 17, 18,
                          18, 19, 16,
                          22, 21, 20,
                          20, 23, 22
                         };

    GLushort border_indices[] = {0, 1, 1, 2, 2, 3, 3, 0,
                                 4, 5, 5, 6, 6, 7, 7, 4,
                                 8, 9, 9, 10, 10, 11, 11, 8,
                                 12, 13, 13, 14, 14, 15, 15, 12,
                                 16, 17, 17, 18, 18, 19, 19, 16,
                                 20, 21, 21, 22, 22, 23, 23, 20
                                };

    GLfloat points[] = {-0.5, 0.5, 0.5, 1,
                        -0.5, -0.5, 0.5, 1,
                        0.5, -0.5, 0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, 0.5, -0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,

                        -0.5, 0.5, 0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, -0.5, 0.5, 1,

                        0.5, -0.5, 0.5, 1,
                        0.5, -0.5, -0.5, 1,
                        0.5, 0.5, -0.5, 1,
                        0.5, 0.5, 0.5, 1,

                        -0.5, -0.5, 0.5, 1,
                        -0.5, -0.5, -0.5, 1,
                        -0.5, 0.5, -0.5, 1,
                        -0.5, 0.5, 0.5, 1,
                       };

    GLfloat colors[72];
    for (int i = 0; i < 24; i++)
    {
        colors[3*i] = Color.r;
        colors[3*i+1] = Color.g;
        colors[3*i+2] = Color.b;
    }

    GLfloat color_cube_colors[] = {1, 0, 0,
                                   1, 0, 0,
                                   1, 0, 0,
                                   1, 0, 0,
                                   0, 1, 0,
                                   0, 1, 0,
                                   0, 1, 0,
                                   0, 1, 0,
                                   0, 0, 1,
                                   0, 0, 1,
                                   0, 0, 1,
                                   0, 0, 1,
                                   1, 1, 0,
                                   1, 1, 0,
                                   1, 1, 0,
                                   1, 1, 0,
                                   0, 1, 1,
                                   0, 1, 1,
                                   0, 1, 1,
                                   0, 1, 1,
                                   1, 0, 1,
                                   1, 0, 1,
                                   1, 0, 1,
                                   1, 0, 1,
                                  };

    GLfloat border_colors[72];
    for (int i = 0; i < 24; i++)
    {
        border_colors[3*i] = BoarderColor.r;
        border_colors[3*i+1] = BoarderColor.g;
        border_colors[3*i+2] = BoarderColor.b;
    }

    glBindVertexArray(vboptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    if (isColorCube)
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(color_cube_colors), color_cube_colors);
    else
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);

    glBindVertexArray(vboptrborder);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptrborder);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(border_indices), border_indices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, bufptrborder);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(border_colors), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(border_colors), border_colors);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vColor);
}

/**
\brief Draws the box to the screen.

*/

void Cube::draw()
{
    if (drawFaces)
    {
        glBindVertexArray(vboptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptr);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
    }

    if (drawBorder)
    {
        glBindVertexArray(vboptrborder);
        glLineWidth(2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboptrborder);
        glDrawElements(GL_LINES, 48, GL_UNSIGNED_SHORT, NULL);
        glLineWidth(1);
    }
}
