// #define sensor_01_ECHO_PIN 2
// #define sensor_01_TRIG_PIN 3
// // #define sensor_02_ECHO_PIN 4
// // #define sensor_02_TRIG_PIN 5
// // #define sensor_03_ECHO_PIN 6
// // #define sensor_03_TRIG_PIN 7
// // #define sensor_04_ECHO_PIN 8
// // #define sensor_04_TRIG_PIN 9
// #define BUZZER_PIN 10
// #define LED_PIN 12

// #define NUMBER_OF_SENSORS 1
    
// struct sensor_data {
//   int echo_pin;
//   int trig_pin;
//   int measured_distance_cm;
// };

// struct sensor_data sensor[NUMBER_OF_SENSORS];

// const unsigned int BAUD_RATE = 9600;
// const unsigned int SAEFTY_DISTANCE = 100;

// void setup() {

//   sensor[0].echo_pin = sensor_01_ECHO_PIN;
//   sensor[0].trig_pin = sensor_01_TRIG_PIN;
//   // sensor[1].echo_pin = sensor_02_ECHO_PIN;
//   // sensor[1].trig_pin = sensor_02_TRIG_PIN;
//   // sensor[2].echo_pin = sensor_03_ECHO_PIN;
//   // sensor[2].trig_pin = sensor_03_TRIG_PIN;
//   // sensor[3].echo_pin = sensor_04_ECHO_PIN;
//   // sensor[3].trig_pin = sensor_04_TRIG_PIN;

//   pinMode(sensor[0].trig_pin, OUTPUT);
//   pinMode(sensor[0].echo_pin, INPUT);
//   // pinMode(sensor[1].trig_pin, OUTPUT);
//   // pinMode(sensor[1].echo_pin, INPUT);
//   // pinMode(sensor[2].trig_pin, OUTPUT);
//   // pinMode(sensor[2].echo_pin, INPUT);
//   // pinMode(sensor[3].trig_pin, OUTPUT);
//   // pinMode(sensor[3].echo_pin, INPUT);

//   // pinMode(TRIG_PIN, OUTPUT);
//   // pinMode(ECHO_PIN, INPUT);
//   pinMode(BUZZER_PIN, OUTPUT);
//   pinMode(LED_PIN, OUTPUT);
//   Serial.begin(BAUD_RATE);
// }

// void loop() {

//   for (int i = 0; i < NUMBER_OF_SENSORS; i++)
//   {
//     digitalWrite(sensor[i].trig_pin, LOW);
//     delayMicroseconds(2);
//     digitalWrite(sensor[i].trig_pin, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(sensor[i].trig_pin, LOW);  

//     const unsigned long duration = pulseIn(sensor[i].echo_pin, HIGH);
//     sensor[i].measured_distance_cm = duration/29/2;
//     int distance = sensor[i].measured_distance_cm;

//     if(duration == 0) {
//       Serial.println("Warning: no pulse from sensor");
//     } else {
//       if (distance <= SAEFTY_DISTANCE) {
        
//         long tone_duration = round(500 * (1 - distance /SAEFTY_DISTANCE));
//         tone(BUZZER_PIN, 2500, tone_duration);
//         // delay(tone_duration);
//         // tone(BUZZER_PIN, 2500, tone_duration);


//         digitalWrite(LED_PIN, HIGH);
//       } else {
//         noTone(BUZZER_PIN);
//         digitalWrite(LED_PIN, LOW);
//       }
      
//       Serial.print("Sensor ");
//       Serial.print(i);
//       Serial.print(" distance to nearest object: ");
//       Serial.print(distance);
//       Serial.println(" cm");
//     }
//   }

  
//   delay(500);
// }

#define sensor_01_ECHO_PIN 2
#define sensor_01_TRIG_PIN 3
#define sensor_02_ECHO_PIN 4
#define sensor_02_TRIG_PIN 5
#define sensor_03_ECHO_PIN 6
#define sensor_03_TRIG_PIN 7
#define sensor_04_ECHO_PIN 8
#define sensor_04_TRIG_PIN 9
#define BUZZER_PIN 10
#define LED_PIN 12

#define NUMBER_OF_SENSORS 2
#define SAFETY_DISTANCE 50 // Centimeters
// #define MAX_TONE_DURATION 500 // Milliseconds
// #define BUZZER_FREQUENCY 2500 // Hertz
#define BAUD_RATE = 9600;

#define arrSize(x) sizeof(x) / sizeof(x[0])

struct Sensor {
  int echo_pin;
  // int trig_pin;
  long measured_distance_cm;
};

Sensor sensors[NUMBER_OF_SENSORS];
long distances_read[NUMBER_OF_SENSORS];

void setup() {
  initializeSensors();
  Serial.begin(9600);
}

void loop() {

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    measureDistance(sensors[i]);
    distances_read[i] = sensors[i].measured_distance_cm;
    printDistance(i, sensors[i].measured_distance_cm);
  }
  
  long min_distance = getMin(distances_read, arrSize(distances_read));
  handleAlarm(min_distance);
  delay(20); 
}

void initializeSensors() {
  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    sensors[i].echo_pin = sensor_01_ECHO_PIN + i * 2;
    // sensors[i].trig_pin = sensor_01_TRIG_PIN + i * 2;
    // pinMode(sensors[i].trig_pin, OUTPUT);
    pinMode(sensors[i].echo_pin, INPUT);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void measureDistance(Sensor &sensor) {
  // digitalWrite(sensor.trig_pin, LOW);
  // delayMicroseconds(2);
  // digitalWrite(sensor.trig_pin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(sensor.trig_pin, LOW);

  unsigned long duration = pulseIn(sensor.echo_pin, HIGH);
  if (duration == 0) {
    Serial.println("Warning: no pulse from sensor");
    sensor.measured_distance_cm = -1; // Indica distancia inválida
  } else {
    sensor.measured_distance_cm = duration / 29 / 2;
  }
}

void handleAlarm(long measured_distance_cm) {
  
  int tone_duration;
  if (measured_distance_cm >= 10 && measured_distance_cm <= SAFETY_DISTANCE) {
    tone_duration = map(measured_distance_cm, 10, SAFETY_DISTANCE, 20, 500); // Ajusta los valores según sea necesario
    // Hace sonar el buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50); // Duración del pitido
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
	  delay(tone_duration);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void printDistance(int sensorIndex, long distance) {
  if (distance >= 0) {
    Serial.print("Sensor ");
    Serial.print(sensorIndex);
    Serial.print(" distance to nearest object: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}

long getMin(long a[], int size) {
  long min = a[0];
  for (int i = 0; i < size; i++) {
    if (a[i] < min)
      min = a[i];
  }
  return min;
}