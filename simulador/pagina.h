// raw string literals, c++11: https://stackoverflow.com/a/25021520 e https://stackoverflow.com/a/30308184

static const char pagina[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
<head>
<meta charset="utf-8">

<script>
/*
Clusterize.js - v0.18.0 - 2017-11-04
https://clusterize.js.org/
(fixes pra IE <= 9 removidos)
*/
!function(t,e){"undefined"!=typeof module?module.exports=e():"function"==typeof define&&"object"==typeof define.amd?define(e):this.Clusterize=e()}(0,function(){"use strict";function t(t,e,o){return e.addEventListener?e.addEventListener(t,o,!1):e.attachEvent("on"+t,o)}function e(t,e,o){return e.removeEventListener?e.removeEventListener(t,o,!1):e.detachEvent("on"+t,o)}function o(t){return"[object Array]"===Object.prototype.toString.call(t)}function n(t,e){return window.getComputedStyle?window.getComputedStyle(e)[t]:e.currentStyle[t]}var r=navigator.platform.toLowerCase().indexOf("mac")+1,s=function(n){if(!(this instanceof s))return new s(n);var i=this,l={rows_in_block:50,blocks_in_cluster:4,tag:null,show_no_data_row:!0,no_data_class:"clusterize-no-data",no_data_text:"No data",keep_parity:!0,callbacks:{}};i.options={};for(var a,c=["rows_in_block","blocks_in_cluster","show_no_data_row","no_data_class","no_data_text","keep_parity","tag","callbacks"],h=0;a=c[h];h++)i.options[a]=void 0!==n[a]&&null!=n[a]?n[a]:l[a];for(var u,_=["scroll","content"],h=0;u=_[h];h++)if(i[u+"_elem"]=n[u+"Id"]?document.getElementById(n[u+"Id"]):n[u+"Elem"],!i[u+"_elem"])throw new Error("Error! Could not find "+u+" element");i.content_elem.hasAttribute("tabindex")||i.content_elem.setAttribute("tabindex",0);var p=o(n.rows)?n.rows:i.fetchMarkup(),m={},g=i.scroll_elem.scrollTop;i.insertToDOM(p,m),i.scroll_elem.scrollTop=g;var f=!1,d=0,w=!1,v=function(){r&&(w||(i.content_elem.style.pointerEvents="none"),w=!0,clearTimeout(d),d=setTimeout(function(){i.content_elem.style.pointerEvents="auto",w=!1},50)),f!=(f=i.getClusterNum())&&i.insertToDOM(p,m),i.options.callbacks.scrollingProgress&&i.options.callbacks.scrollingProgress(i.getScrollProgress())},b=0,k=function(){clearTimeout(b),b=setTimeout(i.refresh,100)};t("scroll",i.scroll_elem,v),t("resize",window,k),i.destroy=function(t){e("scroll",i.scroll_elem,v),e("resize",window,k),i.html((t?i.generateEmptyRow():p).join(""))},i.refresh=function(t){(i.getRowsHeight(p)||t)&&i.update(p)},i.update=function(t){p=o(t)?t:[];var e=i.scroll_elem.scrollTop;p.length*i.options.item_height<e&&(i.scroll_elem.scrollTop=0,f=0),i.insertToDOM(p,m),i.scroll_elem.scrollTop=e},i.clear=function(){i.update([])},i.getRowsAmount=function(){return p.length},i.getScrollProgress=function(){return this.options.scroll_top/(p.length*this.options.item_height)*100||0};var T=function(t,e){var n=o(e)?e:[];n.length&&(p="append"==t?p.concat(n):n.concat(p),i.insertToDOM(p,m))};i.append=function(t){T("append",t)},i.prepend=function(t){T("prepend",t)}};return s.prototype={constructor:s,fetchMarkup:function(){for(var t=[],e=this.getChildNodes(this.content_elem);e.length;)t.push(e.shift().outerHTML);return t},exploreEnvironment:function(t,e){var o=this.options;o.content_tag=this.content_elem.tagName.toLowerCase(),t.length&&(this.content_elem.children.length<=1&&(e.data=this.html(t[0]+t[0]+t[0])),o.tag||(o.tag=this.content_elem.children[0].tagName.toLowerCase()),this.getRowsHeight(t))},getRowsHeight:function(t){var e=this.options,o=e.item_height;if(e.cluster_height=0,t.length){var r=this.content_elem.children,s=r[Math.floor(r.length/2)];if(e.item_height=s.offsetHeight,"tr"==e.tag&&"collapse"!=n("borderCollapse",this.content_elem)&&(e.item_height+=parseInt(n("borderSpacing",this.content_elem),10)||0),"tr"!=e.tag){var i=parseInt(n("marginTop",s),10)||0,l=parseInt(n("marginBottom",s),10)||0;e.item_height+=Math.max(i,l)}return e.block_height=e.item_height*e.rows_in_block,e.rows_in_cluster=e.blocks_in_cluster*e.rows_in_block,e.cluster_height=e.blocks_in_cluster*e.block_height,o!=e.item_height}},getClusterNum:function(){return this.options.scroll_top=this.scroll_elem.scrollTop,Math.floor(this.options.scroll_top/(this.options.cluster_height-this.options.block_height))||0},generateEmptyRow:function(){var t=this.options;if(!t.tag||!t.show_no_data_row)return[];var e,o=document.createElement(t.tag),n=document.createTextNode(t.no_data_text);return o.className=t.no_data_class,"tr"==t.tag&&((e=document.createElement("td")).colSpan=100,e.appendChild(n)),o.appendChild(e||n),[o.outerHTML]},generate:function(t,e){var o=this.options,n=t.length;if(n<o.rows_in_block)return{top_offset:0,bottom_offset:0,rows_above:0,rows:n?t:this.generateEmptyRow()};var r=Math.max((o.rows_in_cluster-o.rows_in_block)*e,0),s=r+o.rows_in_cluster,i=Math.max(r*o.item_height,0),l=Math.max((n-s)*o.item_height,0),a=[],c=r;i<1&&c++;for(var h=r;h<s;h++)t[h]&&a.push(t[h]);return{top_offset:i,bottom_offset:l,rows_above:c,rows:a}},renderExtraTag:function(t,e){var o=document.createElement(this.options.tag);return o.className=["clusterize-extra-row","clusterize-"+t].join(" "),e&&(o.style.height=e+"px"),o.outerHTML},insertToDOM:function(t,e){this.options.cluster_height||this.exploreEnvironment(t,e);var o=this.generate(t,this.getClusterNum()),n=o.rows.join(""),r=this.checkChanges("data",n,e),s=this.checkChanges("top",o.top_offset,e),i=this.checkChanges("bottom",o.bottom_offset,e),l=this.options.callbacks,a=[];r||s?(o.top_offset&&(this.options.keep_parity&&a.push(this.renderExtraTag("keep-parity")),a.push(this.renderExtraTag("top-space",o.top_offset))),a.push(n),o.bottom_offset&&a.push(this.renderExtraTag("bottom-space",o.bottom_offset)),l.clusterWillChange&&l.clusterWillChange(),this.html(a.join("")),"ol"==this.options.content_tag&&this.content_elem.setAttribute("start",o.rows_above),this.content_elem.style["counter-increment"]="clusterize-counter "+(o.rows_above-1),l.clusterChanged&&l.clusterChanged()):i&&(this.content_elem.lastChild.style.height=o.bottom_offset+"px")},html:function(t){this.content_elem.innerHTML=t},getChildNodes:function(t){for(var e=t.children,o=[],n=0,r=e.length;n<r;n++)o.push(e[n]);return o},checkChanges:function(t,e,o){var n=e!=o[t];return o[t]=e,n}},s});</script>

<script>

let keyReg = 0;
let keyStatusReg = 0;

let kbdReg = 0;
let kbdStatusReg = 0;


NOP  = 0;
STA  = 4;
STS  = 5;
LDA  = 8;
LDS  = 9;
ADD  = 12;
ADC  = 13;
SUB  = 14;
SBC  = 15;
OR   = 16;
XOR  = 17;
AND  = 20;
NOT  = 24;
SHL  = 28;
SHR  = 29;
SRA  = 30;
JMP  = 32;
JN   = 36;
JP   = 37;
JZ   = 40;
JNZ  = 41;
JC   = 44;
JNC  = 45;
IN   = 48;
OUT  = 49;
JSR  = 52;
RET  = 54;
PUSH = 56;
POP  = 57;
TRAP = 60;
HLT  = 63;

ORG  = 100;
END  = 101;
DS   = 102;
DB   = 103;
DW   = 104;
STR  = 105;
EQU  = 106;
LDI  = 107;

VAZIA = 254;
INVALIDA = 255;


let nome_instrucao = [];
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


NADA        = 0;
DIRETO      = 1;
INDIRETO    = 2;
IMEDIATO    = 4;
IMEDIATO_16 = 8;
VALOR       = 16;
LISTA       = 32;
CADEIA      = 64;

  
let maior = 0;
let menor = 65536;

let memoria = [];

let disas_ativo = "";
let breakpoint_ultimo = "";
let breakpoint = -1;


let memoria_dados = [];

let clusterize;

let PC = 0;

document.addEventListener("DOMContentLoaded", function(event) {

  function gera_linha(linha) {
  
      let p = linha*8; // posicao inicial
      let numlinha = ("0000" + p.toString(16).toUpperCase()).substr(-4);
  
      return '<div><span class="num_linha">' + numlinha + ':</span>' +
      '<span id="pos' + (p)   + '">' + pega_memoria(p)   + '</span>' +
      '<span id="pos' + (p+1) + '">' + pega_memoria(p+1) + '</span> ' +
      '<span id="pos' + (p+2) + '">' + pega_memoria(p+2) + '</span>' +
      '<span id="pos' + (p+3) + '">' + pega_memoria(p+3) + '</span> ' +
      '<span id="pos' + (p+4) + '">' + pega_memoria(p+4) + '</span>' +
      '<span id="pos' + (p+5) + '">' + pega_memoria(p+5) + '</span> ' +
      '<span id="pos' + (p+6) + '">' + pega_memoria(p+6) + '</span>' +
      '<span id="pos' + (p+7) + '">' + pega_memoria(p+7) + '</span></div>';
  }

  function cria_memoria() {
  
    let memoria = document.querySelector("#memoria_conteudo");
    memoria.innerHTML = "";
    
    let mem_tamanho = 64*1024;
    
    for(let i = 0; i < mem_tamanho/8; i++) {
      memoria_dados.push(gera_linha(i));
    }
    
    clusterize = new Clusterize({
      rows: memoria_dados,
      scrollId: 'memoria',
      contentId: 'memoria_conteudo'
    });
  }

  cria_memoria();
  
  let host = location.hostname;
  if(host === "") host = "localhost";
  
  let connection = new WebSocket('ws://'+host+':81/', ['arduino']);
  
  connection.onopen = function () {
    connection.send('[WebSocket] Conectado! ' + new Date());
  };
  
  connection.onerror = function (error) {
    console.log('[WebSocket] Não foi possível se conectar! ', error);
  };
  
  connection.onmessage = function (e) {
    console.log('[WebSocket] Mensagem: ', e.data);
    
    let dados = JSON.parse(e.data);
    
    if(dados.hasOwnProperty("memoria"))
      atualiza_memoria(dados);
    else if(dados.hasOwnProperty("io"))
      atualiza_io(dados);
  };
  
  
  function pega_memoria(endereco) {
    if(memoria[endereco] == undefined)
      return "00";

    else
      return (memoria[endereco] <= 0xF ? "0" : "") + memoria[endereco].toString(16).toUpperCase();
  }
  
  function pega_memoria_int(endereco) {
    return memoria[endereco] == undefined ? 0 : memoria[endereco];
  }
  
  
  function disas() {
  
    if(memoria.length == 0) return;
  
    document.querySelector("#codigo").innerHTML = "";
    document.querySelector("#disas_msg").innerHTML = "(Clique para adicionar um breakpoint)";
  
    while(PC <= maior) {
      
      let opcode = memoria[PC] >> 2;
      let modo_enderecamento = 1 << (memoria[PC] & 3);
      
      let nome = nome_instrucao[opcode];
      
      let sem_parametros = [NOP, NOT, SHL, SHR, SRA, RET, PUSH, POP, HLT];
      let param = !sem_parametros.includes(opcode);
      
      let op_log = '';
      
      let operando;
      let pos = PC;
      let cod = pega_memoria(PC) + " ";
      
      PC++;
      
      /* se o opcode tem parametros (operando) */
      if(param) {
        switch(modo_enderecamento) {
          case DIRETO: /* operando de 16 bits */
          op_log = parseInt(pega_memoria(PC+1) + pega_memoria(PC), 16);
          cod += pega_memoria(PC) + " " + pega_memoria(PC+1);
          PC += 2;
          break;
          
          case INDIRETO: /* operando de 16 bits */
          op_log = '@' + parseInt(pega_memoria(PC+1) + pega_memoria(PC), 16);
          cod += pega_memoria(PC) + " " + pega_memoria(PC+1);
          PC += 2;
          break;
          
          case IMEDIATO: /* operando de 8 bits */
            op_log = '#' + parseInt(pega_memoria(PC), 16);
            cod += pega_memoria(PC) + " \u0020 ";
          PC += 1;
          break;
          
          case IMEDIATO_16: /* operando de 16 bits */
          op_log = '#' + parseInt(pega_memoria(PC+1) + pega_memoria(PC), 16);
          cod += pega_memoria(PC) + " " + pega_memoria(PC+1);
          PC += 2;
          break;
          
          default:
            break;
        }
      }
      else
        cod += " \u0020 \u0020 ";
      
      let posHex4casas = (pos).toString(16).toUpperCase() + "";
      while(posHex4casas.length < 4) posHex4casas = "0" + posHex4casas;
      
      let filho = document.createElement("div");
      filho.id = "disas_pos" + pos;
      filho.dataset.posicao = pos;
      filho.innerHTML = posHex4casas + " \u0020 \u0020" + cod + " \u0020 \u0020" + nome + " " + op_log;
      filho.addEventListener("click", function() {
        let pos = parseInt(this.dataset.posicao, 10);
        
        if(breakpoint != -1)
          document.querySelector("#disas_pos" + breakpoint).classList.remove("breakpoint");
        
        /* clicando no mesmo = removendo breakpoint */
        if(pos == breakpoint) {
          breakpoint = -1;
          envia_bp(false);
        }
        
        /* adicionando */
        else {
          this.classList.add("breakpoint");
          breakpoint = pos;
          envia_bp(true);
        }
      });
      
      document.querySelector("#codigo").appendChild(filho);
      
      if(opcode == HLT) break;
    }
  }
  
  
  
  function atualiza_memoria(dados) {
  
    /* limpar memoria, começar do zero e rodar o disassembler no final*/
    if(dados.hasOwnProperty("limpar")) {
      memoria = [];
      memoria_dados = [];
      clusterize.clear();
      clusterize.destroy(true);
      cria_memoria();
      menor = 65536;
      maior = 0;
      PC = 0;
      
      if(dados.hasOwnProperty("breakpoint"))
        breakpoint = parseInt(dados["breakpoint"], 10);
    }
  
    let mem = dados["memoria"];
    let reg = dados["registradores"];
    
    PC = parseInt(reg["pc"], 16);
    
    for(el in reg) document.getElementById(el).innerHTML = reg[el].toUpperCase();
    for(el in mem) {
      
      if(document.getElementById("pos" + el) != null)
        document.getElementById("pos" + el).innerHTML = mem[el].toUpperCase();

      let posicao = parseInt(el, 10);
      let conteudo = parseInt(mem[el], 16);
      
      memoria[posicao] = conteudo;
      
      // atualizando linhas do scroll de memoria do Clusterize
      let linha_pos = Math.floor(posicao/8);
      memoria_dados[linha_pos] = gera_linha(linha_pos);
      
      if(posicao > maior) maior = posicao;
      if(posicao < menor) menor = posicao;
    }
    
    if(dados.hasOwnProperty("limpar")) {
      disas();
    }
    
    if(disas_ativo != "" && document.querySelector(disas_ativo) != null)
      document.querySelector(disas_ativo).classList.remove("ativo");
    
    disas_ativo = "#disas_pos" + parseInt(reg["pc"], 16);
    
    if(document.querySelector(disas_ativo) == null)
      console.log(disas_ativo + " é nulo");
    else 
      document.querySelector(disas_ativo).classList.add("ativo");
    
    
    if(breakpoint != -1)
      document.querySelector("#disas_pos" + breakpoint).classList.add("breakpoint");
  }
  
  
  function atualiza_io(dados) {
    let io = dados["io"];
    
    
    if(io.hasOwnProperty("keyReg"))
      keyReg = parseInt(io["keyReg"], 10);
      
    keyStatusReg = parseInt(io["keyStatusReg"], 10);
    kbdReg = parseInt(io["kbdReg"], 10);
    kbdStatusReg = parseInt(io["kbdStatusReg"], 10);
    
    if(io.hasOwnProperty("keyReg")) {
      document.querySelector("#painel").value = keyReg;
      
      let chaves = document.querySelectorAll(".switch");
      let num = keyReg;
      for(let i = 7; i >= 0; i--) {
        if(num & 1) chaves[i].className = "switch on";
        else chaves[i].className = "switch";
        num >>= 1;
      }
    }
    
    document.querySelector("#painel_pronto").innerHTML = keyStatusReg ? "Sim" : "Não";
    document.querySelector("#keypad_pronto").innerHTML = kbdStatusReg ? "Sim" : "Não";
    document.querySelector("#visor").innerHTML = io["displayReg"].toUpperCase();
    document.querySelector("#banner").innerHTML = io["bannerReg"];
    
  }
  
  
  document.querySelector("#form_arquivo").onsubmit = function() {
        let file = document.getElementById('arquivo').files[0];
        let reader = new FileReader();

        reader.onload = function(e) {
            let conteudo = reader.result;
            connection.send('envia,' + conteudo);
        };

        reader.readAsText(file);

    return false;
  };
  
  
  let botoes = document.querySelectorAll("#acoes button");
  for(let i = 0; i < botoes.length; i++) {
    botoes[i].addEventListener("click", function() {
      let caminho = this.id;
      
      connection.send(caminho);
    });
  }
  
  
  function envia_io() {
    
    let bytearray = new Uint8Array(8);
    bytearray[0] = "i".charCodeAt(0);
    bytearray[1] = "o".charCodeAt(0);
    bytearray[2] = ",".charCodeAt(0);
    bytearray[3] = keyReg;
    bytearray[4] = keyStatusReg;
    bytearray[5] = (kbdReg.length >= 0) ? kbdReg.charCodeAt(0) : 0;
    bytearray[6] = kbdStatusReg;
    bytearray[7] = 0;
    
    console.log("Enviarei: " + bytearray);
    connection.send(bytearray);
  }
  
  function envia_bp(tem_bp) {
    let bytearray = new Uint8Array(6);
    bytearray[0] = "b".charCodeAt(0);
    bytearray[1] = "p".charCodeAt(0);
    bytearray[2] = tem_bp ? ",".charCodeAt(0) : "n".charCodeAt(0);
    bytearray[3] = breakpoint >> 8;
    bytearray[4] = breakpoint & 0xFF;
    bytearray[5] = 0;
    
    console.log("Enviarei: " + bytearray);
    connection.send(bytearray);
  }
  
  
  document.querySelector("#painel").addEventListener("keyup", function() {
    let valor = parseInt(this.value, 10);
    if(isNaN(valor)) valor = -1;
    else if(valor < 0) valor = 0;
    else if(valor > 255) valor = 255;
    
    this.value = valor == -1 ? "" : valor;
    
    keyReg = valor;

    envia_io();
  });
  
  document.querySelector("#form_painel").addEventListener("submit", function(e) {
    e.preventDefault();
    keyStatusReg = 1;
    document.querySelector("#painel_pronto").innerHTML = "Sim";
    
    envia_io();
    
    return false;
  });
  
  
  let teclas = document.querySelectorAll("#keypad button");
  for(let i = 0; i < teclas.length; i++) {
    teclas[i].addEventListener("click", function() {
      let tecla = this.innerHTML;
      kbdReg = tecla;
      kbdStatusReg = 1;
      document.querySelector("#keypad_pronto").innerHTML = "Sim";
      
      envia_io();
    });
  }
  
  let chaves = document.querySelectorAll(".switch");
  for(let i = 0; i < chaves.length; i++) {
    chaves[i].addEventListener("click", function() {
      if(this.className != "switch") // ligado
        this.className = "switch";
      else 
        this.className = "switch on";
      
      let num = 0;
      let chaves = document.querySelectorAll(".switch");
      chaves.forEach(el => {
        if(el.className != "switch")
          num += 1 << el.getAttribute("data-bit");
      });
      
      document.querySelector("#painel").value = num;
      
      keyReg = num;

      envia_io();
    });
  }
  
  document.querySelector("#painel").value = "0";
  
  
  
  /*
  let json2 = {
    "limpar": "sim",
    
    "registradores": {
    "pc": "0000",
    "acc": "BB",
    "sp": "00CC",
    "n": "0",
    "z": "1",
    "c": "0"
    },

    "memoria": {
    "0": "C2",
    "1": "01",
    "2": "52",
    "3": "01",
    "4": "A0",
    "7": "C2",
    "9": "C6",
    "11": "80"
    }
  };
  
  setTimeout( function() { atualiza_memoria(json2); }, 2000);
  */
  
});
</script>
<style>
body {
  font: 120% sans-serif;
}

#memoria {
  padding: 0.5em 0.5em 0.5em 0;
  font: 130% monospace;
  display: inline-block;
  width: auto;
  height: calc(100vh - 12em);
  overflow-y: scroll;
  background: #f4f4f4;
  margin-top: 30px;
  border-radius: 5px;
  border: 1px solid #ccc;
}

