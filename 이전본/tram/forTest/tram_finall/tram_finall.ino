#include <SoftwareSerial.h>

//motor
#define MOTOR_A_1 3     //모터A의 +출력핀은 3번핀입니다
#define MOTOR_A_2 11    //모터A의 -출력핀은 11번핀입니다
#define MOTOR_B_1 5     //모터B의 +출력핀은 5번핀입니다
#define MOTOR_B_2 6     //모터B의 -출력핀은 6번핀입니다
#define MOTOR_SPEED 60  //모터의 기준속력입니다(0~255), 라인트레이서는 저속주행을 권장드립니다.

#define LINESENS_L 7    //왼쪽 라인센서 입력핀입니다.
#define LINESENS_R 8    //오른쪽 라인센서 입력핀입니다.
int speed_variable = 200;   //모터 속도 제어값 150이상 값 추천

//bluetooth setting
#define BLUETOOTH_RX 13
#define BLUETOOTH_TX 12
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);



void setup() {
  //TCCR1B = TCCR1B & 0b11111000 | 0x05;  //저속주행이 가능하도록 모터A PWM 주파수 변경  
  //TCCR2B = TCCR2B & 0b11111000 | 0x07;  //저속주행이 가능하도록 모터B PWM 주파수 변경
  //serial
  Serial.begin(9600);
  
  //motor

  
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  
  pinMode(LINESENS_L, INPUT);
  pinMode(LINESENS_R, INPUT);
  
  //bluetooth 설정
  bluetooth.begin(9600);

  Serial.write("초기 세팅 완료!");
}

void loop() {
   RcCar_control_analog(); 

   //bluetooth 
    if (bluetooth.available() ) {
    Serial.write(bluetooth.read());
  }

  if (bluetooth.available() ) {
    Serial.write(bluetooth.write("00"));
  }

}


void RcCar_control_digital() 
{
  boolean line_l = digitalRead(LINESENS_L), line_r = digitalRead(LINESENS_R); 
  if(line_l == 0 && line_r == 0)      
  {
    Serial.write("1\n");
    digitalWrite(MOTOR_A_1,HIGH);
    digitalWrite(MOTOR_A_2,LOW);
    digitalWrite(MOTOR_B_1,HIGH);
    digitalWrite(MOTOR_B_2,LOW);
  }
  else if(line_l == 1 && line_r == 0) 
  {
    Serial.write("2\n");
    digitalWrite(MOTOR_A_1,HIGH);
    digitalWrite(MOTOR_A_2,LOW);
    digitalWrite(MOTOR_B_1,LOW);
    digitalWrite(MOTOR_B_2,LOW);
  }
  else if(line_l == 0 && line_r == 1)  
  {
    Serial.write("3\n");
    digitalWrite(MOTOR_A_1,LOW);
    digitalWrite(MOTOR_A_2,LOW);
    digitalWrite(MOTOR_B_1,HIGH);
    digitalWrite(MOTOR_B_2,LOW);
  }
  else
  {
    Serial.write("4\n");
    digitalWrite(MOTOR_A_1,LOW);
    digitalWrite(MOTOR_A_2,LOW);
    digitalWrite(MOTOR_B_1,LOW);
    digitalWrite(MOTOR_B_2,LOW);
  }
}


void RcCar_control_analog() 
{
  boolean line_l = digitalRead(LINESENS_L), line_r = digitalRead(LINESENS_R); 
  if(line_l == 0 && line_r == 0)      
  {
    Serial.write("1\n");
    analogWrite(MOTOR_A_1,speed_variable);
    analogWrite(MOTOR_A_2,0);
    analogWrite(MOTOR_B_1,speed_variable);
    analogWrite(MOTOR_B_2,0);
  }
  else if(line_l == 1 && line_r == 0) 
  {
    Serial.write("2\n");
    analogWrite(MOTOR_A_1,speed_variable);
    analogWrite(MOTOR_A_2,0);
    analogWrite(MOTOR_B_1,0);
    analogWrite(MOTOR_B_2,0);
  }
  else if(line_l == 0 && line_r == 1)  
  {
    Serial.write("3\n");
    analogWrite(MOTOR_A_1,0);
    analogWrite(MOTOR_A_2,0);
    analogWrite(MOTOR_B_1,speed_variable);
    analogWrite(MOTOR_B_2,0);
  }
  else
  {
    Serial.write("4\n");
    analogWrite(MOTOR_A_1,0);
    analogWrite(MOTOR_A_2,0);
    analogWrite(MOTOR_B_1,0);
    analogWrite(MOTOR_B_2,0);
  }
}
