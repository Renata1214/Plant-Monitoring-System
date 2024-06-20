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
```
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/envIII
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/envIII
*
* Product: ENVIII_SHT30_QMP6988.  环境传感器
* Date: 2022/7/20
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

void loop() {
    pressure = qmp6988.calcPressure();
    if (sht30.get() == 0) {  // Obtain the data of shT30.  获取sht30的数据
        tmp = sht30.cTemp;   // Store the temperature obtained from shT30.
                             // 将sht30获取到的温度存储
        hum = sht30.humidity;  // Store the humidity obtained from the SHT30.
                               // 将sht30获取到的湿度存储
    } else {
        tmp = 0, hum = 0;
    }
    M5.lcd.fillRect(0, 20, 100, 60,
                    BLACK);  // Fill the screen with black (to clear the
                             // screen).  将屏幕填充黑色(用来清屏)
    M5.lcd.setCursor(0, 20);
    M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n",
                  tmp, hum, pressure);
    delay(2000);
}

```
### Email and Wifi Library.

```

/*
  Rui Santos
  Complete project details at:
   - ESP32: https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/
   - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Example adapted from: https://github.com/mobizt/ESP-Mail-Client
*/

// To send Emails using Gmail on port 465 (SSL), you need to create an app password: https://support.google.com/accounts/answer/185833

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "REPLACE_WITH_YOUR_SSID"
#define WIFI_PASSWORD "REPLACE_WITH_YOUR_PASSWORD"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "YOUR_EMAIL@XXXX.com"
#define AUTHOR_PASSWORD "YOUR_EMAIL_PASS"

/* Recipient's email*/
#define RECIPIENT_EMAIL "RECIPIENTE_EMAIL@XXXX.com"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = "ESP";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP Test Email";
  message.addRecipient("Sara", RECIPIENT_EMAIL);

  /*Send HTML message*/
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /*
  //Send raw text message
  String textMsg = "Hello World! - Sent from ESP board";
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/

  /* Set the custom message header */
  //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void loop(){

}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");
  }
}

```


### Email Sneder Library

A zip file was downloaded from the following link https://www.arduinolibraries.info/libraries/e-mail-sender 

### RTC time library.

```
/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core2 sample source code
*                          配套  M5Core2 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/core2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/core2
*
* Describe: RTC--实时时钟示例
* Date: 2022/1/9
*******************************************************************************
*/
#include <M5Core2.h>


RTC_TimeTypeDef RTCtime;
RTC_DateTypeDef RTCDate;


char timeStrbuff[64];


void flushTime() {
  M5.Rtc.GetTime(&RTCtime);  // Gets the time in the real-time clock.
                             // 获取实时时钟内的时间
  M5.Rtc.GetDate(&RTCDate);
  sprintf(timeStrbuff, "%d/%02d/%02d %02d:%02d:%02d", RTCDate.Year,
          RTCDate.Month, RTCDate.Date, RTCtime.Hours, RTCtime.Minutes,
          RTCtime.Seconds);
  // Stores real-time time and date data
  // to timeStrbuff.
  // 将实时时间、日期数据存储至timeStrbuff
  M5.lcd.setCursor(10, 100);
  // Move the cursor position to (x,y).  移动光标位置到(x,y)处
  M5.Lcd.println(timeStrbuff);
  // Output the contents of.  输出timeStrbuff中的内容
}


void setupTime() {
  RTCtime.Hours = 16;  // Set the time.  设置时间
  RTCtime.Minutes = 51;
  RTCtime.Seconds = 20;
  if (!M5.Rtc.SetTime(&RTCtime)) Serial.println("wrong time set!");
  // and writes the set time to the real
  // time clock. 并将设置的时间写入实时时钟
  RTCDate.Year = 2022;  // Set the date.  设置日期
  RTCDate.Month = 1;
  RTCDate.Date = 9;
  if (!M5.Rtc.SetDate(&RTCDate)) Serial.println("wrong date set!");
}
/* After M5Core2 is started or reset
the program in the setUp () function will be run, and this part will only be run
once. 在 M5Core2
启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin();  // Init M5Core2.  初始化 M5Core2
  delay(1000);
  setupTime();
  M5.Lcd.setTextSize(2);  // Set the text size.  设置文本大小
}


/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  flushTime();
  delay(1000);
}

```