.brilha {
  background: yellow;
}
.num_linha {
  background: #ddd;
  margin-right: 0.7em;
  padding: 0 0.3em 0 0.7em;

}
#memoria_conteudo {
  outline: none;
}

#memoria_conteudo > div:first-of-type > span:first-of-type {
  padding-top: 0.5em;
}


fieldset {
  border: 1px solid #ccc;
  background: #eee;
  padding: 0.6em 1.2em;
  width: auto;
  display: inline-block;
  border-radius: 5px;
  line-height: 160%;

}

#form_arquivo {
  border: 1px solid #ccc;
  background: #eee;
  padding: 0.6em 1.2em;
  margin: 0.6em;
  width: auto;
  border-radius: 5px;
  line-height: 160%;
  font-size: 90%;
}
#form_arquivo input[type="file"] {
  background: #fff;
  border: 1px solid #ccc;
  margin: 0.8em 0;
  border-radius: 5px;
}
.reg {
  border: 1px solid #ccc;
  background: #eee;
  padding: 7px;
  margin: 0.5em 1em 0 0;
  display: inline-block;
  border-radius: 5px;
}

.reg.flag {
}

.reg span {
  font: 120% monospace;
  background: #fff;
  padding: 0.1em 0.3em;
  margin-left: 0.1em;
}

