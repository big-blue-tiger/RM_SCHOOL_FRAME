//
// Created by LEGION on 2021/10/4.
//

#ifndef RM_FRAME_C_USERTASK_H
#define RM_FRAME_C_USERTASK_H
#include "main.h"
#include "Device.h"
#include "Servo.h"
#include "Motor.h"

#define RADpS2RPM(num)(num / (3.1415926f/30.0f))
/*枚举类型定义------------------------------------------------------------*/


/*结构体定义--------------------------------------------------------------*/


/*结构体成员取值定义组------------------------------------------------------*/
/*外部变量声明-------------------------------------------------------------*/

extern Servo ClawServo,TurnLServo,TurnRServo,ShovelLServo,ShovelRServo;

/*外部函数声明-------------------------------------------------------------*/

void UserStop();
void UserHandle();
void UserInit();
void User_motor_set_speed(float Uv);
#endif //RM_FRAME_C_USERTASK_H
