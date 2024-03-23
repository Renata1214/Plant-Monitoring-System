#include <M5Core2.h>
#include <WiFi.h>
#include <EMailSender.h>
#include "M5_ENV.h"
#include <Wire.h>
#include <RTClib.h>

// Period limits for the report and the reminder
#define threshold 0 // if the streak is longer than this, update the report
#define reminder 5 // if the streak is longer than this, send a reminder

// Report time
#define report_hour 22
#define report_minute 48
#define report_second 0

// The user pre-defines their desired plant here
#define myplant "Example 1"
#define low_tmp 22
#define high_tmp 24
#define low_hum 65
#define high_hum 80
#define low_moist 21
#define high_moist 40
#define low_light 2
#define high_light 4


//Class Definitions for the ENV III sensor connection
SHT3X sht30;
QMP6988 qmp6988;

//Variables to save measurements of the ENVIII sensor
float tmp      = 0.0;
float hum      = 0.0;

//Variables to save the address pins of Earth moisture and Light sensor
int lightSensoranalog = 13;
int moistureSensorDigital = 26;
int moistureSensorAnalog = 36;
//Variables to save measurements of Earth Moisture and Light Sensor
int lightdigital = 0;
int lightanalog = 0; // 1 Indicates no light, 0 indicates there is light
double moistureanalog = 0; // 1 Indicates no moisture, 0 indicates there is moisture
double moisture = 0;
double moisture1 = 0;


// RTC variables for current time 
RTC_TimeTypeDef moment; 

int count = 0; // total counter of measurements, universal for all parameters
int count_low[3] = {0, 0, 0}; // counter of periods of low parameters; every parameter has its array
int count_high[3] = {0, 0, 0}; // counter of periods of high parameters; every parameter has its array
int light_count = 0; // counter of instances when there is light
int streak_low[3] = {0, 0, 0}; // temporary streak of lows; only necessary for checking whether a streak should be in the report
int streak_high[3] = {0, 0, 0}; // temporary streak of highs; only necessary for checking whether the streak should be in the report
RTC_TimeTypeDef *period_low[3][2]; // saves the periods of lows for every parameter
RTC_TimeTypeDef *period_high[3][2]; // save the periods of highs for every parameter
float average[3] = {0, 0, 0}; // one value for each parameter
float average_optimal[3] = {0, 0, 0}; // one value for each parameter
float *average_low[3]; // dynamic arrays of low average parameters
float *average_high[3]; // dynamic arrays of high average temperatures
float limits[3][2]; // upper and lower limit for each parameter
int plant; // refers to the name of the plant
// the database of plants
String plants[22] = { myplant, "Spider Plant", "Beans", "Broccoli", "Cabbage", "Carrots", "Cauliflower", "Cucumbers", "Zucchini", "Lettuce", "Peppers (bell type)", "Peas", "Tomatoes", "Pothos", "Snake Plant", "Monstera", "Peace Lily", "Aloe Vera", "Moth Orchid", "Money Tree", "Philodendron", "African Violet" };
bool email = false; 
// the database of parameters
float temperatures[22][2] = { {low_tmp, high_tmp}, {21.1, 32.2}, {18.3, 29.4}, {18.3, 26.7}, {15.6, 18.3}, {15.6, 21.1}, {15.6, 18.3}, {23.9, 29.4}, {15.6, 29.4}, {15.6, 18.3}, {21.1, 26.7}, {12.8, 18.3}, {12.8, 29.4}, {21.1, 32.2}, {15.6, 23.9}, {18.3, 29.4}, {20.0, 29.4}, {10.0, 29.4}, {21.1, 26.7}, {18.3, 26.7}, {23.9, 29.4}, {18.3, 26.7}};
float humidities[22][2] = { {low_hum, high_hum}, {70, 90}, {65, 85}, {65, 80}, {60, 65}, {60, 70}, {60, 65}, {75, 85}, {60, 85}, {60, 65}, {70, 80}, {55, 65}, {55, 85},{70, 90}, {60, 75}, {65, 85}, {68, 85}, {50, 85}, {70, 80}, {65, 80}, {75, 85}, {65, 80} };
float moistures[22][2] = { {low_moist, high_moist}, {21, 40}, {65, 85}, {65, 80}, {60, 70}, {60, 70}, {60, 65}, {75, 85}, {60, 85}, {60, 65}, {70, 80}, {55, 65}, {55, 85},{70, 90}, {60, 75}, {65, 85}, {68, 85}, {21, 60}, {70, 80}, {65, 80}, {75, 85}, {21, 59} };
float light_hours[22][2] = { {low_light, high_light}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8}, {6, 8} };

