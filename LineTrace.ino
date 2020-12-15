int sensor[3] = {8,7,6};
int B_dir = 0; // 進んだ方向を記憶する

/* Motor */
int E1 = 9; //  set the speed pin of motor A as D9
int E2 = 5; //  set the speed pin of motor B as D5
int M1 = 2; // set the direction pin of motor A as D2
int M2 = 4; // set the direction pin of motor B as D4
/* 
モーターの回転速度 
{速度,回転の向き}
*/
int RSpeed[2] = {255,HIGH};
int LSpeed[2] = {255,HIGH};
int PSpeed = 75; // スピードの増減数

/* 前に進む */
void advance(void){
  //B_dir = 0;
  /* モーターが後ろ向きの場合、スピードを落としてから前に変える */
  if(RSpeed[1] == LOW){
    RSpeed[0] -= PSpeed;
    if(RSpeed[0] <= 0){
      RSpeed[1] = HIGH;
    }
  }else{
    RSpeed[0] += PSpeed;
  }
  if(LSpeed[1] == LOW){
    LSpeed[0] -= PSpeed;
    if(LSpeed[0] <= 0){
      LSpeed[1] = HIGH;
    }
  }else{
    LSpeed[0] += PSpeed;
  }
  int_range();
  digitalWrite(M1,LSpeed[1]);  // motor A turns forward, wheel will go forward
  digitalWrite(M2,RSpeed[1]);  // motor B turns forward, wheel will go forward
  analogWrite(E1,LSpeed[0]);    // speed of motor A（can be adjusted according to the actual speed of motor. Turn up the value to accelerate, lower the value to decelerate.) 
  analogWrite(E2,RSpeed[0]);    // speed of motor B（can be adjusted according to the actual speed of motor. Turn up the value to accelerate, lower the value to decelerate.) 
}


/* バックする */
void back(void){
    /* モーターが前向きの場合、スピードを落としてから後ろに変える */
  if(RSpeed[1] == HIGH){
    RSpeed[0] -= PSpeed;
    if(RSpeed[0] <= 0){
      RSpeed[1] = LOW;
    }
  }else{
    RSpeed[0] += PSpeed;
  }
  if(LSpeed[1] == HIGH){
    LSpeed[0] -= PSpeed;
    if(LSpeed[0] <= 0){
      LSpeed[1] = LOW;
    }
  }else{
    LSpeed[0] += PSpeed;
  }

  
  digitalWrite(M1,LSpeed[1]);   // motor A turns reverse, wheel will go backward
  digitalWrite(M2, RSpeed[1]);
  switch(B_dir){ // 右左折した情報をもとにバックする方向を決める
    case 0: // 真後ろ
       break;
    case 2: // 右後ろ
       RSpeed[0] = LSpeed[0] / 2;
       break;
    case 1: // 左後ろ
       LSpeed[0] = RSpeed[0] / 2;
       break;
    default:
       B_dir = 1;
       break;
  }
  int_range();
  analogWrite(E1,LSpeed[0]);     // rotating speed of motor A
  analogWrite(E2,RSpeed[0]);    // rotating speed of motor B
}

/* 左に曲がる  */
void turnL(void){
   B_dir = 1;
  /* 
  左のモーターを後ろ向きに
  右のモーターを前向きに
  */
  if(LSpeed[1] == HIGH){
    LSpeed[0] -= PSpeed;
    if(LSpeed[0] <= 0){
      LSpeed[1] = LOW;
    }
  }else{
    LSpeed[0] += PSpeed;
  }
  if(RSpeed[1] == LOW){
    RSpeed[0] -= PSpeed*2;
    if(RSpeed[0] <= 0){
      RSpeed[1] = HIGH;
    }
  }else{
    RSpeed[0] += PSpeed*2;
  }
   int_range();
   digitalWrite(M1,LSpeed[1]);    // motor A turns reverse and the wheel will go backward 
   digitalWrite(M2, RSpeed[1]);  // motor B turns forward and the wheel goes forward, the smart car will turn left.
   analogWrite(E1,LSpeed[0]);  // speed of motor A
   analogWrite(E2, RSpeed[0]);  // speed of motor B
}

/* 右に曲がる */
void turnR(void){
   B_dir = 2;
   /* 
  左のモーターを前向きに
  右のモーターを後ろ向きに
  */
  if(LSpeed[1] == LOW){
    LSpeed[0] -= PSpeed*2;
    if(LSpeed[0] <= 0){
      LSpeed[1] = HIGH;
    }
  }else{
    LSpeed[0] += PSpeed*2;
  }
  if(RSpeed[1] == HIGH){
    RSpeed[0] -= PSpeed;
    if(RSpeed[0] <= 0){
      RSpeed[1] = LOW;
    }
  }else{
    RSpeed[0] += PSpeed;
  }

   int_range();
   digitalWrite(M1,LSpeed[1]);    // motor A turns reverse and the wheel will go backward 
   digitalWrite(M2, RSpeed[1]);  // motor B turns forward and the wheel goes forward, the smart car will turn left.
   analogWrite(E1,LSpeed[0]);  // speed of motor A
   analogWrite(E2, RSpeed[0]);  // speed of motor B
}

/* 数値範囲内に収める */
void int_range(){
  if(RSpeed[0] > 250){
    RSpeed[0] = 250;
   }
   if(RSpeed[0] < 0){
    RSpeed[0] = 0;
   }
   if(LSpeed[0] > 250){
    LSpeed[0] = 250;
   }
   if(LSpeed[0] < 0){
    LSpeed[0] = 0;
   }
}


void setup() 
{
   for(int i = 0;i < 3;i++){
    pinMode(sensor[i],INPUT);
   }
   Serial.begin(9600);
  pinMode(M1,OUTPUT); // set M1 as OUTPUT mode
  pinMode(M2,OUTPUT); // set M2 as OUTPUT mode
  pinMode(E1,OUTPUT); //  set E1 as OUTPUT mode
  pinMode(E2,OUTPUT); //  set E2 as OUTPUT mode
  advance();
  delay(500);
}

void loop() 
{
  /* センサーの状態を調べる */
  for(int i = 0;i < 3;i++){
    Serial.print("No.");
    Serial.print(i);
    Serial.print(":");
    Serial.println(digitalRead(sensor[i]));
   }

   /* 数値範囲内に収める */
   int_range();

   /* センサーの情報から進む方向を決める */
   if(digitalRead(sensor[0]) == HIGH || digitalRead(sensor[1]) == HIGH || digitalRead(sensor[2]) == HIGH){ // センサーが反応しているとき
     Serial.println("black");
       if(digitalRead(sensor[0]) == HIGH && digitalRead(sensor[2]) == LOW){ // 左のセンサーが反応しているとき
        turnL();
       }else
       if(digitalRead(sensor[2]) == HIGH && digitalRead(sensor[0]) == LOW){ // 右のセンサーが反応しているとき
        turnR();
       }else{
        advance();
       }
       if(digitalRead(sensor[0]) == HIGH && digitalRead(sensor[1]) == HIGH && digitalRead(sensor[2]) == HIGH){
        turnL();
        delay(100);
        turnR();
       }
   }else{ // センサーが反応していない時
    Serial.println("White");
    back();
   }
   Serial.print("RSpeed:");
   Serial.println(RSpeed[0]);
   Serial.print("LSpeed:");
   Serial.println(LSpeed[0]);
    //delay(10);
   
}
