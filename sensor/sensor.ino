#include <DHT11.h>
#include <stdio.h>
#include <SoftwareSerial.h>

// 블루투스 설정 BTSerial(Tx, Rx)
#define BT_RXD 13
#define BT_TXD 12
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
char str[1024];


//센서

// 온습도 센서 설정 (SETUP 필요없음)
#define PIN_TEMHUM A0
DHT11 dht11(PIN_TEMHUM);

//가스 센서
#define PIN_GAS A2

//불꽃감지 센서
#define PIN_FLAME A4

//초음파 센서
#define PIN_TRIG 6
#define PIN_ECHO 7

//사람 인식 센서
#define PIN_PERSON 3





void setup() {
  Serial.begin(9600);          // 아두이노 보드와 PC간의 통신속도 설정
  bluetooth.begin(9600);       // 블루투스 통신 시작

  pinMode(PIN_GAS , INPUT);

  pinMode(PIN_FLAME, INPUT);   // 불꽃 감지 센서를 입력으로 설정

  pinMode(PIN_TRIG, OUTPUT);  // 초음파센서 trigPin
  pinMode(PIN_ECHO, INPUT);  // 초음파센서 echoPin 입력

  pinMode(PIN_PERSON, INPUT);    // 인체감지센서
}

void loop() {

  int gas;
  gas = analogRead(PIN_GAS);  // 가스센서

  // 불꽃감지센서
  int Fval = analogRead(PIN_FLAME);
  //아날로그 값일때 5~10cm : 25~50 >> 바로 멈추기 / 15~cm : 150~600 >> 속도를 줄이기


  // 초음파센서
  long duration;
  int distance;
  digitalWrite(PIN_TRIG, HIGH); // trigPin에서 초음파 발생(echoPin도 HIGH)
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  duration = pulseIn(PIN_ECHO, HIGH);  // echoPin이 HIGH를 유지한 시간 저장
  distance = ((float)(340 * duration) / 1000) / 2;


  // 인체감지센서(PIR)
  int Pval;
  Pval = digitalRead(PIN_PERSON);

  //온습도
  float humi, temp;
  char tmp1[20];
  char tmp2[20];

  dht11.read(humi, temp);
  dtostrf(humi, 4, 1, tmp1);
  dtostrf(temp, 4, 1, tmp2);


  
  bluetooth.write("s");

  Serial.println("gas fire cho person humidity temperature");
  sprintf(str, "%d,%d,%d,%d,%s,%s\n", gas, Fval, distance, Pval, tmp1, tmp2);
  Serial.println(str);
  //Serial.write(str);

  bluetooth.print(str);
  //bluetooth.write(str);

  bluetooth.write("f");

  Serial.flush();
  bluetooth.flush();
  /*
  bluetooth.write(str);
  bluetooth.write(gas);
  bluetooth.write(Fval);  
  bluetooth.write(distance);  
  bluetooth.write(Pval);  
  bluetooth.write(tmp1);
  bluetooth.write(tmp2);
  */





  delay(1200);

}
