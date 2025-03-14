#include "ColorUtils.h"

// Convert RGB to HSV
HSV rgbToHsv(int r, int g, int b) {
    float rf = r / 255.0;
    float gf = g / 255.0;
    float bf = b / 255.0;

    float maxVal = max(rf, max(gf, bf));
    float minVal = min(rf, min(gf, bf));
    float delta = maxVal - minVal;

    float h, s, v;
    v = maxVal; // Value is the maximum RGB component

    if (delta == 0) {
        h = 0; // Undefined hue for grayscale
    } else {
        if (maxVal == rf) {
            h = (gf - bf) / delta + (gf < bf ? 6 : 0);
        } else if (maxVal == gf) {
            h = (bf - rf) / delta + 2;
        } else {
            h = (rf - gf) / delta + 4;
        }
        h *= 60;
    }

    s = (maxVal == 0) ? 0 : delta / maxVal; // Saturation

    HSV hsv = {h, s, v};
    return hsv;
}

// Generate color name and HSV details
String getColorNameAndHsv(HSV hsv) {
    String colorName;

    // Determine color name based on HSV values
    if (hsv.s < 0.1) {
        colorName = "gray";
    } else if (hsv.v < 0.1) {
        colorName = "black";
    } else if (hsv.v > 0.9 && hsv.s < 0.2) {
        colorName = "white";
    } else if (hsv.h >= 0 && hsv.h < 21) {
        colorName = "red";
    } else if (hsv.h >= 21 && hsv.h < 50 && hsv.s < 0.60) {
        colorName = "red orange";
    } else if (hsv.h >= 50 && hsv.h < 70) {
        colorName = "orange";
    } else if ((hsv.h >= 70 && hsv.h < 80) || (hsv.h >= 30 && hsv.h < 50 && hsv.s > 0.60)) {
        colorName = "yellow orange";
    } else if (hsv.h >= 80 && hsv.h < 96) {
        colorName = "yellow";
    } else if (hsv.h >= 96 && hsv.h < 115) {
        colorName = "yellow green";
    } else if (hsv.h >= 115 && hsv.h < 150) {
        colorName = "green";
    } else if (hsv.h >= 150 && hsv.h < 195) {
        colorName = "blue green";
    } else if (hsv.h >= 195 && hsv.h < 220) {
        colorName = "blue";
    } else if (hsv.h >= 220 && hsv.h < 320) {
        colorName = "purple";
    }
    //  else if (hsv.h >= 285 && hsv.h < 315) {
    //     colorName = "purple";
    // } else if (hsv.h >= 315 && hsv.h < 345) {
    //     colorName = "red purple";
    // } 
    else {
        colorName = "red"; // Wraps around to Red
    }

    // Create a single-line output with HSV values and color name
    String output = "Color: " + colorName;
    output += " | Hue: " + String(hsv.h, 2) + "°";
    output += " | Saturation: " + String(hsv.s * 100, 2) + "%";
    output += " | Value: " + String(hsv.v * 100, 2) + "%";
    Serial.println(colorName);
    return output;
}

String getColorName(HSV hsv) {
    if (hsv.s < 0.1) {
        return "gray";
    } else if (hsv.v < 0.1) {
        return "black";
    } else if (hsv.v > 0.9 && hsv.s < 0.2) {
        return "white";
    } else if (hsv.h >= 0 && hsv.h < 21) {
        return "red";
    } else if (hsv.h >= 21 && hsv.h < 50 && hsv.s < 0.60){
        return "red orange";
    } else if ((hsv.h >= 50 && hsv.h < 70)){
        return "orange";
    } else if ((hsv.h >= 70 && hsv.h < 80) || (hsv.h >= 30 && hsv.h < 50 && hsv.s > 0.60)) {
        return "yellow orange";
    } else if (hsv.h >= 80 && hsv.h < 96) {
        return "yellow";
    } else if (hsv.h >= 96 && hsv.h < 115) {
        return "yellow green";
    } else if (hsv.h >= 115 && hsv.h < 150) {
        return "green";
    } else if (hsv.h >= 150 && hsv.h < 195) {
        return "blue green";
    } else if (hsv.h >= 195 && hsv.h < 220) {
        return "blue";
    } else if (hsv.h >= 220 && hsv.h < 320) {
        return "purple";
    }
    //  else if (hsv.h >= 285 && hsv.h < 315) {
    //     colorName = "purple";
    // } else if (hsv.h >= 315 && hsv.h < 345) {
    //     colorName = "red purple";
    // }
    else {
        return "red"; // Wraps around to Red
    }

    
}


// Extract HSV values from sensor data
HSV getHSVFromSensor(Adafruit_TCS34725 &sensor) {
    uint16_t r, g, b, c;
    sensor.getRawData(&r, &g, &b, &c);

    // Normalize the RGB values to the 0-255 range
    int r255 = (int)((r / (float)c) * 255);
    int g255 = (int)((g / (float)c) * 255);
    int b255 = (int)((b / (float)c) * 255);

    // Convert normalized RGB to HSV
    return rgbToHsv(r255, g255, b255);
}
