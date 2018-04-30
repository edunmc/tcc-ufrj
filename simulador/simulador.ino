#include <Wire.h>

#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h> // servidor WebSockets
#include <ESP8266WebServer.h> // servidor HTTP

#include <Keypad.h>
#include <Keypad_I2C.h>


#include "memoria.cpp"


// display
#define D0 16
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

Adafruit_SSD1306 display(-1);


// keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
 
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

Keypad_I2C keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, 0x20 /* I2CADDR */, PCF8574 /* IC */);


extern int PC;
extern int SP;
extern int ACC;
extern int ALU_N;
extern int ALU_Z;
extern int ALU_C;

bool executando = false;

int quantos = 0;
String inputString = "";
boolean stringComplete = false;

Memoria memoria;
int pos = 0;

int breakpoint = -1;


void setup() {

  // serial
  Serial.setDebugOutput(true);
  Serial.begin(115200);

  // simulador
  setup_simula();

  // keypad
  Wire.begin();
  keypad.begin();

  // display
  setup_display();

  // wifi
  setup_wifi();

  // push button
  pinMode(D4, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D7, INPUT_PULLUP);
}

unsigned long tempo = 0, ultimo_tempo = 0, ultimo_tempo2 = 0;

int botao = LOW, ultimo_botao = LOW;
int estado = 2;
int pode_ir = false;


extern int kbdReg;
extern int kbdStatusReg;

void loop() {

  // web
  web_loop();


  // recebimento de arquivo pela serial
  if(estado == 2)
    recebeSerial();


  // keypad
  char key = keypad.getKey();
  
  if(key != NO_KEY){
    kbdReg = key;
    kbdStatusReg = 1;

    envia_io(false);
    
    Serial.print("tecla pressionada: ");
    Serial.println(key);
  }


  // push buttons
  if(estado == 4)
    checaBotao();
  

  // atualização do display
  tempo = millis(); // tempo em ms (1s = 1000ms)
  
  // mascara pra pegar os 8 ultimos bits e ver se sao 0 = a cada 256 ms (2^8)
  if((tempo & 0xFF) == 0 && tempo != ultimo_tempo && !executando) {
    atualizaDisplay();
    ultimo_tempo = tempo;
  }
  
  // mascara pros 9 ultimos bits = a cada 512 ms
  // mascara pros 8 ultimos bits = a cada 256 ms
  if((tempo & 0x1FF) == 0 && tempo != ultimo_tempo2) {
    
    if(PC == breakpoint) executando = false;
    
    if(pode_ir) {
      executando = true;
      pode_ir = false;
    }
    
    if(executando) {
      executa_instrucao();
      atualizaDisplay();
    }
    ultimo_tempo2 = tempo;
  }
}


int botao1last = HIGH;
int botao2last = HIGH;
int botao3last = HIGH;
int botao4last = HIGH;

void checaBotao() {
  int botao1 = digitalRead(D5);
  int botao2 = digitalRead(D6);
  int botao3 = digitalRead(D7);
  int botao4 = digitalRead(D4);
  
  // não podemos usar o D8:
  // Since GPIO15 (D8) is used by the boot mode process, most of the boards have this pin pulled down by an external resistor, which is stronger than internal weak pullup. 
  // https://www.esp8266.com/viewtopic.php?f=32&t=11614#p54376

  if(botao1 != botao1last) {
    if(botao1 == LOW) {
      Serial.println("botao1");
      simula_play();
    }
    botao1last = botao1;
  }
  if(botao2 != botao2last) {
    if(botao2 == LOW) {
      Serial.println("botao2");
      simula_step();
    }
    botao2last = botao2;
  }
  if(botao3 != botao3last) {
    if(botao3 == LOW) {
      Serial.println("botao3");
      simula_pause();
    }
    botao3last = botao3;
  }
  if(botao4 != botao4last) {
    if(botao4 == LOW) {
      Serial.println("botao4");
      simula_stop();
    }
    botao4last = botao4;
  }
}


// le os dados da serial e coloca no array de memoria
void recebeSerial() {
  while(Serial.available()) {
    unsigned char c = (unsigned char) Serial.read();
    processa_byte(c);
  }
}

unsigned char char2byte(char c) {
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'F') return c - 'A' + 10;
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;
}

extern int displayReg;
extern String bannerReg;

void atualizaDisplay() {
  display.clearDisplay();
  display.setCursor(0,0);
  
  if(estado == 1) { // nao mais usado
    display.print("Bem vindo. Pressione o botao para comecar...");
  }
  else if(estado == 2) {
    display.print("Aguardando envio do\narquivo Intel HEX...");
  }
  else if(estado == 3) { // nao mais usado
    display.print("PC ");
    display.print(PC >> 4, HEX);
    display.print(PC & 0xF, HEX);
    display.print(", ACC ");
    display.print(ACC >> 4, HEX);
    display.print(ACC & 0xF, HEX);
    display.print(", SP ");
    display.print(SP >> 4, HEX);
    display.print(SP & 0xF, HEX);
    display.print("\n");
    
    // escrevendo conteúdo da memória
      for(int i = 0; i < 24; i++) {
        unsigned char b = memoria.pega(i);
        display.print(b >> 4, HEX);
        display.print(b & 0xF, HEX);

        // dar espaço depois dos ímpares, pra ficar: 0000 0000 0000
        if(i & 1) display.print(" ");

        // a cada 8 bytes pula linha
        if(((i+1) & 7) == 0) display.print("\n");
      }
  }
  else {
    display.setCursor(4, 0);
    display.print("PC ");
    display.print(PC >> 4, HEX);
    display.print(PC & 0xF, HEX);
    display.print(", ACC ");
    display.print(ACC >> 4, HEX);
    display.print(ACC & 0xF, HEX);
    display.print(", SP ");
    display.print(SP >> 4, HEX);
    display.print(SP & 0xF, HEX);

    display.setCursor(24, 11);
    display.print("N ");
    display.print(ALU_N);
    display.print(", Z ");
    display.print(ALU_Z);
    display.print(", C ");
    display.print(ALU_C);
    
    display.setCursor(16, 23);
    display.print(bannerReg);

    display.drawRect(13, 21, 101, 11, WHITE);
  }
  display.display();
}

