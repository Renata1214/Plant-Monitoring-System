# Plant-Monitoring-System


## Introduction.
Taking care of a plant has consistently proven to be a daunting task for the majority of the population. This is commonly explained by the lack of knowledge there is regarding how one can satisfy the needs of a plant, such as soil moisture, temperature, and lighting. In fact, according to different statistics an average "plant parent" accidentally kills seven plants before being able to successfully maintain a plant alive for extended periods of time (Mayers, 2023). With the COVID-19 pandemic causing an 18% surge in demand for houseplants, more people became interested in taking care of their plants' health despite the challenges of returning to in-person work.
To address this problem, we propose a system that provides the users with an update of the plant’s abiotic factors status, including soil moisture, temperature, humidity, and illumination. The primary objective of this system is to give its users (amateur “plant parents”) the information they need in order to make more informed decisions while taking better care of their plants. Even though the program will not be able to ensure the survival of the plant, as the program would need to monitor many other factors, it will maximize the survival probabilities of the plant regardless of the time invested on it, by reducing the number of careless mistakes done by users.
Project Development.

## Hardware.
The hardware/devices that were utilized for the project include:
M5Stack Core 2. Microcontroller in charge of processing the program, displaying the output, and overall carrying out all the functions stated in the code.
ExtPort board. Extension board used to have access to the Analog to Digital pin on the M5 Stack Core 2 (needed by the Earth Moisture Sensor as well as Light Intensity Unit).
ENV III sensor. Sensor to record the temperature and humidity of the environment.
LIGHT Intensity sensor. Sensor to identify the presence or the absence of light.				     
EARTH moisture sensor. Sensor to measure the moisture content of the soil.

## Software.

Software Components Utilized.
The software components that were utilized for this project included:
Arduino IDE
ENV III library from M5 Stack Core2. 
Wifi and Email libraries. 
Real Time setting (RTC) library from Arduino IDE examples. 


## Overview of the code. 


### Input and Output Diagram.

The expected input from the user can be divided in two categories: expected input directly at the code, and expected input through the use of the M5 Stack Core 2. The input on the code will include the email address that should be written on the line code number 85, in order to assign the email address to the variable recipient. Then, on line number 74 and 75 the user should input their Wifi SSID, and password. Finally, if the user desires to monitor a plant that is not within the menu, the user will have to manually write down the name of the plant, and the upper and lower limits of the suitable range of measurements for temperature, humidity, soil moisture, and number of light hours the plant requires. This will be done through the lines 17 to 25 of the code. 


Regarding the input through the M5 Stack Core 2, the user will have to use the buttons on the screen in order to select the type of plant they aim to monitor. Botton A will be used to go back through the options, button B will be used to access the next option, and button C will be used to confirm the selection. 

The expected output of the program consists solely of three components. The first one is the set of reminders that are meant to be sent every time one of the parameters (temperature, humidity, soil moisture) has been having critical values for more than half an hour. The reminder will consist of an email with one line informing the user which of the parameters is not within the optimal values, and whether it is below or above this range. The second output is a daily report scheduled to be sent at a certain hour of the day in which the user will be informed of the initial and final time  at which any of the parameters was above or below the optimal measurements, and the averages during those periods of time. The third output is simply the display of the values of the measurements taken in real time of the parameters. 


### General logic of the code.

The only input obtained from the sensor while using M5Stack is the choice of the plant that had previously been hardcoded in the program. In addition to the plant itself, the limits of the parameters (temperature, humidity, moisture, light) had also been hardcoded. Therefore, this ensures that there will not be any errors from the user’s side. The code makes use of many global variables, since these variables must not be changed through each iteration in void loop(). One such variable is an array of averages of high temperature. Resetting this array would make it useless.

The setup contains the menu which displays messages to the user, as well as allowing the user to select a plant. The main purpose of the setup is to obtain the choice of the plant and other required variables will be determined accordingly. 

Since there are too many variables and functions involved, it would be best to go through the logic chronologically. Firstly, the program displays the values of the parameters obtained, as well as the current time. These values are then passed to the function called process(type, value), where type refers to the parameter (0 - temperature, 1 - humidity, 2 - moisture) and “value” is the value read from the sensor. 

