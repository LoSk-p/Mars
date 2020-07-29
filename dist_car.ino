const uint8_t EN1    = 6;       //Левые колёса
const uint8_t L_PWM1 = 8;       
const uint8_t R_PWM1 = 7;       
const uint8_t EN2    = 10;      //Правые колёса
const uint8_t L_PWM2 = 11;
const uint8_t R_PWM2 = 12;
const float rad = 0.06;          //Радиус колёс, м
int val;
int LED = 13;
int times = 0;
int times_prev = 0;
int del;
float num_rev;               //Кол-во оборотов колеса
float d = 0;
int stop_flag = 0;
int direct;
int count = 0;
int flag = 0;
int flag_ch = 0;
float distanse;
int flag_dist = 0;

void move_forvard() {                              
    delay(2);
    digitalWrite(L_PWM1, LOW );  
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    200 ); 
    delay(2); 
    digitalWrite(L_PWM2, LOW );  
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 ); 
}

void move_back() {                                 
    delay(2);
    digitalWrite(L_PWM1, HIGH);  
    digitalWrite(R_PWM1, LOW );  
    digitalWrite(EN1,    140 );
    delay(2);
    digitalWrite(L_PWM2, HIGH);  
    digitalWrite(R_PWM2, LOW );  
    digitalWrite(EN2,    140 );
}

void stop_move() {                                 
    delay(2);
    digitalWrite(L_PWM1, HIGH); 
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    0 );
    delay(2);
    digitalWrite(L_PWM2, HIGH); 
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    0 );
}

void turn_right() {                                 
    delay(2);
    digitalWrite(L_PWM2, LOW );    //Правые колеса вперед
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 );
    delay(2);
    digitalWrite(L_PWM1, HIGH);    //Левые колеса назад 
    digitalWrite(R_PWM1, LOW );  
    analogWrite (EN1,    140 );
}

void turn_left() {                                   
    delay(2);
    digitalWrite(L_PWM1, LOW );    //Левые колеса вперед
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    140 );
    delay(2);
    digitalWrite(L_PWM2, HIGH);    //Правые колеса назад 
    digitalWrite(R_PWM2, LOW );  
    analogWrite (EN2,    140 );
    
}

float dist() {                 //Вычисление пройденного расстояния в оборотах с предыдущего вызова функции
  int Htime;                //Длительность верхнего импульса
  int Ltime;                //Длительность нижнего импульса
  float obs;                //Обороты в секунду
  float fr;                 //Частота импульсов
  float obor = 0;             //Кол-во оборотов 
  Htime = pulseIn(3,HIGH);
  Ltime = pulseIn(3,LOW);
  if (Htime != 0 && Ltime != 0) {
    fr = 100000/(Htime + Ltime);
  }else {
    fr = 0;
  }
  obs = fr/24;
  times_prev = times;
  times = millis();
  del = times - times_prev;
  obor = obor + obs*del/1000;
  return obor;
}

void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);
  pinMode(3,   INPUT);
  pinMode(LED, OUTPUT);
  pinMode(EN1,    OUTPUT);     // Конфигурируем вывод EN1    как выход (выход Arduino, вход драйвера)
  pinMode(L_PWM1, OUTPUT);     // Конфигурируем вывод L_PWM1 как выход (выход Arduino, вход драйвера)
  pinMode(R_PWM1, OUTPUT);     // Конфигурируем вывод R_PWM1 как выход (выход Arduino, вход драйвера)
  pinMode(EN2,    OUTPUT);     // Конфигурируем вывод EN2    как выход (выход Arduino, вход драйвера)
  pinMode(L_PWM2, OUTPUT);     // Конфигурируем вывод L_PWM2 как выход (выход Arduino, вход драйвера)
  pinMode(R_PWM2, OUTPUT);     // Конфигурируем вывод R_PWM2 как выход (выход Arduino, вход драйвера)
}

void loop()
{
  /*if (millis()%500 == 0){
  Serial3.print(direct);
  Serial3.print(" c= ");
  Serial3.println(count);}*/
    //Serial.print("direct = ");
    //Serial.print(int(direct - '0'));
    //Serial.print(", count = ");
    //Serial.println(int(count - '0'));
    switch (direct) {
      case '1':                            //Поворот налево на определённый угол (2-45, 4-90, 9-180)
        //Serial.println("Turning left");
        if (count != 0){
          flag_dist = 1;
        distanse = (count - '0')/20.3;
        flag_ch = 1;
        turn_left();
        direct = 0;
        count = 0;
        }
        break;
      case '2':                                   //Движение вперед на определённое расстояние
        //Serial.println("Moving forward");
        if (count != 0){
          flag_dist = 1;
        distanse = count - '0';
        flag_ch = 1;
        move_forvard();
        //delay(20);
        direct = 0;
        count = 0;
        }
        break;
      case '3':                                  //Поворот направо на определённый угол (2-45, 4-90, 9-180)
        //Serial.println("Turning right");
        if (count != 0){
          flag_dist = 1;
        distanse = (count - '0')/20.3;
        Serial.print("distanse = ");
        Serial.println(distanse);
        flag_ch = 1;
        turn_right();
        direct = 0;
        count = 0;
        }
        break;
      case '4':                                  //Движение назад на определённое расстояние
        //Serial.println("Moving back");
        if (count != 0){
          flag_dist = 1;
        distanse = count - '0';
        flag_ch = 1;
        move_back();
        direct = 0;
        count = 0;
        }
        break;
     /* case 83:
        //Serial.println("Stop");
        stop_move();
        //digitalWrite(LED, HIGH);
        //delay(500);
        //digitalWrite(LED, LOW);
        break;*/
}

  if (flag_ch == 1){
    if (flag_dist == 1) {
      dist();
      flag_dist = 0;
    }
    Serial.print("d = ");
    Serial.print(d);
    d = d + 2*PI*rad*dist();
    Serial.println(d);
  }


if (!Serial3.available()){
  if (flag_ch == 0 || d > distanse){
  stop_move();
  flag_ch = 0;
  d = 0;
  digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
} else {
  digitalWrite(LED, LOW);
}
}

void serialEvent3() {
  if (Serial3.available())
  {
    val = Serial3.read();
    if (flag == 1) {
      flag = 0;
      count = val;
      Serial.print("count = ");
      Serial.println(count);
      val = 0;
    }
    Serial.print("val = ");
    Serial.println(val);
        switch (val) {
      case 'a':
        flag_ch = 0;
        //Serial.println("Turning left");
        turn_left();
        delay(28);
        break;
      case 'w':
        flag_ch = 0;
        //Serial.println("Moving forward");
        move_forvard();
        delay(28);
        break;
      case 'd':
        flag_ch = 0;
        //Serial.println("Turning right");
        turn_right();
        delay(28);
        break;
      case 's':
        flag_ch = 0;
        //Serial.println("Moving back");
        move_back();
        delay(28);
        break;
      case 32:
        flag_ch = 0;
        //Serial.println("Stop");
        stop_move();
        //digitalWrite(LED, HIGH);
        //delay(500);
        //digitalWrite(LED, LOW);
        break;
      case '1':
        flag_ch = 0;
        flag = 1;
        direct = '1';
        break;
      case '2':
        flag_ch = 0;
        flag = 1;
        direct = '2';
        break;
      case '3':
        flag_ch = 0;
        flag = 1;
        direct = '3';
        break;
      case '4':
        flag_ch = 0;
        flag = 1;
        direct = '4';
        break;
}
    
  }
}