input#arquivo {
  width: 100%;
}

#registradores {
  display: inline-block;
}

.left {
  float: left;
}
.right {
  float: right;
}

.clear {
  clear: both;
}

#esq {
  float: left;
}
#dir {
  float: left;
  margin-left: 2em;
}
.text_right {
  text-align: right;
}

#disassembly {
  min-width: 15em;
}

#acoes, #io, #disassembly {
  border: 1px solid #ccc;
  background: #eee;
  padding: 0.6em 1.2em;
  margin: 0.6em;
  line-height: 160%;
  border-radius: 5px;
  line-height: 160%;
  font-size: 90%;
}

#io, #disassembly {
  margin-top: 2em;
}



#acoes .botoes {
  font-size: 75%;
}

#acoes .botao {
  margin: 0.5em 0.3em 0.5em 0;

  text-align: center;
  position: relative;
  display: inline-block;
}

#acoes button {
  padding: 0;
  vertical-align: middle;
  width: 2.2em;
  height: 2.2em;

}

#acoes .botao div {
  background-size: 50% auto;
  background-repeat: no-repeat;
  background-position: center;
  width: 100%;
  height: 100%;
  position: absolute;
  top: 0;
  left: 0;
  pointer-events:none;
}

#io .interno {
  font-size: 80%;
  
}

