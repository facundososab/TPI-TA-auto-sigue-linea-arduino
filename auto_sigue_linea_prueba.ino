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

// PID
const float Kp = 3.0, Ki = 0.2, Kd = 8.0;
int P = 0, I = 0, D = 0;
int ultimoError = 0;

// Velocidades
const int VELOCIDAD_BASE = 150;
const int VELOCIDAD_MAX = 255;
const int VELOCIDAD_ATRAS = -100;

void setup() {
  pinMode(sensorI, INPUT);
  pinMode(sensorM, INPUT);
  pinMode(sensorD, INPUT);

  pinMode(mPin1, OUTPUT);
  pinMode(mPin2, OUTPUT);
  pinMode(enablePin1y2, OUTPUT);

  pinMode(mPin3, OUTPUT);
  pinMode(mPin4, OUTPUT);
  pinMode(enablePin3y4, OUTPUT);
}

void loop() {
  int i = digitalRead(sensorI);
  int m = digitalRead(sensorM);
  int d = digitalRead(sensorD);

  int error = calcularError(i, m, d);
  P = error;
  I += error;
  D = error - ultimoError;
  ultimoError = error;

  // Anti-windup
  if (abs(error) < 50) I = 0;

  int PID = -(P * Kp + I * Ki + D * Kd); // Signo invertido según lógica del robot

  int velI = VELOCIDAD_BASE - PID;
  int velD = VELOCIDAD_BASE + PID;

  // Condiciones especiales
  if (i == HIGH && m == LOW && d == LOW) { // Solo izquierda
    velI = 0;
    velD = VELOCIDAD_MAX;
  } else if (i == LOW && m == LOW && d == HIGH) { // Solo derecha
    velI = VELOCIDAD_MAX;
    velD = 0;
  } else if (i == HIGH && m == HIGH && d == LOW) { // izquierda + medio
    velI = VELOCIDAD_ATRAS;
    velD = VELOCIDAD_MAX;
  } else if (i == LOW && m == HIGH && d == HIGH) { // derecha + medio
    velI = VELOCIDAD_MAX;
    velD = VELOCIDAD_ATRAS;
  } else if ((i == HIGH && m == LOW && d == HIGH) || (i == LOW && m == LOW && d == LOW) || (i == HIGH && m == HIGH && d == HIGH)) {
    // Ambos lados, ninguno o todos activos -> seguir recto
    velI = VELOCIDAD_BASE;
    velD = VELOCIDAD_BASE;
  }

  cambiarVelocidad(velI, velD);
}

// Códigos de error:
// 0 = centro (1000), izquierda = 0, derecha = 2000
int calcularError(int i, int m, int d) {
  if (i == LOW && m == HIGH && d == LOW) return 0;       // Centro
  if (i == HIGH && m == LOW && d == LOW) return -1000;   // Izquierda
  if (i == LOW && m == LOW && d == HIGH) return 1000;    // Derecha
  if (i == HIGH && m == HIGH && d == LOW) return -500;   // Izq + medio
  if (i == LOW && m == HIGH && d == HIGH) return 500;    // Der + medio
  if (i == HIGH && m == LOW && d == HIGH) return 0;      // I + D
  if (i == LOW && m == LOW && d == LOW) return 0;        // Ninguno
  if (i == HIGH && m == HIGH && d == HIGH) return 0;     // Todos
  return 0;
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

  analogWrite(enablePin1y2, constrain(velI, 0, 255));
  analogWrite(enablePin3y4, constrain(velD, 0, 255));
}
