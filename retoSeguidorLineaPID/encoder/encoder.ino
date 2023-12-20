// Define the pins for the encoder
const int encoder = 3;
// Variables for encoder and speed calculation
volatile long cont = 0;
unsigned long prevTime = 0;
const int encoderResolution = 40; // Number of pulses per revolution
const unsigned long sampleInterval = 75; // Time interval for speed calculation (in milliseconds)

//pines Motor right
int PinIN3 = 9;
int PinIN4 = 10;
int PinENB = 11;
//pines motor left
int PinENA = 6;
int PinIN1 = 7;
int PinIN2 = 8;

void setup() {
  Serial.begin(9600);
  // Enable interrupt on digital pin 2
  //pinMode(encoderLeft,INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(encoderLeft), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderRight), updateEncoder, CHANGE);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinENB,OUTPUT);
  pinMode(PinIN1, OUTPUT);
  pinMode(PinIN2, OUTPUT);
  pinMode(PinENA,OUTPUT);

}
void loop() {
  //motor left
  digitalWrite(PinIN1,HIGH);
  digitalWrite(PinIN2,LOW);
  digitalWrite(PinENA,HIGH);
  //motor right
  digitalWrite(PinIN3,HIGH);
  digitalWrite(PinIN4,LOW);
  digitalWrite(PinENB,HIGH);
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= sampleInterval) {
    // Calculate speed in RPM
    long pulseRight = contRight/2.0;
    contRight = 0.0;
    float elapsedTime = (float)(currentTime - prevTime) / 1000.0; // Convert to seconds
    float speedRight = (pulseRight / (float)encoderResolution) / elapsedTime; // revs per second
    //Serial.print("Speed left motor: ");
    //Serial.print("Speed right motor: ");
    Serial.println(speedRight);
    //Serial.println(" revs per second");
    //Serial.println();
    prevTime = currentTime;
    /*
    //borrar:
    if(currentTime>=t_end){
      digitalWrite(PinIN3,LOW);
      digitalWrite(PinIN4,LOW);
      digitalWrite(PinENB,LOW);
    }
    */
  }
}
void updateEncoder(){
  // Update the pulse count based on encoder transitions
  cont++;
}