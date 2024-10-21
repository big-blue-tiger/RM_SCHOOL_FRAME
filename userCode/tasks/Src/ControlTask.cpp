//
// Created by LEGION on 2021/10/4.
//
#include "ControlTask.h"
#define optional_down 0
#define optional_up 1  //不确定，等待调试时确定

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
void CtrlHandle(){
    if (RemoteControl::rcInfo.sRight == DOWN_POS){//右侧三档，急停模式
        ChassisStop();
        UserStop();
    }else {//其他正常模式
        switch (Get_Control_Mode_Index()) {
            case 1://左侧一档
                ChassisSetVelocity(RemoteControl::rcInfo.right_col*4.2,
                                   RemoteControl::rcInfo.right_rol*4.2,RemoteControl::rcInfo.left_rol*60);

                if (RemoteControl::rcInfo.optional[0] == optional_up && RemoteControl::rcInfo.optional[1] == optional_up){
                    // 吃球平台舵机控制至贴近地面
                    // 滚筒电机驱动吃球
                }
                else{
                    // 吃球平台舵机控制 参数：float RemoteControl::rcInfo.left_col 大小-1到1
                }
                if (RemoteControl::rcInfo.optional[0] == optional_down && RemoteControl::rcInfo.optional[1] == optional_up){
                    // 滚筒电机驱动土球
                }
                break;
            case 2://左侧二档
                uint8_t clawState;
                if (RemoteControl::rcInfo.sRight == UP_POS){
                    clawState = 0;
                }else if(RemoteControl::rcInfo.sRight == MID_POS) {
                    clawState = 1;
                }
				break;
			case 3:
            default:
				break;
        }

    }

}

