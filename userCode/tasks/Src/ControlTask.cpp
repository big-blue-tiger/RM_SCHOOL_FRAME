//
// Created by LEGION on 2021/10/4.
//
#include "ControlTask.h"
#define optional_down 0
#define optional_up 1  //不确定，等待调试时确定

int step_control_flag = 0;// 1为上台阶中，2为下台阶中，0为其他状态
int step_processing_time = 0;//控制上下台阶的时间状态
float current_angle=90.0;
int Get_Control_Mode_Index() // 获取模式编号
{
    if(RemoteControl::rcInfo.sLeft == UP_POS && RemoteControl::rcInfo.sRight == UP_POS)
        return 1;
    if(RemoteControl::rcInfo.sLeft == UP_POS && RemoteControl::rcInfo.sRight == MID_POS)
        return 2;
    if(RemoteControl::rcInfo.sLeft == MID_POS && RemoteControl::rcInfo.sRight == UP_POS)
        return 3;
    return 0;
}

SERVO_INIT_T Claw_Servo_Init ={
   .servoType = POSITION_180,
   .servoID = SERVO_ID_1,
   .firstAngle = 90,
   .angleLimit_Min = 0,
   .angleLimit_Max = 180
};
Servo ClawServo(&Claw_Servo_Init);   //声明舵机

SERVO_INIT_T TurnL_Servo_Init ={
        .servoType = POSITION_180,
        .servoID = SERVO_ID_2,
        .firstAngle = 90,
        .angleLimit_Min = 0,
        .angleLimit_Max = 180
};
Servo TurnLServo(&TurnL_Servo_Init);

void CtrlHandle(){
		ClawServo.Handle();
    TurnLServo.Handle();//这一段最好放在初始化的地方，不要放在循环里面
    if (RemoteControl::rcInfo.sRight == DOWN_POS){//右侧三档，急停模式
        ChassisStop();
        UserStop();
				ClawServo.stop();
				TurnLServo.stop();
    }else {//其他正常模式
        switch (Get_Control_Mode_Index()) {
            case 1://左侧一档
                step_processing_time = 0;

                ChassisSetVelocity(RemoteControl::rcInfo.right_col*4.2,
                                   RemoteControl::rcInfo.right_rol*4.2,RemoteControl::rcInfo.left_rol*60);

                if (RemoteControl::rcInfo.optional[0] == optional_up && RemoteControl::rcInfo.optional[1] == optional_up){
										HAL_Delay(500);//这一段存疑
									if (RemoteControl::rcInfo.optional[0] == optional_up && RemoteControl::rcInfo.optional[1] == optional_up){
                    ClawServo.SetTargetAngle(10.0);
										TurnLServo.SetTargetAngle(10.0);
									   current_angle=10.0;}// 吃球平台舵机控制至贴近地面,参数可调
                    // 滚筒电机驱动吃球，恒定速率
                }
                else{
                    ClawServo.SetTargetAngle(current_angle+RemoteControl::rcInfo.left_col*90.0);//参数可以调整
										TurnLServo.SetTargetAngle(current_angle+RemoteControl::rcInfo.left_col*90.0);
									current_angle=current_angle+RemoteControl::rcInfo.left_col*90.0;// 吃球平台舵机控制 参数：float RemoteControl::rcInfo.left_col 大小-1到1
                }
                if (RemoteControl::rcInfo.optional[0] == optional_down && RemoteControl::rcInfo.optional[1] == optional_up){
                    // 滚筒电机驱动吐球，恒定速率
                }
                break;
            case 2://左侧二档
                ChassisSetVelocity(RemoteControl::rcInfo.right_col*4.2,
                                   RemoteControl::rcInfo.right_rol*4.2,RemoteControl::rcInfo.left_rol*60);

                if (RemoteControl::rcInfo.optional[1] == optional_down){
                    step_processing_time = 0;
                    ClawServo.SetTargetAngle(current_angle+RemoteControl::rcInfo.left_col*90.0);
										TurnLServo.SetTargetAngle(current_angle+RemoteControl::rcInfo.left_col*90.0);
										current_angle=current_angle+RemoteControl::rcInfo.left_col*90.0;// 吃球平台舵机控制 参数：float RemoteControl::rcInfo.left_rol 大小-1到1
                    // 底盘前后轮中间舵机控制 参数：float RemoteControl::rcInfo.left_col 大小-1到1
                }
                else{
                    if (RemoteControl::rcInfo.optional[0] == optional_up){
                        if (step_control_flag != 1){
                            step_control_flag = 1;
                            step_processing_time = 0;
                        }
                        step_processing_time++;
                        if (step_processing_time < 500){
                            // 执行上坡第一步
                        }
                        else if (step_processing_time > 500){
                            //执行上坡第二部
                        }
                    }
                    else if (RemoteControl::rcInfo.optional[0] == optional_down){
                        if (step_control_flag != 2){
                            step_control_flag = 2;
                            step_processing_time = 0;
                        }
                        step_processing_time++;
                        if (step_processing_time < 500){
                            // 执行下坡第一步
                        }
                        else if (step_processing_time > 500){
                            //执行下坡第二部
                        }
                    }
                }
				break;
			case 3:
			    step_processing_time = 0;
                ChassisSetVelocity(RemoteControl::rcInfo.right_col*4.2,
                                   RemoteControl::rcInfo.right_rol*4.2,0.0);
                // 滚筒电机驱动吃吐球 参数：float RemoteControl::rcInfo.left_rol
                switch (RemoteControl::rcInfo.optional[0]*2+RemoteControl::rcInfo.optional[1])
                {
                    case optional_down*2+optional_down:

                    ClawServo.SetTargetAngle(current_angle+RemoteControl::rcInfo.left_col*90.0);
										TurnLServo.SetTargetAngle(current_angle+RemoteControl::rcInfo.left_col*90.0);
										current_angle=current_angle+RemoteControl::rcInfo.left_col*90.0;// 吃球平台舵机控制 参数：float RemoteControl::rcInfo.left_col 大小-1到1
                        break;
                    case optional_up*2+optional_down:
													ClawServo.SetTargetAngle(30.0);
													TurnLServo.SetTargetAngle(30.0);
													current_angle=30.0;
                        // 吃球平台舵机与地面一致
                        break;
                    case optional_down*2+optional_up:

                        ClawServo.SetTargetAngle(30.0);
													TurnLServo.SetTargetAngle(30.0);
													current_angle=30.0;// 吃球平台舵机与前哨站高度一致，参数调整
                        break;
                    case optional_up*2+optional_up:

                        ClawServo.SetTargetAngle(30.0);
													TurnLServo.SetTargetAngle(30.0);
													current_angle=30.0;// 吃球平台舵机与基地高度一致，参数调整
                        break;
                    default:
                        break;
                }
                break;
            default:
				break;
        }

    }

}

