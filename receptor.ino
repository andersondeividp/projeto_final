#include<VirtualWire.h>

#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

String a;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //vw_set_rx_pin(7);
  //vw_setup(2000);
  //vw_rx_start();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();      
  Serial.println("\nAguardando comunicacao...");  
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  /*
  uint8_t message[VW_MAX_MESSAGE_LEN];    
  uint8_t msgLength = VW_MAX_MESSAGE_LEN;
   if (vw_get_message(message, &msgLength)){              
      for (int i = 0; i < msgLength; i++){          
         Serial.write(message[i]);              
      }
      Serial.println();
   }
   */
}