.play {
  background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='357' height='357' viewBox='0 0 357 357'%3E%3Cpath d='M38.25 0v357l280.5-178.5'/%3E%3C/svg%3E");
}

.pause {
  background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='357' height='357' viewBox='0 0 357 357'%3E%3Cpath d='M25.5 357h102V0h-102v357zm204-357v357h102V0h-102z'/%3E%3C/svg%3E");
}

.stop {
  background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='306' height='306' viewBox='0 0 306 306'%3E%3Cpath d='M0 0h306v306H0z'/%3E%3C/svg%3E");
}

.step {
  background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='306' height='306' viewBox='0 0 306 306'%3E%3Cpath d='M0 306l216.75-153L0 0v306zM255 0v306h51V0h-51z'/%3E%3C/svg%3E");
}


#banner {
  font-family: monospace;
  background: black;
  font-size: 140%;
  color: #00ff00;
  white-space: pre;
}

#visor {
  font-family: monospace;
  background: black;
  font-size: 300%;
  line-height: 100% !important;
  color: #00ff00;
  white-space: pre;
}

.largo {
  display: block;
}

fieldset.alto {
  /* height: 6em; */
  float: left;
}

fieldset {
  margin-top: 10px;
  margin-bottom: 10px;
}

#keypad button {
  border-width: 3px;
  font-family: monospace;
}

