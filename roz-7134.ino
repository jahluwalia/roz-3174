#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>
#include "Waveshare_LCD1602_RGB.h"

// Pins
#define DS18B20_PIN 2     // DS18B20 connected to pin 2
#define TRIGGER_PIN 9     // HC-SR04 trigger pin connected to pin 9
#define ECHO_PIN 10       // HC-SR04 echo pin connected to pin 10
#define MAX_DISTANCE 200  // Maximum distance we want to measure (in cm)

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
const Color COLOR_RED = { 255, 0, 0 };
const Color COLOR_GREEN = { 0, 255, 0 };
const Color COLOR_BLUE = { 0, 0, 255 };
const Color COLOR_YELLOW = { 255, 255, 0 };
const Color COLOR_PURPLE = { 128, 0, 128 };
const Color COLOR_ORANGE = { 255, 165, 0 };
const Color COLOR_WHITE = { 255, 255, 255 };

// Waveshare LCD setup
Waveshare_LCD1602_RGB lcd(16, 2);

/**
 * FUNCTION: setLCDMessageAndColor
 * This function displays a message on Roz's screen and changes the backlight color.
 * It clears the screen before writing, sets the cursor for each line,
 * and then writes the text provided for each line.
 * 
 * In the story: Roz is sharing messages with the world or warning about events.
 * 
 * Parameters:
 * - line1: The message for the first line of the LCD screen.
 * - line2: The message for the second line of the LCD screen.
 * - color: The RGB color for the LCD backlight.
 */
void setLCDMessageAndColor(const char* line1, const char* line2, const Color& color) {
  lcd.setRGB(color.red, color.green, color.blue);  // Set the color of the LCD backlight
  lcd.clear();                                    // Clear the screen to remove previous messages
  lcd.setCursor(0, 0);                            // Set cursor to the first line
  lcd.send_string(line1);                         // Write the first line to the LCD
  
  lcd.setCursor(0, 1);                            // Set cursor to the second line
  lcd.send_string(line2);                         // Write the second line to the LCD
}

const int MAX_RECORDS = 10;  // Maximum number of temperature records
float tempRecords[MAX_RECORDS];
unsigned long timeRecords[MAX_RECORDS];
int recordIndex = 0;
// Time and temperature thresholds
const unsigned long timeThreshold = 10000;  // 10 seconds
const float tempDropThreshold = 10.0;       // Temperature drop threshold (e.g., 10°F)
const float tempRiseThreshold = 10.0;       // Temperature rise threshold (e.g., 10°F)

/**
 * FUNCTION: addTemperatureRecord
 * This function keeps track of the most recent temperature readings and their timestamps.
 * It stores the temperature and time in a circular buffer (fixed-size memory).
 * 
 * In the story: Roz remembers the recent temperatures to figure out if something unusual is happening.
 * 
 * Parameters:
 * - currentTemperature: The current temperature reading from the sensor.
 * - currentTime: The current time in milliseconds.
 */
void addTemperatureRecord(float currentTemperature, unsigned long currentTime) {
  tempRecords[recordIndex] = currentTemperature;
  timeRecords[recordIndex] = currentTime;
  recordIndex = (recordIndex + 1) % MAX_RECORDS; // Wrap around if the index exceeds MAX_RECORDS
}

/**
 * FUNCTION: checkTemperatureDrop
 * This function checks if the temperature has dropped significantly within the last 10 seconds.
 * 
 * In the story: Roz detects if a snowstorm is coming.
 * 
 * Parameters:
 * - currentTemperature: The current temperature reading.
 * - currentTime: The current time in milliseconds.
 * 
 * Returns:
 * - true if a significant drop is detected; false otherwise.
 */
bool checkTemperatureDrop(float currentTemperature, unsigned long currentTime) {
  for (int i = 0; i < MAX_RECORDS; i++) {
    if (currentTime - timeRecords[i] <= timeThreshold) {
      if (tempRecords[i] - currentTemperature >= tempDropThreshold) {
        return true;
      }
    }
  }
  return false;
}

