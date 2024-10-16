#include <OneWire.h>
#include <DallasTemperature.h>
#include <NewPing.h>

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

// Variables for RGB LED
int rainbow[][3] = {
  {255, 0, 0},   // Red
  {255, 127, 0}, // Orange
  {255, 255, 0}, // Yellow
  {0, 255, 0},   // Green
  {0, 0, 255},   // Blue
  {75, 0, 130},  // Indigo
  {143, 0, 255}  // Violet
};

int index = 0;
unsigned long previousMillis = 0;    // Stores last LED update time
const long interval = 1000;          // Interval at which to change LED color

void setup() {
  Serial.begin(9600);

  // Initialize the DS18B20 sensor
  sensors.begin();

  // Initialize RGB LED pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.println("Roz is ready!");
}

void loop() {
  // Read temperature from DS18B20
  sensors.requestTemperatures();
  float temperature = sensors.getTempFByIndex(0);  // Get temperature in Fahrenheit
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°F");

  // Read distance from HC-SR04
  unsigned int distance = sonar.ping_cm();
  if (distance > 0) {
    Serial.print("Proximity: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Proximity: Out of range");
  }

  // Change the RGB LED color every second (1000ms)
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Set the RGB LED to the next color in the rainbow
    analogWrite(RED_PIN, rainbow[index][0]);
    analogWrite(GREEN_PIN, rainbow[index][1]);
    analogWrite(BLUE_PIN, rainbow[index][2]);

    index++;
    if (index >= 7) {
      index = 0;
    }
  }

  delay(1000);  // Delay between sensor readings
}