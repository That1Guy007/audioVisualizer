#include "fft_SFML.h"
/**
\file fft_SFML.cpp
\brief Performs FFT, open audio buffer and interprets data.

This class Performs FFT and plays with audio.

\author    Carlos Hernandez
\version   1
\date      05/8/2018

*/

/**
\brief Constructor

\param in --- path to audio file, set to default which is included in package.

Creates rendering window, loads the shaders, and sets some initial data settings.

*/
fft_SFML::fft_SFML(){
    audioPath = "./excitable.wav"; ///ask for path to file or use default path
    //audioPath = "./highlands.wav";

    soundBuffer.loadFromFile(audioPath); ///Load audio from the audio path
    audio.setBuffer(soundBuffer); ///set buffer to the sound
    audio.setLoop(false); ///set loop to false
    counter1 = 0; ///initialize counter1

    numSamples = soundBuffer.getSampleCount(); ///grabs the number of samples within the audio file
    audioSamples.assign(soundBuffer.getSamples(), soundBuffer.getSamples() + soundBuffer.getSampleCount() );///assigns the audio samples to the vector
    sampleRate = soundBuffer.getSampleRate();///get the sample rate of the audio, samples per second
    overallPeakMag[0] = 0;///initializing Data
    overallPeakMag[1] = 0;
    overallPeakMag[2] = 0;
    overallPeakMag[3] = 0;
    overallPeakMag[4] = 0;
    timePerVisual = 1/(sampleRate/(float)fftBuffer); ///calculating time between each visual

    peakMag = (double *)malloc( (((numSamples/fftBuffer) * 5 ) +1) * sizeof(double)); ///allocate memory for holding peak mags per freq
    //zero  out peak magnitude
    for(int i = 0; i <= (numSamples/fftBuffer)*5; i++){
        peakMag[i] = 0; ///zero out the peakmags
    }
}
/**
\brief Destructor

Destroy plan and free the results complex structure. Also delete the frames and peakmag allocation

*/
fft_SFML::~fft_SFML(){
    fftw_destroy_plan(plan);
    fftw_free(result);
    delete frames;
    delete peakMag;
}


/**
\brief play the audio

*/
void fft_SFML::soundStart(){
    audio.play();
}
/**
\brief Pause the audio

*/
void fft_SFML::soundPause(){
    audio.pause();
}
/**
\brief Perform the fft on the whole data of the audio
*/
void fft_SFML::performFFT(){
    //Perform FFT on set of Data
    int buffLen = fftBuffer;
    int numFftSamples = numSamples / fftBuffer; ///num of fftSamples

    frames = (double *) malloc(fftBuffer * sizeof(double));///allocating memory

    for(int i = 0; i <= numFftSamples; i++){
        if(numSamples % fftBuffer != 0){
            if(i == numFftSamples)
                buffLen = numSamples - (numFftSamples * fftBuffer); ///incase there are left over samples
        }
        else{
            if(i == numFftSamples){
                return;
            }
        }

        frames = &audioSamples[fftBuffer * i]; ///needed for the fftw plan
        plan = fftw_plan_dft_r2c_1d(buffLen, frames, result, FFTW_ESTIMATE); ///the plan of execution to perfomr the fft
        fftw_execute(plan); ///plan execution
        for(int i = 0; i < buffLen/2; i++){
            int freq = ( i * sampleRate / buffLen); ///determine the freq
            double mag = sqrt((result[i][0] * result[i][0]) + (result[i][1] * result[i][1]) ); ///calculate the magnitude

                if(freq > 19 && freq <= 140){
                    if(mag > peakMag[counter1]){
                        peakMag[counter1] = mag;
                    }
                    if(mag > overallPeakMag[0]){
                            overallPeakMag[0] = mag;
                        }
                }
                else if(freq >140 && freq <=400){
                    if(mag > peakMag[counter1 + 1]){
                        peakMag[counter1 + 1] = mag;
                    }
                    if(mag > overallPeakMag[1]){
                            overallPeakMag[1] = mag;
                        }
                }
                else if(freq > 400 && freq <= 2600){
                    if(mag > peakMag[counter1 + 2]){
                        peakMag[counter1 + 2] = mag;
                    }
                    if(mag > overallPeakMag[2]){
                            overallPeakMag[2] = mag;
                        }
                }
                else if(freq > 2600 && freq <= 5200){
                    if(mag > peakMag[counter1 + 3]){
                        peakMag[counter1 + 3] = mag;
                    }
                    if(mag > overallPeakMag[3]){
                            overallPeakMag[3] = mag;
                        }
                }
                else if(freq > 5200){
                    if(mag > peakMag[counter1 + 4]){
                        peakMag[counter1 + 4] = mag;
                    }
                    if(mag > overallPeakMag[4]){
                            overallPeakMag[4] = mag;
                        }
                }
        }

        counter1+= 5;
    }

}
/**
\brief get the array filled with magnitudes of fft data

*/
void fft_SFML::getPeakMag(double* array_to_be_filled){
    for(int i = 0; i < (numSamples/fftBuffer) *5; i++){
        array_to_be_filled[i] = peakMag[i];
    }
}
/**
\brief Return the time per visual

*/
float fft_SFML::getTimePerVisual(){
    return timePerVisual;
}
/**
\brief Return the number of samples

*/
int fft_SFML::getNumSamples(){
    return numSamples;
}

/**
\brief get the max mag data

*/
void fft_SFML::getMaxMag( double* overallMagArr){
    for(int i = 0; i < 5; i ++){
            overallMagArr[i] = overallPeakMag[i];
    }
}
/**
\brief Return Playing offset of audio

*/
float fft_SFML::grabPlayingOffset(){
    //printf("%.6f",  audio.getPlayingOffset().asSeconds());
    return audio.getPlayingOffset().asSeconds();
}
/**
\brief return whether or not the audio is playing

*/
sf::SoundSource::Status fft_SFML::isPlaying(){
    return audio.getStatus();
}
