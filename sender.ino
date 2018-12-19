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

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


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

  //SPI.begin();  
  mfrc522.PCD_Init();

  prev_milles = 0;
  intervalo = 500;
  v_led_red = 0;  
}

void loop() {  
  if (mfrc522.PICC_IsNewCardPresent()){        
    if(v_identificada == 0){
      v_identificada = v_ocupada;
      if(v_identificada == 1)
        enviarMensagemVagaAutorizada();          
    }       
  } 
  int distance= getDistance();
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
  //Serial.println(m);
  //Serial.println(prev_milles);
  //Serial.println(intervalo);
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

void enviarMensagemVagaLivre(){
  const char text[] = "vaga 1 livre";
  radio.write(&text, sizeof(text));  
}

void enviarMensagemVagaOcupada(){
  Serial.println("Enviando...");
  const char text[] = "vaga 1 ocupada";
  radio.write(&text, sizeof(text));      
  Serial.println("Fim");
}

void enviarMensagemVagaAutorizada(){
  const char text[] = "vaga 1 autorizada";
  radio.write(&text, sizeof(text));      
}

void initRadio(){
  Serial.println("Comunicando...");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  const char text[] = "Contectado";
  radio.write(&text, sizeof(text));
  Serial.println("Fim");
}