## Equations to predict measurements.

In order to ensure that the program could satisfy its objectives without the need of 3 different sensors, our team explored the possibility of predicting the values of earth moisture and light intensity from the data obtained from the ENV III sensor. The research is shown below.

We need to find a way to obtain values that do not have a sensor by combining other values, for soil moisture use ENVII values and the amount and frequency of watering, and for illuminance use time and dataset for the illuminance index in the UAE.
There is no direct mathematical formula to approximate soil moisture based on the parameters you mentioned because soil moisture is affected by several variables, including soil type, plant type, temperature, humidity, and rainfall. However, here are some general guidelines that can help approximate soil moisture:

1. Air moisture: High humidity in the air can reduce the rate of evapotranspiration from the soil, resulting in higher soil moisture levels. Conversely, low humidity in the air can increase the evapotranspiration rate, leading to lower soil moisture levels.
   
2. Watering frequency: If you water your plant too frequently, the soil may become waterlogged, reducing the amount of oxygen available to the roots, and potentially leading to root rot. On the other hand, if you water your plant too infrequently, the soil may become too dry, which can cause the plant to wilt or die. Water when the top 1-2 inches of soil feel dry to the touch [make an assumption].
   
3. Amount of water: The amount of water you give your plant will affect soil moisture levels. Overwatering can lead to waterlogged soil, while underwatering can cause the soil to dry out.


Don’t forget about this parameters:
A value of 0 means soil moisture is at the wilting point (very dry) while a value of 1 means soil moisture is at saturation (very wet). A value of 0.54 means near optimum soil moisture for plant growth while values below 0.2 and above 0.8 indicate drought and excess moisture stress. 

The majority of flowers, trees, and shrubs require moisture levels between 0.21 - 0.40, while all vegetables require soil moisture between 0.41 and 0.80.

While these factors can give you a rough approximation of soil moisture, it's important to note that they are not accurate measurements. For more precise measurements of soil moisture, use a soil moisture sensor or conduct a physical soil moisture test.

Obtained values from Research papers. 
SOIL MOISTURE -

a*RH*(T-b) = Soil Moisture Content, a and b are constants that depend on soil type and other factors, RH is the relative humidity (between 0 and 1), and T is the temperature in Celsius.

https://hess.copernicus.org/preprints/hess-2023-44/hess-2023-44.pdf

ILLUMINANCE

I = (T/25)^2 * K , I is the illuminance in lux, T is the temperature in Celsius and K is a constant that depends on the latitude and longitude of the location.

https://www.bksv.com/media/doc/18-231.pdf 


## Errors and Challenges.


Reminders should be sent at more accurate intervals. With the current code, if the parameter was below or above the optimal value for a time below the threshold variable, the reminder is not sent regardless of how close the period of time was to the threshold. Although this will still be mentioned on the daily report, perhaps the user could benefit more by getting a reminder at the moment. 

Setting the time of the M5Stack to real-time. 
Maintaining the M5 Stack up to the actual hour generally requires trial and error by setting the time manually. Thus, it makes the time displayed on the screen inaccurate.

Connecting multiple sensors to the M5Stack. We had to acquire an export base to connect the two of the sensors we used. 

## Debugging. 

1. M5Stack Core 2 not connecting to the internet.
It turns out that M5Stack Core 2 cannot connect to the internet like regular devices. There have been multiple attempts and modifications to the code in order to establish internet connection. The device could not connect to the university’s WiFi due to security protocols. The device also did not connect to hotspots of mobile phones, which led us to believe that the problem is in the code. However, by accident, we discovered that we need to allow “Maximize Compatibility” so that Core 2 can connect to the hotspot. This bug was out of the scope of programming, but rather connection and security issues. 

2. M5Stack Core 2 not sending email.
This was a mixed bug, both due to code (initially) and Google security protocols. However, after adjusting the necessary security settings, the code worked without problems.

