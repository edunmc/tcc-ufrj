#include <ESP8266WiFi.h>

#define N_PAGINAS 64
#define TAM_PAGINA (64*1024 / N_PAGINAS)

#ifndef NULL
#define NULL 0
#endif

class Memoria {

  unsigned char* paginas[N_PAGINAS] = {NULL};

  private:

  public:
    Memoria() {
    }
    
    // atribuição de valor
    //unsigned char& operator [](unsigned short int pos) {
    unsigned char& ref(unsigned short int pos) {

      int pagina = pos >> 10;
      int offset = pos & 0x3FF;

      if(paginas[pagina] == NULL) {
        paginas[pagina] = (unsigned char*) malloc(sizeof(unsigned char) * TAM_PAGINA);
        for(int i = 0; i < TAM_PAGINA; i++) paginas[pagina][i] = 0;
      }

      return paginas[pagina][offset];
    }

    // leitura de valor
    //unsigned char operator [](unsigned short int pos) const {
    unsigned char pega(unsigned short int pos) {

      int pagina = pos >> 10;
      int offset = pos & 0x3FF;

      if(paginas[pagina] == NULL)
        return 0;
      
      return paginas[pagina][offset];
    }

    void limpa() {
      Serial.println("limpando memoria...");
      for(int i = 0; i < N_PAGINAS; i++) {
        if(paginas[i] != NULL) {
           for(int j = 0; j < TAM_PAGINA; j++) paginas[i][j] = 0;
        }
      }
    }

    String imprime_conteudo() {

      String resultado = "";
      bool primeiro = true;
     
      for(int i = 0; i < N_PAGINAS; i++) {
        if(paginas[i] == NULL) continue;
        
        for(int j = 0; j < TAM_PAGINA; j++) {
          if(paginas[i][j] != 0) {
            if(primeiro) primeiro = false;
            else resultado += ",\n";
  
            resultado += "\"";
            resultado += ((i << 10) | j);
            resultado += "\": \"";
            resultado += String((paginas[i][j]) >> 4, HEX);
            resultado += String((paginas[i][j]) & 0xF, HEX);
            resultado += "\"";
          }
        }
      }

      return resultado;
    }
};

