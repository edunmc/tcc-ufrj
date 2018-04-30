#include "pagina.h"

// timeout antes de desistir da conexao wifi
#define WIFI_TIMEOUT 10

// informacoes da rede wifi (definidos em wifi_info.ino)
// renomeie o arquivo "wifi_info.sample.ino" para "wifi_info.ino" e edite o valor das variaveis
extern const char* ssid;
extern const char* password;

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket(81);


void setup_wifi() {
  // wifi
  WiFiMulti.addAP(ssid, password);
  
  display.clearDisplay();
  display.setCursor(0,0);

  display.print("Conectando...");
  display.display();

  int tempo_faltante = WIFI_TIMEOUT * 1000;
  
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    tempo_faltante -= 100;
    if(tempo_faltante < 0) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.print("Nao foi possivel se \n"
                    "conectar. Interface \n"
                    "web nao estara dis- \n"
                    "ponivel...");
      display.display();
      delay(3000);
      return;
    }
  }

  display.print("\nConectado.\nIP ");
  display.print(WiFi.localIP());
  display.display();

  Serial.println();
  Serial.print("Conectado, IP ");
  Serial.println(WiFi.localIP());

  // servidor websocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // servidor http
  server.on("/", []() {
    // send index.html
    Serial.println("req /");
    server.send(200, "text/html", FPSTR(pagina));
    //resposta(server.client());
  });

  server.begin();

  delay(3000);
}

// disas = rodar o disassembler na interface web
void envia_memoria_atual(boolean disas) {
  String memoriaatual = memoria_atual(disas);
  webSocket.broadcastTXT(memoriaatual);
}

void envia_memoria_modificada(String alteracoes) {
  String memoriaatual = memoria_modificada(alteracoes);
  webSocket.broadcastTXT(memoriaatual);
}


void envia_io(boolean painel) {
  String resultado = "";

  resultado += ("{\n");


  resultado += ("\"io\": {\n");


  if(painel) {
    resultado += ("\"keyReg\": \"");
    resultado += String(keyReg);
    resultado += ("\",\n");
  }
  
  resultado += ("\"keyStatusReg\": \"");
  resultado += keyStatusReg == 0 ? "0" : "1";
  resultado += ("\",\n");

  resultado += ("\"kbdReg\": \"");
  resultado += String(kbdReg);
  resultado += ("\",\n");

  resultado += ("\"kbdStatusReg\": \"");
  resultado += kbdStatusReg == 0 ? "0" : "1";
  resultado += ("\",\n");

  resultado += ("\"displayReg\": \"");
  resultado += String((displayReg >> 4)  & 0xF, HEX);
  resultado += String((displayReg)       & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"bannerReg\": \"");
  resultado += bannerReg;
  resultado += ("\"\n");
  
  resultado += ("}\n");

  resultado += ("}\n");

  webSocket.broadcastTXT(resultado);
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Websocket disconectado!\n", num);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Websocket conectado: %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        //webSocket.sendTXT(num, "Connected");

        // envia conteudo da memoria
        envia_memoria_atual(true);
        envia_io(true);
      }
      break;

    case WStype_TEXT: {

      Serial.printf("[%u] get Text: %s\n", num, payload);
      String acao = (char *) payload;

      if (acao.startsWith("envia,")) {
        String arquivo = acao.substring(6);
        memoria.limpa();
        PC = 0;
        SP = 0;
        ACC = 0;
        ALU_N = 0;
        ALU_Z = 0;
        ALU_C = 0;
        processa_arquivo(arquivo);
        breakpoint = -1;
        envia_memoria_atual(true);
      }
      else if (acao.equals("play")) {
        simula_play();
      }
      else if (acao.equals("step")) {
        simula_step();
      }
      else if (acao.equals("pause")) {
        simula_pause();
      }
      else if (acao.equals("stop")) {
        simula_stop();
      }
    }
      break;
      
    case WStype_BIN: {
      Serial.printf("[%u] get bin: %s\n", num, payload);
      
      
      String acao = (char *) payload;

      if (acao.startsWith("io,")) {
        //String dados = acao.substring(3);
        // enviado em sequência (keyReg, keyStatusReg, kbdReg, kbdStatusReg)

        //Serial.printf("%d,%d,%d,%d,%d\n", dados[0], dados[1], dados[2], dados[3], dados[4]);
        Serial.printf("io chegando: %d,%d,%d,%d,%d\n", payload[3], payload[4], payload[5], payload[6], payload[7]);
        atualiza_io(payload[3], payload[4], payload[5], payload[6]);
      }

      // definindo o breakpoint
      else if (acao.startsWith("bp,")) {
        Serial.printf("breakpoint chegando: %d,%d\n", payload[3], payload[4]);
        breakpoint = (payload[3] << 8) | payload[4];
        Serial.printf("Breakpoint definido: %d\n", breakpoint);
      }

      // removendo o breakpoint
      else if (acao.startsWith("bpn")) {
        breakpoint = -1;
        Serial.printf("Breakpoint removido\n", breakpoint);
      }
      
    }
      break;
  }
}




