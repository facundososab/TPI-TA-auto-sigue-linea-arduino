// ==== Sensores ====
const int pinSensorI = 13;
const int pinSensorM = 12;
const int pinSensorD = 11;

// ==== Motor Izquierdo ====
const int in1 = 6;
const int in2 = 5;
const int enableI = 2;

// ==== Motor Derecho ====
const int in3 = 4;
const int in4 = 3;
const int enableD = 7;

// ==== PID ====
const float Kp = 5, Ki = 0.5, Kd = 20;
int P = 0, I = 0, D = 0;
int ultimoError = 0;

// ==== Configuración general ====
const int velocidadBase = 150;  // Menor a 255 para dejar margen al PID
const int pwmMax = 255;
const int pwmMin = 0;

void setup() {
  // Sensores
  pinMode(pinSensorI, INPUT);
  pinMode(pinSensorM, INPUT);
  pinMode(pinSensorD, INPUT);

  // Motores
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT); pinMode(enableI, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT); pinMode(enableD, OUTPUT);

  // Inicializamos todo en apagado
  detenerMotores();
}

void loop() {
  // Leer sensores
  int sI = digitalRead(pinSensorI);
  int sM = digitalRead(pinSensorM);
  int sD = digitalRead(pinSensorD);

  // Obtener posición deseada
  int posicionActual = calcularPosicion(sI, sM, sD);
  int error = posicionActual - 1000;

  // Control PID
  P = error;
  I += error;
  D = error - ultimoError;
  ultimoError = error;

  int salidaPID = P * Kp + I * Ki + D * Kd;

  // Calcular velocidades
  int velD = constrain(velocidadBase + salidaPID, pwmMin, pwmMax);
  int velI = constrain(velocidadBase - salidaPID, pwmMin, pwmMax);

  moverMotores(velI, velD);
}

void moverMotores(int velI, int velD) {
  // Motor Izquierdo
  if (velI >= 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    velI = -velI;
  }

  // Motor Derecho
  if (velD >= 0) {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    velD = -velD;
  }

  analogWrite(enableI, constrain(velI, pwmMin, pwmMax));
  analogWrite(enableD, constrain(velD, pwmMin, pwmMax));
}

int calcularPosicion(int i, int m, int d) {
  // Codificamos posibles combinaciones
  if (i == LOW && m == HIGH && d == LOW)  return 1000;
  if (i == LOW && m == LOW  && d == HIGH) return 2000;
  if (i == HIGH && m == LOW && d == LOW)  return 0;
  if (i == LOW && m == HIGH && d == HIGH) return 1500;
  if (i == HIGH && m == HIGH && d == LOW) return 500;
  if (i == HIGH && m == LOW && d == HIGH) return 1000; // Mantener recto
  if (i == LOW && m == LOW && d == LOW)   return 1000; // Sin línea (recto)
  return 1000; // Todos HIGH (probablemente intersección)
}

void detenerMotores() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW); analogWrite(enableI, 0);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW); analogWrite(enableD, 0);
}
