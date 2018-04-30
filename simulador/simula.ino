enum cod_instrucao { // códigos das instruções
    NOP  = 0,
    STA  = 4,
    STS  = 5,
    LDA  = 8,
    LDS  = 9,
    ADD  = 12,
    ADC  = 13,
    SUB  = 14,
    SBC  = 15,
    OR   = 16,
    XOR  = 17,
    AND  = 20,
    NOT  = 24,
    SHL  = 28,
    SHR  = 29,
    SRA  = 30,
    JMP  = 32,
    JN   = 36,
    JP   = 37,
    JZ   = 40,
    JNZ  = 41,
    JC   = 44,
    JNC  = 45,
    IN   = 48,
    OUT  = 49,
    JSR  = 52,
    RET  = 54,
    PUSH = 56,
    POP  = 57,
    TRAP = 60,
    HLT  = 63,

    ORG  = 100,
    END  = 101,
    DS   = 102,
    DB   = 103,
    DW   = 104,
    STR  = 105,
    EQU  = 106,
    LDI  = 107,

    VAZIA = 254,
    INVALIDA = 255
};

enum Parametros { // tipos de parâmetros
    NADA        = 0,
    DIRETO      = 1,
    INDIRETO    = 2,
    IMEDIATO    = 4,
    IMEDIATO_16 = 8,
    VALOR       = 16,
    LISTA       = 32,
    CADEIA      = 64
};

enum Mascaras {
  BYTE_MASK = 0xFF,
  WORD_MASK = 0xFFFF,
  CARRY_MASK = 0x100,
  MAX_MEM = 0xFFFF
};

extern Memoria memoria;
extern int estado;

// parâmetros (modos de endereçamento) de cada opcode
int ender[120];

// nome das instruções
char *nome_instrucao[256];

int PC = 0;
int SP = 0;
int ACC = 0;
int ALU_N = 0;
int ALU_Z = 0;
int ALU_C = 0;
int PC_inicial = 0;

int displayReg  = 0;
String bannerReg = "   SAPIENS-8";

int keyReg = 0;
int keyStatusReg = 0;

int kbdReg = 0;
int kbdStatusReg = 0;


/*
int BYTE_MASK = 0xff;    // 8 bits
int WORD_MASK = 0xffff;  // 16 bits
int CARRY_MASK = 0x0100; // bit depois dos 8 primeiros
int MAX_MEM = 0xFFFF;    // 16 bits
*/

void setup_simula() {
  // parâmetros (modos de endereçamento) de cada opcode
  ender[NOP] = NADA;
  ender[STA] = DIRETO+INDIRETO;
  ender[STS] = DIRETO+INDIRETO;
  ender[LDA] = DIRETO+INDIRETO+IMEDIATO;
  ender[LDS] = DIRETO+INDIRETO+IMEDIATO_16;
  ender[ADD] = DIRETO+INDIRETO+IMEDIATO;
  ender[ADC] = DIRETO+INDIRETO+IMEDIATO;
  ender[SUB] = DIRETO+INDIRETO+IMEDIATO;
  ender[SBC] = DIRETO+INDIRETO+IMEDIATO;
  ender[OR]  = DIRETO+INDIRETO+IMEDIATO;
  ender[XOR] = DIRETO+INDIRETO+IMEDIATO;
  ender[AND] = DIRETO+INDIRETO+IMEDIATO;
  ender[NOT] = NADA;
  ender[SHL] = NADA;
  ender[SHR] = NADA;
  ender[SRA] = NADA;
  ender[JMP] = DIRETO+INDIRETO;
  ender[JN]  = DIRETO+INDIRETO;
  ender[JP]  = DIRETO+INDIRETO;
  ender[JZ]  = DIRETO+INDIRETO;
  ender[JNZ] = DIRETO+INDIRETO;
  ender[JC]  = DIRETO+INDIRETO;
  ender[JNC] = DIRETO+INDIRETO;
  ender[IN]  = IMEDIATO;
  ender[OUT] = IMEDIATO;
  ender[JSR] = DIRETO+INDIRETO;
  ender[RET] = NADA;
  ender[PUSH] = NADA;
  ender[POP] = NADA;
  ender[TRAP] = DIRETO+INDIRETO;
  ender[HLT] = NADA;
  ender[ORG] = VALOR;
  ender[END] = VALOR;
  ender[DS]  = VALOR;
  ender[DB]  = VALOR+LISTA;
  ender[DW]  = VALOR+LISTA;
  ender[STR] = CADEIA;
  ender[EQU] = VALOR;
  ender[LDI] = VALOR;


  nome_instrucao[NOP] = "NOP";
  nome_instrucao[STA] = "STA";
  nome_instrucao[STS] = "STS";
  nome_instrucao[LDA] = "LDA";
  nome_instrucao[LDS] = "LDS";
  nome_instrucao[ADD] = "ADD";
  nome_instrucao[ADC] = "ADC";
  nome_instrucao[SUB] = "SUB";
  nome_instrucao[SBC] = "SBC";
  nome_instrucao[OR] = "OR";
  nome_instrucao[XOR] = "XOR";
  nome_instrucao[AND] = "AND";
  nome_instrucao[NOT] = "NOT";
  nome_instrucao[SHL] = "SHL";
  nome_instrucao[SHR] = "SHR";
  nome_instrucao[SRA] = "SRA";
  nome_instrucao[JMP] = "JMP";
  nome_instrucao[JN] = "JN";
  nome_instrucao[JP] = "JP";
  nome_instrucao[JZ] = "JZ";
  nome_instrucao[JNZ] = "JNZ";
  nome_instrucao[JC] = "JC";
  nome_instrucao[JNC] = "JNC";
  nome_instrucao[IN] = "IN";
  nome_instrucao[OUT] = "OUT";
  nome_instrucao[JSR] = "JSR";
  nome_instrucao[RET] = "RET";
  nome_instrucao[PUSH] = "PUSH";
  nome_instrucao[POP] = "POP";
  nome_instrucao[TRAP] = "TRAP";
  nome_instrucao[HLT] = "HLT";
  nome_instrucao[ORG] = "ORG";
  nome_instrucao[END] = "END";
  nome_instrucao[DS] = "DS";
  nome_instrucao[DB] = "DB";
  nome_instrucao[DW] = "DW";
  nome_instrucao[STR] = "STR";
  nome_instrucao[EQU] = "EQU";
  nome_instrucao[LDI] = "LDI";
  nome_instrucao[VAZIA] = "VAZIA";
  nome_instrucao[INVALIDA] = "INVALIDA";
}



