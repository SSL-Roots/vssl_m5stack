#ifndef ROBOT_INFORMATION_HPP //ヘッダーファイルを作るときに先頭に記載する
#define ROBOT_INFORMATION_HPP//変数を定義

#include <M5Stack.h>

//テストモードではMAX値を使わず、設定された値をそのまま送信する
#define MAX_VELOCITY 1.0
#define MAX_ROTATION_VELOCITY 1.0
// #define MAX_VELOCITY 2.0
// #define MAX_ROTATION_VELOCITY 6.28

typedef union{
    struct{
    uint32_t KICKER_CHARGE:1;
    uint32_t KICKER_RELEASE:1;
    uint32_t DRIBBLER_SPEED:2;
    uint32_t :4;
    };
    int8_t ROBOT_COMMAND;
}__ROBOT_COMMANDbits;


typedef class RobotInformations{
    public:
        RobotInformations(void);
        void setRobotVelocity(float v_x, float v_y, float v_omega);
        float getVelocity_x(void);
        float getVelocity_y(void);
        float getVelocity_omega(void);

        void setChargeFlag(bool chg);
        void setChargeFlag(void);
        void clrChargeFlag(void);
        bool getChargeFlag(void);

        void setKickFlag(bool kick);
        void setKickFlag(void);
        void clrKickFlag(void);
        bool getKickFlag(void);

        void setBallPosition(bool ball);
        bool isBallKeep(void);

        void setDribbleStrength(float dribbler);
        float getDribbleStrength(void);

        void makeCommunicateData(void); //I2C送信用データ生成
        void getCommunicateData(uint8_t *send_data_add);
    private:
        float vx, vy, omega, dribble;
        bool is_kick, is_charge, is_ball;
        __ROBOT_COMMANDbits send_commands;
        uint8_t m_send_data[7]={0x05,0x01, 0, 0, 0, 0, 0};

        void setVelocityCommand(void);
        void setKickCommand(void);
        void setChecksum(void);
        void setRobotCommand(void);

}RobotInformations;

#endif /*ROBOT_INFORMATION_HPP*///ヘッダーファイルを作るときの最後に記載する