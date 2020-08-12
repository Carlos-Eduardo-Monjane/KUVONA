#include <SoftwareSerial.h>
#include <NewPing.h>
#include <NewTone.h>

// Pins
#define TONE_PIN 6
#define BT_TX_PIN 13
#define BT_RX_PIN 9
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 160 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


// Bluetooth
byte  input = 0; // Value passed in by the bluetooth
int output = 0;

// Melody (liberated from the toneMelody Arduino example sketch by Tom Igoe).
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

SoftwareSerial bluetoothModule =  SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
  Serial.begin(9600);
  bluetoothModule.begin(9600);
}

void loop() {

  delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  
  //Manter a precisão, aciona o TRIGGER 70 vezes e confirmando o obstaculo, envia o echo.
  int pulsos = sonar.ping_median(5);
  int distancia = sonar.convert_cm(pulsos); 

  //Verificar obstaculo
  if(distancia != 0){

    Serial.print("Ping: ");
    Serial.print(sonar.convert_cm(pulsos)); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");
    //Senddd
    output = sonar.convert_cm(pulsos);
    enviarResult(output);

  for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    int noteDuration = 1000/noteDurations[thisNote];
    NewTone(TONE_PIN, melody[thisNote], noteDuration); // Play thisNote for noteDuration.
    delay(noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.
  }

  while(0); // Stop (1) (so it doesn't repeat forever driving you crazy--you're welcome).
    } 
}

void enviarResult(float valor) {
  String string = String(valor); // Convert to string and send
  bluetoothModule.print(' ' + string + '#'); // With the character "#" the app understands the sending is finished
}

    