// acoes do simulador

void simula_play() {
  executando = true;
  pode_ir = true;
}
void simula_step() {
  executa_instrucao();
}

void simula_pause() {
  executando = false;
}

void simula_stop() {
  executando = false;
  PC = PC_inicial;
  SP = 0;
  ACC = 0;
  ALU_N = 0;
  ALU_Z = 0;
  ALU_C = 0;
  envia_memoria_atual(false);
}



int atualiza_flags(int resultado) {
  ALU_N = (resultado & 0x80) != 0;
  ALU_Z = (resultado & 0xFF) == 0;
  ALU_C = (resultado & CARRY_MASK) != 0;
  return resultado & BYTE_MASK;
}

int atualiza_io(int _keyReg, int _keyStatusReg, int _kbdReg, int _kbdStatusReg) {

  Serial.print("io recebido: \n");
  Serial.print("dados[0] = ");
  Serial.println(_keyReg);
  Serial.print("dados[1] = ");
  Serial.println(_keyStatusReg);
  Serial.print("dados[2] = ");
  Serial.println(_kbdReg);
  Serial.print("dados[3] = ");
  Serial.println(_kbdStatusReg);
  
  keyReg = _keyReg;
  keyStatusReg = _keyStatusReg;
  kbdReg = _kbdReg;
  kbdStatusReg = _kbdStatusReg;
}



/*
v1 = mem[e]
v2 = mem[e+1]

se 8 bits: v1
se 16 bits: (v2 << 8) | v1
*/
int pega_memoria(int endereco, int quantos_bits) {
  int e = endereco & WORD_MASK;
  int v1 = memoria.pega(e) & 0xFF;
  int v2;
  
  if(quantos_bits == 8)
    v2 = 0;
  else {
    e = (endereco+1) & WORD_MASK;
    v2 = memoria.pega(e) & 0xFF;
  }
  
  return (v2 << 8) | v1;
}


String alteracoes_memoria = "";


void escreve_memoria(unsigned short int endereco, unsigned char valor) {
  memoria.ref(endereco) = valor;

  if(valor != 0) {
    
    if(alteracoes_memoria.length() > 0)
      alteracoes_memoria += ",\n";

    alteracoes_memoria += "\"";
    alteracoes_memoria += endereco;
    alteracoes_memoria += "\": \"";
    alteracoes_memoria += String(valor >> 4, HEX);
    alteracoes_memoria += String(valor & 0xF, HEX);
    alteracoes_memoria += "\"";
  }
}


