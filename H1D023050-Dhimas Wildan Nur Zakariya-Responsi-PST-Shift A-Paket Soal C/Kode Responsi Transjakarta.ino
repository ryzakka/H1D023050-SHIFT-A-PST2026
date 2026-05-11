#include <Arduino.h>
#include <Servo.h>

Servo myservo;

const int buttonPin = 2;     //Pin tombol
const int servoPin = 9;      //Pin servo
const int ledPinMerah = 12;  //Pin Merah
const int ledPinHijau = 11;  //Pin Hijau

int pos = 0;                 //Sudut servo   
bool lastButtonState = HIGH; //Kondisi akhir tidak ditekan

void setup() { 
  pinMode(ledPinMerah, OUTPUT);
  pinMode(ledPinHijau, OUTPUT); 
} 

void loop() {
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    myservo.write(90);
    digitalWrite(ledPinHijau, HIGH);
    digitalWrite(ledPinMerah, LOW);
    delay(50);
  } else {
    myservo.write(0);
    digitalWrite(ledPinHijau, LOW);
    digitalWrite(ledPinMerah, HIGH);
    delay(50);
  }

  lastButtonState = currentButtonState;
}
