#include <SoftwareSerial.h>

#define SENSOR_COUNT 4
#define MOTOR_LEFT 5
#define MOTOR_RIGHT 6

#define BT_TX 12
#define BT_RX 13

SoftwareSerial BTSerial(BT_TX, BT_RX);

unsigned char blackmax[SENSOR_COUNT];
unsigned char whitemax[SENSOR_COUNT];
unsigned char sensor[SENSOR_COUNT];
short position;

void printSensor(unsigned char* value);

void setup() {
  // Initialize
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);

  BTSerial.begin(9600);
//
//  for (;;) {
//    if (BTSerial.available()) {
//      Serial.write(BTSerial.read());
//    }
//    if (Serial.available()) {
//      BTSerial.write(Serial.read());
//    }
//    delay(1);
//  }

  // Get sensor value
  Serial.println("Press enter to check sensor value");
  while (Serial.read() != '\n') { }

  Serial.println(" .. Press enter to exit .. ");
  while (Serial.read() != '\n') {
    for (int i = 0; i < SENSOR_COUNT; i++) {
      sensor[i] = 255 - (analogRead(i) >> 2);
    }
    printSensor(sensor);
    delay(100);
  }

  unsigned char analogValue;
  // Get blackmax
  Serial.println("Press enter to check blackmax");
  while (Serial.read() != '\n') { }

  for (int i = 0; i < SENSOR_COUNT; i++) {
    blackmax[i] = 0;
  }

  Serial.println(" .. Press enter to exit .. ");
  while (Serial.read() != '\n') {
    for (int i = 0; i < SENSOR_COUNT; i++) {
      analogValue = 255 - (analogRead(i) >> 2);

      if (blackmax[i] < analogValue) {
        blackmax[i] = analogValue;
      }
    }
    printSensor(blackmax);
  }

  // Get whitemax
  Serial.println("Press Enter to check whitemax");
  while (Serial.read() != '\n') { }

  for (int i = 0; i < SENSOR_COUNT; i++) {
    whitemax[i] = 0;
  }

  Serial.println(" .. Press enter to exit .. ");
  while (Serial.read() != '\n') {
    for (int i = 0; i < SENSOR_COUNT; i++) {
      analogValue = 255 - (analogRead(i) >> 2);

      if (whitemax[i] < analogValue) {
        whitemax[i] = analogValue;
      }
    }
    printSensor(whitemax);
  }
}

void loop() {
  static short pos_n = 0;
  static short pos_m = 0;
  static float pos_k = 0.007f;
  static short sensor_dist[SENSOR_COUNT] = { -96, -32, 32, 96 };
  static short velo = 75;
  static float v_l = 0.0f;
  static float v_r = 0.0f;

  for (;;) {
    // Normalization (Using constrain)
    for (int i = 0; i < SENSOR_COUNT; i++) {
      sensor[i] = 255 - (analogRead(i) >> 2);
      sensor[i] = constrain(sensor[i], blackmax[i], whitemax[i]);
      // https://www.arduino.cc/reference/ko/language/functions/math/map/
      sensor[i] = map(sensor[i], blackmax[i], whitemax[i], 0, 255);
    }
    printSensor(sensor);
    
    // Get position value
    pos_n = 0;
    pos_m = 0;
    for (int i = 0; i < SENSOR_COUNT; i++) {
      pos_n += sensor[i] * sensor_dist[i];
      pos_m += sensor[i];
    }

    if (pos_m == 0) position = 0;
    else position = pos_n / pos_m;

    v_l = velo * (1.0f + (pos_k * position));
    v_l = constrain(v_l, 0, 255);
    v_r = velo * (1.0f - (pos_k * position));
    v_r = constrain(v_r, 0, 255);

    if (v_l < 20) {
      for (int i = 0; i < 10; i++)
        analogWrite(MOTOR_LEFT, 255);
    }
    if (v_r < 20) {
      for (int i = 0; i < 10; i++)
        analogWrite(MOTOR_RIGHT, 255);
    }

    analogWrite(MOTOR_LEFT, v_l);
    analogWrite(MOTOR_RIGHT, v_r);
  }
}

void printSensor(unsigned char* value) {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    Serial.print(value[i]);
    Serial.print('\t');
  }
  Serial.print('\n');
}