3. M5Stack Core 2 refers back to the setup() from loop().
This was one of the greatest mysteries in our development. The program would suddenly jump from void loop() to void setup(), which we did not know was even possible. We did not get help from a third party either, so we had to figure out the issue on our own. It turns out that Arduino will decide to call setup() if there is a memory management problem in one of the functions. In our case, the code would go outside the range of some arrays, which is mainly due to the fact that the code uses multiple multidimensional arrays, some of them even dynamic, so it was to expect such a bug. We had to take a more cautious look at the boundaries of each array and ensure that we are getting the desired indices. Such debugging was difficult to perform through dry run, but the obtained reports and reminders prove the successfulness of the code. 

4. Minor bugs.
There have been several minor bugs not worth further elaboration. One of them was formatting the time, i.e. instead of getting 16:4:3, we should get 16:04:03. Or, declaring variables as integers instead of floats and thus losing the necessary accuracy.


## Conclusion and Future Work.

GARDEN-01 is a successful coding and hardware project that utilizes the ENVIII, LIGHT, and Earth Moisture sensors to measure plant parameters, process data, and send reports and reminders. Through our dedication, study, and practice, as well as support from the Engineering faculty, we were able to overcome setbacks and complications to improve the health and well-being of our plants.

In the future, we envision a polished version of GARDEN-01 that can be extended to multiple plants and includes soil nutrient analysis to facilitate agriculture in times of climate uncertainty. Although we have completed the project, GARDEN-01 has left an indelible mark on our coding paths and experiences. We hope to revisit and continue its development in the near future. Overall, we are proud of the work we have accomplished and the impact it has had on the plants we have cared for.

## Reflection on Learnings.
The knowledge acquired and learning objectives achieved  from this project include:


To efficiently use the available libraries or snippets of code on the web and other platforms, it is important to understand the code inside them and specifically select the functions that are needed by your program. 


Understanding how each hardware component works and how they relate to one another before coding is essential to avoid mistakes of incompatibility. Learning about how the sensors communicate with the M5Stack, and vice versa would have allowed us to save time, since although the code was simple and straightforward, using the incorrect functions due to a misunderstanding of how signals are sent led to continuously unwanted results. 


As much as it is important to understand hardware and software individually, it is also important to know how to combine them and test the project as a whole. It is much more difficult to test the output when the input is provided through hardware, rather than custom digital input. 

A lot of the time must be spent on fixing “minor” issues that actually contribute to the majority of the code. For example, how to format the message properly, how to display the menu, make sure that all the arrays are handled correctly, even though the logic seems “simple”, etc. Attention to detail is the key to successfully completing projects like these.  

## Citations.

 Nemlyc, & Zhao, T. (2022, November 10). M5unit-env/unit_enviii_m5core2.ino at master · m5stack/m5unit-env. GitHub. Retrieved May 5, 2023, from https://github.com/m5stack/M5Unit-ENV/blob/master/examples/Unit_ENVIII_M5Core2/Unit_ENVIII_M5Core2.ino

RTC. M5Stack. (n.d.). Retrieved May 5, 2023, from http://docs.m5stack.com/en/api/core2/rtc_api 

Zhao, T. (2022, July 4). M5Stack/examples/unit/light at master · M5STACK/m5stack. GitHub. Retrieved May 5, 2023, from https://github.com/m5stack/M5Stack/tree/master/examples/Unit/LIGHT 


Su, D., Zhou, J., Yin, Z., Feng, H., Zheng, X., Han, X., and Hou, Q.: A calculation method of unsaturated soil water content based on thermodynamic equilibrium, Hydrol. Earth Syst. Sci. Discuss. [preprint], https://doi.org/10.5194/hess-2023-44, in review, 2023. 


Zhao, T. (2022, July 4). M5Stack/examples/unit/earth at master · M5STACK/m5stack. GitHub. Retrieved May 5, 2023, from https://github.com/m5stack/M5Stack/tree/master/examples/Unit/EARTH 


Renzo Mischianti. (2023, February 16). EmailSender Libraries. MIT. Retrieved May 5, 2023, from https://www.arduinolibraries.info/libraries/e-mail-sender


Rindorff, H. J. (n.d.). Luminance reflectances explanation and calculation of contrast. Retrieved April 27, 2023, from https://www.bksv.com/doc/18-231.pdf


Santos, Rui. ESP32 send emails using SMTP server: HTML, text, attachments (Arduino IDE). Random Nerd Tutorials. (2022, October 26). Retrieved May 5, 2023, from https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/  






