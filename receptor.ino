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

unsigned int no = 1 ;
unsigned int perimetro = 1;
const int b_time = 5000; //milleseconds

/*
unsigned int no = 2;
unsigned int perimetro = 2;
const int b_time = 10000; //milleseconds
*/

int aux_time = 0;

RF24 radio(8,7); // CE, CSN

void setup() {  
  Serial.begin(9600);
  radio.begin();  
  radio.openReadingPipe(0, frequency);
  radio.openWritingPipe(frequency);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if (text[0] != '\0'){      
      if(acceptMessage(text) == 1){        
        modificarPerimetroMensagem(text);
        enviarMensagem(text); 
      }              
    }
  }
  int t = millis();
  if(t - aux_time > b_time){
    aux_time = t;
    char text[32] = ".";  
    char strNo[32] = "";
    itoa(no,strNo,32);    
    strcat(text,strNo);    
    strcat(text,".");    
    strcat(text,"L");    
    adicionarPerimetroMensagem(text);    
    enviarMensagem(text);
  }
}

void enviarMensagem(char text[]){  
  char textFlush[32] = "HELLO WORLD";
  strcpy(textFlush,text);
  radio.stopListening();
  radio.write(&textFlush, sizeof(textFlush));   
  radio.startListening();   
}


int acceptMessage(char text[]){
  char *token = strtok(text,".");
  int perimetroMensagem = atoi(token);
  return perimetroMensagem >  perimetro;
}

void modificarPerimetroMensagem(char text[]){  
  char *token = strchr(text, '.');
  strcpy(text,token);
  adicionarPerimetroMensagem(text);
}

void adicionarPerimetroMensagem(char text[]){
  char strPerimetro[32] = "";  
  itoa(perimetro,strPerimetro,32);  
  strcat(strPerimetro,text);
  strcpy(text,strPerimetro);
}
