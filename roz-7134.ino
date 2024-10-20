#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>
#include "Waveshare_LCD1602_RGB.h"

// Pins
#define DS18B20_PIN 2    // DS18B20 connected to pin 2
#define TRIGGER_PIN 9    // HC-SR04 trigger pin connected to pin 9
#define ECHO_PIN 10      // HC-SR04 echo pin connected to pin 10
#define MAX_DISTANCE 200 // Maximum distance we want to measure (in cm)

// RGB LED Pins
#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN 3

// DS18B20 Temperature Sensor Setup
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

// HC-SR04 Proximity Sensor Setup
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Color structure to store RGB values
struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

// Define some standard colors
const Color COLOR_RED = {255, 0, 0};
const Color COLOR_GREEN = {0, 255, 0};
const Color COLOR_BLUE = {0, 0, 255};
const Color COLOR_YELLOW = {255, 255, 0};
const Color COLOR_PURPLE = {128, 0, 128};
const Color COLOR_ORANGE = {255, 165, 0};
const Color COLOR_WHITE = {255, 255, 255};

// Custom characters
byte snowflake_char[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b01010,
  0b10101,
  0b00100,
  0b01010,
  0b00100
};

byte heart_char[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte tree_char[8] = {
  0b00100,
  0b01110,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00100
};

byte raindrop_char[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00000
};

byte sun_char[8] = {
  0b00100,
  0b10101,
  0b01110,
  0b11111,
  0b01110,
  0b10101,
  0b00100,
  0b00000
};

byte house_char[8] = {
  0b00100,
  0b01110,
  0b11111,
  0b10101,
  0b10101,
  0b11111,
  0b00000,
  0b00000
};

byte gosling_char[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b00100,
  0b01110,
  0b10100,
  0b00100,
  0b00000
};

byte robot_char[8] = {
   0b01110,  // Top of the head
  0b10001,  // Sides of the head
  0b10101,  // Eyes
  0b10001,  // Middle part (between eyes)
  0b11111,  // Mouth (wide)
  0b10001,  // Base of the face
  0b10001,  // Chin
  0b00000   // Empty
};

// Waveshare LCD setup
Waveshare_LCD1602_RGB lcd(16, 2);

// Load custom characters into the LCD
void loadCustomCharacters() {
  lcd.customSymbol(0, snowflake_char);
  lcd.customSymbol(1, heart_char);
  lcd.customSymbol(2, tree_char);
  lcd.customSymbol(3, raindrop_char);
  lcd.customSymbol(4, sun_char);
  lcd.customSymbol(5, house_char);
  lcd.customSymbol(6, gosling_char);
  lcd.customSymbol(7, robot_char);
}

// Function to print strings with custom characters using if-statements
void printWithCustomCharacters(const char* line) {
  while (*line) {
    if (strncmp(line, "_snowflake", 10) == 0) {
      lcd.write_char(0);  // Snowflake custom character
      line += 10;
    } else if (strncmp(line, "_heart", 6) == 0) {
      lcd.write_char(1);  // Heart custom character
      line += 6;
    } else if (strncmp(line, "_tree", 5) == 0) {
      lcd.write_char(2);  // Tree custom character
      line += 5;
    } else if (strncmp(line, "_raindrop", 9) == 0) {
      lcd.write_char(3);  // Raindrop custom character
      line += 9;
    } else if (strncmp(line, "_sun", 4) == 0) {
      lcd.write_char(4);  // Sun custom character
      line += 4;
    } else if (strncmp(line, "_house", 6) == 0) {
      lcd.write_char(5);  // House custom character
      line += 6;
    } else if (strncmp(line, "_gosling", 8) == 0) {
      lcd.write_char(6);  // Gosling custom character
      line += 8;
    } else if (strncmp(line, "_robot", 6) == 0) {
      lcd.write_char(7);  // Robot custom character
      line += 6;
    } else {
      lcd.write_char(*line);  // Write regular characters
      line++;
    }
  }
}

// Function to set the message and color on the LCD screen
void setLCDMessageAndColor(const char* line1, const char* line2, const Color &color) {
  lcd.setRGB(color.red, color.green, color.blue);  // Set the color of the LCD backlight

  lcd.setCursor(0, 0);  // Set cursor to the first line
  printWithCustomCharacters(line1);  // Print the first line with custom character handling

  lcd.setCursor(0, 1);  // Set cursor to the second line
  printWithCustomCharacters(line2);  // Print the second line with custom character handling
}

void setup() {
  Serial.begin(9600);

  // Initialize the DS18B20 sensor
  sensors.begin();

  // Initialize RGB LED pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Initialize the LCD
  lcd.init();
  loadCustomCharacters();

  Serial.println("Roz is ready!");

  // Example of using custom characters in messages
  setLCDMessageAndColor("Hello Friends,", "I am Roz the _tree _robot", COLOR_GREEN);
}

void loop() {
  // Your loop code for temperature, proximity, etc.
  delay(1000);  // Delay between sensor readings
}