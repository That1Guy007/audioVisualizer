#ifndef FFT_SFML_H_INCLUDED
#define FFT_SFML_H_INCLUDED
#include <GL/glew.h>


//#include    "programDefines.h"
#include    "ProgramDefines.h"
#include	<fftw3.h>
#include <vector>
#include    <math.h>
//#include    "callback.h"
//#include "Box.h"
//#include "GraphicsEngine.h"

#include    <SFML/Graphics.hpp>
#include    <SFML/OpenGL.hpp>
#include    <SFML/System.hpp>
#include    <SFML/Audio.hpp>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
/**
\file fft_SFML.h
\brief Header for fft_SFML.cpp.

This class performs the FFT and manipulates the audio buffer.

\author    Carlos Hernandez
\version   1
\date      05/8/2018

*/


class fft_SFML {
private:
    //soundBuffer which interacts with the audio file.
    sf::SoundBuffer soundBuffer; ///<sound buffer
    sf::Sound audio; ///< audio obj
    float timePerVisual; ///<Time per visual
    //holds sample data....Perform FFT on this
    std::vector<double> audioSamples; ///<actual audio samples

    std::uint64_t numSamples; ///< Num of samples
    unsigned int sampleRate; ///<Sample Rate
    //points to the next sample.
    std::size_t m_currentSample;

    const char* audioPath;  ///< audio path for wav file

    double *frames, *peakMag, overallPeakMag[5];  ///< frames are sample data for FFT, peakmag holds data per frequencies, and overall peak mag holds max mags per freq

    int counter1, magIndex; ///<counter 1 is for general,

    fftw_complex result[fftBuffer];///<Complex buffer which holds the FFT data

    fftw_plan plan;///<the plan or course of execution to perform the FFT
/*
Sample rate(number of samples read per second)
Samples (number of samples to be read; the amplitude of the signal to be played)


Knowing this information, we can then perform an FTT on the audio before it is played and then as the music is playing
we can ask the time offset of where the audio is and when it reaches certain checkpoints, display the correct audio data visual
*/


public:
    //Constructor
    fft_SFML();
    //Destructor
    ~fft_SFML();
    //playFunct
    void soundStart();
    void soundPause();
    void getPeakMag(double*);
    float getTimePerVisual();
    int getNumSamples();
    void getMaxMag(double*);
    float grabPlayingOffset();
    sf::SoundSource::Status isPlaying();

    //FTTdata
    void performFFT();

    //do windowing function if I have time

};

#endif // FFT_SFML_H_INCLUDED
