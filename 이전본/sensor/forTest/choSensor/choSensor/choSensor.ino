int trig = 6;      //Trig pin
int echo = 7;      //Echo pin

void setup(){
  pinMode(trig,OUTPUT);     //trig 핀 모드 설정
  pinMode(echo,INPUT);      //echo 핀 모드 설정
  Serial.begin(9600);       //시리얼통신 시작, 통신속도 9600       
}

void loop() {
  float Length, distance;   // Lengtg 와 distance 지역변수로 선언

// trigger 신호 발생 (10us)
  digitalWrite(trig, LOW);     //초기화
  delay(2);
  digitalWrite(trig, HIGH);    // trigger 신호 발생 (10us)
  delay(10);
  digitalWrite(trig, LOW);

// Echo 신호 입력
/*pulsein()는 아두이노 기본 함수로 입력신호가 High 또는 Low가 되는 시간을 측정해주는 함수, 
pulseIn 함수를 통해 echo핀에 초음파가 돌아오는 시간을 측정*/
  Length = pulseIn(echo, HIGH);  

// 거리계산
// 거리를 계산하는 공식을 코딩, 10000는 cm로 단위변환을 위해 사용함
  distance = ((float)(340 * Length) / 10000) / 2;  

// Serial 모니터로 출력
  Serial.println(Length);
  Serial.print(distance);
  Serial.println(" Cm");
//시리얼 모니터로 데이터를 보기위해서는 아두이노 창의 맨 오른쪽 돋보기 처럼 생긴 아이콘을 누르시면 새 창이 하나 떠요!

// 1sec마다 출력
  delay(1000);
}
