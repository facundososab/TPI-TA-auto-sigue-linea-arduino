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
const float Kp = 3.0, Ki = 0.2, Kd = 8.0;
int P = 0, I = 0, D = 0;
int ultimoError = 0;

// === Velocidades ===
const int VELOCIDAD_BASE   = 150;
const int VELOCIDAD_MAX    = 255;
const int VELOCIDAD_ATRAS  = -100;

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
  const int i = digitalRead(sensorI);
  const int m = digitalRead(sensorM);
  const int d = digitalRead(sensorD);

  int velI, velD;

  // === Casos especiales ===
  if (i && m && d) {                    // Todos detectan
    velI = VELOCIDAD_BASE;
    velD = VELOCIDAD_BASE;
  } else if (!i && !m && !d) {         // Ninguno detecta
    velI = VELOCIDAD_BASE;
    velD = VELOCIDAD_BASE;
  } else if (i && !m && d) {           // Solo laterales
    velI = VELOCIDAD_BASE;
    velD = VELOCIDAD_BASE;
  } else if (i && m && !d) {           // Curva cerrada izquierda
    velI = VELOCIDAD_ATRAS;
    velD = VELOCIDAD_MAX;
  } else if (!i && m && d) {           // Curva cerrada derecha
    velI = VELOCIDAD_MAX;
    velD = VELOCIDAD_ATRAS;
  } else if (i && !m && !d) {          // Solo izquierda
    velI = VELOCIDAD_ATRAS;
    velD = VELOCIDAD_MAX;
  } else if (!i && !m && d) {          // Solo derecha
    velI = VELOCIDAD_MAX;
    velD = VELOCIDAD_ATRAS;
  } else {
    // === Caso "normal": aplicar PID ===
    int error = calcularError(i, m, d);
    P = error;
    I += error;
    D = error - ultimoError;
    ultimoError = error;

    if (abs(error) < 50) I = 0; // Anti-windup

    int PID = -(P * Kp + I * Ki + D * Kd); // Corrección invertida

    velI = VELOCIDAD_BASE - PID;
    velD = VELOCIDAD_BASE + PID;
  }

  cambiarVelocidad(velI, velD);
}

// === Calcula el error lógico para el PID ===
int calcularError(int i, int m, int d) {
  if (!i &&  m && !d) return 0;       // Solo medio
  if ( i && !m && !d) return -1000;   // Solo izquierda
  if (!i && !m &&  d) return 1000;    // Solo derecha
  if ( i &&  m && !d) return -500;    // Izquierda + medio
  if (!i &&  m &&  d) return 500;     // Derecha + medio
  return 0;
}

// === Controla motores según la velocidad (incluye reversa) ===
void cambiarVelocidad(int velI, int velD) {
  // Umbral mínimo para que los motores realmente giren (evita que se queden clavados)
  const int PWM_MIN = 100;

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

  // Aplicar PWM con umbral mínimo
  velI = constrain(velI, 0, 255);
  velD = constrain(velD, 0, 255);

  if (velI > 0 && velI < PWM_MIN) velI = PWM_MIN;
  if (velD > 0 && velD < PWM_MIN) velD = PWM_MIN;

  analogWrite(enablePin1y2, velI);
  analogWrite(enablePin3y4, velD);
}

