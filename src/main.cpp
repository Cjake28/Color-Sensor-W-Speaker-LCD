#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_TCS34725.h>
#include <LittleFS.h>
#include "AudioManager.h"
#include "ColorUtils.h"

#define P_button 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C
#define OLED_RESET -1

#define WHITE 1
#define BLACK 0

unsigned long lastButtonPressed = 0;
unsigned long lastlcdShowed = 0;
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);
    pinMode(P_button, INPUT_PULLUP);

    Wire.begin(21, 19);

    if (!display.begin(OLED_I2C_ADDRESS)) {
        Serial.println(F("SH1106 allocation failed"));
        for (;;);
    }

    if (tcs.begin()) {
        Serial.println("TCS34725 found and initialized.");
        tcs.setInterrupt(true); // Enable interrupt, but it won't trigger unless cleared
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        for (;;);
    }

    if (!LittleFS.begin()) {
        Serial.println("Failed to initialize LittleFS");
        for (;;);
    }

    setupAudio();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.display();

    Serial.print("ESP32 CPU is running at: ");
    Serial.print(getCpuFrequencyMhz()); // Get CPU frequency in MHz
    Serial.println(" MHz");

    esp_sleep_enable_ext0_wakeup((gpio_num_t)P_button, 0); // Wake up on LOW signal
}

void loop() {
    unsigned long currentTime = millis();

    if (digitalRead(P_button) == LOW && currentTime - lastButtonPressed >= 500) {
        lastButtonPressed = currentTime;

        // Enable sensor and LED
        tcs.setInterrupt(false); // Disable interrupt to enable measurements
        tcs.enable(); // Power up the sensor
        delay(200); // Allow the sensor to stabilize

        // Update display with color information
        display.clearDisplay();
        HSV hsv = getHSVFromSensor(tcs);
        Serial.println(hsv.h);
        Serial.println(hsv.s);
        Serial.println(hsv.v);
        Serial.println(getColorNameAndHsv(hsv));
        // display.setCursor(0, 0);
        // display.println(getColorNameAndHsv(hsv));
        // display.display();

        // Play audio based on color
        String colorName = getColorName(hsv);
        display.setCursor(0, 0);
        display.println(colorName);
        display.clearDisplay();
        
        // Set bigger text size (adjust as needed)
        int textSize = (colorName.length() > 6) ? 2 : 3;
        display.setTextSize(textSize);  // You can increase to 3, but check if it fits
        display.setTextColor(WHITE);

        // Get text width & height
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(colorName, 0, 0, &x1, &y1, &w, &h);

        // Center text
        int x = (SCREEN_WIDTH - w) / 2;
        int y = (SCREEN_HEIGHT - h) / 2;

        display.setCursor(x, y);
        display.println(colorName);
        display.display();
        String audioFilePath = "/" + colorName + ".wav";

        if (LittleFS.exists(audioFilePath)) {
            playAudio(audioFilePath.c_str());
        } else {
            Serial.println("Audio file not found: " + audioFilePath);
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("File Not Found");
            display.display();
        }

        // Turn off sensor and LED after use
        tcs.setInterrupt(true); // Re-enable interrupt
        lastlcdShowed = currentTime;
    }

    if (currentTime - lastlcdShowed >= 3500) {
        display.clearDisplay();
        display.display();
    }

    if (wav && wav->isRunning()) {
        if (!wav->loop()) {
            wav->stop();
            Serial.println("Playback finished");
        }
    }

    if (currentTime - lastButtonPressed >= 10000) {
        Serial.println("Entering Light Sleep...");
        delay(10);
        esp_light_sleep_start(); // CPU will pause here
        Serial.println("Woke up from Light Sleep!");
    }
}