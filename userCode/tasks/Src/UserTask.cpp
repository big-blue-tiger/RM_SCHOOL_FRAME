//
// Created by LEGION on 2021/10/4.
//
#include "UserTask.h"

PID_Regulator_t userPidRegulator = {
        .kp = 60,
        .ki = 0,
        .kd = 0,
        .componentKpMax = 10000,
        .componentKiMax = 0,
        .componentKdMax = 0,
        .outputMax = 10000 // 2006 电机输出电流上限，可以调小，勿调大
};

MOTOR_INIT_t userMotorInit = {
        .speedPIDp = &userPidRegulator,
        .anglePIDp = nullptr,
        ._motorID = MOTOR_ID_1,
        .reductionRatio = 36.0f,
        .ctrlType = POSITION_Double,
};

Motor UserMotor(MOTOR_ID_5,&userMotorInit);

void User_motor_set_speed(float Uv) //Uv角速度，单位：rad/s
{
    UserMotor.SetTargetSpeed(RADpS2RPM(Uv));
}
/***
 * 在这里放入xxx.stop()即可使舵机,电机在遥控器急停挡位断电
 */
void UserStop(){
    UserMotor.Stop();
    ClawServo.stop();
    TurnLServo.stop();
}

/***
 * 在这里写入初始化内容
 */
void UserInit(){
    ClawServo.Handle();
    TurnLServo.Handle();//这一段最好放在初始化的地方，不要放在循环里面
}

/***
 * 用户自定义任务主循环
 */
void UserHandle(){
    UserMotor.Handle();
}