//the network to connect to
const String ssid = "a";
const String password = "Aleks1234";
//teh server that we will send and receive email messages through it and its port
const char* smtp_server = "smtp.gmail.com";
const int smtp_port = 587;
//the email of the m5stack and its password
const char* email_username = "garden01monitor@gmail.com";
const char* email_password = "aana oxiw rojm mmyn";
//using the library EMailSender to assign email_username as the sender
EMailSender emailSend(email_username, email_password);
//the email of the recipient
const char* recipient = "something@gmail.com";

//Declare variable that will serve as Index in Menu to access the plant options in String plants
int selectedOption=0;
//Variable to save the Plant option selected by the user
int userchoice = -1;

// function to send email reports
void send_report(){
  EMailSender::EMailMessage message; //creating an instance 
  //composing email subject with current date
  message.subject = "Garden report";
  String textMsg; //message content 

  // the email may be sent in the middle of a streak, so end times need to be set in that case
  for(int i = 0; i < 3; i++){
    if(streak_high[i] > threshold){
      period_high[i][1][count_high[i] - 1] = moment;
      average_high[i][count_high[i] - 1] /= streak_high[i];
    }
    streak_high[i] = 0;
    if(streak_low[i] > threshold){
      average_low[i][count_low[i] - 1] /= streak_low[i];
      period_low[i][1][count_low[i] - 1] = moment;
    }
    streak_low[i] = 0;
  }

  //adding the averages and periods of data to the email
  textMsg += "Average temperature: " + String(average[0] / count) + ".";
  textMsg += "\nAverage optimal temperature: " + String(average_optimal[0] / (count - count_high[0] - count_low[0])) + ".";
  textMsg += "\nPeriods of low temperature:\n";
  for(int i = 0; i < count_low[0]; i++){
    String minute1, second1, minute2, second2;
    if(period_low[0][0][i].Minutes < 10) minute1 = "0" + String(period_low[0][0][i].Minutes);
    else minute1 = String(period_low[0][0][i].Minutes);
    if(period_low[0][0][i].Seconds < 10) second1 = "0" + String(period_low[0][0][i].Seconds);
    else second1 = String(period_low[0][0][i].Seconds);
    if(period_low[0][1][i].Minutes < 10) minute2 = "0" + String(period_low[0][1][i].Minutes);
    else minute2 = String(period_low[0][1][i].Minutes);
    if(period_low[0][1][i].Seconds < 10) second2 = "0" + String(period_low[0][1][i].Seconds);
    else second2 = String(period_low[0][1][i].Seconds);
    textMsg += "\nTime: " + String(period_low[0][0][i].Hours) + ":" + minute1 + ":" + second1 + " - ";
    textMsg += String(period_low[0][1][i].Hours) + ":" + minute2 + ":" + second2 + ". ";
    textMsg += "\nAverage temperature for this period: " + String(average_low[0][i]) + ".\n\n";
  }
  textMsg += "\nPeriods of high temperature:\n";
  for(int i = 0; i < count_high[0]; i++){
    String minute1, second1, minute2, second2;
    if(period_high[0][0][i].Minutes < 10) minute1 = "0" + String(period_high[0][0][i].Minutes);
    else minute1 = String(period_high[0][0][i].Minutes);
    if(period_high[0][0][i].Seconds < 10) second1 = "0" + String(period_high[0][0][i].Seconds);
    else second1 = String(period_high[0][0][i].Seconds);
    if(period_high[0][1][i].Minutes < 10) minute2 = "0" + String(period_high[0][1][i].Minutes);
    else minute2 = String(period_high[0][1][i].Minutes);
    if(period_high[0][1][i].Seconds < 10) second2 = "0" + String(period_high[0][1][i].Seconds);
    else second2 = String(period_high[0][1][i].Seconds);
    textMsg += "\nTime: " + String(period_high[0][0][i].Hours) + ":" + minute1 + ":" + second1 + " - ";
    textMsg += String(period_high[0][1][i].Hours) + ":" + minute2  + ":" + second2 + ". ";
    textMsg += "\nAverage temperature for this period: " + String(average_high[0][i]) + ".\n\n";
  }


  textMsg += "\nAverage humidity: " + String(average[1] / count) + ".\n\n";
  textMsg += "\nAverage optimal humidity: " + String(average_optimal[1] / (count - count_high[1] - count_low[1])) + ".\n\n";
  textMsg += "\nPeriods of low humidity:\n";
  for(int i = 0; i < count_low[1]; i++){
    String minute1, second1, minute2, second2;
    if(period_low[1][0][i].Minutes < 10) minute1 = "0" + String(period_low[1][0][i].Minutes);
    else minute1 = String(period_low[1][0][i].Minutes);
    if(period_low[1][0][i].Seconds < 10) second1 = "0" + String(period_low[1][0][i].Seconds);
    else second1 = String(period_low[1][0][i].Seconds);
    if(period_low[1][1][i].Minutes < 10) minute2 = "0" + String(period_low[1][1][i].Minutes);
    else minute2 = String(period_low[1][1][i].Minutes);
    if(period_low[1][1][i].Seconds < 10) second2 = "0" + String(period_low[1][1][i].Seconds);
    else second2 = String(period_low[1][1][i].Seconds);
    textMsg += "\nTime: " + String(period_low[1][0][i].Hours) + ":" + minute1 + ":" + second1 + " - ";
    textMsg += String(period_low[1][1][i].Hours) + ":" + minute2 + ":" + second2 + ". ";
    textMsg += "\nAverage humidity for this period: " + String(average_low[1][i]) + ".\n\n";
  }
  textMsg += "\nPeriods of high humidity:\n";
  for(int i = 0; i < count_high[1]; i++){
    String minute1, second1, minute2, second2;
    if(period_high[1][0][i].Minutes < 10) minute1 = "0" + String(period_high[1][0][i].Minutes);
    else minute1 = String(period_high[1][0][i].Minutes);
    if(period_high[1][0][i].Seconds < 10) second1 = "0" + String(period_high[1][0][i].Seconds);
    else second1 = String(period_high[1][0][i].Seconds);
    if(period_high[1][1][i].Minutes < 10) minute2 = "0" + String(period_high[1][1][i].Minutes);
    else minute2 = String(period_high[1][1][i].Minutes);
    if(period_high[1][1][i].Seconds < 10) second2 = "0" + String(period_high[1][1][i].Seconds);
    else second2 = String(period_high[1][1][i].Seconds);
    textMsg += "\nTime: " + String(period_high[1][0][i].Hours) + ":" + minute1 + ":" + second1 + " - ";
    textMsg += String(period_high[1][1][i].Hours) + ":" + minute2 + ":" + second2 + ". ";
    textMsg += "\nAverage humidity for this period: " + String(average_high[1][i]) + ".\n\n";
  }


  textMsg += "\nAverage moisture: " + String(average[2] / count) + ".\n\n";
  textMsg += "\nAverage optimal moisture: " + String(average_optimal[2] / (count - count_high[2] - count_low[2])) + ".\n\n";
  textMsg += "\nPeriods of low moisture:\n";
  for(int i = 0; i < count_low[2]; i++){
    String minute1, second1, minute2, second2;
    if(period_low[2][0][i].Minutes < 10) minute1 = "0" + String(period_low[2][0][i].Minutes);
    else minute1 = String(period_low[2][0][i].Minutes);
    if(period_low[2][0][i].Seconds < 10) second1 = "0" + String(period_low[2][0][i].Seconds);
    else second1 = String(period_low[2][0][i].Seconds);
    if(period_low[2][1][i].Minutes < 10) minute2 = "0" + String(period_low[2][1][i].Minutes);
    else minute2 = String(period_low[2][1][i].Minutes);
    if(period_low[2][1][i].Seconds < 10) second2 = "0" + String(period_low[2][1][i].Seconds);
    else second2 = String(period_low[2][1][i].Seconds);
    textMsg += "\nTime: " + String(period_low[2][0][i].Hours) + ":" + minute1 + ":" + second1 + " - ";
    textMsg += String(period_low[2][1][i].Hours) + ":" + minute2 + ":" + second2 + ". ";
    textMsg += "\nAverage moisture for this period: " + String(average_low[2][i]) + ".\n\n";
  }
  textMsg += "\nPeriods of high moisture:\n";
  for(int i = 0; i < count_high[2]; i++){
    String minute1, second1, minute2, second2;
    if(period_high[2][0][i].Minutes < 10) minute1 = "0" + String(period_high[2][0][i].Minutes);
    else minute1 = String(period_high[2][0][i].Minutes);
    if(period_high[2][0][i].Seconds < 10) second1 = "0" + String(period_high[2][0][i].Seconds);
    else second1 = String(period_high[2][0][i].Seconds);
    if(period_high[2][1][i].Minutes < 10) minute2 = "0" + String(period_high[2][1][i].Minutes);
    else minute2 = String(period_high[2][1][i].Minutes);
    if(period_high[2][1][i].Seconds < 10) second2 = "0" + String(period_high[2][1][i].Seconds);
    else second2 = String(period_high[2][1][i].Seconds);
    textMsg += "\nTime: " + String(period_high[2][0][i].Hours) + ":" + minute1 + ":" + second1 + " - ";
    textMsg += String(period_high[2][1][i].Hours) + ":" + minute2 + ":" + second2 + ". ";
    textMsg += "\nAverage moisture for this period: " + String(average_high[2][i]) + ".\n\n";
  }

  textMsg += "\nHours of sunlight: " + String(light_count / 1800) + " hours, " + String((light_count % 1800) / 30) + " minutes, and " + String(((light_count % 1800) % 30) / 30) + " seconds.";
  if(light_count < 1800 * light_hours[userchoice][0]) textMsg += "This is below recommended hours of sunlight.\n";
  else if(light_count > 1800 * light_hours[userchoice][1]) textMsg += "This is above recommended hours of sunlight.\n";
  else textMsg += "This is in the recommended range of hours of sunlight.\n";

  //sends email with the text generated to the respective recepient
  message.message = textMsg;
  EMailSender::Response resp = emailSend.send(recipient, message);
}