/**
 * FUNCTION: checkTemperatureRise
 * This function checks if the temperature has risen significantly within the last 10 seconds.
 * 
 * In the story: Roz detects if there's a fire nearby.
 * 
 * Parameters:
 * - currentTemperature: The current temperature reading.
 * - currentTime: The current time in milliseconds.
 * 
 * Returns:
 * - true if a significant rise is detected; false otherwise.
 */
bool checkTemperatureRise(float currentTemperature, unsigned long currentTime) {
  for (int i = 0; i < MAX_RECORDS; i++) {
    if (currentTime - timeRecords[i] <= timeThreshold) {
      if (currentTemperature - tempRecords[i] >= tempRiseThreshold) {
        return true;
      }
    }
  }
  return false;
}

/**
 * FUNCTION: setup
 * This is where Roz "wakes up" and prepares her sensors and screen.
 * It only runs once when Roz is turned on.
 * 
 * In the story: Roz gets ready to greet the world and remember the initial temperature.
 */
void setup() {
  Serial.begin(9600); // Start communication so Roz can "talk" to us through the computer

  // Turn on the temperature sensor
  sensors.begin();

  // Set up Roz's screen
  lcd.init();

  // Take the first temperature reading to fill Roz's memory
  sensors.requestTemperatures();
  float initialTemperature = sensors.getTempFByIndex(0);
  for (int i = 0; i < MAX_RECORDS; i++) {
    tempRecords[i] = initialTemperature;
    timeRecords[i] = millis(); // Record the time Roz started
  }

  // Say hello to the world!
  Serial.println("Roz is ready!");
  setLCDMessageAndColor("My Name is Roz,", "The Wild Robot", COLOR_GREEN);
}

/**
 * FUNCTION: loop
 * This is where Roz continuously checks her environment for changes.
 * It repeats over and over while Roz is powered on.
 * 
 * In the story: Roz monitors the temperature and proximity to detect important events and respond.
 */
void loop() {
  unsigned long currentTime = millis(); // The current time in milliseconds

  // Read the temperature
  sensors.requestTemperatures();
  float currentTemperature = sensors.getTempFByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(currentTemperature);
  Serial.println("°F");

  // Save the temperature and time
  addTemperatureRecord(currentTemperature, currentTime);

  // Priority 1: Check for snowstorm (big temperature drop)
  if (checkTemperatureDrop(currentTemperature, currentTime)) {
    Serial.println("Temperature dropped by 10°F or more within 10 seconds!");
    setLCDMessageAndColor("A snow storm!", "Get to Shelter!", COLOR_BLUE);
    delay(15000); // Wait so Roz can show this message
    return; // Skip checking anything else for now
  }

  // Priority 1: Check for fire (big temperature rise)
  if (checkTemperatureRise(currentTemperature, currentTime)) {
    Serial.println("Temperature rose by 10°F or more within 10 seconds!");
    setLCDMessageAndColor("Danger, Fire!", "Evacuate!", COLOR_RED);
    delay(15000); // Wait so Roz can show this message
    return; // Skip checking anything else for now
  }

  // Priority 2: Check proximity (how close something is to Roz)
  unsigned int distance = sonar.ping_cm(); // Measure the distance
  if (distance > 0) {
    Serial.print("Proximity: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 10 && distance < 50) { // Something is close, but not too close
      setLCDMessageAndColor("Hi! How can", "I assist you?", COLOR_PURPLE);
    } else if (distance < 5) { // Something is too close
      setLCDMessageAndColor("Danger!", "Please step back", COLOR_RED);
    } else { // Nothing is close
      setLCDMessageAndColor("My Name is Roz,", "The Wild Robot", COLOR_GREEN);
    }
    delay(1000); // Wait a little before checking again
    return;
  }

  // Priority 3: Default display (nothing urgent is happening)
  Serial.println("Proximity: Out of range");
  setLCDMessageAndColor("My Name is Roz,", "The Wild Robot", COLOR_GREEN);

  delay(10); // Small delay to stabilize readings
}
