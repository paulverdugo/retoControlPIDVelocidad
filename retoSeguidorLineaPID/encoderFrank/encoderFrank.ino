///-------------------------------ENCODDER---------------------------------------------
int N = 20;                                              // nùmero de ranuras del encoder
float diametro = 6.5;                                    // diametro de la llanta cm
int contadorTicks = 6;                                  // nùmero de ticks para calculo de velocidad
int tam = 10;                                       // tamaño del vector del calculo de promedio, se debe descomentar la linea que se vaya a usar
///------------------------------- PARA CONTROLAR MOTOR---------------------------------------------

const int motorPin1 = 10; // Conectar al pin IN1 del puente H
const int motorPin2 = 9; // Conectar al pin IN2 del puente H
const int ENB = 11; // Conectar al pin Enable del puente H

const int motorPin3 = 8; // Conectar al pin IN1 del puente H
const int motorPin4 = 7; // Conectar al pin IN2 del puente H
const int ENA = 6; // Conectar al pin Enable del puente H

int speed = 255; // Velocidad del motor (valor de PWM)

///------------------------------- Variables de motor derecho---------------------------------------------

volatile unsigned muestreoActualInterrupcionR = 0;        // variables para definiciòn del tiempo de interrupciòn y calculo de la velocidad motor derecho
volatile unsigned muestreoAnteriorInterrupcionR = 0;
double deltaMuestreoInterrupcionR = 0;

int encoderR = 2;   // pin de conexiòn del encoder derecho

double frecuenciaR = 0;                                  // frecuencia de interrupciòn llanta R
double Wr = 0;                                           // Velocidad angular R
double Vr = 0;                                           // velocidad Lineal
int CR = 0;                                             // contador ticks
float vectorR[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    // vector de almacenamiento de datos para promedio del tiempo de interrupciones


void setup() {
  attachInterrupt(digitalPinToInterrupt(encoderR),REncoder,FALLING);                // linea para añadir una interrupciòn a un PIN
    // Define los pines como salidas
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Serial.begin(2000000);   
  Serial.println("LABEL,hora");                                                            // inicio de la comunicaciòn serial
  delay(1000);
}

void REncoder() {                                                                                    // funciòn de interrupciòn del enconder llanta derecha
      
      CR++;

      if (CR == contadorTicks){
          float media = 0;
          deltaMuestreoInterrupcionR = muestreoActualInterrupcionR -  muestreoAnteriorInterrupcionR;     // diferencia tiempos de interruciones de ticks del motor     
          
           for(int i=0;i < tam-1;i++){                                                                    // relleno del vector para calculo posterior del promedio
              vectorR[i]=vectorR[i+1];
            }
           vectorR[tam-1]=deltaMuestreoInterrupcionR ;                                                     // ùltimo dato del vector 

           for(int i=0;i<tam;i++){                                                                        // Càlculo de la media del vector
              media = vectorR[i]+ media;
            }
            media = media/tam;
            deltaMuestreoInterrupcionR = media;                                                            // se reemplaza por el valor de su medìa. 
           
            frecuenciaR = (1000)/ deltaMuestreoInterrupcionR;                                       // frecuencia de interrupciòn 
          
          muestreoAnteriorInterrupcionR = muestreoActualInterrupcionR;                                   // se actualiza el tiempo de interrupciòn anterior
          CR = 0;                       //Reinicio de contador
      } 
 } 

void loop() { 
    
    //MOTOR HACIA DELANTE//
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    digitalWrite(motorPin3, LOW);
    digitalWrite(motorPin4, HIGH);
    analogWrite(ENA, speed);      
    analogWrite(ENB, speed); 

    muestreoActualInterrupcionR = millis();                     // se asigna el tiempo de ejecuciòn a el muestreo actual

    Wr = contadorTicks*((2*3.141516)/N)*frecuenciaR;                        // frecuencia angular Rad/s
    Vr= Wr*(diametro/2);                                      // velocidad lineal cm/s
                                     // velocidad lineal cm/s     
    
    Serial.print("DATA,TIME,");
    Serial.print(",");
    Serial.println(Wr);
}
