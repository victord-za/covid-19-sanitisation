/*
========================================
PSA i-Sanitise
========================================

Implemented on an Arduino Uno board.

A script to operate the PSA i-Sanitise Tunnel. An PIR sensor is placed by the tunnel spray point.
When the PIR sensor has not been activated, a green LED light indicates clients may walk through 
the tunnel. Once the PIR sensor is activated a delay timer is activated, the green light is 
switched off and the red light switched on. The solenoid is also activated in a pulse sequence 
(on-off-on-off-on-off). Once the sequence is complete, the red light is switched off and the green
light switched on, indicating the next client may walk through.

There is also a low level sensor on the pressure vessel of the pump. Once the level drops below the low level switch,
the current running sequence is completed as normal. After this sequence, the red light starts to flash and the solenoid is 
deactivated until the level returns above the low level switch.

There is an analog level sensor on the tank. The tank level is displayed on an LED display screen.

Finally, there is a high level sensor in the drip tray. Once the level reaches this sensor, the drip tray pump is 
switched for a specified amount of time.

The solenoid, drip pump, pump, green light and red light are powered through a relay.
The drip level and pump level switches pass through a voltage divider.

*/

//========================================

// --------CONSTANTS---------------

// declare pins for digital IO

const int pirPin = 2;                   // choose the pin for PIR sensor
const int solenoidPin = 3;                // choose the pin for the solenoid
const int drippumpPin = 4;                // choose the pin for drip tray pump
const int pumpPin = 5;                    // choose the pin for pump
const int tanklevelPin = 6;               // choose the pin for pump level sensor
const int driplevelPin = 7;               // choose the pin for drip tray level sensor
const int greenPin = 9;                   // choose the pin for green light
const int redPin = 10;                    // choose the pin for red light

// constant values

const int pulseTime = 3000;               // number of milliseconds during pulse
const int drippumpTime = 5000;            // number of milliseconds for drip tray pump
const int waitTime = 1000;                // number of milliseconds to wait after PIR sensor is activated

//========================================

// --------VARIABLES---------------

int optval = 0;                           //  variale for reading PIR pin status
int pumpval = 0;                          //  variale for reading pump level sensor pin status
int dripval = 0;                          //  variale for reading drip tray level sensor pin status
int tankval = 0;                          //  variale for reading tank level sensor pin status

//========================================

void setup() {
  
  // pin assignment.
  
  pinMode(pirPin, INPUT);               //  declare PIR sensor as input
  pinMode(driplevelPin, INPUT);           //  declare drip tray level sensor as input
  pinMode(tanklevelPin, INPUT);           //  declare tank level sensor as input
  pinMode(greenPin, OUTPUT);              //  declare green light as output
  pinMode(redPin, OUTPUT);                //  declare red light as output
  pinMode(solenoidPin, OUTPUT);           // declare solenoid as output
  pinMode(pumpPin, OUTPUT);               //  declare pump as output
  pinMode(drippumpPin, OUTPUT);           //  declare drip tray pump as output
  
  Serial.begin(9600);                     // initializes serial communication to 9600 bits per second
}

//========================================

void loop() {
    
    // read input values.
    optval = digitalRead(pirPin);       // read PIR sensor input value
    tankval = digitalRead(tanklevelPin);  // read pump level sensor input value
    dripval = digitalRead(driplevelPin);  // read drip tray level sensor input value

    // run functions
    monitorpump();
    lightreset();
    solenoidpulse();
    monitordriptray();
}

//========================================

void monitorpump() {
  
  while(pumpval == 0){
    digitalWrite(greenPin, LOW);        // turn of green light
    digitalWrite(solenoidPin, LOW);     // disable solenoid
    // flash red light
    digitalWrite(redPin, HIGH);
    delay(waitTime);
    digitalWrite(redPin, LOW);
    delay(waitTime);
  }
}

//========================================

void lightreset() {
  digitalWrite(greenPin, HIGH);         // turn on green light
  digitalWrite(redPin, LOW);            // turn off red light
}

//========================================

void solenoidpulse() {
    if (optval == 1){
      digitalWrite(greenPin, LOW);      // turn off green light
      digitalWrite(redPin, HIGH);       // turn on red light
      digitalWrite(solenoidPin, HIGH);  // turn on the solenoid
      delay(pulseTime);                 // keep solenoid on for specified amount of miliseconds
      digitalWrite(solenoidPin, LOW);   // turn off solenoid
      delay(waitTime);                  // keep solenoid off for specified amount of miliseconds allowing current client to walk away      
      digitalWrite(greenPin, HIGH);     // turn on green light
      digitalWrite(redPin, LOW);        // turn off red light
      delay(waitTime);                  // keep solenoid off for specified amount of miliseconds allowing current client to walk away
    }
    else if (optval == 0) {
      digitalWrite(solenoid, LOW);      // turn off sonoid
      digitalWrite(greenPin, HIGH);     // turn on green light
      digitalWrite(redPin, LOW);        // turn off red light      
    }
}

//========================================

void monitordriptray() {
  if(dripval == 1){
    digitalWrite(drippumpPin, HIGH);    // turn on drip tray pump
    delay(drippumpTime);                // keep drip tray pump for specified amount of miliseconds
    digitalWrite(drippumpPin, LOW);     // turn off drip tray pump
  }
  else  {
    digitalWrite(drippumpPin, LOW);     // turn off drip tray pump
  }
}

//========================================
