#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <Arduino.h>
#include <Adafruit_TCS34725.h> // Include this for the sensor

struct HSL {
    float h;  // Hue (0 - 360)
    float s;  // Saturation (0 - 1)
    float l;  // Lightness (0 - 1)
};

// Function declarations
HSL rgbToHsl(int r, int g, int b);
String getColorNameAndHsl(HSL hsl);
String getColorName(HSL hsl);
HSL getHSLFromSensor(Adafruit_TCS34725 &sensor); // Pass the sensor as a parameter

#endif