This function decides whether the value is too low, too high, or optimal and updates corresponding averages. In addition to that, the function checks for how much time a variable has not been optimal (if it is). If the period is longer than a certain threshold, the function indicates that this period is worth noting in the report. If this period is too long, the function will also call another function, send_reminder(type, reason) (“reason” can be either “low” or “high”), that will send an email to the user. This email is a single sentence that says “The temperature / humidity / moisture has been too high / low for the past half an hour.”. 

The function “process” also makes use of resize_average(arr, old_size, new_size) and resize_period(arr, old_size, new_size) (these are essentially the same functions, just for different data types). Since the number of periods of low / high parameters cannot be known in advance, these functions dynamically resize the arrays of periods and averages, so as to accommodate new incoming values. 

After the values have been processed through “process”, the program checks whether it is the time to send a report through send_report(). This function has no parameters because it uses global variables that have been altered through “process”. The format of the email sent can be found later in this report.

## Test Cases.


#### Test Case 1. 
Let the user select a random option of the menu initially displayed. The user should be able to go up to the last option of the menu and return to the first option through the use of the buttons. 

##### Expected Input.

- Selection from the options in the menu (except the option of My Plant).
- Email address to which the user wants the program to send the reports to. 
- Internet SSID address and Password.

##### Expected Output.

- Report of the plant with information that varies according to the values recorded by the temperature, moisture, humidity and light sensors. If the temperature, humidity or moisture was above or below the ideal range set on the code, the email should contain the initial and final time for when this undesired change in temperature, humidity or moisture occurred, as well as the average value of the corresponding factor during that period of time. 


Reminder. For the sake of time, a reminder will be sent to the user’s email when the temperature, humidity, or moisture are below or above the ideal values for more than a minute. The reminder should simply be an email informing the user whether the value of the parameter is too high or too low. 


The M5 Stack Core 2 screen should simply display the current measurements being recorded, as well as the time.
  
#### Test Case 2. 
Let the user select the option of “My Plant” such that the user can introduce a new plant option in the menu that they will later select for the program to run. The user would define the variables of My Plant, humidity, moisture, temperature, and light to the values they desire.

##### Expected Input.

- Selection from the options in the menu to be My Plant.
- Email Address to which the user wants the program to send the reports to. 
- Internet SSID address and Password.
- Name of the plant the user wants to input.
- Range of the ideal temperature for the plant.
- Range of the ideal humidity for the plant.
- Range of the ideal soil moisture for the soil.
- Range of the ideal number of light hours for the plant.

##### Expected Output. 

- Report of the plant with information that varies according to the values recorded by the temperature, moisture, humidity and light sensors. If the temperature, humidity or moisture was above or below the ideal range set on the code, the email should contain the initial and final time for when this undesired change in temperature, humidity or moisture occurred, as well as the average value of the corresponding factor during that period of time.


Reminder. For the sake of time, a reminder will be sent to the user’s email when the temperature, humidity, or moisture are below or above the ideal values for more than a minute. The reminder should simply be an email informing the user whether the value of the parameter is too high or too low. 


The M5 Stack Core 2 screen should simply display the current measurements being recorded. 


#### Test Case 3. 
The temperature, humidity, and moisture should be exposed to values above and below the recommended ones.

##### Expected Input.


- Selection from the options in the menu (except the option of My Plant).
- Email address to which the user wants the program to send the reports to. 
- Internet SSID address and Password.

##### Expected Output.

Reminders about any concerning change in temperature, humidity, and soil moisture; in case the current value goes over or under the ideal range. 
Report that will be sent to the provided email address and include the changes mentioned in the reminders.
We expect the report to state the initial time and the final time at which the temperature was at a higher value that the optimal one, as well as the average of the temperature for this period.

## Libraries used


Regarding the software component of the project, multiple libraries from the internet and from the example codes given by Arduino were utilized in order to efficiently fulfill some of the functions of the program. Below the libraries from which different pieces of code were taken can be found below:

### ENV III sensor library. 
'''
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/envIII
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/envIII
*
* Product: ENVIII_SHT30_QMP6988.  环境传感器
* Date: 2022/7/20
*******************************************************************************
  Please connect to Port A(22、21),Read temperature, humidity and atmospheric
  pressure and display them on the display screen
  请连接端口A(22、21),读取温度、湿度和大气压强并在显示屏上显示
*/
#include <M5Stack.h>
#include "M5_ENV.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

void setup() {
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    Wire.begin();  // Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
    qmp6988.init();
    M5.lcd.println(F("ENVIII Unit(SHT30 and QMP6988) test"));
}
'''