#disassembly #codigo{
  margin-left: -1em;
}

#disassembly #codigo div {
  font-family: monospace;
  white-space: pre;
  padding: 0 2em;
}
#disassembly #codigo div.ativo {
  background-image: linear-gradient(90deg, transparent 1.2em, #bcd0e1 1em);
}

#disassembly #codigo div.breakpoint {
  background: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='510' height='510'%3E%3Cpath d='M255 0C114.75 0 0 114.75 0 255s114.75 255 255 255 255-114.75 255-255S395.25 0 255 0z'/%3E%3C/svg%3E") 4px center / 8px 8px no-repeat;
  /* image, position, / size, repeat */
}

#disassembly #codigo div.ativo.breakpoint {
  background: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='510' height='510'%3E%3Cpath d='M255 0C114.75 0 0 114.75 0 255s114.75 255 255 255 255-114.75 255-255S395.25 0 255 0z'/%3E%3C/svg%3E") 4px center / 8px 8px no-repeat, linear-gradient(90deg, transparent 1.2em, #bcd0e1 1em) !important;
}

#disassembly #codigo {
  margin-top: 0.5em;
}

#disassembly #codigo div:hover {
  cursor: pointer;
}

.switch {
  background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAC8AAABKCAMAAAAcwP13AAAAMFBMVEVHcEwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAlTPQ5AAAAD3RSTlMAHcVYUeGPgA1t8tQ1rLjsQDWmAAABRElEQVRIx+2X2RKDIAxFjSAEUPn/v20rKmASt4d2OuN9zRkDNwtt0/y/UIsCBu99lOUoH3bw2FE+7upHPGMEPPzDP/zDf4NvGd3ab/1FPlzko73Ie+D5oIjMFBiF72vp6VE8H5A+belNQv78liYYpoAR7ku9Rk8DmW9pAkeLkM4ovbYtCSRzeK+XK5eB1J/J64EmIIHEYycUgQTm/tec1x+pTWCZF87rSWMdWHjwQhGgrs46j04qgq0CeX6DUASsqpOvP3tNJznxHov6Yvb64MdWzNeHfX4s++1zfb3Pm5JvVyNEqaqf3dyMIx1kZYslmBcBcuOaxyBs5mXQ0ipNvWLIfPEL7S1fJGZN4IYGWF4fHH/LYyN0tGF5+fi62gCrqx2jKjGEeEZFYpikzjQPHTtJWphrQcxmQmdNIWvsIuf6m/+NXna7S/rJw+qhAAAAAElFTkSuQmCC") center center / 100% 100% no-repeat;
  width: 24px;
  height: 37px;
  float: left;
  margin-bottom: 10px;
}

