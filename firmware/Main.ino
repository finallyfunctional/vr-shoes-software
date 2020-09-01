int ledPin = 2;
int buttonPin1 = 4;
int buttonPin2 = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
}

void loop() {
  int button1Value = digitalRead(buttonPin1);
  int button2Value = digitalRead(buttonPin2);
  if (button1Value == LOW || button2Value == LOW){
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
