#include <Servo.h>  // Librería para controlar el servo

// Pines de fotoresistencias, LEDs, buzzer, sensor ultrasónico y servo
const int fotoPin[] = {A0, A1, A2, A3};  // Pines de las fotoresistencias
const int ledPin[] = {8, 9, 10, 11};     // Pines de los LEDs
const int displayPin[] = {1, 2, 3, 4};   // Pines de las entradas A, B, C, D del CD4511BE
const int buzzerPin = 12;                 // Pin del buzzer
const int trigPin = 6;                   // Pin TRIG del sensor ultrasónico
const int echoPin = 7;                   // Pin ECHO del sensor ultrasónico
const int servoPin = 5;                  // Nuevo pin del servo
int umbralLuz = 100;                     // Umbral de luz (ajustado a 100)
int umbralDistancia = 10;                // Umbral de distancia (en cm) para activar el servo y buzzer

Servo servoMotor;  // Crear objeto servo

void setup() {
  // Configurar pines de LEDs, display, buzzer, sensor ultrasónico y servo
  for (int i = 0; i < 4; i++) {
    pinMode(ledPin[i], OUTPUT);
    pinMode(displayPin[i], OUTPUT);
  }
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializar el servo
  servoMotor.attach(servoPin);
  servoMotor.write(0);  // Posición inicial del servo en 0 grados

  Serial.begin(9600);  // Iniciar comunicación serial para monitoreo
}

void loop() {
  int espaciosDisponibles = 0;

  // Revisar cada fotoresistencia y controlar LEDs
  for (int i = 0; i < 4; i++) {
    int valorLuz = analogRead(fotoPin[i]);  // Leer cada fotoresistencia
    Serial.print("Fotoresistencia ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(valorLuz);               // Mostrar valor en el monitor serial

    if (valorLuz > umbralLuz) {             // Si hay suficiente luz (espacio libre)
      digitalWrite(ledPin[i], HIGH);        // Enciende el LED correspondiente
      espaciosDisponibles++;                // Aumenta el conteo de espacios disponibles
    } else {
      digitalWrite(ledPin[i], LOW);         // Apaga el LED correspondiente (espacio ocupado)
    }
  }

  // Enviar espacios disponibles directamente al display de 7 segmentos
  for (int i = 0; i < 4; i++) {
    int bit = bitRead(espaciosDisponibles, i);  // Leer cada bit del número de espacios disponibles
    digitalWrite(displayPin[i], bit);           // Enviar el bit a la entrada correspondiente del CD4511BE
  }

  // Medir distancia con el sensor ultrasónico
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convertir tiempo en distancia (en cm)
  
  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Activar el buzzer como sirena si no hay espacios disponibles y el vehículo está cerca
  if (espaciosDisponibles == 0 && distance <= umbralDistancia) {
    for (int i = 0; i < 5; i++) {      // Ajusta el número de repeticiones según prefieras
      tone(buzzerPin, 1000);           // Tono a 1000 Hz
      delay(200);                      // Sonido encendido por 200 ms
      noTone(buzzerPin);               // Apagar el tono
      delay(200);                      // Silencio por 200 ms
    }
  } else {
    noTone(buzzerPin);                 // Asegúrate de que el buzzer esté apagado cuando haya espacios o el vehículo esté lejos
  }

  // Mover el servo a 90 grados si un vehículo se acerca (distancia < 10 cm)
  if (distance <= umbralDistancia) {
    servoMotor.write(90);  // Mueve el servo a 90 grados
    delay(8000);
    servoMotor.write(0);  // Mueve el servo a 90 grados
  } else {
    servoMotor.write(0);   // Mueve el servo de regreso a 0 grados
  }

  delay(500);  // Pausa breve para lecturas estables
}
