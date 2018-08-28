#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonPin = 7;
int buttonState;
int lastButtonState = LOW;

int timer = 10;
unsigned long previousMillis = 0;

const int servoPin = 8;
Servo myServo;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

const String STOPPED = "STOPPED", STARTED = "STARTED", PAUSED = "PAUSED";
String timerState = STOPPED;

void setup() {
  myServo.attach(servoPin);
  myServo.write(180);

  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("status: ");
  lcd.print(timerState);
  lcd.setCursor(0, 1);
  lcd.print("timer: ");
  lcd.print(timer);
  lcd.print("s");
}

void loop() {
  checkButtons();
  updateTimer();
}

void checkButtons() {
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      Serial.print("buttonState");
      Serial.println(buttonState);
      if (buttonState == LOW) {
        if (timerState == STOPPED) {
          resetTimer();
          timerState = STARTED;
          updateScreen();
        } else if (timerState == STARTED) {
          timerState = STOPPED;
          resetTimer();
          updateScreen();
        }
      }
    }
  }

  lastButtonState = reading;
}

void resetTimer() {
  timer = 10;
}

void updateTimer() {
  if (timerState == STARTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      if (timer > 0) {
        timer = timer - 1;
        updateScreen();
      } else {
        myServo.write(158);
        delay(500);
        myServo.write(180);
        delay(1000);
        timerState = STOPPED;
        timer = 10;
        timerState = STARTED;
      }
    }
  }
}

void updateScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("status: ");
  lcd.print(timerState);
  lcd.setCursor(0, 1);
  lcd.print("timer: ");
  lcd.print(timer);
  lcd.print("s");
}