//send reminders to the user via email in case one or multiple of the parameters have crossed their limits.
void send_reminder(int type, String reason){ // reason = "low" or reason = "high"
  EMailSender::EMailMessage message;
  String textMsg; 
  if(type == 0){
    message.subject = "REMINDER FOR TEMPERATURE";
    textMsg = "The temperature has been too " + reason + " for the past half an hour.\n";
  }

  else if(type == 1){
    message.subject = "REMINDER FOR HUMIDITY";
    textMsg = "The humidity has been too " + reason + " for the past half an hour.\n";
  }

  else if(type == 2) {
    message.subject = "REMINDER FOR MOISTURE";
    textMsg = "The soil moisture has been too " + reason + " for the past half an hour.\n";
  }

  message.message = textMsg;
  EMailSender::Response resp = emailSend.send(recipient, message);
}

// creates a new array and sets the new element equal to zero
void resize_average(float **arr, int old_size, int new_size){
  float *temp_arr = new float[old_size]; // temporary array for copying
  for(int i = 0; i < old_size; i++) temp_arr[i] = (*arr)[i]; // copying the content of the original array to the temporary one
  delete[] *arr; // deleting the old original array
  *arr = new float[new_size]; // declaring a new array
  for(int i = 0; i < old_size; i++) (*arr)[i] = temp_arr[i]; // copying the content from the temporary array
  delete[] temp_arr; // deleting the temporary array
  (*arr)[old_size] = 0; // initializing the value of the last element
}

