#include <EEPROM.h>

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


void move_forvard() {                              //2
    delay(40);
    digitalWrite(L_PWM1, LOW );  
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    140 ); 
    delay(40); 
    digitalWrite(L_PWM2, LOW );  
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 ); 
}

void move_back() {                                 //4
    delay(40);
    digitalWrite(L_PWM1, HIGH);  
    digitalWrite(R_PWM1, LOW );  
    digitalWrite(EN1,    140 );
    delay(40);
    digitalWrite(L_PWM2, HIGH);  
    digitalWrite(R_PWM2, LOW );  
    digitalWrite(EN2,    140 );
}

void stop_move() {                                 //5
    delay(40);
    digitalWrite(L_PWM1, HIGH); 
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    140 );
    delay(40);
    digitalWrite(L_PWM2, HIGH); 
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 );
}

void turn_right() {                                 //3
    delay(40);
    digitalWrite(L_PWM2, LOW );    //Правые колеса вперед
    digitalWrite(R_PWM2, HIGH);  
    analogWrite (EN2,    140 );
    delay(40);
    digitalWrite(L_PWM1, HIGH);    //Левые колеса назад 
    digitalWrite(R_PWM1, LOW );  
    analogWrite (EN1,    140 );
}

void turn_left() {                                   //1
    delay(40);
    digitalWrite(L_PWM1, LOW );    //Левые колеса вперед
    digitalWrite(R_PWM1, HIGH);  
    analogWrite (EN1,    140 );
    delay(40);
    digitalWrite(L_PWM2, HIGH);    //Правые колеса назад 
    digitalWrite(R_PWM2, LOW );  
    analogWrite (EN2,    140 );
    
}

float dist() {                 //Вычисление пройденного расстояния
  int Htime;                //Длительность верхнего импульса
  int Ltime;                //Длительность нижнего импульса
  float obs;    //Обороты в секунду
  float fr;     //Частота импульсов
  float obor = 0;    //Кол-во оборотов 
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
  //Serial.begin(9600);
  pinMode(3,   INPUT);
  pinMode(LED, OUTPUT);
  pinMode(EN1,    OUTPUT);     // Конфигурируем вывод EN1    как выход (выход Arduino, вход драйвера)
  pinMode(L_PWM1, OUTPUT);     // Конфигурируем вывод L_PWM1 как выход (выход Arduino, вход драйвера)
  pinMode(R_PWM1, OUTPUT);     // Конфигурируем вывод R_PWM1 как выход (выход Arduino, вход драйвера)
  pinMode(EN2,    OUTPUT);     // Конфигурируем вывод EN2    как выход (выход Arduino, вход драйвера)
  pinMode(L_PWM2, OUTPUT);     // Конфигурируем вывод L_PWM2 как выход (выход Arduino, вход драйвера)
  pinMode(R_PWM2, OUTPUT);     // Конфигурируем вывод R_PWM2 как выход (выход Arduino, вход драйвера)
}


void loop() {
  float distanse;
  int dis;
  if (Serial3.available())  {
    val = Serial3.read();
    switch (val) {
      case '1':
        Serial3.println("Turning left");
        turn_left();
        break;
      case '2':
      Serial3.println("Moving forward");
        move_forvard();
        break;
      case '3':
      Serial3.println("Turning right");
        turn_right();
        break;
      case '4':
      Serial3.println("Moving back");
        move_back();
        break;
      case '5':
      Serial3.println("Stop");
        stop_move();
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
        break;
      case '6':
        d = 0;
        move_forvard();
        while (d < 2) {
          delay(40);
          if (stop_flag == 1) {
            break;
          }
          d = d + 2*PI*rad*dist();
          //Serial3.println(d);
        }
        stop_move();
        Serial3.println("Done");
        delay(40);
        break;
      case 'w':              //вперед на определенное расстояние
        d = 0;
        while (!Serial3.available());
        distanse = Serial3.read() - '0';
        Serial3.print("Moving forward ");
        Serial3.print(distanse);
        Serial3.println(" m");
        //dis = distanse.toInt();
        //Serial3.println(dis);
        move_forvard();
        while (d < distanse) {
          delay(40);
          if (stop_flag == 1) {
            break;
          }
          d = d + 2*PI*rad*dist();
          //Serial3.println(d);
        }
        stop_move();
        Serial3.println("Done");
        delay(40);
        break;
      case 'a':                 //Влево на определенный угол 4-90; 2-45; 8-180
        d = 0;
        while (!Serial3.available());
        distanse = Serial3.read() - '0';
        Serial3.print("Turning right ");
        Serial3.println(distanse*22.5);
        //dis = distanse.toInt();
        //Serial3.println(dis);
        turn_left();
        while (d*5.714 < distanse) {
          delay(40);
          if (stop_flag == 1) {
            break;
          }
          d = d + 2*PI*rad*dist();
          //Serial3.println(d);
        }
        stop_move();
        Serial3.println("Done");
        delay(40);
        break;  
      case 'd':                 //Вправо на определенный угол 4-90; 2-45
        d = 0;
        while (!Serial3.available());
        distanse = Serial3.read() - '0';
        Serial3.print("Turning right ");
        Serial3.println(distanse*22.5);
        //dis = distanse.toInt();
        //Serial3.println(dis);
        turn_right();
        while (d*5.714 < distanse) {
          delay(40);
          if (stop_flag == 1) {
            break;
          }
          d = d + 2*PI*rad*dist();
          //Serial3.println(d);
        }
        stop_move();
        Serial3.println("Done");
        delay(40);
        break; 
      case 's':              //Назад на определенное расстояние
        d = 0;
        while (!Serial3.available());
        distanse = Serial3.read() - '0';
        Serial3.print("Moving back ");
        Serial3.print(distanse);
        Serial3.println(" m");
        //dis = distanse.toInt();
        //Serial3.println(dis);
        move_back();
        while (d < distanse) {
          delay(40);
          if (stop_flag == 1) {
            break;
          }
          d = d + 2*PI*rad*dist();
          //Serial3.println(d);
        }
        stop_move();
        Serial3.println("Done");
        delay(40);
        break;    
    }
  }
  delay(40);
  num_rev = num_rev + dist();
  EEPROM.put(0, num_rev);
  delay(40);
}

/*void serialEvent3() {
  int st;
  st = Serial3.read();
  if (st == '5') {
    stop_flag = 1;
    stop_move();
  }
}
*/
