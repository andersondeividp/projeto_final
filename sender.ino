
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9

const int TRIG_PIN = 5;
const int ECHO_PIN = 4;
const int LED_PIN_GREEN = 6;
const int LED_PIN_YELLOW = 2;
const int LED_PIN_RED = 3;


long t_base;
long t_base_min;
long t_max;
long t_min;
long v_ocupada;
long v_identificada;

long prev_milles;
long intervalo;
int v_led_red;

RF24 radio(8,7);  // CE, CSN

unsigned char frequency = 0x1;
unsigned int no = 2 ;
unsigned int perimetro = 2;


MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);  
  initSensor();
  initLeds();
  initRadio();
  t_max = 2000;
  t_min = 500;
  t_base = 0;
  t_base_min = 0;
  v_ocupada = 0;  
  v_identificada = 0;

  SPI.begin();  
  mfrc522.PCD_Init();

  prev_milles = 0;
  intervalo = 500;
  v_led_red = 0;  
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
    
  if (mfrc522.PICC_IsNewCardPresent()){       
     Serial.println("IN...");
    if(v_identificada == 0){
      v_identificada = v_ocupada;
      if(v_identificada == 1)
        enviarMensagemVagaAutorizada();          
    }       
  } 
  
  int distance=  ();
  if(distance > 10){   
    long mi = millis();
    if(t_base > 0 && mi < t_base + t_max && mi - t_base_min> t_min){
      ascenderLuzAmarela();
    }else{
      ascenderLuzVerde();
      if(t_base != 0){
        v_ocupada = 0;
        v_identificada = 0;
        enviarMensagemVagaLivre();
      }
      t_base = 0;  
      t_base_min = 0;           
    }     
  }else{  
    t_base = millis();
    ascenderLuzVermelha();    
    if(t_base_min == 0){
      t_base_min = t_base;      
      if(v_ocupada == 0){
        v_ocupada = 1;
        enviarMensagemVagaOcupada();           
      }      
    }  
  }   
}

void initSensor(){
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

int getDistance(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration*0.034/2;
}

void initLeds(){
  pinMode(LED_PIN_RED,OUTPUT);
  pinMode(LED_PIN_GREEN,OUTPUT);
  pinMode(LED_PIN_YELLOW,OUTPUT);
}

void ascenderLuzAmarela(){
  digitalWrite(LED_PIN_YELLOW,HIGH);
  digitalWrite(LED_PIN_RED,LOW);
  digitalWrite(LED_PIN_GREEN,LOW);
}

void ascenderLuzVermelha(){
  digitalWrite(LED_PIN_YELLOW,LOW);  
  digitalWrite(LED_PIN_GREEN,LOW);
  long m = millis();   
  if(v_identificada == 0){    
    if(m - prev_milles > intervalo){
      prev_milles = m;
      if(v_led_red == 0){        
        v_led_red = 1;
        digitalWrite(LED_PIN_RED,HIGH);
      }else{        
        v_led_red = 0;
        digitalWrite(LED_PIN_RED,LOW);
      }
    }
  }else{
    v_led_red = 1;
    digitalWrite(LED_PIN_RED,HIGH);
  }     
}

void ascenderLuzVerde(){
  digitalWrite(LED_PIN_GREEN,HIGH);
  digitalWrite(LED_PIN_RED,LOW);
  digitalWrite(LED_PIN_YELLOW,LOW);    
}


void montarMensagem(char msg[]){
    char text[32] = ".";  
    char strNo[32] = "";
    itoa(no,strNo,32);    
    strcat(text,strNo);    
    strcat(text,".");    
    strcat(text,msg);
    strcpy(msg,text);
}

void enviarMensagemVagaLivre(){
  char text[32] = "L";
  montarMensagem(text);
  adicionarPerimetroMensagem(text);  
  enviarMensagem(text);  
}

void enviarMensagemVagaOcupada(){  
  char text[32] = "O";
  montarMensagem(text);
  adicionarPerimetroMensagem(text);  
  enviarMensagem(text);
}

void enviarMensagemVagaAutorizada(){
  char text[32] = "A"; 
  montarMensagem(text);
  adicionarPerimetroMensagem(text);  
  enviarMensagem(text);     
}

void initRadio(){  
  radio.begin();  
  radio.openReadingPipe(0, frequency);
  radio.openWritingPipe(frequency);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();  
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

