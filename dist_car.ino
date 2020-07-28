const uint8_t EN1    = 6;        // № вывода Arduino к которому подключены входы драйвера L_EN и R_EN. Можно указать любой вывод Arduino поддерживающий ШИМ.
const uint8_t L_PWM1 = 8;        // № вывода Arduino к которому подключён  вход  драйвера L_PWM.       Можно указать любой вывод Arduino, как цифровой, так и аналоговый.
const uint8_t R_PWM1 = 7;        // № вывода Arduino к которому подключён  вход  драйвера R_PWM.       Можно указать любой вывод Arduino, как цифровой, так и аналоговый.
const uint8_t EN2    = 10;
const uint8_t L_PWM2 = 11;
const uint8_t R_PWM2 = 12;
const float rad = 0.06;          //Радиус колес, м
int val;
int LED = 13;
int times = 0;
int times_prev = 0;
int del;
float num_rev;               //Кол-во оборотов колеса
float d = 0;
int stop_flag = 0;
int direct;
int count;
int flag = 0;
int flag_ch = 0;

void move_forvard() {                              //2
    delay(2);
    digitalWrite(L_PWM1, LOW );  
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    200 ); 
    delay(2); 
    digitalWrite(L_PWM2, LOW );  
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 ); 
}

void move_back() {                                 //4
    delay(2);
    digitalWrite(L_PWM1, HIGH);  
    digitalWrite(R_PWM1, LOW );  
    digitalWrite(EN1,    140 );
    delay(2);
    digitalWrite(L_PWM2, HIGH);  
    digitalWrite(R_PWM2, LOW );  
    digitalWrite(EN2,    140 );
}

void stop_move() {                                 //5
    delay(2);
    digitalWrite(L_PWM1, HIGH); 
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    0 );
    delay(2);
    digitalWrite(L_PWM2, HIGH); 
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    0 );
}

void turn_right() {                                 //3
    delay(2);
    digitalWrite(L_PWM2, LOW );    //Правые колеса вперед
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 );
    delay(2);
    digitalWrite(L_PWM1, HIGH);    //Левые колеса назад 
    digitalWrite(R_PWM1, LOW );  
    analogWrite (EN1,    140 );
}

void turn_left() {                                   //1
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
    /*switch (val) {
      case 'a':
        //Serial.println("Turning left");
        turn_left();
        break;
      case 119:
        Serial.println("Moving forward");
        move_forvard();
        delay(20);
        break;
      case 'd':
        //Serial.println("Turning right");
        turn_right();
        break;
      case 's':
        //Serial.println("Moving back");
        move_back();
        break;
      case 83:
        //Serial.println("Stop");
        stop_move();
        //digitalWrite(LED, HIGH);
        //delay(500);
        //digitalWrite(LED, LOW);
        break;
}*/

if (!Serial3.available()){
stop_move();
digitalWrite(LED, HIGH);
} else {
  digitalWrite(LED, LOW);
}
}

void serialEvent3() {
  if (Serial3.available())
  {
    val = Serial3.read();
    Serial.println(val);
        switch (val) {
      case 'a':
        //Serial.println("Turning left");
        turn_left();
        delay(30);
        break;
      case 119:
        Serial.println("Moving forward");
        move_forvard();
        delay(30);
        break;
      case 'd':
        //Serial.println("Turning right");
        turn_right();
        delay(30);
        break;
      case 's':
        //Serial.println("Moving back");
        move_back();
        delay(30);
        break;
      case 83:
        //Serial.println("Stop");
        stop_move();
        //digitalWrite(LED, HIGH);
        //delay(500);
        //digitalWrite(LED, LOW);
        break;
}
    
  }
}