.switch.on {
  background: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAC8AAABKCAMAAAAcwP13AAAAMFBMVEVHcEwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAlTPQ5AAAAD3RSTlMAHcVYUeGPgA1t8jXUrLgqo6lsAAABR0lEQVRIx+2WSRKDIBBFQZDR4f63TcLYQDdaWWSR8q8s/xPsiYKx72WsVkBa6SxrPcIf50T7gPNzKgnIIDHnVaG38462zC/Na7ciCo5JPLEMlIMBdPsaKnMK5yXCW7gzlQcgHxze88s8ANHxu4TLNNrBziWT3JxgGTKAzNtUikOM0mDn/PPvRz0vsYD8p2PlnRYCzxctelR+DWVVc95WXtayuGVQbDhnKh9C38AqzaCu0Ci8LXnqpBsj89yVPKGD6jt+p9rtaI3ES5CnRqIzIp9iklSw1Yi8Ik4ZNhjx89SixKhAI/4ekfp0ekADzBfR+O3MVR5JPVKTgmtiDruaZBxJvceMycGzYQOdD8JxahWYEixeVENNLvghCzeP/Zu8x/kFUTAYziOHJn/4h3/4h/8FT+r3/PR6viKXV3d5pepuCJIUZ3+mFwbFS+27F3vBAAAAAElFTkSuQmCC") center center / 100% 100% no-repeat;
}

