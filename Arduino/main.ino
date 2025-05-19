// === CONFIGURATION MOTEURS ===
#define ENA 5
#define IN1 6
#define IN2 7

#define ENB 10
#define IN3 8
#define IN4 9

// === CAPTEUR ULTRASON ===
#define TRIG 2
#define ECHO 3

// === CAPTEUR IR (ligne) ===
#define IR_SENSOR A0

// === CONSTANTES ===
const int SPEED = 180;
const int TURN_SPEED = 160;
const int STOP_DELAY = 50; // ms entre actions

// === SETUP ===
void setup() {
  Serial.begin(9600);

  // moteurs
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // capteurs
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR_SENSOR, INPUT);

  stopMotors();
  delay(500);
  Serial.println("READY");
}

// === BOUCLE PRINCIPALE ===
void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopMotors(); break;
      case 'D': sendDistance(); break;
      case 'I': sendIR(); break;
      default:
        Serial.println("ERR");
        break;
    }
  }
}

// === FONCTIONS MOTEUR ===
void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
  delay(STOP_DELAY);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, SPEED); analogWrite(ENB, SPEED);
  delay(STOP_DELAY);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, TURN_SPEED); analogWrite(ENB, TURN_SPEED);
  delay(STOP_DELAY);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, TURN_SPEED); analogWrite(ENB, TURN_SPEED);
  delay(STOP_DELAY);
}

void stopMotors() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

// === FONCTIONS CAPTEURS ===
void sendDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.println(distance);
}

void sendIR() {
  int value = analogRead(IR_SENSOR);
  Serial.println(value);
}
