#include "ColorUtils.h"
#include <Adafruit_TCS34725.h>

HSL rgbToHsl(int r, int g, int b) {
    float rf = r / 255.0;
    float gf = g / 255.0;
    float bf = b / 255.0;

    float maxVal = max(rf, max(gf, bf));
    float minVal = min(rf, min(gf, bf));
    float h, s, l;

    l = (maxVal + minVal) / 2;

    if (maxVal == minVal) {
        h = s = 0; // Achromatic (grey)
    } else {
        float d = maxVal - minVal;
        s = l > 0.5 ? d / (2.0 - maxVal - minVal) : d / (maxVal + minVal);

        if (maxVal == rf) {
            h = (gf - bf) / d + (gf < bf ? 6 : 0);
        } else if (maxVal == gf) {
            h = (bf - rf) / d + 2;
        } else {
            h = (rf - gf) / d + 4;
        }

        h *= 60;
    }

    HSL hsl = {h, s, l};
    return hsl;
}

String getColorNameAndHsl(HSL hsl) {
    String colorName;

    // Determine color name based on HSL values
    if (hsl.s < 0.1) {
        colorName = "gray";
    } else if (hsl.l < 0.1) {
        colorName = "black";
    } else if (hsl.l > 0.9) {
        colorName = "white";
    } else if (hsl.h >= 0 && hsl.h < 15) {
        colorName = "red";
    } else if (hsl.h >= 15 && hsl.h < 45) {
        colorName = "red orange";
    } else if (hsl.h >= 45 && hsl.h < 75) {
        colorName = "orange";
    } else if (hsl.h >= 75 && hsl.h < 105) {
        colorName = "yellow orange";
    } else if (hsl.h >= 105 && hsl.h < 135) {
        colorName = "yellow";
    } else if (hsl.h >= 135 && hsl.h < 165) {
        colorName = "yellow green";
    } else if (hsl.h >= 165 && hsl.h < 195) {
        colorName = "green";
    } else if (hsl.h >= 195 && hsl.h < 225) {
        colorName = "blue green";
    } else if (hsl.h >= 225 && hsl.h < 255) {
        colorName = "blue";
    } else if (hsl.h >= 255 && hsl.h < 285) {
        colorName = "blue purple";
    } else if (hsl.h >= 285 && hsl.h < 315) {
        colorName = "purple";
    } else if (hsl.h >= 315 && hsl.h < 345) {
        colorName = "red purple";
    } else {
        colorName = "red"; // Wraps around to Red
    }

    // Create a single-line output with HSL values and color name
    String output = "Color: " + colorName;
    output += " | Hue: " + String(hsl.h, 2) + "°";
    output += " | Saturation: " + String(hsl.s * 100, 2) + "%";
    output += " | Lightness: " + String(hsl.l * 100, 2) + "%";

    return output;
}

String getColorName(HSL hsl) {
    String colorName;

    // Determine color name based on HSL values
    if (hsl.s < 0.1) {
        colorName = "gray";
    } else if (hsl.l < 0.1) {
        colorName = "black";
    } else if (hsl.l > 0.9) {
        colorName = "white";
    } else if (hsl.h >= 0 && hsl.h < 15) {
        colorName = "red";
    } else if (hsl.h >= 15 && hsl.h < 45) {
        colorName = "red orange";
    } else if (hsl.h >= 45 && hsl.h < 75) {
        colorName = "orange";
    } else if (hsl.h >= 75 && hsl.h < 105) {
        colorName = "yellow orange";
    } else if (hsl.h >= 105 && hsl.h < 135) {
        colorName = "yellow";
    } else if (hsl.h >= 135 && hsl.h < 165) {
        colorName = "yellow green";
    } else if (hsl.h >= 165 && hsl.h < 195) {
        colorName = "green";
    } else if (hsl.h >= 195 && hsl.h < 225) {
        colorName = "blue green";
    } else if (hsl.h >= 225 && hsl.h < 255) {
        colorName = "blue";
    } else if (hsl.h >= 255 && hsl.h < 285) {
        colorName = "blue purple";
    } else if (hsl.h >= 285 && hsl.h < 315) {
        colorName = "purple";
    } else if (hsl.h >= 315 && hsl.h < 345) {
        colorName = "red purple";
    } else {
        colorName = "red"; // Wraps around to Red
    }

    // Additional cases for Brown and Pink based on saturation and lightness
    // if (hsl.l > 0.2 && hsl.l < 0.5 && hsl.s < 0.5) {
    //     colorName = "Brown";
    // } else if (hsl.l > 0.6 && hsl.s > 0.4) {
    //     colorName = "Pink";
    // }

    // Return the formatted string for audio
    return colorName;
}

HSL getHSLFromSensor(Adafruit_TCS34725 &sensor) {
    uint16_t r, g, b, c;
    sensor.getRawData(&r, &g, &b, &c);

    // Normalize the RGB values to the 0-255 range
    int r255 = (int)((r / (float)c) * 255);
    int g255 = (int)((g / (float)c) * 255);
    int b255 = (int)((b / (float)c) * 255);

    // Convert normalized RGB to HSL
    return rgbToHsl(r255, g255, b255);
}