void executa_instrucao() {

  // C    2
  // 1100 0010

  // Nas instruções, o primeiro byte sempre contém o código de operação nos 6 bits mais significativos e, quando for o caso, o modo de endereçamento nos 2 bits menos significativos.

  int opcode = memoria.pega(PC) >> 2;                   // 6 bits mais significativos
  int modo_enderecamento = 1 << (memoria.pega(PC) & 3); // 2 bits menos significativos
  PC = (PC + 1) & WORD_MASK;
  
  //Instrucao instrucao = tabela_instrucoes.find(opcode);
  int parametros = ender[opcode];
  
  
  int operando = 0;
  int resultado;

  
  alteracoes_memoria = "";

  
  if(parametros != NADA) {
    switch(modo_enderecamento) {
      case DIRETO:
        operando = pega_memoria (PC, 16);
        PC = (PC + 2) & WORD_MASK;
        break;
        
      case INDIRETO:
        operando = pega_memoria (pega_memoria (PC, 16), 16);
        PC = (PC + 2) & WORD_MASK;
        break;
      
      case IMEDIATO:
        operando = pega_memoria (PC, 8);
        PC = (PC + 1) & WORD_MASK;
        break;
      
      case IMEDIATO_16:
        operando = pega_memoria (PC, 16);
        PC = (PC + 2) & WORD_MASK;
        break;
    }
  }
  
  
  switch(opcode) {
    case LDA:
    case ADD:
    case ADC:
    case SUB:
    case SBC:
    case OR:
    case XOR:
    case AND:
      if(modo_enderecamento == DIRETO || modo_enderecamento == INDIRETO)
        operando = pega_memoria(operando, 8);
      break;
    default:
      break;
  }
  
  switch(opcode) {
    case NOP: break;
    
    case LDA: ACC = operando;
              atualiza_flags(ACC);
              break;
    
    case STA: escreve_memoria(operando, ACC);
              break;
    
    case LDS: SP = operando;
              break;
    
    case STS: escreve_memoria(operando, SP & BYTE_MASK);
              escreve_memoria(operando+1, (SP >> 8) & BYTE_MASK);
              break;
    
    case ADD: resultado = ACC + operando;
              atualiza_flags(resultado);
              ACC = resultado & BYTE_MASK;
              break;
      
    case ADC: resultado = ACC + operando + ALU_C;
              atualiza_flags(resultado);
              ACC = resultado & BYTE_MASK;
              break;
      
    case SUB: resultado = ACC - operando;
              atualiza_flags(resultado);
              ACC = resultado & BYTE_MASK;
              break;
      
    case SBC: resultado = ACC - operando - ALU_C;
              atualiza_flags(resultado);
              ACC = resultado & BYTE_MASK;
              break;
    
    case OR:  ACC = (ACC | operando) & BYTE_MASK;
              atualiza_flags(ACC);
              ALU_C = 0;
              break;
    
    case XOR: ACC = (ACC ^ operando) & BYTE_MASK;
              atualiza_flags(ACC);
              ALU_C = 0;
              break;
      
    case AND: ACC = (ACC & operando) & BYTE_MASK;
              atualiza_flags(ACC);
              ALU_C = 0;
              break;
      
    case NOT: ACC = (~ACC) & BYTE_MASK;
              atualiza_flags(ACC);
              ALU_C = 0;
              break;
      
    case SHL: resultado = ACC << 1;
              atualiza_flags(resultado);
              ACC = resultado & BYTE_MASK;
              break;
      
    case SHR: ALU_C = ACC & 1;
              ACC = ACC >> 1;
              atualiza_flags(ACC);
              break;
      
    case SRA: ALU_C = ACC & 1;
              ACC = (ACC & 0x80) | (ACC >> 1);
              atualiza_flags(ACC);
              break;
      
    case JMP: PC = operando;
              break;
      
    case JN:  if(ALU_N)  PC = operando;
              break;
    
    case JP:  if(!ALU_N && !ALU_Z) PC = operando;
              break;
      
    case JZ:  if(ALU_Z)  PC = operando;
              break;
      
    case JNZ: if(!ALU_Z) PC = operando;
              break;
      
    case JC:  if(ALU_C)  PC = operando;
              break;
      
    case JNC: if(!ALU_C) PC = operando;
              break;

    case IN:       if(operando == 0) (ACC = keyReg, keyStatusReg = 0);
              else if(operando == 1)  ACC = keyStatusReg;
              else if(operando == 2) (ACC = kbdReg, kbdStatusReg = 0);
              else if(operando == 3)  ACC = kbdStatusReg;
              break;
    
    case OUT:      if(operando == 0) displayReg = ACC;
              else if(operando == 1) {}//keyStatusReg = ACC & 1;
              else if(operando == 2) {
                                       bannerReg = bannerReg + (char) ACC;
                                       while(bannerReg.length() > 16) bannerReg.remove(0, 1);
                                     }
              else if(operando == 3) bannerReg = "";
              break;
    
    case JSR:  memoria.ref(--SP) = (PC >> 8) & BYTE_MASK;
               memoria.ref(--SP) = PC & BYTE_MASK;
               PC = operando;
               break;
    
    case RET:  PC = (memoria.pega(SP) | (memoria.pega(SP+1) << 8)) & WORD_MASK;
               SP += 2;
               break;
    
    case PUSH: escreve_memoria(--SP, ACC);
    
    case POP:  ACC = memoria.pega(SP++);
               atualiza_flags(ACC);
    
    case TRAP: execTrap(ACC, operando);
               break;
      
    case HLT:  executando = false;
               estado = 4;
               PC--;
               break;
    
    default: break;
  }

  if(opcode == IN || opcode == OUT)
    envia_io(false);

  //envia_memoria_atual();
  envia_memoria_modificada(alteracoes_memoria);

}


