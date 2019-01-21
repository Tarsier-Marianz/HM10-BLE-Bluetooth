#include <SoftwareSerial.h>
#define LED_PIN 13      // The output pin for built in LED
#define PWM_PIN 3      // The output pin for RGB LED

SoftwareSerial mySerial(7, 8); // RX, TX
// Connect HM10      Arduino Uno
//     Pin 1/TXD          Pin 7
//     Pin 2/RXD          Pin 8

boolean stringComplete  = false;  // whether the string is complete
String cmd              = "";     // a string to hold incoming command
int rgbValue            = 0;
void setup() {
  Serial.begin(9600);
  // If the baudrate of the HM-10 module has been updated,
  // you may need to change 9600 by another value
  // Once you have found the correct baudrate,
  // you can update it using AT+BAUDx command
  // e.g. AT+BAUD0 for 9600 bauds
  mySerial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
}

void loop() {
  /*
    if (mySerial.available()) {
    char c = mySerial.read();
    //Serial.println("Got input:");
    if (c != '0') {
      // Non-zero input means "turn on LED".
      Serial.println("  on");
      digitalWrite(LED_PIN, HIGH);
    } else {
      // Input value zero means "turn off LED".
      Serial.println("  off");
      digitalWrite(LED_PIN, LOW);
    }
    }
  */
  listenBluetoothEvent();
  getRGBValue();
  analogWrite(PWM_PIN, rgbValue );
  Serial.print("Value: ");
  Serial.print(rgbValue);
  Serial.print("  CMD: ");
  Serial.println(cmd);
  delay(1);
}

void getRGBValue() {
  if (stringComplete) {
    cmd.trim();
    if (cmd == "red" || cmd == "RED") {
      rgbValue = 102;
    } else  if (cmd == "green" || cmd == "GREEN") {
      rgbValue = 204;
    } else  if (cmd == "blue" || cmd == "BLUE") {
      rgbValue = 255;
    } else {
      rgbValue = 0;
    }
    Serial.println(cmd);
    // clear the string command:
    cmd = "";
    stringComplete = false;
    
    // turn off RGB LED
    //analogWrite(PWM_PIN, 0 );
    //delay(3000);
  }
}

void listenBluetoothEvent() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();      // get new byte

    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    //if (inChar == '\015') {  //if enter key is CARRIAGE RETURN (DEC =13, HEX= D, OCT= 015)
    if (inChar == '#') {  //if enter key is CARRIAGE RETURN (DEC =13, HEX= D, OCT= 015)
      stringComplete = true;
      Serial.println('\015');
    } else {
      Serial.write(inChar);
      // add it to the inputString:
      cmd += inChar;
    }
  }
}

