#include "AudioManager.h"

// Declare global variables for audio playback
AudioGeneratorWAV *wav = nullptr;
AudioFileSourceLittleFS *file = nullptr;
AudioOutputI2S *out = nullptr;

// Initializes LittleFS and I2S
void setupAudio() {
    if (!LittleFS.begin()) {
        Serial.println("Failed to mount LittleFS");
        while (1); // Halt execution if LittleFS fails
    }
    Serial.println("LittleFS mounted successfully");

    // Initialize I2S
    out = new AudioOutputI2S();
    out->begin();
}

// Plays the specified WAV file
void playAudio(const char *filename) {
    // Stop any currently playing audio
    if (wav != nullptr && wav->isRunning()) {
        wav->stop();
    }
    
    delete wav;
    delete file;
    wav = nullptr;
    file = nullptr;

    // Open the specified WAV file
    file = new AudioFileSourceLittleFS(filename);
    if (!file || !file->isOpen()) {
        Serial.println("Failed to open WAV file!");
        return;
    }

    // Start playback
    wav = new AudioGeneratorWAV();
    if (wav->begin(file, out)) {
        Serial.print("Playing file: ");
        Serial.println(filename);
    } else {
        Serial.println("Failed to start playback!");
        delete wav;
        delete file;
        wav = nullptr;
        file = nullptr;
    }
}
