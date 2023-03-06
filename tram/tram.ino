#include <stdio.h>
#define MOTOR_A_a 3     //모터A의 +출력핀은 3번핀입니다
#define MOTOR_A_b 11    //모터A의 -출력핀은 11번핀입니다
#define MOTOR_B_a 5     //모터B의 +출력핀은 5번핀입니다
#define MOTOR_B_b 6     //모터B의 -출력핀은 6번핀입니다
#define MOTOR_SPEED 120  //모터의 기준속력입니다(0~255), 라인트레이서는 저속주행을 권장드립니다.
#define LINESENS_L A1    //왼쪽 라인센서 입력핀입니다.
#define LINESENS_R A0    //오른쪽 라인센서 입력핀입니다.

#include <SoftwareSerial.h>

//rc car
unsigned char m_a_spd = 0, m_b_spd = 0; //모터의 속력을 저장하는 전역변수
boolean m_a_dir = 0, m_b_dir = 0; //모터의 방향을 결정하는 전역변수

//rc car variable
int line_l, line_r;
//bluetooth
int Tx = 12;
int Rx = 13;

SoftwareSerial bluetooth(Tx,Rx);

//bluetooth variable
#define TRIGGER_LENGTH 5   //트램 제어 시 필요한 trigger 종류(길이)
char str_send[1024];

//drive Rc car problem variable
int detectProblem[TRIGGER_LENGTH] = {0,0,0,0,0};
int detectProblem_tmp[TRIGGER_LENGTH] = {0,0,0,0,0};
//gas, fire, ultraSound , person,trafficLight,obstacle

int  probolemStatusChange = 1;
// 1 ok , -1 no

int rcCanDrive = 1;
// 1 ok , -1 no

//bluetooth variable
int flag = -1;
int cnt = 0;
char save_data[TRIGGER_LENGTH]; 



void setup()  
{
  
  bluetooth.begin(9600);       // 블루투스 통신 시작
  
  TCCR1B = TCCR1B & 0b11111000 | 0x05;  //저속주행이 가능하도록 모터A PWM 주파수 변경
  TCCR2B = TCCR2B & 0b11111000 | 0x07;  //저속주행이 가능하도록 모터B PWM 주파수 변경
  
  //모터 제어 핀들을 출력으로 설정합니다.
  pinMode(MOTOR_A_a, OUTPUT);
  pinMode(MOTOR_A_b, OUTPUT);
  pinMode(MOTOR_B_a, OUTPUT);
  pinMode(MOTOR_B_b, OUTPUT);

  //라인센서 핀들을 입력으로 설정합니다.
  pinMode(LINESENS_L, INPUT);
  pinMode(LINESENS_R, INPUT);

  Serial.begin(9600);
  Serial.println("start");
}

void loop()
{
  if(rcCanDrive==1){
      linetrace_val();  //입력된 데이터에 따라 모터에 입력될 변수를 조정하는 함수
      motor_drive();    //모터를 구동하는 함수
      Serial.println("gogogoo");
  }
  else{
    Serial.println("nonono");
  }


   //bluetooth receive
  if (bluetooth.available()) {
    
    char fromRaspiData = (char)bluetooth.read();
    //Serial.println(fromRaspiData); 
    if(fromRaspiData=='s'){
      //Serial.println("start");
        flag = 1;
        save_data[cnt] = fromRaspiData;

        cnt=0;
    }
    else if(fromRaspiData=='f'){
        //Serial.println("finish");
        Serial.println("tram trigger data FROM raspi "); 
        if( (cnt >= TRIGGER_LENGTH) & (flag == 1) ){
          save_data[cnt] = fromRaspiData;
          RcCar_StatusSaveToTmp();
          RcCar_statusRecieve(save_data);
           //bluetooth send
           if(is_RcCar_StatusChange()==1){ 
            Serial.println("tram data TO raspi"); 
            bluetooth.write("s");
            
            sprintf(str_send, "%d,%d,%d,%d,%d,%d,%d,%d,%d",rcCanDrive,line_l,line_r,detectProblem[0], detectProblem[1], detectProblem[2],  detectProblem[3],  detectProblem[4]);
            bluetooth.print(str_send);
            
            bluetooth.write("f");

            bluetooth.flush();

            is_RcCar_CanDrive();
          }
          else{
             //Serial.println("notchange"); 
          }
        }
        flag = -1;
        cnt = 0;  
    }
    else if(flag==1){
        if(fromRaspiData!=','){
          save_data[cnt] = fromRaspiData;
        
          cnt += 1;
        }       
    }
    
  }
  
}



