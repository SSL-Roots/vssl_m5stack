#include <M5Stack.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <geometry_msgs/msg/twist.h>

#include "secrets.h"
#include "robot_information.hpp"

rcl_publisher_t publisher;
rcl_subscription_t subscriber;
std_msgs__msg__Int32 msg;
geometry_msgs__msg__Twist sub_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

//I2C
TwoWire *i2cWire;
uint8_t send_data[7],read_data[4],succes_byte,state;
bool is_robot_i2c=false, is_gyro_i2c=false;

RobotInformations robot_command;
static float vx,vy,omega,dribble;

geometry_msgs__msg__Twist twist_msg;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){M5.Lcd.printf("err:%d\n", temp_rc); error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}

void sub_callback(const void* msgin)
{
  const geometry_msgs__msg__Twist* msg = (const geometry_msgs__msg__Twist*) msgin;
  twist_msg = *msg;

  robot_command.setRobotVelocity(msg->linear.x, msg->linear.y, msg->angular.z);

//I2Cの送信
  robot_command.makeCommunicateData();
  robot_command.getCommunicateData(send_data);
  Wire.beginTransmission(0x54);
  succes_byte = Wire.write(send_data, 7);
  state = Wire.endTransmission();
  if(state == 0){
    is_robot_i2c  = true;
  }else{
    is_robot_i2c  = false;
  }
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL)
  {
    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);

    M5.Lcd.printf("x = %.2f, y = %.2f, z = %.2f\n", twist_msg.linear.x, twist_msg.linear.y, twist_msg.angular.z);
  }
}

void setup() {
    M5.begin();        // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源模块
    M5.Lcd.setTextSize(2);


    Serial.begin(115200);


    IPAddress agent_ip(192, 168, 0, 17);
    size_t agent_port = 8888;

    char ssid[] = SSID;
    char psk[] = PSK;
    set_microros_wifi_transports(ssid, psk, agent_ip, agent_port);
    M5.Lcd.println("wifi connected.");

    // set_microros_serial_transports(Serial);
    // M5.Lcd.println("serial connected.");
delay(2000);

allocator = rcl_get_default_allocator();

// create init_options
RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
M5.Lcd.println("Init: support");

// create node
RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));
M5.Lcd.println("Init: node");

// create subscriber
const rosidl_message_type_support_t * my_type_support =
  ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist);
subscriber = rcl_get_zero_initialized_subscription();
const char* topic_name = "/cmd_vel";
RCCHECK(rclc_subscription_init_default(
  &subscriber,
  &node,
  my_type_support,
  topic_name
));
geometry_msgs__msg__Twist__init(&sub_msg);
M5.Lcd.println("Init: subscriber");

// create timer
const unsigned int timer_timeout = 200;
RCCHECK(rclc_timer_init_default(
  &timer,
  &support,
  RCL_MS_TO_NS(timer_timeout),
  timer_callback));
M5.Lcd.println("Init: timer");

// create executor
RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
RCCHECK(rclc_executor_add_timer(&executor, &timer));
RCCHECK(rclc_executor_add_subscription(
  &executor,
  &subscriber,
  &sub_msg,
  &sub_callback,
  ON_NEW_DATA
));
M5.Lcd.println("Init: executor");

msg.data = 0;

rclc_executor_prepare(&executor);

//I2C
Wire.begin(21,22);
}

void loop() {
RCSOFTCHECK(rclc_executor_spin(&executor));
}