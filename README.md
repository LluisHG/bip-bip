# bip-bip

Bip-bip is an Arduino Nano based car parking sensor project. In this repository is included the arduino code and the hardware used as well as the 3D printed parts to assemble the sensor.


## Hardware
The selected micro controller is an Arduino Nano board, concretely the [Arduino Nano 3,0 Atmega328](https://es.aliexpress.com/item/1005004305055818.html?spm=a2g0o.order_list.order_list_main.11.3af3194dyjIC2R&gatewayAdapt=glo2esp).

The ultrasonic sensors used are the waterprof ultrasonic sensor [SR04M-2](doc:JSN-SR04T-2.0.pdf).

The device to produce the signal to tell the user when the car is getting closer to an obstacle will be an active buzzer.

Below, you can find the schematics of the whole system. 

[Schematics]

## Software

Run the Arduino GUI to open the [main file](bip-bip-sensor/bip-bip-sensor.ino). 
At the beginning of the program, you can see the Arduino Nano PIN initialisations, the number of sensor used and the saefty distance defined:

```c++
int sensor_01_ECHO_PIN = 2;
int sensor_01_TRIG_PIN = 3;
int sensor_02_ECHO_PIN = 4;
int sensor_02_TRIG_PIN = 5;
int sensor_03_ECHO_PIN = 6;
int sensor_03_TRIG_PIN = 7;
int sensor_04_ECHO_PIN = 8;
int sensor_04_TRIG_PIN = 9;
int BUZZER_PIN = 10;
int LED_PIN = 12;
int NUMBER_OF_SENSORS = 2;
int SAFETY_DISTANCE = 50; // Centimeters
int BAUD_RATE = 9600;
```

Note that the `SAEFTY_DISTANCE` is in centimetres and is the farest distance the system starts bip-biping.

Pay attention at the method [measureDistance](https://vscode.dev/github/LluisHG/bip-bip/blob/feature-code-refactor/bip-bip-sensor/bip-bip-sensor.ino#L55). Note that there are some commented code lines. Those lines should be necessary if the sensor worked as expected. In my case and in my configuration, it was only necessary to read the signal at the `ECHO` pin and calculate the distance.

```c++
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
```

Read the sensor documentation to understand its working modes.

## Usage