int RcCar_StatusSaveToTmp(){
  for(int i=0; i<TRIGGER_LENGTH; i++){
    detectProblem_tmp[i] = detectProblem[i];
  }
}

int RcCar_statusRecieve(char data[]){
  for(int i=0; i<TRIGGER_LENGTH; i++){
    Serial.println(data[i]);
    detectProblem[i] = int(data[i]);
  }
  
}

int is_RcCar_StatusChange(){
  for(int i=0; i<TRIGGER_LENGTH; i++){
    if(detectProblem[i] != detectProblem_tmp[i]){
      probolemStatusChange = 1;
      return probolemStatusChange;
    }
  } 
  probolemStatusChange= -1;
  return probolemStatusChange;
 
}


int is_RcCar_CanDrive(){
  for(int i=0; i<TRIGGER_LENGTH; i++){
    if(detectProblem[i] == 48+1){
        rcCanDrive = -1;
        return rcCanDrive;
      }
  }
  //Serial.write("true");
  rcCanDrive = 1;
  return rcCanDrive;
}


void linetrace_val() //입력된 데이터에 따라 모터에 입력될 변수를 조정하는 함수
{
  int an_line_l = analogRead(LINESENS_L), an_line_r = analogRead(LINESENS_R); //왼쪽과 오른쪽 라인센서의 감지값을 변수에 저장합니다.ini
  //Serial.println(an_line_r);

  if(an_line_l < 300)
  {
    line_l = 0;    
  }
  else
  {
    line_l = 1;
  }
  
  if(an_line_r < 300)
  {
    line_r = 0 ;   
  }
  else
  {
    line_r = 1;
  }


  
  if(line_l == 0 && line_r == 0)      //라인이 감지되지 않을때 전진
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = MOTOR_SPEED;  //모터A의 속력값 조정
    m_b_spd = MOTOR_SPEED;  //모터B의 속력값 조정
//    Serial.println('d');

  }
  else if(line_l == 0 && line_r == 1) //오른쪽 센서 감지시 오른쪽으로 전진
  {
    m_a_dir = 1;  //모터A 역방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = 0;  //모터A의 정지
    m_b_spd = constrain(MOTOR_SPEED*2, 0, 255);  //모터B의 속력값 조정
//    Serial.println('l');

  }
  else if(line_l == 1 && line_r == 0)  //왼쪽 센서 감지시 왼쪽으로 전진
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 1;  //모터B 역방향
    m_a_spd = constrain(MOTOR_SPEED*2, 0, 255);  //모터A의 속력값 조정
    m_b_spd = 0;  //모터B의 정지
  //  Serial.println('r');
  
  }
  else
  {
    m_a_dir = 0;  //모터A 정방향
    m_b_dir = 0;  //모터B 정방향
    m_a_spd = 0;  //모터A의 정지
    m_b_spd = 0;  //모터B의 정지
   // Serial.println('s');

  }
}

void motor_drive()  //모터를 구동하는 함수
{
  if(m_a_dir == 0)
  {
    digitalWrite(MOTOR_A_a, LOW);     //모터A+ LOW
    analogWrite(MOTOR_A_b, m_a_spd);  //모터A-의 속력을 PWM 출력
  }
  else
  {
    analogWrite(MOTOR_A_a, m_a_spd);  //모터A+의 속력을 PWM 출력
    digitalWrite(MOTOR_A_b, LOW);     //모터A- LOW
  }
  if(m_b_dir == 1)
  {
    digitalWrite(MOTOR_B_a, LOW);     //모터B+ LOW
    analogWrite(MOTOR_B_b, m_b_spd);  //모터B-의 속력을 PWM 출력
  }
  else
  {
    analogWrite(MOTOR_B_a, m_b_spd);  //모터B+의 속력을 PWM 출력
    digitalWrite(MOTOR_B_b, LOW);     //모터B- LOW
  }
}
