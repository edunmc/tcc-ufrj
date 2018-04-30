void execTrap(int acc, int operando) {


  switch(acc) {
    // TRAPs padrões do Sapiens
    
    case 1: // leitura de um caractere do console
    case 2: // escrita de um caractere do console
    case 3: // leitura de uma linha do console
    case 4: // escrita de uma linha do console
      break;

    case 5: { // pausa por alguns ms
      int tempo = pega_memoria(operando, 16);
      delay(tempo);
      break;
    }

    case 6: { // beep
      int frequencia = pega_memoria(operando, 16);
      int duracao_ms = pega_memoria(operando+2, 16);
      //soundPlay (frequencia, duracao_ms);
      break;
    }

    case 7: { // RNG
      ACC = random(100);
      break;
    }

    case 8: { // definir seed do RNG
      int seed = pega_memoria(operando, 8);
      randomSeed(seed);
      break;
    }


    // Raspberry Pi / Arduino / ESP8266

    // No ESP8266, apenas os pinos D0, e D3 a D8 estão disponiveis
    // (D1 e D2 sao usados pro I2C, D9 e D10 pra serial)
    
    // D0 tem resistor pull-down, todos os outros tem pull-up
    // GPIO 0-15 podem ser OUTPUT, INPUT ou INPUT_PULLUP
    // GPIO 16 (D0) pode ser OUTPUT, INPUT ou INPUT_PULLDOWN_16

    // Pino D0 D1 D2 D3 D4 D5 D6 D7 D8 D9 D10
    // GPIO 16 05 04 00 02 14 12 13 15 03 01
    
    case 101: { // configurar pinos como entrada, saida, pwm ou liberar
      int num = pega_memoria(operando, 8);
      int tipo = pega_memoria(operando+1, 8);

      int modo = (tipo == 1) ? OUTPUT : INPUT;
      // tipo 1 = output
      // tipo 2 = input

      pinMode(num, modo);
      break;
    }
      
    case 102: { // escreve valor de saida do pino
      int num = pega_memoria(operando, 8);
      int valor = pega_memoria(operando+1, 8);

      digitalWrite(num, valor);
      break;
    }
    
    case 103: { // le valor de entrada do pino
      int num = pega_memoria(operando, 8);
      int valor = pega_memoria(operando+1, 8);

      ACC = digitalRead(num);
      break;
    }
    
    case 104: { // configurar resistencia pull-up ou pull-down
      int num = pega_memoria(operando, 8);
      int tipo = pega_memoria(operando+1, 8);

      int modo;

      modo = (tipo == 0) ? INPUT : 
                ( (num == 16) ? INPUT_PULLDOWN_16 : INPUT_PULLUP );
      // tipo 0 = input (sem resistor)
      // qualquer outro = pull-up nos pinos que nao sao o D0 (GPIO 16), pull-down no D0

      pinMode(num, modo);
      break;
    }
    
    case 105: {// duty cicle do PWM do pino (porcentagem que fica em alto) (valor: 0-1023)
      int num = pega_memoria(operando, 8);
      int valor = pega_memoria(operando+1, 16);
    
      analogWrite(num, valor);
      break;
    }


    default:
      break;
  }
}

