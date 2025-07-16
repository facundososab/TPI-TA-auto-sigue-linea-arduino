// Sensores
const int sensorI = 13;
const int sensorM = 12;
const int sensorD = 11;

// Motor izquierdo
const int enablePin1y2 = 7;
const int mPin1 = 6;
const int mPin2 = 5;

// Motor derecho
const int enablePin3y4 = 2;
const int mPin3 = 4;
const int mPin4 = 3;

// Velocidades
const int VEL_BASE = 150;
const int VEL_ALTA = 200;
const int VEL_ATRAS = -100;

void setup() {
  // Configurar pines de sensores
  pinMode(sensorI, INPUT);
  pinMode(sensorM, INPUT);
  pinMode(sensorD, INPUT);

  // Configurar pines de motores
  pinMode(mPin1, OUTPUT);
  pinMode(mPin2, OUTPUT);
  pinMode(enablePin1y2, OUTPUT);

  pinMode(mPin3, OUTPUT);
  pinMode(mPin4, OUTPUT);
  pinMode(enablePin3y4, OUTPUT);
}

void loop() {
  Serial.begin(9600);
  
  int i = digitalRead(sensorI); // izquierda
  int m = digitalRead(sensorM); // medio
  int d = digitalRead(sensorD); // derecha

  int velI = VEL_BASE;
  int velD = VEL_BASE;

  // Todos los sensores = 0 o todos = 1 -> ir recto
  if ((i == LOW && m == LOW && d == LOW) || (i == HIGH && m == HIGH && d == HIGH)) {
    velI = VEL_BASE;
    velD = VEL_BASE;
  }

  // Solo medio = HIGH -> ir recto
  else if (i == LOW && m == HIGH && d == LOW) {
    velI = VEL_BASE;
    velD = VEL_BASE;
  }

  // Solo izquierda detecta -> girar derecha
  else if (i == HIGH && m == LOW && d == LOW) {
    velI = 0;
    velD = VEL_ALTA;
  }

  // Solo derecha detecta -> girar izquierda
  else if (i == LOW && m == LOW && d == HIGH) {
    velI = VEL_ALTA;
    velD = 0;
  }

  // Izquierda + medio -> izquierda retrocede, derecha acelera
  else if (i == HIGH && m == HIGH && d == LOW) {
    velI = VEL_ATRAS;
    velD = VEL_ALTA;
  }

  // Derecha + medio -> derecha retrocede, izquierda acelera
  else if (i == LOW && m == HIGH && d == HIGH) {
    velI = VEL_ALTA;
    velD = VEL_ATRAS;
  }

  // Solo izquierda + derecha -> mantener recto
  else if (i == HIGH && m == LOW && d == HIGH) {
    velI = VEL_BASE;
    velD = VEL_BASE;
  }

  cambiarVelocidad(velI, velD);
}

void cambiarVelocidad(int velI, int velD) {
  // Motor izquierdo
  if (velI >= 0) {
    digitalWrite(mPin1, HIGH);
    digitalWrite(mPin2, LOW);
  } else {
    digitalWrite(mPin1, LOW);
    digitalWrite(mPin2, HIGH);
    velI = -velI;
  }

  // Motor derecho
  if (velD >= 0) {
    digitalWrite(mPin3, HIGH);
    digitalWrite(mPin4, LOW);
  } else {
    digitalWrite(mPin3, LOW);
    digitalWrite(mPin4, HIGH);
    velD = -velD;
  }

  // Aplicar PWM
  analogWrite(enablePin1y2, constrain(velI, 0, 255));
  analogWrite(enablePin3y4, constrain(velD, 0, 255));

  Serial.print("I: "); Serial.print(i);
  Serial.print(" M: "); Serial.print(m);
  Serial.print(" D: "); Serial.println(d);
}
