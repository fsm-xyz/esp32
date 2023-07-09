#include <AccelStepper.h>

TaskHandle_t th_p[1];
// Define some steppers and the pins the will use
AccelStepper stepper1(AccelStepper::DRIVER, 19);
float speed = 0;
char cmd; // 电机指令字符
int data; // 电机指令参数

void runUsrCmd()
{
    switch (cmd)
    {
    case 's': // 用户通过此指令设置电机速度
        speed = data;
        stepper1.setSpeed(data);
        break;
    default: // 未知指令
        Serial.println(F("Unknown Command"));
    }
}

void Core0task(void *pvParameters)
{
    // Task 1 code here

    for (;;)
    {
        if (Serial.available())
        {
            // 检查串口缓存是否有数据等待传输
            cmd = Serial.read(); // 获取电机指令中指令信息
            Serial.print(F("cmd = "));
            Serial.print(cmd);
            Serial.print(F(" , "));

            data = Serial.parseInt(); // 获取电机指令中参数信息
            Serial.print(F("data = "));
            Serial.print(data);
            Serial.println(F(""));

            runUsrCmd();
            Serial.println(cmd);
            Serial.println(data);
        }
        delay(500);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println(F("++++++++++++++++++++++++++++++++++"));
    Serial.println(F("+ Taichi-Maker AccelStepper Demo +"));
    Serial.println(F("+     www.taichi-maker.com       +"));
    Serial.println(F("++++++++++++++++++++++++++++++++++"));
    Serial.println(F(""));
    Serial.println(F("Please input motor command:"));

    xTaskCreatePinnedToCore(Core0task, "Core0task", 4096, NULL, 3, &th_p[0], 0);
    // 1000000.0 166599
    stepper1.setMaxSpeed(1000000);
    stepper1.setSpeed(speed);
}

void loop()
{
    stepper1.runSpeed();
}
