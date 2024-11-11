#include <Servo.h>  // Librería para controlar el servo

// Pines de fotoresistencias, LEDs, buzzer, sensor ultrasónico y servo
const int fotoPin[] = {A0, A1, A2, A3};  // Pines de las fotoresistencias
const int ledPin[] = {13, 12, 11, 10};   // Pines de los LEDs
const int segmentPins[] = {4,3, 0, 1, 2, 5, 6}; // Pines conectados a los segmentos A-G del display de 7 segmentos
const int buzzerPin = A5;                // Pin del buzzer
const int trigPin = 8;                   // Pin TRIG del sensor ultrasónico
const int echoPin = 7;                   // Pin ECHO del sensor ultrasónico
const int servoPin = 9;                  // Nuevo pin del servo
int umbralLuz = 100;                     // Umbral de luz (ajustado a 100)
int umbralDistancia = 10;                // Umbral de distancia (en cm) para activar el servo y buzzer

Servo servoMotor;  // Crear objeto servo

// Definición de segmentos para cada dígito
const bool numeros[10][7] = {
  {0, 0, 0, 0, 0, 0, 1}, // 0
  {1, 0, 0, 1, 1, 1, 1}, // 1
  {0, 0, 1, 0, 0, 1, 0}, // 2
  {0, 0, 0, 0, 1, 1, 0}, // 3
  {1, 0, 0, 1, 1, 0, 0}, // 4
  {0, 1, 0, 0, 1, 0, 0}, // 5
  {0, 1, 0, 0, 0, 0, 0}, // 6
  {0, 0, 0, 1, 1, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 1, 0, 0}  // 9
};

void setup() {
  // Configurar pines de LEDs, display, buzzer, sensor ultrasónico y servo
  for (int i = 0; i < 4; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializar el servo
  servoMotor.attach(servoPin);
  servoMotor.write(0);  // Posición inicial del servo en 0 grados
}

void loop() {
  int espaciosDisponibles = 0;

  // Revisar cada fotoresistencia y controlar LEDs
  for (int i = 0; i < 4; i++) {
    int valorLuz = analogRead(fotoPin[i]);  // Leer cada fotoresistencia

    if (valorLuz > umbralLuz) {             // Si hay suficiente luz (espacio libre)
      digitalWrite(ledPin[i], HIGH);        // Enciende el LED correspondiente
      espaciosDisponibles++;                // Aumenta el conteo de espacios disponibles
    } else {
      digitalWrite(ledPin[i], LOW);         // Apaga el LED correspondiente (espacio ocupado)
    }
  }

  // Mostrar el número de espacios disponibles en el display de 7 segmentos
  mostrarNumero(espaciosDisponibles);

  // Medir distancia con el sensor ultrasónico
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convertir tiempo en distancia (en cm)

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
  if (espaciosDisponibles != 0 && distance <= umbralDistancia) {
    servoMotor.write(90);  // Mueve el servo a 90 grados
    delay(8000);
    servoMotor.write(0);   // Regresar el servo a 0 grados
  } else {
    servoMotor.write(0);   // Asegurar que el servo esté en 0 grados
  }

  delay(500);  // Pausa breve para lecturas estables
}

void mostrarNumero(int numero) {
  // Activa o desactiva los segmentos según el número especificado
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], numeros[numero][i]);
  }
}

void apagarDisplay() {
  // Apaga todos los segmentos del display de 7 segmentos
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], LOW); // Apaga todos los segmentos
  }
}
