#define BLYNK_TEMPLATE_ID "************************"
#define BLYNK_TEMPLATE_NAME "Traffic Light Controller"
 
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
 
char auth[] = "************************";
char ssid[] = "************************";
char pass[] = "************************";
 
// GPIO pins for each traffic signal light (red, yellow, green)
int signal1[] = {26, 25, 33};
int signal2[] = {19, 18, 5};
int signal3[] = {32, 3, 21};
int signal4[] = {17, 16, 15};
 
// Timing delays for light changes (milliseconds)
int redDelay = 5000;
int yellowDelay = 2000;
 
// Ultrasonic sensor pins
int triggerpin1 = 13;
int echopin1 = 12;
int triggerpin2 = 27;
int echopin2 = 14;
int triggerpin3 = 22;
int echopin3 = 23;
int triggerpin4 = 2;
int echopin4 = 4;
 
long duration; // Time for ultrasonic signal to return
int S1, S2, S3, S4; // Distance measurements from each sensor
 
int t = 5; // Threshold distance to check for traffic presence
 
 
void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
 
  // Initialize traffic signal LEDs as outputs
  for (int i = 0; i < 3; i++)
  {
    pinMode(signal1[i], OUTPUT);
    pinMode(signal2[i], OUTPUT);
    pinMode(signal3[i], OUTPUT);
    pinMode(signal4[i], OUTPUT);
  }
 
  // Initialize ultrasonic sensor pins
  pinMode(triggerpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(triggerpin2, OUTPUT);
  pinMode(echopin2, INPUT);
  pinMode(triggerpin3, OUTPUT);
  pinMode(echopin3, INPUT);
  pinMode(triggerpin4, OUTPUT);
  pinMode(echopin4, INPUT);
}
 
 
void loop()
{
  Blynk.run();
 
  // Measure distance from each ultrasonic sensor
  S1 = readDistance(triggerpin1, echopin1);
  S2 = readDistance(triggerpin2, echopin2);
  S3 = readDistance(triggerpin3, echopin3);
  S4 = readDistance(triggerpin4, echopin4);
 
  // Check and update the status of each traffic signal
  if (S1 < t)
  {
    signal1Function();
  }
  else if (S2 < t)
  {
    signal2Function();
  }
  else if (S3 < t)
  {
    signal3Function();
  }
  else if (S4 < t)
  {
    signal4Function();
  }
 
  // Send updated signal status to Blynk
  updateBlynk();
 
  // Wait a bit before next loop iteration
  delay(1000);
}
 
 
int readDistance(int triggerPin, int echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}
 
 
// Define functions for handling each signal
void signal1Function()
{
  controlSignal(signal1, V1, V2, V3);
}
void signal2Function()
{
  controlSignal(signal2, V4, V5, V6);
}
void signal3Function()
{
  controlSignal(signal3, V7, V8, V9);
}
void signal4Function()
{
  controlSignal(signal4, V10, V11, V12);
}
 
 
// Controls the signal light changes and updates Blynk
void controlSignal(int signalPins[], int vPinRed, int vPinYellow, int vPinGreen)
{
  lowAll(); // Reset all signals to red before changing the current one
  digitalWrite(signalPins[0], HIGH); // Turn on red LED
  Blynk.virtualWrite(vPinRed, 255);
  delay(redDelay);
 
  digitalWrite(signalPins[0], LOW); // Turn off red LED
  Blynk.virtualWrite(vPinRed, 0);
 
  digitalWrite(signalPins[2], HIGH); // Turn on green LED
  Blynk.virtualWrite(vPinGreen, 255);
  delay(redDelay);
 
  digitalWrite(signalPins[2], LOW); // Turn off green LED
  Blynk.virtualWrite(vPinGreen, 0);
 
  digitalWrite(signalPins[1], HIGH); // Turn on yellow LED
  Blynk.virtualWrite(vPinYellow, 255);
  delay(yellowDelay);
 
  digitalWrite(signalPins[1], LOW); // Turn off yellow LED
  Blynk.virtualWrite(vPinYellow, 0);
}
 
 
// Check if there is traffic at any signal
bool anyTraffic()
{
  return (S1 < t || S2 < t || S3 < t || S4 < t);
}
 
 
// Set all signals to red
void lowAll()
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(signal1[0], HIGH);
    digitalWrite(signal1[1], LOW);
    digitalWrite(signal1[2], LOW);
    digitalWrite(signal2[0], HIGH);
    digitalWrite(signal2[1], LOW);
    digitalWrite(signal2[2], LOW);
    digitalWrite(signal3[0], HIGH);
    digitalWrite(signal3[1], LOW);
    digitalWrite(signal3[2], LOW);
    digitalWrite(signal4[0], HIGH);
    digitalWrite(signal4[1], LOW);
    digitalWrite(signal4[2], LOW);
  }
}
 
 
// Update Blynk with the current status of traffic signals
void updateBlynk()
{
  Blynk.virtualWrite(V1, digitalRead(signal1[0]) == HIGH ? 255 : 0); // Signal 1 Red
  Blynk.virtualWrite(V2, digitalRead(signal1[1]) == HIGH ? 255 : 0); // Signal 1 Yellow
  Blynk.virtualWrite(V3, digitalRead(signal1[2]) == HIGH ? 255 : 0); // Signal 1 Green
 
  Blynk.virtualWrite(V4, digitalRead(signal2[0]) == HIGH ? 255 : 0); // Signal 2 Red
  Blynk.virtualWrite(V5, digitalRead(signal2[1]) == HIGH ? 255 : 0); // Signal 2 Yellow
  Blynk.virtualWrite(V6, digitalRead(signal2[2]) == HIGH ? 255 : 0); // Signal 2 Green
 
  Blynk.virtualWrite(V7, digitalRead(signal3[0]) == HIGH ? 255 : 0); // Signal 3 Red
  Blynk.virtualWrite(V8, digitalRead(signal3[1]) == HIGH ? 255 : 0); // Signal 3 Yellow
  Blynk.virtualWrite(V9, digitalRead(signal3[2]) == HIGH ? 255 : 0); // Signal 3 Green
 
  Blynk.virtualWrite(V10, digitalRead(signal4[0]) == HIGH ? 255 : 0); // Signal 4 Red
  Blynk.virtualWrite(V11, digitalRead(signal4[1]) == HIGH ? 255 : 0); // Signal 4 Yellow
  Blynk.virtualWrite(V12, digitalRead(signal4[2]) == HIGH ? 255 : 0); // Signal 4 Green
}






