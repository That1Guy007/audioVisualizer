#ifndef TRACK_H_INCLUDED
#define TRACK_H_INCLUDED

#include <GL/glew.h>
#include <math.h>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "ProgramDefines.h"

class Track{
private:
    int rad0, rad1, sides, vPos, vCol;
    GLfloat verts0[1500], verts1[1500], verts2[3000
    ], cameraLoc[2095 * 3];
    GLuint TrackVAO0, TrackVAO1, TrackVAO2;  ///< ID for faces VBO.
    GLuint TrackEBO0, TrackEBO1, TrackEBO2;  ///< ID for faces index array.
    GLuint ArrayBuffer0, ArrayBuffer1, ArrayBuffer2;  ///< ID for faces array buffer.
    //void LoadDataToGraphicsCard();
public:
    Track();
    ~Track();
    void draw();
    void getLocation(GLfloat*, int);
};

#endif // TRACK_H_INCLUDED
