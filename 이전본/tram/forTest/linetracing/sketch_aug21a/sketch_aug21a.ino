#define IN1 11

#define IN2 3

#define IN3 6

#define IN4 5

int leftLineSensor =  8;      // 라인트레이서 왼쪽 센서

int rightLineSensor = 7;      // 라인트레이서 오른쪽 센서



void forward() {      // 전진 : 모터 두 개를 모두 정회전 시킴

      digitalWrite(IN1, HIGH);

      digitalWrite(IN2, LOW);

      digitalWrite(IN3, HIGH);

      digitalWrite(IN4, LOW);    }



void back() {         // 후진 : 모터 두 개를 모두 역회전 시킴

      digitalWrite(IN1, LOW);

      digitalWrite(IN2, HIGH);

      digitalWrite(IN3, LOW);

      digitalWrite(IN4, HIGH);   }



void left() {         // 좌회전 : 오른쪽 모터만 정회전 시킴

      digitalWrite(IN1, LOW);

      digitalWrite(IN2, LOW);

      digitalWrite(IN3, HIGH);

      digitalWrite(IN4, LOW);  }



void right() {      // 우회전 : 왼쪽 모터만 정회전 시킴

      digitalWrite(IN1, HIGH);

      digitalWrite(IN2, LOW);

      digitalWrite(IN3, LOW);

      digitalWrite(IN4, LOW);   }



void stop() {       // 정지 : 2개의 모터 모두 회전 멈춤

      digitalWrite(IN1, LOW);

      digitalWrite(IN2, LOW);

      digitalWrite(IN3, LOW);

      digitalWrite(IN4, LOW);  }



void setup( )  {

  pinMode(IN1, OUTPUT);     // A 모터 1

  pinMode(IN2, OUTPUT);     // A 모터 2

  pinMode(IN3, OUTPUT);     // B 모터 1

  pinMode(IN4, OUTPUT);     // B 모터 2

  pinMode(leftLineSensor, INPUT);

  pinMode(rightLineSensor, INPUT);  

}



void loop( ) {



// 만약 양쪽 센서가 모두 선 ‘미감지’(반사신호’OK’) 경우 : 전진

if (digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {

  stop();

 }  



// 만약 왼쪽 센서만 ‘선감지(반사신호X)’ 경우 :  좌회전

else if (digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {

  left();  

 }   



// 만약 오른쪽 센서만 ‘선감지(반사신호X)’ 경우 :  우회전

else if (!digitalRead(leftLineSensor) && digitalRead(rightLineSensor)) {

  right();

 } 



  // 만약 양쪽 센서 모두 ‘선감지(반사신호X)’ 경우 :  정지

else if (!digitalRead(leftLineSensor) && !digitalRead(rightLineSensor)) {

  forward();  

 } 



}
