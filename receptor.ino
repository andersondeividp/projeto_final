/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

unsigned char frequency = 0x1;

RF24 radio(8,7); // CE, CSN

void setup() {  
  Serial.begin(9600);
  radio.begin();    
  radio.openReadingPipe(0, frequency);  
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  
  Serial.println("Waiting...");
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (text[0] != '\0'){
      montarMensagem(text);
      Serial.println(text);                
    }
  }  
}

char montarMensagem(char text[]){
  char v[32] = "";
  char s[32] = "";

  int i,np = 0;
  for(i = 0; i < strlen(text); i++){
    if(text[i] == '.' ){
      np++;
      continue;
    }      
    if(np == 0)
      continue;
    else if(np == 1)
      append(v,text[i]);      
    else
      append(s,text[i]);        
  }      
  if(strcmp(s,"L") == 0)
    sprintf(text,"VAGA %s LIVRE",v);
  else if(strcmp(s,"O") == 0)
    sprintf(text,"VAGA %s OCUPADA",v);
  else if(strcmp(s,"A") == 0)
    sprintf(text,"VAGA %s AUTORIZADA",v);  
}



void append(char* s, char c)
{
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
}