#painel {
  
}
</style>
</head>

<body>

<div id="esq">
  <form action="envia" method="post" id="form_arquivo" enctype="multipart/form-data">
    Envie o código:<br>
    <input type="file" name="file1" id="arquivo"><br>
    <div class="text_right"><button type="submit">Enviar</button></div>
  </form>
  
  <div id="io">
    Dispositivos de E/S:
    
    <div class="interno">
      <fieldset class="alto">
        <legend>Painel de chaves</legend>
        <form id="form_painel">
          <div class="switch" data-bit="7"></div>
          <div class="switch" data-bit="6"></div>
          <div class="switch" data-bit="5"></div>
          <div class="switch" data-bit="4"></div>
          <div class="switch" data-bit="3"></div>
          <div class="switch" data-bit="2"></div>
          <div class="switch" data-bit="1"></div>
          <div class="switch" data-bit="0"></div>
          
          <div style="clear: both;"></div>
          
          Valor: <input size="2" maxlength="3" id="painel" value="0" readonly>
          
          <button type="submit">Entrar</button>
          
          <br>
          
          <small>Pronto: <span id="painel_pronto">Não</span></small>
        </form>
      </fieldset>
      
      <fieldset class="alto">
        <legend>Visor</legend>
        <div id="visor">00</div>
      </fieldset>
      
      <div class="clear"></div>
      
      <fieldset id="keypad">
        <legend>Keypad</legend>
        <button>1</button><button>2</button><button>3</button><br>
        <button>4</button><button>5</button><button>6</button><br>
        <button>7</button><button>8</button><button>9</button><br>
        <button>*</button><button>0</button><button>#</button><br>
        <small>Pronto: <span id="keypad_pronto">Não</span></small>
      </fieldset>
      <br>

      <fieldset class="largo">
        <legend>Banner</legend>
        <div id="banner"> </div>
      </fieldset>
      
    </div>
    <div class="clear"></div>
  </div>
