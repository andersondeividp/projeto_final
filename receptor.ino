#include<VirtualWire.h>

String a;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vw_set_rx_pin(7);
  vw_setup(2000);
  vw_rx_start();      
  Serial.println("\nAguardando comunicacao...");  
}

void loop() {
  uint8_t message[VW_MAX_MESSAGE_LEN];    
  uint8_t msgLength = VW_MAX_MESSAGE_LEN;
   if (vw_get_message(message, &msgLength)){              
      for (int i = 0; i < msgLength; i++){          
         Serial.write(message[i]);              
      }
      Serial.println();
   }
    
  /*
  if(Serial.available()>0){
    a = Serial.readString();
    Serial.println(a);
  }
  */
}