enum campos {INICIO, QUANT, ENDER, TIPO, DADOS, CHECKSUM};


int modo = INICIO;
int quant_dados; // quantos bytes de dados numa linha do intel hex
int quant_ender; // bytes de endereço numa linha do intel hex (será sempre iniciado com 2)
unsigned int ender_inicio; // endereço do início dos bytes daquela linha
int tipo; // tipo de dados naquela linha
unsigned char dados[256];
int dados_pos;
unsigned char checksum = 0;

unsigned char novoByte;
bool byteCompleto = true;


/* Intel HEX - formato:
 * INICIO (1 caractere ":" - indica início de uma linha)
 * QUANT  (1 byte - indica a quantidade de bytes da parte DADOS)
 * ENDER  (2 bytes - indica o endereço de iniício (offset) dos dados dessa linha na memória)
 * TIPO   (1 byte - indica o tipo dos dados (0 - dados da memória, 1 - EOF, 2 e 3 - registradores) 
 * DADOS  (X bytes - o dado em si. X = tamanho indicado em QUANT)
 * CHECKSUM (1 byte - para checar a integridade da linha)
 */


// processa uma ou mais linhas de um arquivo intel hex de entrada
void processa_arquivo(const String& str) {
  int tam = str.length();
  for (int i = 0; i < tam; i++) {
    unsigned char c = (unsigned char) str[i];
    processa_byte(c);
  }
}


// processa um byte do arquivo de entrada
void processa_byte(unsigned char c) {
  
  if (modo == INICIO) {
    if (c == '\n' || c == '\r' || c == ' ') return;
    if (c == ':') modo = QUANT;
  }

  else {
    // iniciando um byte (pegando sua primeira metade)
    if (byteCompleto) {
      novoByte = char2byte(c) << 4;
    }

    // completando um byte agora
    else {
      novoByte |= char2byte(c);


      checksum += novoByte;

      if (modo == QUANT) {

        quant_dados = novoByte;

        modo = ENDER;
        quant_ender = 2;

        // nao salvar bytes 0
        //if(novoByte != 0) memoria.ref(pos) = novoByte;
      }

      else if (modo == ENDER) {
        quant_ender--;

        // primeiro byte do endereco
        if (quant_ender == 1) {
          ender_inicio = novoByte << 8;
        }

        // segundo
        else {
          ender_inicio |= novoByte;
          modo = TIPO;
        }
      }

      else if (modo == TIPO) {
        tipo = novoByte;
        modo = DADOS;
        dados_pos = 0;
      }

      else if (modo == DADOS) {
        dados[dados_pos] = novoByte;
        dados_pos++;

        Serial.print("dado: ");
        Serial.print(novoByte >> 4, HEX);
        Serial.println(novoByte & 0xF, HEX);

        // acabaram os dados
        if (dados_pos >= quant_dados) {
          modo = CHECKSUM;
        }
      }

      else if (modo == CHECKSUM) {

        // erro!
        if (checksum != 0) {
          Serial.println("Erro de checksum!");
        }

        else {
          Serial.println("Checksum ok");

          // dados
          if (tipo == 0) {

            // adicionando na memoria
            for (int j = 0; j < quant_dados; j++) {
              unsigned char dado = dados[j];
              int posicao = ender_inicio + j;

              // se for 0, só salva se tiver alguma coisa diferente já salvo na posição
              if (dado != 0 || memoria.pega(posicao) != 0)
                memoria.ref(posicao) = dado;
            }
          }

          // fim do arquivo
          else if (tipo == 1) {
            Serial.println("Fim do arquivo, tudo ok!");

            // no hex exportado pelo SimuS, valor inicial do PC é dado no endereco de inicio da linha de EOF
            if(PC == 0) // se nao foi lido nenhum PC (nao teve a linha do tipo 3)
              PC = ender_inicio;


            PC_inicial = PC;


            // alterando o estado do simulador
            // para permitir a execução em casos em que a interface web não pode ser utilizada
            estado = 4;
          }

          // registrador ACC no último byte dos 2 únicos deste tipo
          else if (tipo == 2) {
            ACC = dados[1];
          }

          // registradores SP e PC nos 4 únicos bytes de dados
          else if (tipo == 3) {
            SP = (dados[0] << 8) | dados[1];
            PC = (dados[2] << 8) | dados[3];
          }
        }

        modo = INICIO;
        checksum = 0;
      }
    }

    byteCompleto = !byteCompleto;
  }
}