// creates a new array and sets the new element equal to "moment"
void resize_period(RTC_TimeTypeDef **arr, int old_size, int new_size){
  RTC_TimeTypeDef *temp_arr = new RTC_TimeTypeDef[old_size]; // temporary array for copying
  for(int i = 0; i < old_size; i++) temp_arr[i] = (*arr)[i]; // copying the content of the original array to the temporary one
  delete[] *arr; // deleting the old original array
  *arr = new RTC_TimeTypeDef[new_size]; // declaring a new array
  for(int i = 0; i < old_size; i++) (*arr)[i] = temp_arr[i]; // copying the content from the temporary array
  (*arr)[old_size] = moment; // initializing the value of the last element
  delete[] temp_arr; // deleting the temporary array
}

// analysis of the data: if the values obtained by the sensor are out or in the limits set
void process(float value, int type){
  average[type] += value;


  if(value < limits[type][0]){


    if(streak_low[type] == 0) {
      resize_period(&period_low[type][0], count_low[type], count_low[type] + 1);
      resize_period(&period_low[type][1], count_low[type], count_low[type] + 1);
      resize_average(&average_low[type], count_low[type], count_low[type] + 1);
      period_low[type][count_low[type]][0] = moment;
      count_low[type]++;
    }
    streak_low[type]++;
    average_low[type][count_low[type] - 1] += value;


    if(streak_high[type] > threshold){
      period_high[type][count_high[type] - 1][1] = moment;
      average_high[type][count_high[type] - 1] /= streak_high[type];
    }
    streak_high[type] = 0;


    if(streak_low[type] != 0 && streak_low[type] % reminder == 0){
      send_reminder(type, "low");
    }
  }


  else if(value > limits[type][1]){


    if(streak_high[type] == 0) {
      resize_period(&period_high[type][0], count_high[type], count_high[type] + 1);
      resize_period(&period_high[type][1], count_high[type], count_high[type] + 1);
      resize_average(&average_high[type], count_high[type], count_high[type] + 1);
      period_high[type][count_high[type]][0] = moment;
      count_high[type]++;
    }
    streak_high[type]++;
    average_high[type][count_high[type] - 1] += value;


    if(streak_low[type] > threshold){
      average_low[type][count_low[type] - 1] /= streak_low[type];
      period_low[type][count_low[type] - 1][1] = moment;
    }


    streak_low[type] = 0;
    if(streak_high[type] != 0 && streak_high[type] % reminder == 0){
      send_reminder(type, "high");
    }


  }


  else{


    average_optimal[type] += value;


    if(streak_high[type] > threshold){
      average_high[type][count_high[type] - 1] /= streak_high[type];
      period_high[type][count_high[type] - 1][1] = moment;
    }


    if(streak_low[type] > threshold){
      average_low[type][count_low[type] - 1] /= streak_low[type];
      period_low[type][count_low[type] - 1][1] = moment;
    }


    streak_high[type] = 0;
    streak_low[type] = 0;
  }


}


