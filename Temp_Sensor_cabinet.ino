/*
  _______  _______  _______  ___   _  _______  _______  __    _  _______  __   __
  |       ||       ||       ||   | | ||  _    ||       ||  |  | ||       ||  | |  |
  |    ___||    ___||    ___||   |_| || |_|   ||    ___||   |_| ||       ||  |_|  |
  |   | __ |   |___ |   |___ |      _||       ||   |___ |       ||       ||       |
  |   ||  ||    ___||    ___||     |_ |  _   | |    ___||  _    ||      _||       |
  |   |_| ||   |___ |   |___ |    _  || |_|   ||   |___ | | |   ||     |_ |   _   |
  |_______||_______||_______||___| |_||_______||_______||_|  |__||_______||__| |__|

  _____                 ___                          ___      _    _          _
  |_   _|__ _ __  _ __  / __| ___ _ _  ___ ___ _ _   / __|__ _| |__(_)_ _  ___| |_
   | |/ -_) '  \| '_ \ \__ \/ -_) ' \(_-</ _ \ '_| | (__/ _` | '_ \ | ' \/ -_)  _|
   |_|\___|_|_|_| .__/ |___/\___|_||_/__/\___/_|    \___\__,_|_.__/_|_||_\___|\__|
                |_|
  _               _   _    _    ___      __
  | |__  _   _    /_\ | | _(_)  ( _ )     \ \  __ _ _ __
  | '_ \| | | |  //_\\| |/ / |  / _ \/\    \ \/ _` | '_ \
  | |_) | |_| | /  _  \   <| | | (_>  < /\_/ / (_| | | | |
  |_.__/ \__, | \_/ \_/_|\_\_|  \___/\/ \___/ \__,_|_| |_|
       |___/
*/

/* Pin Out */

static int R_pin = 9;   // Red   LED,  connected to digital pin 9
static int G_pin = 10;  // Green LED,  connected to digital pin 10
static int B_pin = 11;  // Blue  LED,  connected to digital pin 11

/* Pin In */
static int Room_pIn = 4; // Room Temp sensor,   connected to analog pin 4
static int Enty_pIn = 3; // Fan Blow in  Temp sensor,   connected to analog pin 4
static int Exit_pIn = 5; // Fan Blow Out Temp sensor,   connected to analog pin 4

/* Arrays */

//Temperatur

//Log length
static int Logs = 10;
//History
static float TemperatureLog[3][Logs];

//Latest Mesured Temps
static float Temperature[3];

// Temp Pinter to help define current position in array
static int TemperatureLogIndex = 0;

//Temp History Update time
static int TemperatureUpdateTime = 60000;
//1 minut from init of application
static long TimeToUpdateTemperatureLog = TemperatureUpdateTime; 

/* Functions */

//read and set Room Temps
void SetCurrentTemp() {
  Temperature[0] = voltageToTemp(analogRead(Room_pIn)); //Room Temp
  Temperature[1] = voltageToTemp(analogRead(Enty_pIn)); //Fan Entry Temp
  Temperature[2] = voltageToTemp(analogRead(Exit_pIn)); //Exit Temp
}

//Calculated Temperature
static float voltageToTemp(float Voltage) {
  return (Voltage * 25 - 2050) / 100;
}

//set history Temp if Timer has passed
//And move pointer
static void SetNewTemperatureInLog() {
  if(TemperatureLogIndex > (Logs-1))
    { TemperatureLogIndex = 0; }
  
    TemperatureLog[0][TemperatureLogIndex] = Temperature[0];
    TemperatureLog[1][TemperatureLogIndex] = Temperature[1];
    TemperatureLog[2][TemperatureLogIndex] = Temperature[2];
    
    TemperatureLogIndex ++;
    
}

//Initilizer
void setup() {

  Serial.begin(9600);

  pinMode(R_pin, OUTPUT);   // sets the pins as output
  pinMode(G_pin, OUTPUT);
  pinMode(B_pin, OUTPUT);

  //Initilize TempHistory (Set Current Temp)
  SetCurrentTemp();
  for (int arrayPos = 0; arrayPos < 10; arrayPos++) {
    for (int tempSensor = 0 ; tempSensor < 3 ; tempSensor ++) {
      TemperatureLog[tempSensor][arrayPos] = Temperature[tempSensor];
    }
  }

}

//Main Loop
void loop() {
  
  if(TimeToUpdateTemperatureLog > TemperatureUpdateTime && millis() < TemperatureUpdateTime)
  { TemperatureUpdateTime = 0; }
  
  if(TimeToUpdateTemperatureLog <= millis() ){
    //Time To Update Temp Log Temperatur
    SetNewTemperatureInLog();
    
    //set Next update time
    TimeToUpdateTemperatureLog += TemperatureUpdateTime;
  }
  
  SetCurrentTemp();

}

