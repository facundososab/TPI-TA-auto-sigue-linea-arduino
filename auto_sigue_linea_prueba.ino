// Sensores
const int sensorI = 13;
const int sensorM = 12;
const int sensorD = 11;

// Motor izquierdo
const int enablePin1y2 = 7;
const int mPin1 = 6;
const int mPin2 = 5;

// Motor derecho
const int mPin3 = 4;
const int mPin4 = 3;
const int enablePin3y4 = 2;

// PID
const float Kp = 5.0;
const float Ki = 0.5;
const float Kd = 20.0;

int P, I, D;
int ultimoError = 0;
int integral = 0;
int velocidadBase = 150;  // velocidad base
int velocidadMax = 255;

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

  Serial.begin(9600);  // Para debugging
}

void loop() {
  bool S_I = digitalRead(sensorI);
  bool S_M = digitalRead(sensorM);
  bool S_D = digitalRead(sensorD);

  int error;

  // Calcular el error según sensores
  if (S_I && !S_M && !S_D)        error = 2;
  else if (S_I && S_M && !S_D)    error = 1;
  else if (!S_I && S_M && !S_D)   error = 0;
  else if (!S_I && S_M && S_D)    error = -1;
  else if (!S_I && !S_M && S_D)   error = -2;
  else if (!S_I && !S_M && !S_D)  error = ultimoError;  // línea perdida
  else                            error = 0;  // caso I+D+M

  // PID
  P = error;
  integral += error;
  D = error - ultimoError;

  float ajuste = Kp * P + Ki * integral + Kd * D;

  int velIzq = velocidadBase + ajuste;
  int velDer = velocidadBase - ajuste;

  // Limitar velocidades
  velIzq = constrain(velIzq, 0, velocidadMax);
  velDer = constrain(velDer, 0, velocidadMax);

  moverMotores(velIzq, velDer);

  ultimoError = error;

  // Debug
  Serial.print("Error: "); Serial.print(error);
  Serial.print(" | Ajuste: "); Serial.print(ajuste);
  Serial.print(" | Vel I: "); Serial.print(velIzq);
  Serial.print(" | Vel D: "); Serial.println(velDer);

  delay(10);
}

void moverMotores(int velIzq, int velDer) {
  // Motor izquierdo (avanza)
  digitalWrite(mPin1, HIGH);
  digitalWrite(mPin2, LOW);
  analogWrite(enablePin1y2, velIzq);

  // Motor derecho (avanza)
  digitalWrite(mPin3, HIGH);
  digitalWrite(mPin4, LOW);
  analogWrite(enablePin3y4, velDer);
}
