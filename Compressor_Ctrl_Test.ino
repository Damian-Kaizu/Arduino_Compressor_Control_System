//Programa: Display LCD Touch TFT 3.5" com Arduino
//Autor: Arduino e Cia
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>  //Biblioteca grafica
#include <Fonts/FreeSans9pt7b.h>
//Biblioteca Touch
#include <Waveshare_ILI9486.h>
#define YP A3  // Y+ is on Analog1
#define XM A2  // X- is on Analog2
#define YM 9   // Y- is on Digital7
#define XP 8   // X+ is on Digital6

#define TS_MINX 118
#define TS_MINY 92
#define TS_MAXX 906
#define TS_MAXY 951



#define LCD_RESET A4  //Pode ser conectado ao pino reset do Arduino
#define LCD_CS A3     // Chip Select
#define LCD_CD A2     // Command/Data
#define LCD_WR A1     // LCD Write
#define LCD_RD A0     // LCD Read

//Definicao de cores
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

namespace {
Waveshare_ILI9486 Waveshield;
Adafruit_GFX &tft = Waveshield;
}

#define MINPRESSURE 400
#define MAXPRESSURE 1000


// Armazena o estado dos botões
bool valor_botao1 = 0;
bool valor_botao2 = 0;
bool active = true;
String Status = "Running";
int x = 10;
char currentPage, selectedUnit;

void setup() {
  Serial.begin(9600);
  give_status();
  SPI.begin();
  Waveshield.begin();
  //Definicao pinos dos leds
  pinMode(45, OUTPUT);  //Led vermelho
  pinMode(49, OUTPUT);  //Led verde

  Serial.println(F("Teste do display LCD TFT"));
  Serial.println(Status);

  //Mostra o tipo do controlador do display no Serial Monitor
  HomeScreen();
  currentPage = '0';  // Indicates that we are at Home Screen
  selectedUnit = '0';
}

void loop() {
  if (currentPage == '0') {
    TSPoint p = Waveshield.getPoint();
    Waveshield.normalizeTsPoint(p);
    pinMode(XM, OUTPUT);
    digitalWrite(XM, LOW);
    pinMode(YP, OUTPUT);
    digitalWrite(YP, HIGH);
    pinMode(YM, OUTPUT);
    digitalWrite(YM, LOW);
    pinMode(XP, OUTPUT);
    digitalWrite(XP, HIGH);
    Status = 'Running';
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
      p.x = tft.width() - (map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
      p.y = tft.height() - (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));

      //Mostra no serial monitor as coordenadas quando o
      //touch for acionado
      // Serial.println(Status);
      Serial.print("py: ");
      Serial.print(p.y);
      Serial.print(" px: ");
      Serial.println(p.x);
      Serial.println(x);
      if (p.x == 161 || p.y == 73) {
          Back();
        }
      if (p.x == 169 || p.y == 46) {
        Compressor();
      }
      if (p.x > -71 & p.x < -48) {
        //Botao led vermelho
        if (p.y > 45 & p.y < 55) {
          if (valor_botao1 == 0) {

            tft.fillRoundRect(390, 96, 82, 48, 5, RED);
            tft.drawRoundRect(390, 96, 82, 48, 5, WHITE);
            tft.drawRoundRect(390, 96, 82, 48, 5, WHITE);
            Active(410, 125);
            valor_botao1 = !valor_botao1;
            active = true;

            // digitalWrite(45, HIGH);
          } else {

            tft.fillRoundRect(390, 96, 82, 48, 5, BLACK);
            tft.drawRoundRect(390, 96, 82, 48, 5, WHITE);
            tft.drawRoundRect(390, 96, 82, 48, 5, WHITE);
            Inactive(410, 125);
            valor_botao1 = !valor_botao1;
            active = false;

            // digitalWrite(45, LOW);
          }
        }

        if (p.y > 20 & p.y < 24) {
          if (valor_botao2 == 0) {
            tft.fillRoundRect(390, 151, 82, 48, 5, GREEN);
            tft.drawRoundRect(390, 151, 82, 48, 5, WHITE);
            tft.drawRoundRect(390, 151, 82, 48, 5, WHITE);
            Active(410, 180);
            valor_botao2 = !valor_botao2;
            active = true;
            // digitalWrite(49, HIGH);
          } else {
            tft.fillRoundRect(390, 151, 82, 48, 5, BLACK);
            tft.drawRoundRect(390, 151, 82, 48, 5, WHITE);
            tft.drawRoundRect(390, 151, 82, 48, 5, WHITE);
            Inactive(410, 180);
            valor_botao2 = !valor_botao2;
            active = false;
            // digitalWrite(49, LOW);
          }
        }
      }
    }
  }

  // tft.fillRoundRect(0, 35, 479, 60, 5, BLACK);
}


