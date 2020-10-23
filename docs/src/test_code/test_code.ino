#define MOTOR_LEFT 5
#define MOTOR_RIGHT 6

void setup() {
  Serial.begin(9600);

  //motor driver setting: H-bridge
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
}

void loop() {
  int menu = 0;
  if (Serial.available() > 0) {
    while (Serial.read() != '\n');
    while (1) {
      if (0 <= menu && menu <= 3) {
        for (int i = 0; i < 10; i++) {
          Serial.print("IR SENSOR ");
          Serial.print(menu);
          Serial.print(": ");
          Serial.println(analogRead(menu));
          delay(100);
        }
      } else if (menu == 4) {
        Serial.println("MOTOR LEFT ON");
        analogWrite(MOTOR_LEFT, 255);
      } else if (menu == 5) {
        Serial.println("MOTOR RIGHT ON");
        analogWrite(MOTOR_RIGHT, 255);
      }
      while (Serial.read() != '\n');
      analogWrite(MOTOR_LEFT, 0);
      analogWrite(MOTOR_RIGHT, 0);
      if (++menu >= 6)
        menu = 0;
    }
  }
}