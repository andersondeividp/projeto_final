#include<VirtualWire.h>

const int trigPin = 4;
const int echoPin = 5;
const int ledPinY = 2;
const int ledPinR = 7;
const int ledPinG = 6;
const int dataT = 8;

long duration;
int distance;

long start_time;
long end_time;

long t_base;
long t_base_min;
long t_max;
long t_min;

char data[40];

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPinY, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  Serial.begin(9600);
  vw_set_tx_pin(dataT);
  vw_setup(2000);
  start_time = 0;
  end_time = 0;
  t_max = 2000;
  t_min = 500;
  t_base = 0;
  t_base_min = 0;
  char *s = "vaga 1 livre";
  send(s);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  //Serial.print("Distance: ");
  //Serial.println(distance);

  if(distance > 10){   
    long mi = millis();
    if(t_base > 0 && mi < t_base + t_max && mi - t_base_min> t_min){
      digitalWrite(ledPinY,HIGH);
      digitalWrite(ledPinR,LOW);
      digitalWrite(ledPinG,LOW);
    }else{
      digitalWrite(ledPinY,LOW);
      digitalWrite(ledPinG,HIGH);
      digitalWrite(ledPinR,LOW);
      if(t_base != 0){
        char *s = "vaga 1 livre";
        send(s);   
      }
      t_base = 0;  
      t_base_min = 0;           
    }     
  }else{  
    digitalWrite(ledPinR,HIGH);
    digitalWrite(ledPinG,LOW);
    digitalWrite(ledPinY,LOW);
    t_base = millis();
    if(t_base_min == 0){
      t_base_min = t_base;
      char *s = "vaga 1 ocupada";
      send(s);
    }  
  }  
}


void send (char *message)
{
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Aguarda o envio de dados
}


