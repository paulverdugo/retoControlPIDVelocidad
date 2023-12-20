// Definición de los pines para el puente H
const int motorInput1 = 9;
const int motorInput2 = 10;

// Polinomio generador (x^7 + x^6 + 1)
const int feedbackBit = 7;

// Define the pins for the encoder
const int encoderRight = 3;
//const int encoderLeft = 2;
// Variables for encoder and speed calculation
volatile long contRight = 0;
volatile long contLeft = 0;
unsigned long prevTime = 0;
const int encoderResolution = 40; // Number of pulses per revolution
const unsigned long sampleInterval = 75; // Time interval for speed calculation (in milliseconds)
volatile float speedRight;

void setup() {
  Serial.begin(9600);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoder, CHANGE);

  // Inicialización del registro de desplazamiento con un valor inicial no nulo
  uint8_t shiftRegister = 0b0101010; // Puedes elegir un valor inicial diferente
  
  // Bucle para generar la secuencia PRBS
  while (true) {
    int outputBit = shiftRegister & 1;
    
    if (outputBit == 0) {
      digitalWrite(motorInput1, HIGH);
      digitalWrite(motorInput2, LOW);
    } else {
      digitalWrite(motorInput1, LOW);
      digitalWrite(motorInput2, LOW);
    }
    
    // Realizar la retroalimentación del registro de desplazamiento
    int feedback = (shiftRegister >> feedbackBit) & 1;
    
    // Aplicar la retroalimentación al registro de desplazamiento
    shiftRegister = (shiftRegister << 1) | feedback;
    
    //encoder
    unsigned long currentTime = millis();
    // Calculate speed in RPM
    long pulseRight = contRight/2.0;
    contRight = 0.0;
    float elapsedTime = (float)(currentTime - prevTime) / 1000.0; // Convert to seconds
    float speedRight = (pulseRight / (float)encoderResolution) / elapsedTime; // revs per second
    Serial.println(speedRight);

    prevTime = currentTime;
  
    delay(500); // Ajusta la velocidad de cambio de la secuencia PRBS
  }
}

void loop() {
  // El bucle loop no se ejecutará en este ejemplo
}
void updateEncoder() {
  // Update the pulse count based on encoder transitions
  contRight++;
}