</div>


<div id="dir">

  <div id="registradores">
    <div class="left">
      <div class="reg">PC <span id="pc">0000</span></div><br>
      <div class="reg">ACC <span id="acc">00</span></div><br>
      <div class="reg">SP <span id="sp">0000</span></div>
    </div>
    <div class="left">
      <div class="reg">N <span id="n">0</span></div><br>
      <div class="reg">Z <span id="z">0</span></div><br>
      <div class="reg">C <span id="c">0</span></div>
    </div>
    <div class="clear"></div>
  </div>

  <br>

  <div id="memoria">
  <div id="memoria_conteudo"></div>
  </div>

</div>

<div id="dir">
  <div id="acoes">
    Ações:<br>
    <div class="botoes">
      <div class="botao">
        <button id="play"></button>
        <div class="play"></div>
      </div>
      Executar<br>
      
      <div class="botao">
        <button id="step"></button>
        <div class="step"></div>
      </div>
      Passo a passo<br>
      
      <div class="botao">
        <button id="pause"></button>
        <div class="pause"></div>
      </div>
      Parar<br>
      
      <div class="botao">
        <button id="stop"></button>
        <div class="stop"></div>
      </div>
      Reset<br>
      
    </div>
    <div class="clear"></div>
  </div>
  
  <div id="disassembly">
    Disassembly:<br>
    <span id="disas_msg" style="font-size: 75%;">Envie um arquivo.</span>
  
    <div id="codigo">
    </div>
  </div>
</div>

</body></html>

)=====";
