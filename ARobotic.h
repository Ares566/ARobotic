/*
  ARobotic.h - Library for Arduino robotic devices.
  Created by Renat Abaidulin, July 22, 2015.
*/

#ifndef arobotic_h
#define arobotic_h

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
//работа с экраном по умолчанию выключена т.к. библиотека занимает много места
#if defined(AR_USE_LCD)
  #include <OLED_I2C.h>
#endif

#define AR_M_WITH_ENC 1//режим работы с энкодерами 
#define AR_M_WITH_NRF 2//режим работы с nrf24l01 

#define AR_WHEEL_D 65 //диаметр колеса в мм


class ARobotic
{
public:
	/*
	* 	mode - режим работы: с энкодерами или без(???) может  define-ми регулировать режимы работы
	*	ar_width - ширина модели в мм
	*/
	ARobotic(int mode,uint16_t ar_width);

	void moveForward(uint8_t speed, uint16_t distance);//едем вперед
	void moveBackward(uint8_t speed, uint16_t distance);//едем назад
	void turnLeft(int degree);//поворот через левое колесо на определенный градус
	void turnRight(int degree);//поворот через правое колесо на определенный градус
	void stopMoving();//останов обоих моторов
	void waitingDistance4LM(uint16_t distance);//ждем пока левый мотор проедет определенную дистанцию
	void waitingDistance4RM(uint16_t distance);//ждем пока правый мотор проедет определенную дистанцию

	//публичные функции работы с энкодерами колес
	uint16_t getLeftWheelDistance();//пройденное расстояние левого колеса
	uint16_t getRightWheelDistance();//пройденное расстояние правого колеса
	void resetLeftWheelDistance();//обнуление пройденного расстояния левого колеса
	void resetRightWheelDistance();//обнуление пройденного расстояния правого колеса

	//функции для работы с экраном
	#if defined(AR_USE_LCD)
	void lcdPrint(char *msg,int row,int line);//печать сообщения на экране
	#endif

private:
	
	uint16_t _ar_width;
	//функции обрабоки прерываний от энкодера колес
	

};
void encoderFuncM_L();
void encoderFuncM_R();
#endif
