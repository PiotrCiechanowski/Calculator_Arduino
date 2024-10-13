#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const byte ROWS = 4; 
const byte COLS = 4; 

char keys[ROWS][COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

double first = 0;
double second = 0;
double total = 0;
char operatorChar;
bool equalPressed = false;

void displayValue(double value, char op = '\0'); 

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("      Kalkulator       Piotr Ciechanowski                                                Wpisz Liczbe");
  display.display();
  delay(2000);
}

void loop() {
  char key = customKeypad.getKey();

  if (key) {
    if (key >= '0' && key <= '9') {
      if (equalPressed) {
        first = 0;
        equalPressed = false;
      }
      first = first * 10 + (key - '0');
      displayValue(first);
    } else if (key == 'C') {
      first = 0;
      second = 0;
      operatorChar = '\0';
      display.clearDisplay();
      display.display();
    } else if (key == '+' || key == '-' || key == '*' || key == '/') {
      operatorChar = key;
      second = first; 
      first = 0; 
      displayValue(second, operatorChar);
    } else if (key == '=') {
      if (operatorChar) {
        switch (operatorChar) {
          case '+': total = second + first; break;
          case '-': total = second - first; break;
          case '*': total = second * first; break;
          case '/':
            if (first != 0) total = second / first;
            else {
              displayInvalid();
              return;
            }
            break;
        }
        displayResult(total);
        equalPressed = true; 
      }
    }
  }
}

void displayValue(double value, char op) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(3);
  display.print(value);
  if (op) {
    display.setCursor(0, 30);
    display.print(op);
  }
  display.display();
}

void displayResult(double result) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(3);
  display.print("Wynik:");
  display.setCursor(0, 30);
  display.print(result);
  display.display();
}

void displayInvalid() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(3);
  display.print("Blad");
  display.display();
}
