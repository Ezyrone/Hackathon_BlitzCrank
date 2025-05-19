// === MAPPAGE DES PINS ===
// Moteurs avant gauche (FL)
#define FL_IN1 2
#define FL_IN2 3
#define FL_PWM 5

// Moteurs avant droit (FR)
#define FR_IN1 4
#define FR_IN2 7
#define FR_PWM 6

// Moteurs arrière gauche (BL)
#define BL_IN1 8
#define BL_IN2 9
#define BL_PWM 10

// Moteurs arrière droit (BR)
#define BR_IN1 11
#define BR_IN2 12
#define BR_PWM 13

// Capteur ultrason
#define TRIG A1
#define ECHO A2

// Capteur infrarouge (ligne blanche)
#define IR_SENSOR A0

// === CONSTANTES ===
const int SPEED = 180;
const int TURN_SPEED = 160;
const int STOP_DELAY = 50;

// === SETUP ===
void setup() {
  Serial.begin(9600);

  int pins[] = {FL_IN1, FL_IN2, FR_IN1, FR_IN2, BL_IN1, BL_IN2, BR_IN1, BR_IN2};
  for (int i = 0; i < 8; i++) pinMode(pins[i], OUTPUT);

  pinMode(FL_PWM, OUTPUT); pinMode(FR_PWM, OUTPUT);
  pinMode(BL_PWM, OUTPUT); pinMode(BR_PWM, OUTPUT);

  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(IR_SENSOR, INPUT);

  stop_all();
  delay(500);
  Serial.println("READY");
}

// === LOOP PRINCIPALE ===
void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'F': move_forward(); break;
      case 'B': move_backward(); break;
      case 'L': move_left(); break;
      case 'R': move_right(); break;
      case 'C': rotate_cw(); break;
      case 'A': rotate_ccw(); break;
      case 'S': stop_all(); break;
      case 'D': send_distance(); break;
      case 'I': send_ir(); break;
      default: Serial.println("ERR"); break;
    }
  }
}

// === MOUVEMENTS OMNIDIRECTIONNELS ===
void move_forward() {
  drive_motor(FL_IN1, FL_IN2, FL_PWM, SPEED);
  drive_motor(FR_IN1, FR_IN2, FR_PWM, SPEED);
  drive_motor(BL_IN1, BL_IN2, BL_PWM, SPEED);
  drive_motor(BR_IN1, BR_IN2, BR_PWM, SPEED);
}

void move_backward() {
  drive_motor(FL_IN2, FL_IN1, FL_PWM, SPEED);
  drive_motor(FR_IN2, FR_IN1, FR_PWM, SPEED);
  drive_motor(BL_IN2, BL_IN1, BL_PWM, SPEED);
  drive_motor(BR_IN2, BR_IN1, BR_PWM, SPEED);
}

void move_left() {
  drive_motor(FL_IN2, FL_IN1, FL_PWM, SPEED);
  drive_motor(FR_IN1, FR_IN2, FR_PWM, SPEED);
  drive_motor(BL_IN1, BL_IN2, BL_PWM, SPEED);
  drive_motor(BR_IN2, BR_IN1, BR_PWM, SPEED);
}

void move_right() {
  drive_motor(FL_IN1, FL_IN2, FL_PWM, SPEED);
  drive_motor(FR_IN2, FR_IN1, FR_PWM, SPEED);
  drive_motor(BL_IN2, BL_IN1, BL_PWM, SPEED);
  drive_motor(BR_IN1, BR_IN2, BR_PWM, SPEED);
}

void rotate_cw() {
  drive_motor(FL_IN1, FL_IN2, FL_PWM, TURN_SPEED);
  drive_motor(FR_IN2, FR_IN1, FR_PWM, TURN_SPEED);
  drive_motor(BL_IN1, BL_IN2, BL_PWM, TURN_SPEED);
  drive_motor(BR_IN2, BR_IN1, BR_PWM, TURN_SPEED);
}

void rotate_ccw() {
  drive_motor(FL_IN2, FL_IN1, FL_PWM, TURN_SPEED);
  drive_motor(FR_IN1, FR_IN2, FR_PWM, TURN_SPEED);
  drive_motor(BL_IN2, BL_IN1, BL_PWM, TURN_SPEED);
  drive_motor(BR_IN1, BR_IN2, BR_PWM, TURN_SPEED);
}

// === MOTEUR INDIVIDUEL ===
void drive_motor(int in1, int in2, int pwm_pin, int speed) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(pwm_pin, speed);
}

void stop_all() {
  analogWrite(FL_PWM, 0);
  analogWrite(FR_PWM, 0);
  analogWrite(BL_PWM, 0);
  analogWrite(BR_PWM, 0);
}

// === CAPTEURS ===
void send_distance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.println(distance);
}

void send_ir() {
  int value = analogRead(IR_SENSOR);
  Serial.println(value);
}