void Active(int x, int y) {
  tft.setTextColor(BLACK);
  tft.setCursor(x, y);
  tft.println("Start");
  active = true;
  delay(100);
}

void Inactive(int x, int y) {
  tft.setTextColor(WHITE);
  tft.setCursor(x, y);
  tft.println("Stop");
  active = false;
  delay(100);
}

String give_status() {
  if (active == true) {
    Status = "Working";
  } else {
    Status = "Stopped";
  }
}

void HomeScreen() {
  tft.setFont(&FreeSans9pt7b);
  tft.fillScreen(BLACK);
  tft.setRotation(1);

  //Tela Principal
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setCursor(190, 20);
  tft.println("Compressor Control");

  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setCursor(10, 20);
  tft.println("12/12/2000 09:10:10");

  tft.drawRoundRect(20, 30, 100, 50, 5, WHITE);
  // tft.drawRoundRect(100, 50, 180, 50, 5, WHITE);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setCursor(35, 60);
  tft.println("> BACK");

  tft.drawRoundRect(15, 95, 180, 50, 5, WHITE);
  tft.drawRoundRect(200, 95, 180, 50, 5, WHITE);
  tft.setTextColor(RED);
  tft.setTextSize(1);
  tft.setCursor(40, 125);
  tft.println("Compressor 1");

  tft.drawRoundRect(15, 150, 180, 50, 5, WHITE);
  tft.drawRoundRect(200, 150, 180, 50, 5, WHITE);
  tft.setTextColor(GREEN);
  tft.setCursor(40, 180);
  tft.println("Compressor 2");

  // Mensagens desligado
  tft.setTextColor(WHITE);
  tft.setCursor(245, 125);
  tft.println(Status);
  tft.setCursor(245, 180);
  tft.println(Status);
  tft.fillRoundRect(390, 96, 82, 48, 5, GREEN);
  tft.drawRoundRect(390, 96, 82, 48, 5, WHITE);
  tft.drawRoundRect(390, 96, 82, 48, 5, WHITE);
  Active(410, 125);
  tft.fillRoundRect(390, 151, 82, 48, 5, BLACK);
  tft.drawRoundRect(390, 151, 82, 48, 5, WHITE);
  tft.drawRoundRect(390, 151, 82, 48, 5, WHITE);
  Inactive(410, 180);
}
void Compressor() {

  tft.setFont(&FreeSans9pt7b);
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  tft.endWrite();
  //Tela Principal
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setCursor(190, 20);
  tft.println("Compressor Control");

  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setCursor(10, 20);
  tft.println("12/12/2000 09:10:10");

  tft.drawRoundRect(20, 30, 100, 50, 5, WHITE);
  // tft.drawRoundRect(100, 50, 180, 50, 5, WHITE);
  tft.setTextColor(BLUE);
  tft.setTextSize(1);
  tft.setCursor(35, 60);
  tft.println("> BACK");
  TSPoint p = Waveshield.getPoint();
  Waveshield.normalizeTsPoint(p);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = tft.width() - (map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));
    p.y = tft.height() - (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0));
    if (p.x == 161 || p.y == 73) {
      Back();
    }
  }
}

void Back() {
  HomeScreen();
}