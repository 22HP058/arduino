#include <DHT11.h>
int pin = A0;  // 핀설정
DHT11 dht11(pin);
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  float humi, temp;
  dht11.read(humi, temp); 
  Serial.print("humidity:");         
  Serial.println(humi);                 
  Serial.print("temperature:");      
  Serial.println(temp);                 
} 
