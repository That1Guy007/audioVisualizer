#include "Track.h"

/*
NEed to create methods, comment

*/


Track::Track(){
    glGenVertexArrays(1, &TrackVAO0);
    glGenBuffers(1, &TrackEBO0);
    glGenBuffers(1, &ArrayBuffer0);
    glGenVertexArrays(1, &TrackVAO1);
    glGenBuffers(1, &TrackEBO1);
    glGenBuffers(1, &ArrayBuffer1);
    glGenVertexArrays(1, &TrackVAO2);
    glGenBuffers(1, &TrackEBO2);
    glGenBuffers(1, &ArrayBuffer2);

    rad0 = 10;
    rad1 = 9.6;
    sides = 500;
    vPos = 0;
    vCol = 1;

    GLuint indicies0[sides + 3], indicies1[2 * sides + 3];

    GLfloat color[3 * sides], color1[6* sides]; //need to make it so i only hold one color ...

    for(int i = 0; i < sides; i++){
        indicies0[i] = i;
    }

    for(int i =0; i <2 *sides; i++){
        indicies1[i] = i;
    }

    for(int i = 0; i < sides; i++){
    /*
        X coordinate
    */
        verts0[3*i] = rad0 * cos( (  (GLfloat)(2* PI)/sides) * i );
        verts1[3*i] = rad1 * cos( (GLfloat) (2* PI)/(sides) *i );
    /*
        Y coordinate
    */
        verts1[ (3*i ) + 1] = verts0[i*3 +1] = (sin(3 * (( (GLfloat)(2* PI)/sides ) * ((i) )))) - (2* cos(2 * (2* (( (GLfloat)(2* PI)/sides) * (i) + 0.2) ))) + (2*sin(7*(( (GLfloat)(2* PI)/sides) * (i) )));
    /*
        Z coordinate
    */
        verts0[i*3 +2] =rad0 * sin((  (GLfloat)(2* PI)/sides) * (i));
        verts1[i*3 +2] =rad1 * sin(( (GLfloat)(2* PI)/sides) * (i) );
    /*
        Ties coordinate
    */
        verts2[i*6] = verts0[i *3];
        verts2[i*6 + 1]= verts0[i *3+1];
        verts2[i*6 + 2]= verts0[i *3 +2];
        verts2[i*6 + 3]= verts1[i *3];
        verts2[i*6 + 4]= verts1[i *3 + 1];
        verts2[i*6 + 5]= verts1[i *3 +2];
    }

    for(int i = 0; i < 2095; i++){
        cameraLoc[i *3] = 9.8 * cos( (  (GLfloat)(2* PI)/2095) * i );
        cameraLoc[i *3+1] = (sin(3 * ( (GLfloat)(2* PI)/2095 * (i) ))) - (2* cos(2 * (2* (( (GLfloat)(2* PI)/2095) * (i) + 0.2) ))) + (2*sin(7*(( (GLfloat)(2* PI)/2095) * (i) )));
        cameraLoc[i *3 +2] = 9.8 * sin(( (GLfloat)(2* PI)/2095) * (i) );
    }

    for(int i = 0; i < 3*sides; i++){
        color[i] = 1.0;
    }

    for(int i = 0; i < 6*sides; i++){
        color1[i] = 1.0;
    }


    glBindVertexArray(TrackVAO0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TrackEBO0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies0), indicies0, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ArrayBuffer0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts0) + sizeof(color), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts0), verts0);
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(verts0), sizeof(color), color);

    glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vCol, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verts0)));

    glEnableVertexAttribArray(vPos);
    glEnableVertexAttribArray(vCol);
//VAO2
    glBindVertexArray(TrackVAO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TrackEBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies0), indicies0, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ArrayBuffer1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts1) + sizeof(color), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts1), verts1);
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(verts1), sizeof(color), color);

    glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vCol, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verts1)));

    glEnableVertexAttribArray(vPos);
    glEnableVertexAttribArray(vCol);
//VAO3
    glBindVertexArray(TrackVAO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TrackEBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies1), indicies1, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ArrayBuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts2) + sizeof(color1), NULL, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts2), verts2);
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(verts2), sizeof(color1), color1);

    glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glVertexAttribPointer(vCol, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verts2)));

    glEnableVertexAttribArray(vPos);
    glEnableVertexAttribArray(vCol);
}

Track::~Track(){
    glBindVertexArray(TrackVAO0);
    glDeleteBuffers(1, &ArrayBuffer0);
    glDeleteBuffers(1, &TrackEBO0);

    glBindVertexArray(TrackVAO1);
    glDeleteBuffers(1, &ArrayBuffer1);
    glDeleteBuffers(1, &TrackEBO1);

    glBindVertexArray(TrackVAO2);
    glDeleteBuffers(1, &ArrayBuffer2);
    glDeleteBuffers(1, &TrackEBO2);
}

void Track::draw(){
    glBindVertexArray(TrackVAO0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TrackEBO0);
    glDrawElements(GL_LINE_LOOP, sides, GL_UNSIGNED_INT, NULL);
/*
May just add on to an existing EBO to connect the lines IE the ties

*/
    glBindVertexArray(TrackVAO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TrackEBO1);
    glDrawElements(GL_LINE_LOOP, sides, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(TrackVAO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TrackEBO2);
    glDrawElements(GL_LINES, sides * 2, GL_UNSIGNED_INT, NULL);
}

void Track::getLocation(GLfloat* arr, int x){

    arr[0] = cameraLoc[3 * x];
    arr[1] = cameraLoc[3 * x + 1];
    arr[2] = cameraLoc[3 * x + 2];
}
