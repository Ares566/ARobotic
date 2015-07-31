/*
  ARobotic.cpp - Library for Arduino robotic devices.
  Created by Renat Abaidulin, July 22, 2015.
*/


#include "ARobotic.h"

#define ENL 5           // ШИМ вывод для управления правым двигателем 0 - стоп, подтянуто на землю
#define ENR 6           // ШИМ вывод для управления левым двигателем  0 - стоп, подтянуто на землю
#define DIRL 4          // Направление вращение левого двигателя 
#define DIRR 7          // Направление вращение правого двигателя 


ARobotic::ARobotic(int mode,uint16_t ar_width){
  lw_enc_counter = rw_enc_counter = 0;
  _ar_width = ar_width;

  pinMode(ENL, OUTPUT);
  pinMode(ENR, OUTPUT);
  pinMode(DIRL, OUTPUT);
  pinMode(DIRR, OUTPUT);
  //если режим работы с энкодерами
  if (mode && AR_M_WITH_ENC){
	  //подключаем энкодеры
	  pinMode(2, INPUT);   //2 пин отвечает за 0 прерывание
	  digitalWrite(2, HIGH);       // включаем подтягивающий резистор
	  attachInterrupt(0, encoderFuncM_L, CHANGE);  // энкодер левого колеса

	  pinMode(3, INPUT);   //3 пин отвечает за 1 прерывание
	  digitalWrite(3, HIGH);       // включаем подтягивающий резистор
	  attachInterrupt(1, encoderFuncM_R, CHANGE);  // энкодер правого колеса
  }
}

/*
*  Работа с  моторами
*  при передаче ненулевых параметров выход из функции только по истечении dest
*/
//едем вперед
void ARobotic::moveForward(uint8_t speed, uint16_t distance){
	
	digitalWrite(DIRL,HIGH);
	digitalWrite(DIRR,HIGH);
	digitalWrite(ENR,speed);
	digitalWrite(ENL,speed);

	//обработка distance
	if (distance){
		waitingDistance4LM(distance);
		stopMoving();
	}
}

//едем назад
void ARobotic::moveBackward(uint8_t speed, uint16_t distance){

	digitalWrite(DIRL,LOW);
	digitalWrite(DIRR,LOW);
	digitalWrite(ENR,speed);
	digitalWrite(ENL,speed);

	//обработка distance
	if (distance){
		waitingDistance4LM(distance);
		stopMoving();
	}
}

//поворот через левое колесо на определенный градус
void ARobotic::turnLeft(int degree){

	//поворачиваем вперед...
	if (degree>0){
		digitalWrite(DIRR,HIGH);
	}else
		digitalWrite(DIRR,LOW);//...или назад
	
	//скорость разворота
	uint8_t speed = 220;
	//длина дуги
	uint16_t ArcLen = round(PI * _ar_width * degree / 1800);
	
	//если развернуться нужно на мешьше оборота колеса, делаем скорость ниже
	if (ArcLen < 20){
		speed = 200;
	}

	digitalWrite(ENL,0);//левое колесо на месте
	digitalWrite(ENR,speed);//правое крутим
	
	waitingDistance4RM(ArcLen);
	stopMoving();
}

//поворот через правое колесо на определенный градус
void ARobotic::turnRight(int degree){
	
	//поворачиваем вперед...
	if (degree>0){
		digitalWrite(DIRL,HIGH);
	}else
		digitalWrite(DIRL,LOW);//...или назад

	//скорость разворота
	uint8_t speed = 220;
	//длина дуги
	uint16_t ArcLen = round(PI * _ar_width * degree / 1800);

	//если развернуться нужно на мешьше оборота колеса, делаем скорость ниже
	if (ArcLen < 20){
		speed = 200;
	}

	digitalWrite(ENL,speed);
	digitalWrite(ENR,0);

	waitingDistance4LM(ArcLen);
	stopMoving();
}

//останов обоих моторов
void ARobotic::stopMoving(){
	digitalWrite(ENR,0);
	digitalWrite(ENL,0);
}

//ждем пока левый мотор проедет определенную дистанцию
void ARobotic::waitingDistance4LM(uint16_t distance){
	uint16_t iCurDistance = getLeftWheelDistance();
	while((iCurDistance + distance) > getLeftWheelDistance()){}
}

//ждем пока правый мотор проедет определенную дистанцию
void ARobotic::waitingDistance4RM(uint16_t distance){
	uint16_t iCurDistance = getRightWheelDistance();
	while((iCurDistance + distance) > getRightWheelDistance()){}
}
/*
*  Работа с энкодерами моторов
*  подсчитываем пройденное расстояние
*/
//функция вызываемая прерыванием для подсчета оборотов левого колеса
void  ARobotic::encoderFuncM_L() {
  if (digitalRead(2) == HIGH) {
    lw_enc_counter++;
  } 
}
//функция вызываемая прерыванием для подсчета оборотов левого колеса
void ARobotic::encoderFuncM_R() {
  if (digitalRead(3) == HIGH) {
    rw_enc_counter++;
  } 

}

//сколько сантиметров проехало левое колесо
uint16_t ARobotic::getLeftWheelDistance(){
	//один оборот двигателя равен 20 импульсам диаметр колеса в мм
	return round( lw_enc_counter * PI*AR_WHEEL_D / 200 );
}

//сколько сантиметров проехало правое колесо
uint16_t ARobotic::getRightWheelDistance(){
	//один оборот двигателя равен 20 импульсам диаметр колеса в мм
	return round( rw_enc_counter * PI*AR_WHEEL_D / 200 );
}

//обнуление пройденного расстояния левого колеса
void ARobotic::resetLeftWheelDistance(){
	lw_enc_counter = 0;
}

//обнуление пройденного расстояния правого колеса
void ARobotic::resetRightWheelDistance(){
	rw_enc_counter = 0;
}