String res_header =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "Connection: close\r\n"
  "\r\n";

String res_header_json =
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: application/json\r\n"
  "Connection: close\r\n"
  "\r\n";


String memoria_atual(boolean disas) {

  String resultado = "";

  resultado += ("{\n");

  if(disas) {
    resultado += "\"limpar\": \"sim\",\n\n";

    resultado += "\"breakpoint\": \"";
    resultado += breakpoint;
    resultado += "\",\n\n";
  }

  resultado += ("\"registradores\": {\n");

  resultado += ("\"pc\": \"");
  resultado += String((PC >> 12) & 0xF, HEX);
  resultado += String((PC >> 8)  & 0xF, HEX);
  resultado += String((PC >> 4)  & 0xF, HEX);
  resultado += String((PC)       & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"acc\": \"");
  resultado += String(ACC >> 4, HEX);
  resultado += String(ACC & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"sp\": \"");
  resultado += String((SP >> 12) & 0xF, HEX);
  resultado += String((SP >> 8)  & 0xF, HEX);
  resultado += String((SP >> 4)  & 0xF, HEX);
  resultado += String((SP)       & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"n\": \"");
  resultado += (ALU_N);
  resultado += ("\",\n");

  resultado += ("\"z\": \"");
  resultado += (ALU_Z);
  resultado += ("\",\n");

  resultado += ("\"c\": \"");
  resultado += (ALU_C);
  resultado += ("\"\n");

  resultado += ("},\n\n");


  resultado += ("\"memoria\": {\n");
  resultado += memoria.imprime_conteudo();
  resultado += ("}\n");

  resultado += ("}\n");


  return resultado;
}


String memoria_modificada(String alteracoes) {

  String resultado = "";

  resultado += ("{\n");


  resultado += ("\"registradores\": {\n");

  resultado += ("\"pc\": \"");
  resultado += String((PC >> 12) & 0xF, HEX);
  resultado += String((PC >> 8)  & 0xF, HEX);
  resultado += String((PC >> 4)  & 0xF, HEX);
  resultado += String((PC)       & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"acc\": \"");
  resultado += String(ACC >> 4, HEX);
  resultado += String(ACC & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"sp\": \"");
  resultado += String((SP >> 12) & 0xF, HEX);
  resultado += String((SP >> 8)  & 0xF, HEX);
  resultado += String((SP >> 4)  & 0xF, HEX);
  resultado += String((SP)       & 0xF, HEX);
  resultado += ("\",\n");

  resultado += ("\"n\": \"");
  resultado += (ALU_N);
  resultado += ("\",\n");

  resultado += ("\"z\": \"");
  resultado += (ALU_Z);
  resultado += ("\",\n");

  resultado += ("\"c\": \"");
  resultado += (ALU_C);
  resultado += ("\"\n");

  resultado += ("},\n\n");


  resultado += ("\"memoria\": {\n");
  resultado += alteracoes;
  resultado += ("}\n");

  resultado += ("}\n");



  return resultado;
}


void resposta_404(WiFiClient& client) {
  client.print(res_header);

  client.print(
    "<!DOCTYPE HTML>"
    "<html><head><meta charset=\"utf-8\"></head><body>"
    "Página não encontrada!"
    "</body></html>");
}



void web_loop() {
  /// websockets
  webSocket.loop();

  // http
  server.handleClient();
}

