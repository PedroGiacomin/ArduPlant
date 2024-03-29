#include <virtuabotixRTC.h>

//Pinos do modulo clock
#define CLK 4
#define DAT 3
#define RST 2

#define RELE 7
#define BOTAO 8

// Objeto do Real Time Clock (RTC)
virtuabotixRTC myRTC(CLK, DAT, RST);

//-----VARIAVEIS DE DADOS-----//
//Guardam o momento de ligar a bomba
int hora_acionamento = 19;
int min_acionamento = 25;

//-----VARIAVEIS DE CONTROLE-----// 
//Guarda o estado do botao
bool pressionado = 0;

//Guarda simplesmente se a bomba ligou 
bool bomba_ja_ligou = 0;

void setup()  {
  //MONITOR
  Serial.begin(9600);

  //RELE
  //O rele liga no LOW
  pinMode(RELE, OUTPUT);
  //BOTAO
  pinMode(BOTAO, INPUT);
  
  //CLOCK
  //SETUP INICIAL: Carrega data atual -> Comenta a linha abaixo -> Carrega de novo
  // segundos, minutos, horas, dia da semana, dia do mês, mês, ano
  //myRTC.setDS1302Time(0, 23, 19, 3, 24, 8, 2021);
}

/*
 * De maneira geral, o codigo faz a bomba ligar quando o botao eh pressionado
 * OU quando chega na hora pre setada para ele ligar. A ideia eh que essa hora 
 * seja recebida por meio do app com o modulo wifi e fique salva na memoria do 
 * arduino.
*/

void loop()  {
  //Funcao necessaria do relogio
  myRTC.updateTime();
  printClock(); //no monitor serial
  //Serial.print("Pressionado = ");
  //Serial.print(pressionado);
  //Serial.print("\n");
  digitalWrite(RELE, HIGH);

  //Liga o rele se o botao for pressionado 
  pressionado = digitalRead(BOTAO);
  //Aciona o RELE por 30 s no pressionar do botao 
  if(pressionado == 1){
    ligaRele();
  }
  //Se nao estiver pressionado
  else{
     //Liga o rele se chegar a hora
    if(myRTC.hours == hora_acionamento && myRTC.minutes == min_acionamento && bomba_ja_ligou == 0){ 
      ligaRele();
      bomba_ja_ligou = 1;
    }
    
    if(myRTC.minutes != min_acionamento)
      bomba_ja_ligou = 0;
  }

}

//-----FUNCOES-----//

void ligaRele(){
//Aciona o RELE por 30 s 
    digitalWrite(RELE, LOW);
    //Nao sei pq mas o rele esta invertido, liga no LOW
    delay(30000);
    digitalWrite(RELE, HIGH);
}

void printClock(){
   //Printa a data e hora do clock no monitor serial 
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print("  ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
  //Delay entre um print e outro
  delay(1000);
}
