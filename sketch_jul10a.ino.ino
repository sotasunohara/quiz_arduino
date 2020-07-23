#include <DFRobotDFPlayerMini.h>

#include <SoftwareSerial.h>
#include<LiquidCrystal.h>

int u1_button=8;
int u2_button=9;
int u3_button=10;
int correct_button=6;
int wrong_button=7;

bool flag=true;//ボタン押す権利
bool flag_point=true;
int u1_point=0;
int u2_point=0;
int u3_point=0;
int ans_user=0;

int problem_num=0;//問題番号

LiquidCrystal lcd(12,11,5,4,3,2);

SoftwareSerial mysoftserial(1,0);//RX,TX
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  // put your setup code here, to run once:
  pinMode(u1_button,INPUT);//回答者
  pinMode(u2_button,INPUT);
  pinMode(u3_button,INPUT);
  
  pinMode(correct_button,INPUT);//司会者
  pinMode(wrong_button,INPUT);

  mysoftserial.begin(9600);//シリアルポート　転送速度9600bps


  
  lcd.begin(16,2);
  lcd.clear();

  while(!myDFPlayer(mysoftserial)){
    lcd.print("Error");
    delay(500);
    lcd.clear();
  }
  myDFPlayer.volume(15);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(flag_point==true){
    lcd.setCursor(0,0);
    
    String s="1:";
    s.concat(u1_point);
    s.concat(",2:");
    s.concat(u2_point);
    s.concat(",3:");
    s.concat(u3_point);
    lcd.print(s);
    lcd.setCursor(0,1);
    flag_point=false;
    /*ここで問題文読み上げる*/
    if(prblem_num>3){
      problem_num=0;
    }
    myDFPlayer.play(problem_num);
  }
  if(flag==false){//誰かに解答権があるとき
    if(digitalRead(correct_button)==HIGH){//正解ボタンを押す
      if(ans_user==1){//ユーザ1が正解した
        u1_point++;
      }else if(ans_user==2){//2が正解した
        u2_point++;
      }else if(ans_user==3){//3が正解した
        u3_point++;
      }
      flag=true;
      ans_user=0;//解答権を初期化する
      flag_point=true;//点数表示更新を許可する
      myDFPlayer.play(100);//正解音声出力
      problem_num++;//問題文更新
      lcd.clear();
    }else if(digitalRead(wrong_button)==HIGH){//不正解ボタンを押す
      flag=true;//解答権をリセット
      ans_user=0;
      flag_point=true;
      myDFPlayer.play(101);//不正解音声出力
      lcd.clear();
    }
  }else{//解答ボタンが押せるとき
    if(digitalRead(u1_button)==HIGH){
      flag=false;
      ans_user=1;//解答権を1に
    }else if(digitalRead(u2_button)==HIGH){
      flag=false;
      ans_user=2;//解答権を2に
    }else if(digitalRead(u3_button)==HIGH){
      flag=false;
      ans_user=3;//解答権を3に
    }
    if(ans_user!=0)
      lcd.print(ans_user);
  }
  delay(50);
}
