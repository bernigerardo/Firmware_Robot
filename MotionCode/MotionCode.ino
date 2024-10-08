#include <Servo.h>

// Pines de entrada

// Movimiento Robot
const int VerticalJoystickPin = A7; // Entradas analógica para la señal proporcional (0-5V)
const int HorizontalJoystickPin = A6;
const int VDirForwardPin = 3; // Dirección hacia delante 
const int VDirBackwardPin = 4; // Dirección hacia atrás
const int HDirRightPin = 5; // Dirección hacia la derecha  
const int HDirLeftPin = 6; // Dirección hacia la izquierda

// Rodillo
int rodillo = 7;
int angulo;
int step = 1;
int anguloR = 113;
int anguloNuevo;

Servo rightServo;
Servo leftServo;
Servo Roller;

void setup() {
  pinMode(VerticalJoystickPin, INPUT);
  pinMode(HorizontalJoystickPin, INPUT);
  pinMode(VDirForwardPin, INPUT);
  pinMode(VDirBackwardPin, INPUT);
  pinMode(HDirRightPin, INPUT);
  pinMode(HDirLeftPin, INPUT);
  pinMode(rodillo, INPUT);

  // Inicializar motores
  rightServo.attach(9);
  leftServo.attach(10);
  Roller.attach(11);

  // Colocar motores en la posición media al iniciar
  rightServo.write(90);
  leftServo.write(90);
  Roller.write(90);
}

void loop() {
  // Leer la señal del joystick (0-1023)
  int verticalJoystickValue = analogRead(VerticalJoystickPin);
  int horizontalJoystickValue = analogRead(HorizontalJoystickPin);
  
  // Leer los pines de dirección
  int verticalForwardSignal = digitalRead(VDirForwardPin);
  int verticalBackwardSignal = digitalRead(VDirBackwardPin);
  int horizontalRightSignal = digitalRead(HDirRightPin);
  int horizontalLeftSignal = digitalRead(HDirLeftPin);

  // Mapear la señal del joystick a un rango de ángulos del servo (0-90 para atrás, 90-180 para adelante)
  int VerticalServoAngle = map(verticalJoystickValue, 0, 1023, 0, 90);
  int HorizontalServoAngle = map(horizontalJoystickValue, 0, 1023, 0, 90);

  //Control principal por el joystick Vertical
  if ((verticalForwardSignal == HIGH && verticalBackwardSignal == LOW) || (verticalForwardSignal == LOW && verticalBackwardSignal == HIGH)){
    if (verticalForwardSignal == HIGH){
      //Mover ambos motores hacia delante
      rightServo.write(90 + VerticalServoAngle);
      leftServo.write(90 + VerticalServoAngle);
    } else if (verticalBackwardSignal == HIGH) {
      //Mover ambs motores hacia atrás
      rightServo.write(90 - VerticalServoAngle);
      leftServo.write(90 - VerticalServoAngle);
    }
  } else {
    //Si el Joystick vertical no está en uso
    if ((horizontalRightSignal == HIGH && horizontalLeftSignal == LOW) || (horizontalRightSignal == LOW && horizontalLeftSignal == HIGH)) {
      if (horizontalRightSignal == HIGH){
        //Giro a la derecha: Motor derecho hacia atras y el izquierdo hacia delante
        rightServo.write(90 - HorizontalServoAngle);
        leftServo.write(90 + HorizontalServoAngle);
      } else if (horizontalLeftSignal == HIGH){
        //Giro a la izquierda: Motor derecho hacia delante e izquierdo hacia atrás
        rightServo.write(90 + HorizontalServoAngle);
        leftServo.write(90 - HorizontalServoAngle);
      }
    } else {
        rightServo.write(90);
        leftServo.write(90);
    }
  }

  //Roller
  angulo = Roller.read();
  //Si se acciona el rodillo
  if (digitalRead(rodillo) == HIGH){
    //Inicio suave en rampa
    while (angulo<anguloR){
      angulo = Roller.read();
      anguloNuevo = angulo + step;
      Roller.write(anguloNuevo);
      delay(200);
    }
  }
  //Si se desactiva el rodillo
  if(digitalRead(rodillo) == LOW){
    //Apagado ruave en rampa
    while (angulo>90){
      angulo = Roller.read();
      anguloNuevo = angulo - step;
      Roller.write (anguloNuevo);
      delay(200);
    }
    //Posición estática en 90°
    if(angulo<=90){
      Roller.write(90);
      delay(200);
    }
  }

}