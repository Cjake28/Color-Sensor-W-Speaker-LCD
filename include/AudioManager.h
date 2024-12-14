#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <AudioFileSourceLittleFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

extern AudioGeneratorWAV *wav;
extern AudioFileSourceLittleFS *file;
extern AudioOutputI2S *out;

void setupAudio();
void playAudio(const char *filename);

#endif
