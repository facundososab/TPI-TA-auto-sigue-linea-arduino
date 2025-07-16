// Sensores
const int sensorI = 13;
const int sensorM = 12;
const int sensorD = 11;

// Motor izquierdo
const int enablePinIzq = 7;
const int mIzq1 = 6;
const int mIzq2 = 5;

// Motor derecho
const int mDer1 = 4;
const int mDer2 = 3;
const int enablePinDer = 2;

// Parámetros PID
const float Kp = 5.0, Ki = 0.5, Kd = 20.0;
int P = 0, I = 0, D = 0;

int ultimoError = 0;
const int referencia = 1000;
const int velocidadBase = 200;

void setup() {
  pinMode(sensorI, INPUT);
  pinMode(sensorM, INPUT);
  pinMode(sensorD, INPUT);

  pinMode(mIzq1, OUTPUT);
  pinMode(mIzq2, OUTPUT);
  pinMode(enablePinIzq, OUTPUT);

  pinMode(mDer1, OUTPUT);
  pinMode(mDer2, OUTPUT);
  pinMode(enablePinDer, OUTPUT);

  // Inicialización de motores en reposo
  digitalWrite(mIzq1, LOW);
  digitalWrite(mIzq2, LOW);
  analogWrite(enablePinIzq, 0);

  digitalWrite(mDer1, LOW);
  digitalWrite(mDer2, LOW);
  analogWrite(enablePinDer, 0);

  // Serial.begin(9600); // Activá esto si querés debuggear
  delay(1000); // Espera para estabilizar sensores
}

void loop() {
  int inputI = digitalRead(sensorI);
  int inputM = digitalRead(sensorM);
  int inputD = digitalRead(sensorD);

  int pos = posicion(inputI, inputM, inputD);
  int error = pos - referencia;

  // PID
  P = error;
  I += error;
  I = constrain(I, -300, 300); // Anti-windup
  D = error - ultimoError;
  ultimoError = error;

  int PID = P * Kp + I * Ki + D * Kd;

  int velocidadDer = constrain(velocidadBase + PID, -255, 255);
  int velocidadIzq = constrain(velocidadBase - PID, -255, 255);

  cambiar_velocidad(velocidadIzq, velocidadDer); // ¡OJO! Motor izquierdo primero

  /*
  // Debug por serial
  Serial.print("Error: ");
  Serial.print(error);
  Serial.print(" | PID: ");
  Serial.print(PID);
  Serial.print(" | Vel I: ");
  Serial.print(velocidadIzq);
  Serial.print(" | Vel D: ");
  Serial.println(velocidadDer);
  */
}

void cambiar_velocidad(int velIzq, int velDer) {
  // Motor izquierdo
  if (velIzq < 0) {
    digitalWrite(mIzq1, LOW);
    digitalWrite(mIzq2, HIGH);
    velIzq = -velIzq;
  } else {
    digitalWrite(mIzq1, HIGH);
    digitalWrite(mIzq2, LOW);
  }

  // Motor derecho
  if (velDer < 0) {
    digitalWrite(mDer1, LOW);
    digitalWrite(mDer2, HIGH);
    velDer = -velDer;
  } else {
    digitalWrite(mDer1, HIGH);
    digitalWrite(mDer2, LOW);
  }

  analogWrite(enablePinIzq, constrain(velIzq, 0, 255));
  analogWrite(enablePinDer, constrain(velDer, 0, 255));
}

int posicion(int i, int c, int d) {
  if (i == LOW && c == HIGH && d == LOW) return 1000;  // Centro
  if (i == LOW && c == LOW && d == HIGH) return 2000;  // Derecha
  if (i == HIGH && c == LOW && d == LOW) return 0;     // Izquierda
  if (i == LOW && c == HIGH && d == HIGH) return 1500; // Centro + Derecha
  if (i == HIGH && c == HIGH && d == LOW) return 500;  // Centro + Izquierda
  if (i == HIGH && c == LOW && d == HIGH) return 1000; // Izq + Der (cruce)
  if (i == LOW && c == LOW && d == LOW) return ultimoError > 0 ? 2000 : 0; // Línea perdida → sigue última dirección
  return 1000; // Por defecto: mantener recto
}
