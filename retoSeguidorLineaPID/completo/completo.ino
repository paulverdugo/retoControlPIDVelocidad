//variables para control PID
double dt, last_time;
double integral, previous, output = 0;
double kp, ki, kd;
double speed_ref=0;
int input=0;

// Define the pins for the encoder
const int encoderRight = 3;

// Variables for encoder and speed calculation
volatile long contRight = 0;
unsigned long prevTime = 0;
const int encoderResolution = 40; // Number of pulses per revolution
const unsigned long sampleInterval = 40; // Time interval for speed calculation (in milliseconds)
float speedRight=0;

//pines Motor right
int PinIN3 = 9;
int PinIN4 = 10;
int PinENB = 11;
//pines motor left
int PinENA = 6;
int PinIN1 = 7;
int PinIN2 = 8;

void setup() {
  kp = 12.645;
  ki = 78.713;
  kd = 0.16842;
  last_time = 0;
  Serial.begin(9600);
  delay(100);
  attachInterrupt(digitalPinToInterrupt(encoderRight), updateEncoder, CHANGE);
  pinMode(PinIN3, OUTPUT);
  pinMode(PinIN4, OUTPUT);
  pinMode(PinENB,OUTPUT);
  //pinMode(PinIN1, OUTPUT);
  //pinMode(PinIN2, OUTPUT);
  //pinMode(PinENA,OUTPUT);

}
void loop() {
  if (Serial.available()>0){
    //input de velocidad para referencia PID 0-1.5 revs por min
    float newInput=Serial.parseFloat();
    if(newInput>0&&newInput<=1.5){
      speed_ref=newInput;
    }
    if(newInput==0.1){
      speed_ref=0;
    }
  }
  
  double now = millis();
  dt = (now - last_time)/1000.00;
  last_time = now;
  float error = speed_ref - speedRight;
  output = PID(error);
  
  if(output<0){
    output=0;
  }
  Serial.print(speed_ref);
  Serial.print(",");
  Serial.print(speedRight);
  Serial.print(",");
  Serial.println(output);

  //pwm
  analogWrite(PinENB, output);
  //analogWrite(PinENA, output);

  //motor left fwd
  digitalWrite(PinIN1,HIGH);
  digitalWrite(PinIN2,LOW);
  //motor right fwd
  digitalWrite(PinIN3,HIGH);
  digitalWrite(PinIN4,LOW);
  
  unsigned long currentTime = millis();
  if (currentTime - prevTime >= sampleInterval) {
    // Calculate speed in RPM
    long pulseRight = contRight/2.0;
    contRight = 0.0;
    float elapsedTime = (float)(currentTime - prevTime) / 1000.0; // Convert to seconds
    speedRight = (pulseRight / (float)encoderResolution) / elapsedTime; // revs per second
    prevTime = currentTime;
  }
  delay(100);
}
void updateEncoder() {
  // Update the pulse count based on encoder transitions
  contRight++;
}
double PID(double error){
  double proportional = error;
  integral += error * dt;
  double derivative = (error - previous) / dt;
  previous = error;
  double output = (kp * proportional) + (ki * integral) + (kd * derivative);
  return output;
}