// Contrôle moteur via L298N
#define ENA 5
#define IN1 6
#define IN2 7

#define ENB 10
#define IN3 8
#define IN4 9

// Capteur ultrason
#define TRIG 2
#define ECHO 3

// Capteur IR (ligne blanche)
#define IR_SENSOR A0

// Vitesses prédéfinies
#define SPEED_LOW 120
#define SPEED_MED 180
#define SPEED_HIGH 255

void setup() {
  Serial.begin(9600);
  
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  pinMode(IR_SENSOR, INPUT);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    
    // Commandes basiques
    switch(cmd) {
      case 'F': 
        // Vérifie s'il y a un paramètre de vitesse
        if (Serial.available()) {
          char speed = Serial.read();
          if (speed == 'L') forward(SPEED_LOW);
          else if (speed == 'M') forward(SPEED_MED);
          else if (speed == 'H') forward(SPEED_HIGH);
          else forward(SPEED_MED); // Par défaut
        } else {
          forward(SPEED_MED); // Par défaut
        }
        break;
      case 'B': 
        if (Serial.available()) {
          char speed = Serial.read();
          if (speed == 'L') backward(SPEED_LOW);
          else if (speed == 'M') backward(SPEED_MED);
          else if (speed == 'H') backward(SPEED_HIGH);
          else backward(SPEED_MED);
        } else {
          backward(SPEED_MED);
        }
        break;
      case 'L': 
        if (Serial.available()) {
          char speed = Serial.read();
          if (speed == 'L') turnLeft(SPEED_LOW);
          else if (speed == 'M') turnLeft(SPEED_MED);
          else if (speed == 'H') turnLeft(SPEED_HIGH);
          else turnLeft(SPEED_MED);
        } else {
          turnLeft(SPEED_MED);
        }
        break;
      case 'R': 
        if (Serial.available()) {
          char speed = Serial.read();
          if (speed == 'L') turnRight(SPEED_LOW);
          else if (speed == 'M') turnRight(SPEED_MED);
          else if (speed == 'H') turnRight(SPEED_HIGH);
          else turnRight(SPEED_MED);
        } else {
          turnRight(SPEED_MED);
        }
        break;
      case 'S': stopMotors(); break;
      case 'D': sendDistance(); break;
      case 'I': sendIR(); break;
    }
  }
}

void forward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void backward(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void turnLeft(int speed) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void turnRight(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void sendDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.println(distance); // cm
}

void sendIR() {
  int value = analogRead(IR_SENSOR);
  Serial.println(value);
}
