// === Sensores ===
const int sensorI = 13;
const int sensorM = 12;
const int sensorD = 11;

// === Motor izquierdo ===
const int enablePin1y2 = 7;
const int mPin1 = 6;
const int mPin2 = 5;

// === Motor derecho ===
const int enablePin3y4 = 2;
const int mPin3 = 4;
const int mPin4 = 3;

// === PID ===
const float Kp = 0.7, Ki = 0.0005, Kd = 0.02;
int P = 0, I = 0, D = 0;
int ultimoError = 0;

// === Velocidades ===
const int VELOCIDAD_BASE   = 255;
const int VELOCIDAD_MAX    = 255;
const int VELOCIDAD_MIN    = 100;   // Mínimo útil
const int VELOCIDAD_ATRAS  = -200;  // Para giros en eje más marcados

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

  // === Controlador PID ===
  P = error;
  I += error;
  D = error - ultimoError;
  ultimoError = error;

  // Anti-windup: resetear I cuando está cerca del centro
  if (abs(error) < 50) I = 0;

  int PID = -(P * Kp + I * Ki + D * Kd);  // Corregimos dirección

  // === Salidas a motores ===
  int velI = VELOCIDAD_BASE - PID;
  int velD = VELOCIDAD_BASE + PID;

  cambiarVelocidad(velI, velD);
}

// === Cálculo de error lógico para PID ===
int calcularError(int i, int m, int d) {
  if (!i &&  m && !d) return 0;       // Solo medio
  if ( i && !m && !d) return -1000;   // Solo izquierda
  if (!i && !m &&  d) return 1000;    // Solo derecha
  if ( i &&  m && !d) return -1500;   // Curva cerrada izquierda
  if (!i &&  m &&  d) return 1500;    // Curva cerrada derecha
  if ( i && !m &&  d) return 0;       // I + D → mantener centro
  if (!i && !m && !d) return 0;       // Ninguno → mantener
  if ( i &&  m &&  d) return 0;       // Todos → mantener
  return 0;
}

// === Controla motores (incluye reversa y PWM mínimo útil) ===
void cambiarVelocidad(int velI, int velD) {
  // Lógica motor izquierdo
  if (velI >= 0) {
    digitalWrite(mPin1, HIGH);
    digitalWrite(mPin2, LOW);
  } else {
    digitalWrite(mPin1, LOW);
    digitalWrite(mPin2, HIGH);
    velI = -velI;
  }

  // Lógica motor derecho
  if (velD >= 0) {
    digitalWrite(mPin3, HIGH);
    digitalWrite(mPin4, LOW);
  } else {
    digitalWrite(mPin3, LOW);
    digitalWrite(mPin4, HIGH);
    velD = -velD;
  }

  // PWM mínimo útil
  velI = constrain(velI, 0, VELOCIDAD_MAX);
  velD = constrain(velD, 0, VELOCIDAD_MAX);

  if (velI > 0 && velI < VELOCIDAD_MIN) velI = VELOCIDAD_MIN;
  if (velD > 0 && velD < VELOCIDAD_MIN) velD = VELOCIDAD_MIN;

  analogWrite(enablePin1y2, velI);
  analogWrite(enablePin3y4, velD);
}