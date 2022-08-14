#include <M5Stack.h>

void setup() {
    M5.begin();        // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源模块
    M5.Lcd.setTextSize(4);
    M5.Lcd.print("Hello Taichi World");  // Print text on the screen (string)
                                  // 在屏幕上打印文本(字符串)
}

void loop() {
}