void setup() {

  M5.begin();
  Serial.begin(9600);  
  M5.Lcd.setTextColor(GREEN);  
  M5.Lcd.setTextSize(2); 

  // Connect to WiFi

  WiFi.begin(ssid.c_str(), password.c_str());
  
  while(userchoice == -1){
  //Variable to keep the menu running
  bool var = true;
    
  // Introductory explanation given to the User about the program and the usage of buttons
  M5.Lcd.println("Welcome User!");
  M5.Lcd.println("You are using GARDEN-01");
  M5.Lcd.println("A device designed to take care of your plant");
  
  M5.Lcd.println("Press button B to rotate");
  M5.Lcd.println("through options.");
  M5.Lcd.println("Once you find your plant");
  M5.Lcd.println("press button C to confirm selection");
  delay (5000);

  // Display the first Option outside loop so that there is no need for the user to press any button. (Automatic display of menu)
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.drawString(plants[selectedOption],70 - plants[selectedOption].length(), 100, 2);
  userchoice = selectedOption;
  M5.update();

    // Menu starts running
   while (var){ 
      if (M5.BtnB.wasReleased()) 
      {
          //If there are other options after the previously displayed one
          if (selectedOption<21){
            selectedOption = (selectedOption + 1); 
            userchoice=selectedOption;
            M5.Lcd.clear(BLACK);
            M5.Lcd.setCursor(0, 0);
            //Display what is the option
            M5.Lcd.drawString(plants[selectedOption], 70 - plants[selectedOption].length(), 100, 2);
          }
          //If there are no other options after the previously displayed one
          else if (selectedOption>=21){
          selectedOption = 22;
            M5.Lcd.clear(BLACK);
            M5.Lcd.setTextSize(3); 
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.print("There is no more options please"); 
            M5.Lcd.print("press button A to");
            M5.Lcd.print("return to the");
            M5.Lcd.print("previous options");
          }
      } 
      //Let the user go back to the previous options as long as there is an option before the previously selected one
      else if (M5.BtnA.wasReleased() && selectedOption!=0){
          selectedOption = (selectedOption - 1); 
          userchoice=selectedOption;
          M5.Lcd.clear(BLACK);
          M5.Lcd.setCursor(0, 0);
          M5.Lcd.drawString(plants[selectedOption], 70 - plants[selectedOption].length(), 100, 2);
      }
      //If the user tries to find an option previous to the first option display message stating error
      else if (M5.BtnA.wasReleased() && selectedOption==0){
          M5.Lcd.clear(BLACK);
          M5.Lcd.setCursor(0, 0);
          M5.Lcd.print ("There is no previous option, please change to the next option using botton B.");
      }
      //If the user wants to confirm the selection for the plant
      else if (M5.BtnC.wasReleased() && selectedOption<22) 
      {
          M5.Lcd.clear(BLACK);
          M5.Lcd.setCursor(0, 0);
          M5.Lcd.print("Option Selected");
          M5.Lcd.drawString(plants[selectedOption], 70 - plants[selectedOption].length(), 100, 2);
          M5.Lcd.print("\n");
          userchoice=selectedOption;
          var=false; // condition to terminate the loop
      } 
        M5.update ();
    }

    //Set the variable selectedOption to zero so that when the program begins to run again, the user can begin to navigate the menu from option 1.
    selectedOption=0;
  }


  //Screen Printing configuration of text
  M5.lcd.setTextSize(4);


  //Initializing configuration for connection of sensors
  Wire.begin();  // Adding the I2C bus.
  qmp6988.init();


  //Setting the pins to input mode for the Earth moisture and light sensor.
  pinMode(moistureSensorDigital, INPUT);

  moment.Hours = 23;
  moment.Minutes = 45;
  moment.Seconds = 30;
  limits[0][0] = temperatures[userchoice][0]; // low temp
  limits[0][1] = temperatures[userchoice][1]; // high temp
  limits[1][0] = humidities[userchoice][0]; // low hum
  limits[1][1] = humidities[userchoice][1]; // high hum
  limits[2][0] = moistures[userchoice][0]; // low moist
  limits[2][1] = moistures[userchoice][1]; // high moist
  // setting the initial size for dynamic arrays
  for(int i = 0; i < 3; i++){
    period_low[i][0] = new RTC_TimeTypeDef[0];
    period_low[i][1] = new RTC_TimeTypeDef[0];
    period_high[i][0] = new RTC_TimeTypeDef[0];
    period_high[i][1] = new RTC_TimeTypeDef[0];
    average_low[i] = new float[0];
    average_high[i] = new float[0];
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  m5.update();
  count++; // increment the counter of all measurements
  //moment = rtc.now();
  // READING VALUES
  //Measurements of ENV III sensor
  
  if (sht30.get() == 0) {
    tmp = sht30.cTemp; 
    hum = sht30.humidity;
  }
  else {
    tmp = 0, hum = 0;
  }
  //Reading the measurements of Earth Moisture sensor
  //Reading the measurements of Earth Moisture sensor
  moisture = digitalRead(moistureSensorDigital);
  moistureanalog = analogRead(moistureSensorAnalog);
  Serial.println(moistureanalog);
  moisture1 = (100 - ((moistureanalog/4095.00)*100));
  Serial.println(moisture1);
  //Reading the measurements of Light sensor.
  lightanalog = analogRead (lightSensoranalog);
  //Printing on the screen the ENVIII measurements.
  M5.Lcd.clear ();
  M5.Lcd.setCursor(0,0);
  M5.lcd.setTextSize(4);
  M5.Lcd.printf("Temp: %2.1fC \r\nHumid: %2.0f%% \r\nMoist: %2.0f%%\r\nLight: %2.1f \r\n\n",tmp, hum, moisture1, lightanalog);
  M5.Rtc.GetTime(&moment);
  M5.lcd.setTextSize(3);
  if(moment.Minutes < 10 && moment.Seconds < 10) M5.Lcd.printf("Time: %d:0%d:0%d", moment.Hours, moment.Minutes, moment.Seconds);
  else if(moment.Seconds < 10) M5.Lcd.printf("Time: %d:%d:0%d", moment.Hours, moment.Minutes, moment.Seconds);
  else if(moment.Minutes < 10) M5.Lcd.printf("Time: %d:0%d:%d", moment.Hours, moment.Minutes, moment.Seconds);
  else M5.Lcd.printf("Time: %d:%d:%d", moment.Hours, moment.Minutes, moment.Seconds);
  Serial.println();
  // PROCESSING THE OBTAINED VALUES
  process(tmp, 0);
  process(hum, 1);
  process(moisture1, 2);
  light_count += (1 - lightanalog);
  // SEND AN EMAIL
  if(moment.Hours == report_hour && moment.Minutes > report_minute && moment.Seconds > report_second && !email) {
    send_report();
    email = true;
  }
  /*if(moment.Hours == report_hour && moment.Minutes == report_minute && moment.Seconds == report_second+1) {
    send_report();
  }
  if(moment.Hours == report_hour && moment.Minutes == report_minute && moment.Seconds == report_second+2) {
    send_report();
  }*/
  // DELAY
  delay(2000);

}