#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <Arduino.h>
#include <Adafruit_TCS34725.h> // Include this for the sensor

struct HSV {
    float h;  // Hue (0 - 360)
    float s;  // Saturation (0 - 1)
    float v;  // Value (0 - 1)
};

// Function declarations
HSV rgbToHsv(int r, int g, int b);
String getColorNameAndHsv(HSV hsv);
String getColorName(HSV hsv);
HSV getHSVFromSensor(Adafruit_TCS34725 &sensor); // Pass the sensor as a parameter

#endif
