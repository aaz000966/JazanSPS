/* Ospom include.cpp File
 *
 * Copyright 2015 Scott Tomko, Greg Tomko, Linda Close
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * (GNU General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:  Staff@ReefPOM.com
*/


#include "Ospom.h"
#include "EEPROMex.h"
//#include "CapacitiveSensor.h"    //Capacitive Sensing Library

// Constructor 
// Function that handles the creation and setup of instances 



Ospom::Ospom()
{
  // initialize this instance's variables
  //set pinModes here;
  char GroupID[9] = "        ";
  String InputString = "";         // a string to hold incoming data
  boolean stringcomplete = false;  // whether the string is complete
  boolean groupCommand;
  boolean ElementCommand;
  boolean startupAid;
  int GroupCommandInt;
  char CommandCharArray[9]; // = "        ";
  char ElementCommandChar;
  int ElementCommandInt;
  int numReadings;  //Make this internet adjustable? (5)
  boolean SendData;
  boolean Pause;
  int ElementNumber;
  float ElementCommandFloat;
  boolean TriacDimming;
  boolean ZeroCross;
}
//Functions in this library are written here

  //This function is called once in Arduino's Setup, and loads all the necessary information into ram
void Ospom::Setup() {
  Serial.begin(57600);
  InputString.reserve(200);  // reserve 200 bytes for the inputString:
  NumberOfTriacs = 0;
  //Having these written out twice as const int's saves a lot of RAM
  const int  EEPIDLoc[21] = {9,28,47,66,85,104,123,142,161,180,199,218,237,256,275,294,313,332,351,370};
	//Location of Element Type (Sensor, actuator, nothing)
  const int EEPTypLoc[21] = {17,36,55,74,93,112,131,150,169,188,207,226,245,264,283,302,321,340,359,378};
	//Location of Element Function (Analog Read, Digital Read, Analog Write, ect)
  const int EEPFuncLoc[21] = {18,37,56,75,94,113,132,151,170,189,208,227,246,265,284,303,322,341,360,379};
	//Location of Element Slope(if sensor) or Fail Safe(if actuator) value
  const int EEPSlopeFSLoc[21] = {20,39,58,77,96,115,134,153,172,191,210,229,248,267,286,305,324,343,362,381};
	//Location of Element Y-intercept(if sensor) or Extra(if actuator)
  const int EEPYintExLoc[21] = {24,43,62,81,100,119,138,157,176,195,214,233,252,271,290,309,328,347,366,385};
  const int EEPPinLoc[21] = {389,391,393,395,397,399,401,403,405,407,409,411,413,415,417,419,421,423,425,427};

  //Load constant's from Arduino array's into Element-Arrays
  for (int i = 0; i < 20; i++) {
        //Add the EEPROM locations & arduino pins to the Sensor Element Arrays
    ElementType[i] = EEPROM.read(EEPTypLoc[i]);  //use the stored eeprom data if it's not from the EEPROM Loader Program
    ElementFunction[i] = EEPROM.readInt(EEPFuncLoc[i]);
    ElementSlopeFS[i] = EEPROM.readFloat(EEPSlopeFSLoc[i]);  //Loads sensor slope or actuator Fail Safe
    ElementYintEx[i] = EEPROM.readFloat(EEPYintExLoc[i]);  //Loads Sensors Y-intercept, or actuator Extra
    ElementPin[i] = EEPROM.readInt(EEPPinLoc[i]);  //Loads the pins in
    if (ElementType[i] == 's') {
      if (ElementFunction[i] == 1) {
        pinMode(ElementPin[i], INPUT);  //Set the Analog sensor pins as inputs  
      }
      else if (ElementFunction[i] == 2) {
	pinMode(ElementPin[i], INPUT_PULLUP);  //Set the Digital sensor pins as inputs w/ pullup
      }
    }
    else if (ElementType[i] == 'a') { 
      pinMode(ElementPin[i], OUTPUT);  //Set the actuator pins as outputs
      //Find out how many triacs there are
      if (ElementFunction[i] == 5) { //5=triac
        NumberOfTriacs++;
      }
    }
    // initialize all the analog readings to 0:
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
      ElementVal[i][thisReading] = 0;
    }
  }

  //Assign a new group ID if it can't find it's group ID
  if (EEPROM.read(0) != 'g') {
    EEPROM.updateBlock<char>(0,"00000000", 8);
  }
  WatchDogEnable = EEPROM.read(8);  //Watchdog Enable/Disable 
  WatchDogTime = millis();  //Delay the watchdog
  EEPROM.readBlock<char>(0, GroupID, 8);
 
      //Ask odroid for Actuator Data at Startup
  Serial.print(GroupID);
  Serial.print("/");
  Serial.println("SendActData");

  startupAid = true;
}

//This is the main function that gets called from the Arduino program
void Ospom::Run(int SensorReadDelayMS)
{
  //Having these written out twice as const int's saves a lot of RAM
  const int  EEPIDLoc[23] = {9,28,47,66,85,104,123,142,161,180,199,218,237,256,275,294,313,332,351,370};
	//Location of Element Type (Sensor, actuator, nothing)
  const int EEPTypLoc[23] = {17,36,55,74,93,112,131,150,169,188,207,226,245,264,283,302,321,340,359,378};
	//Location of Element Function (Analog Read, Digital Read, Analog Write, ect)
  const int EEPFuncLoc[23] = {18,37,56,75,94,113,132,151,170,189,208,227,246,265,284,303,322,341,360,379};
	//Location of Element Slope(if sensor) or Fail Safe(if actuator) value
  const int EEPSlopeFSLoc[23] = {20,39,58,77,96,115,134,153,172,191,210,229,248,267,286,305,324,343,362,381};
	//Location of Element Y-intercept(if sensor) or Extra(if actuator)
  const int EEPYintExLoc[23] = {24,43,62,81,100,119,138,157,176,195,214,233,252,271,290,309,328,347,366,385};

  unsigned long TimeNow = 0;
  static unsigned long SensorStreamStartTime;
  static unsigned long LastMeasurement;
  static unsigned long LastSend;
  static unsigned int SendDelay;

  if (TriacDimming) {
    triacDimming();
  }

  if (incomingUSBmsg()) {
    if (groupCommand) {
      //Do Group Command Stuff
      switch (groupCommandVal()) {
        case 0:
          sendGroupID();
          break;
        case 10:
          sendSensorData();
          break;
        case 11:
	  SendDelay = GroupCommandInt;  //Grab the time in milliseconds
          SensorStreamStartTime = millis();                   //to delay before sending again
          SendData = true;
          break;
        case 13:
          sendActuatorData();
          break;
        case 17:
          watchDog();
          break;
        case 18:
          setGroupID();
          break;
        default:
          fail();
          break;
      }
    }
	//Element Command Section accepts commands and takes action
    else if (ElementCommand) {
	//Store the Element ID we are working with in a temporary variable
      char ElementID[9] = "        ";  //A temporary Variable for the ELement ID
      EEPROM.readBlock<char>(EEPIDLoc[ElementNumber], ElementID, 8);  //Reads the Element ID from EEPROM
      //Do what the command Character says
      if (ElementCommandChar == 'R') {
         //Read the element value, and send it back to the odroid 
        Serial.print(GroupID);
        Serial.print("/");
        Serial.print(ElementID);
        Serial.print(":");
        Serial.println(ElementCalVal[ElementNumber]);
      }
      else if (ElementCommandChar == 'I') {
         //Change the Elements ID
        EEPROM.updateBlock<char>(EEPIDLoc[ElementNumber], CommandCharArray, 8);
        success();
      }
      else if (ElementCommandChar == 'T') {
         //Change the Elements Type (s = sensor, a = actuator, n = nothing, z=SetBy EEPROM Loader Program)
	EEPROM.write(EEPTypLoc[ElementNumber],CommandCharArray[0]);
	ElementType[ElementNumber] = EEPROM.read(EEPTypLoc[ElementNumber]);
        success();
      }
      else if (ElementCommandChar == 'C') {
         //Change the Elements Function (0=unused, 1=analogRead, 2=digitalRead, 3=analogWrite, 
			// 4=digitalWrite, 5=Triac, 10=SetBy EEPROM Loader Program)
	EEPROM.updateInt(EEPFuncLoc[ElementNumber], ElementCommandInt);
	ElementFunction[ElementNumber] = EEPROM.readInt(EEPFuncLoc[ElementNumber]);
        success();
      }
      else if (ElementCommandChar == 'A') {
	//Read the Element Type and Send it back to the odroid
	Serial.print(GroupID);
        Serial.print("/");
        Serial.print(ElementID);
        Serial.print(":");
        Serial.println(ElementType[ElementNumber]);
      }
      else if (ElementCommandChar == 'D') {
	//Read the Element Function and Send it back to the odroid
	Serial.print(GroupID);
        Serial.print("/");
        Serial.print(ElementID);
        Serial.print(":");
        Serial.println(ElementFunction[ElementNumber]);
      }
      else if (ElementCommandChar == 'M') {
        //Change the Elements Calibration Slope
        EEPROM.updateFloat(EEPSlopeFSLoc[ElementNumber], ElementCommandFloat);  //Save value to EEPROM);
        ElementSlopeFS[ElementNumber] = ElementCommandFloat;
        success();
      }
      else if (ElementCommandChar == 'B') {
        //Change the Elements Calibration Slope
        EEPROM.updateFloat(EEPYintExLoc[ElementNumber], ElementCommandFloat);  //Save value to EEPROM);
        ElementYintEx[ElementNumber] = ElementCommandFloat;
        success();
      }
      else if (ElementCommandChar == 'W') {
	//Write (Set) the Actuator
	if (ElementType[ElementNumber] == 'a') {  //Make sure it's an actuator (so we don't break sensors)
	  if ((ElementFunction[ElementNumber] == 3) && (ElementCommandInt != 100) && (ElementCommandInt != 0)) {  //AnalogWrite
	    if ((ElementCommandInt > 0) && (ElementCommandInt < 100)) {
	      ElementCalVal[ElementNumber] = ElementCommandInt;  //Save the Actuator Value so it can tell what it is if asked
	      ElementCommandInt = ElementCommandInt * 255 / 100; //convert to 0-255 for PWM
	      analogWrite(ElementPin[ElementNumber], ElementCommandInt);  //AnalogWrite (PWM) if it's a 1-99 value
	      success();
	    }
	    else {
	      fail();
	    }
	  }     //This does the digital writes, and also takes care of 100% on and 100% off PWM dimming
	  else if ((ElementFunction[ElementNumber] == 4) || (ElementFunction[ElementNumber] == 3)) {  //DigitalWrite
	    ElementCalVal[ElementNumber] = ElementCommandInt;  //Save the Actuator Value so it can tell what it is if asked
	    if (ElementCommandInt == 0) { //0 means turn off the actuator
	      digitalWrite(ElementPin[ElementNumber], LOW);
	      success();
	    }
	    else if (ElementCommandInt == 100) {  //100 means turn on the actuator
	      digitalWrite(ElementPin[ElementNumber], HIGH);
	      success();
	    }
	    else {
	      fail();
	    }
	  }
	  else if (ElementFunction[ElementNumber] == 5) { //Triac Dimming 115v/240v AC Outlets
	      //Decides if the input was appropriate, and sets the Stored Value betwenn 0 and 100 %
	    if (ElementCommandInt == 0) {
	      digitalWrite(ElementPin[ElementNumber], LOW);
	      ElementCalVal[ElementNumber] = ElementCommandInt;  //Save the Actuator Value so it can tell what it is if asked
	      success();
	    }
	    else if (ElementCommandInt == 100) {  //100 means turn on the outlet all the way
	      digitalWrite(ElementPin[ElementNumber], HIGH);
	      ElementCalVal[ElementNumber] = ElementCommandInt;
	      success();
	    }
	    else if ((ElementCommandInt > 0.00) && (ElementCommandInt < 100.00)) {
	      ElementCalVal[ElementNumber] = ElementCommandInt;
	      ElementTotalValue[ElementNumber] = ElementCalVal[ElementNumber];
	      ElementTotalValue[ElementNumber] = 80 * ElementTotalValue[ElementNumber];
	      ElementTotalValue[ElementNumber] = 8000 - ElementTotalValue[ElementNumber];
	      ElementYintEx[ElementNumber] = 1;
	    //  ElementCalVal[ElementNumber] = 80 * ElementCalVal[ElementNumber];  //convert to Delay:
            //  ElementCalVal[ElementNumber] = 8000 - ElementCalVal[ElementNumber];  // MilliDelay=8300-(83*%Dim)
	      Serial.println(ElementCalVal[ElementNumber]);
	      Serial.println(ElementTotalValue[ElementNumber]);
	      success();
	    }
	    else {
	      fail();
	    }
	      //Sets the TriacDimming Boolean to true if any of the outlets are dimmed between 1 and 99%
	    TriacDimming = false;
	    for (int i = 0; i < 20; i++) {
   	     if (ElementType[i] == 'a') {   //s = sensor, a = actuator, n = nothing
    	       if (ElementFunction[i] == 5) {  // 5=Triac Dimming
		 if ((ElementCalVal[i] < 100.00) && (ElementCalVal[i] > 0.00)) {
		   TriacDimming = true;
		 }
	       }
	     }
	   }
	  }
	  else {
	    fail();
	  }
   	}
	else {
	  fail();
	}
      }
      else if (ElementCommandChar == 'S') {  //Not sure if storing an in where a float goes sometimes will matter
	//Set Fail Safe Value
	EEPROM.updateInt(EEPSlopeFSLoc[ElementNumber], ElementCommandInt);
	ElementSlopeFS[ElementNumber] = ElementCommandInt;
        success();
      }
      else if (ElementCommandChar == 'F') {
	//Read Fail Safe Value
	Serial.print(GroupID);
        Serial.print("/");
        Serial.print(ElementID);
        Serial.print(":");
        Serial.println(EEPROM.readInt(EEPSlopeFSLoc[ElementNumber]));
      }
      else {
        fail();  //Return a -1
      }
      clearTheMsg(); 

    }
    else {
      fail();
    }

    clearTheMsg();
  }

  //Measure sensor elements (with the delay decided in the main sketch), put them in an array
  TimeNow = millis();  //Check the time
  if (TimeNow > LastMeasurement + SensorReadDelayMS) {
    LastMeasurement = millis();
    readSensors();  
  }

  //Send a Stream of Sensor Data for 1 minute if it has been requested
  if (SendData) {
    if (SensorStreamStartTime + 60000 > TimeNow)  {  //Send Data Stream
      if (Pause) {
        Pause = false;
      }
      else {
        if (TimeNow > LastSend + SendDelay) {
          sendSensorData();
          LastSend = millis();
        }
      }
    }
    else {  //  Stop Sending Data
      SendData = false;
      SensorStreamStartTime = 0;
      SendDelay = 0;
      LastSend = 0;
    }
  }
}


void Ospom::success(void)
{
  Serial.print(GroupID);  //Print out ID
  Serial.print("/");
  Serial.println("1");
  clearTheMsg();
}

void Ospom::fail(void)
{
  Serial.print(GroupID);  //Print out ID
  Serial.print("/");
  Serial.println("-1");
  clearTheMsg();
}

void Ospom::clearTheMsg(void)
{
  InputString = "";
  stringcomplete = false;
  groupCommand = false;
  ElementCommand = false;
  for(int i = 0; i < 8; i++) {
    CommandCharArray[i] = ' ';
  }   
  ElementCommandChar = ' ';
  ElementNumber = 0;
  ElementCommandFloat = 0;
  ElementCommandInt = 0;
}


int Ospom::groupCommandVal(void) {
       //Secondary Serial String Processing Input to use in group commands
  int CommandInt = InputString.charAt(8) - '0';  //converting char to int
  int Comand1 = InputString.charAt(9) - '0';  //converting char 2 to int
  if ((Comand1 >= 0) && (Comand1 <= 9) && (Comand1 != ':')) {
    CommandInt = CommandInt * 10;
    CommandInt += Comand1;  
  } 
//This part gets further command info
  String serialReadString = "";
  if (((InputString.charAt(9) - '0') >= 0) && ((InputString.charAt(9) - '0') <=9)) {  //If it is a 2 digit request
    for (int i = 11; i < InputString.length(); i++) {
      char ActuatorRead = InputString.charAt(i);
      serialReadString += ActuatorRead;
    }
  }
  else {   //If it is a 1 digit request
    for (int i = 10; i < InputString.length(); i++) {
      char ActuatorRead = InputString.charAt(i);
      serialReadString += ActuatorRead;
    }
  }
  serialReadString.toCharArray(CommandCharArray, serialReadString.length());    //convert to Char array
  GroupCommandInt = serialReadString.toInt();  //An integer of the serial input
  return CommandInt;
}

void Ospom::sendGroupID(void) {
  Serial.print(GroupID);
  Serial.print("/");
  Serial.println(GroupID);
  clearTheMsg();
}

void Ospom::sendSensorData(void) {
//Send Back Calibrated Sensor Data to Ai
    //ToDo: Put this array in PROGMEM
  const int  EEPIDLoc[23] = {9,28,47,66,85,104,123,142,161,180,199,218,237,256,275,294,313,332,351,370};
  Serial.print(GroupID);  //Group ID
  Serial.print("/");
    //Count How many Sensors we have to send data for
  int NumberOfSensors = 0;
  for (int i = 0; i < 20; i++) {   
    if ((ElementType[i] == 's') && (ElementFunction[i] != 0)) {
      NumberOfSensors++;
    }
  }
    //Send The Data
  int NumberOfSentSensors = 0;
  for (int i = 0; i < 20; i++) {   
    if ((ElementType[i] == 's') && (ElementFunction[i] != 0)) {   //Type: s = sensor, a = actuator, n = nothing
		 //Function: 0=unused, 1=analogRead, 2=digitalRead, 3=analogWrite, 4=digitalWrite, 5=triac, 10=EEPLoaded
      NumberOfSentSensors++;
      char ElementID[9] = "        ";  //A temporary Variable for the ELement ID
      EEPROM.readBlock<char>(EEPIDLoc[i], ElementID, 8);  //Reads the Element ID from EEPROM
      Serial.print(ElementID);
      Serial.print(":");
      Serial.print(ElementCalVal[i]);
      if (NumberOfSentSensors < NumberOfSensors) {  //This makes it so there isnt an extra comma at the end.
	Serial.print(",");
      }
      else {
	Serial.println();
      }
    }
  }
  clearTheMsg();
}


void Ospom::sendActuatorData(void) {
   //ElementCalVal when speaking of an Actuator refers to it's value
    //ToDo: Put this array in PROGMEM
  const int  EEPIDLoc[23] = {9,28,47,66,85,104,123,142,161,180,199,218,237,256,275,294,313,332,351,370};
  Serial.print(GroupID);  //Group ID
  Serial.print("/");
    //Count How many Actuators we have to send data for
  int NumberOfActuators = 0;
  for (int i = 0; i < 20; i++) {   
    if ((ElementType[i] == 'a') && (ElementFunction[i] != 0)) {
      NumberOfActuators++;
    }
  }
   //Send The Data
  int NumberOfSentActuators = 0;
  for (int i = 0; i < 20; i++) {   
    if ((ElementType[i] == 'a') && (ElementFunction[i] != 0)) {   //Type: s = sensor, a = actuator, n = nothing
		 //Function: 0=unused, 1=analogRead, 2=digitalRead, 3=analogWrite, 4=digitalWrite, 5=triac, 10=EEPLoaded
      char ElementID[9] = "        ";  //A temporary Variable for the ELement ID
      NumberOfSentActuators++;
      EEPROM.readBlock<char>(EEPIDLoc[i], ElementID, 8);  //Reads the Element ID from EEPROM
      Serial.print(ElementID);
      Serial.print(":");
      Serial.print(ElementCalVal[i]);
      if (NumberOfSentActuators < NumberOfActuators) {  //This makes it so there isnt an extra comma at the end.
	Serial.print(",");
      }
      else {
	Serial.println();
      }
    }
  }
  clearTheMsg();
}



void Ospom::setGroupID(void) {
  EEPROM.updateBlock<char>(0,CommandCharArray, 8);  //Write ID
  EEPROM.readBlock<char>(0, GroupID, 8);  //Read ID
  success();
}

//Deals with incoming Serial Messages
boolean Ospom::incomingUSBmsg(void) {
 while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    InputString += inChar;
    // if the incoming character is a '!', see who it's for
    if (inChar == '!') {
      String GroupIDIn = "";
      for (int i = 0; i<= 7; i++) {  //Read the ID
        char inChar = InputString.charAt(i);
        GroupIDIn += inChar;
      }
      if (GroupIDIn == GroupID) {   //If the ID from serial matches the ID from eeprom
      if (SendData) {
      Pause = true;  //if it's sending continuous data, stop for a bit
      }
        //decide what kind of message it is
	if ((InputString.charAt(8) == 's') || (InputString.charAt(8) == 'a')) {
          ElementCommand = true;
	  stringcomplete = true;
          elementParse();
	}
        else if (((InputString.charAt(8) - '0') >= 0) && ((InputString.charAt(8) - '0') <=9)) {
          groupCommand = true;
	  stringcomplete = true;
        }
        else {
          fail();
  	}
      }
      else if ((InputString.charAt(0) == '0') && (InputString.length() == 2)) {
        sendGroupID();
     }
     else {
       fail();  //Return a -1
     }
     GroupIDIn = "";
    }

  }
  return stringcomplete;
}

void Ospom::elementParse(void) {
//############################  WORKING ON THIS SECTION NOW Also ##############################################################
  const int  EEPIDLoc[23] = {9,28,47,66,85,104,123,142,161,180,199,218,237,256,275,294,313,332,351,370};
 char ElementIDIn[9] = "        ";
 String serialReadString = "";
      //Load the Element ID
  for (int i = 8; i < 16; i++) {  
    char InChar = InputString.charAt(i);
    serialReadString += InChar;
  }
      //convert to Char array for comparison
  serialReadString.toCharArray(ElementIDIn, (serialReadString.length()+1));  //+1 to make it work
  serialReadString = ""; 
      //Compare it to the string of stored element id's, and say which # it is
  boolean foundElement = true;
  for (int i = 0; i < 20; i++) {
      char ElementID[9] = "        ";  //A temporary Variable for the ELement ID
      EEPROM.readBlock<char>(EEPIDLoc[i], ElementID, 8);  //Reads the Element ID from EEPROM
      if (strcmp(ElementID, ElementIDIn) == 0) {  //strcmp compares the two char arrays & returns a 0 if they are the same
        ElementNumber = i;
        foundElement = false;
      }
      if ((i == (20 - 1)) && (foundElement)) {
        fail();
      }
  }
  //clear the string
  serialReadString = "";
     //Load the Element Command Character  
  ElementCommandChar = InputString.charAt(16);
     //Load the Element Command Value
  for (int i = 18; i < 26; i++) {  
    char InChar = InputString.charAt(i);
    serialReadString += InChar;
  }
  serialReadString.toCharArray(CommandCharArray, (serialReadString.length()+1));   //+1 to make it work
  ElementCommandFloat = serialReadString.toFloat();
  ElementCommandInt = serialReadString.toInt();
  serialReadString = ""; 
}

void Ospom::readSensors(void) {
  static int index;
  static int counter;
  static int counter1;
  numReadings = 5;
  boolean advance = true;

  for (int i = 0; i < 20; i++) {
    if (ElementType[i] == 's') {   //s = sensor, a = actuator, n = nothing
      if (ElementFunction[i] == 1) {  // 0=unused, 1=analogRead, 2=digitalRead, 3=analogWrite, 4=digitalWrite, 5=triac
						//6=Flow, 7=Level, 10=Loaded by the generic EEPROMLoader program
	//Analog Reading Section
	ElementVal[i][index] = ElementVal[i][index] + analogRead(ElementPin[i]); // add 3 sensor readigns together
/*        Serial.print("AnalogRead Pin: ");
	Serial.print(ElementPin[i]);
	Serial.print(", ");
	Serial.println(analogRead(ElementPin[i]));
*/
      }
    }
  }
  counter++;
  if (counter > 2) {
    counter = 0;
    for (int i = 0; i < 20; i++) {
      if (ElementType[i] == 's') {
        if (ElementFunction[i] == 1) {
	  ElementTotalValue[i] = ElementTotalValue[i] + ElementVal[i][index]; // add the reading to the total
	    // calculate the average: 
	  if (startupAid){
	    ElementAvg[i] = ElementTotalValue[i] / ((index + 1) * 3);   //fixes wrong avg at the beginning
	  }
	  else {
	    ElementAvg[i] = (ElementTotalValue[i] / (numReadings * 3));
	  }
	    //Calibrate
	  ElementCalVal[i] = ElementAvg[i] - ElementYintEx[i];
	  ElementCalVal[i] = ElementCalVal[i] / ElementSlopeFS[i];
	  }
	}
      }
/*
Serial.print("index: ");
Serial.println(index);
Serial.print("counter: ");
Serial.println(counter);
Serial.print("counter1: ");
Serial.println(counter1);
Serial.print("ElementVal[i][index]: ");
Serial.println(ElementVal[9][index]);
Serial.print(ElementVal[9][0]);
Serial.print(",");
Serial.print(ElementVal[9][1]);
Serial.print(",");
Serial.print(ElementVal[9][2]);
Serial.print(",");
Serial.print(ElementVal[9][3]);
Serial.print(",");
Serial.println(ElementVal[9][4]);	
*/

    index++;
    if (index >= numReadings) {
      index = 0;
      startupAid = false;
    }
    //Clean Up
    for (int i = 0; i < 20; i++) {
      if (ElementType[i] == 's') {
        if (ElementFunction[i] == 1) {
    	  ElementTotalValue[i] = ElementTotalValue[i] - ElementVal[i][index]; // subtract the last reading:
    	  ElementVal[i][index] = 0;  //clear the added up element value
	}
      }
    }
/*
Serial.println("After the Cleanup");
Serial.print("ElementVal[i][index]: ");
Serial.println(ElementVal[9][index]);
Serial.print(ElementVal[9][0]);
Serial.print(",");
Serial.print(ElementVal[9][1]);
Serial.print(",");
Serial.print(ElementVal[9][2]);
Serial.print(",");
Serial.print(ElementVal[9][3]);
Serial.print(",");
Serial.println(ElementVal[9][4]);

Serial.print("ElementTotalValue: ");
Serial.println(ElementTotalValue[9]);
Serial.print("ElementAvg: ");
Serial.println(ElementAvg[9]);
Serial.print("ElementCalVal: ");
Serial.println(ElementCalVal[9]);
*/
  }



/*
  for (int i = 0; i < 20; i++) {
    if (ElementType[i] == 's') {   //s = sensor, a = actuator, n = nothing
      if (ElementFunction[i] == 1) {  // 0=unused, 1=analogRead, 2=digitalRead, 3=analogWrite, 4=digitalWrite, 5=triac
						//6=Flow, 7=Level, 10=Loaded by the generic EEPROMLoader program
	//Analog Reading Section
Serial.print("ElementVal[i][index]: ");
Serial.println(ElementVal[i][index]);
	ElementVal[i][index] = ElementVal[i][index] + analogRead(ElementPin[i]); // add 3 sensor readigns together

if (i == 9) {
Serial.print("ElementNumber: ");  //########DEBUGGING
Serial.println(i+1);
Serial.print("index: ");
Serial.println(index);
Serial.print("counter: ");
Serial.println(counter);
Serial.print("counter1: ");
Serial.println(counter1);
Serial.print("ElementVal[i][index]: ");
Serial.println(ElementVal[i][index]);
Serial.print(ElementVal[i][0]);
Serial.print(",");
Serial.print(ElementVal[i][1]);
Serial.print(",");
Serial.print(ElementVal[i][2]);
Serial.print(",");
Serial.print(ElementVal[i][3]);
Serial.print(",");
Serial.println(ElementVal[i][4]);	
}

	if (counter > 2) {  //
	  ElementTotalValue[i] = ElementTotalValue[i] + ElementVal[i][index]; // add the reading to the total:
	  // calculate the average: 
	  if (startupAid){
  	    ElementAvg[i] = ElementTotalValue[i] / ((index + 1) * 3);   //fixes wrong avg at the beginning
	  }
	  else {
  	     ElementAvg[i] = (ElementTotalValue[i] / (numReadings * 3));
	  }
	  //Calibrate
  	  ElementCalVal[i] = ElementAvg[i] + ElementYintEx[i];
  	  ElementCalVal[i] = ElementCalVal[i] / ElementSlopeFS[i];
//Serial.print("ElementCalVal[i]: ");
//Serial.println(ElementCalVal[i]);

	  // advance to the next position in the array:  
//***Something Seems Fishy around here****
	  if (advance) {  //Index everthing forward once the for-loop runs 3 times
  	    index = index + 1;
	    counter = 0;
	    advance = false;
	  }
Serial.println("zeroing ElementVal[i][index]");
	  ElementTotalValue[i] = ElementTotalValue[i] - ElementVal[i][index]; // subtract the last reading:
	  ElementVal[i][index] = 0;  //clear the added up element value
Serial.print("ElementVal[i][index]: ");
Serial.println(ElementVal[i][index]);
	  if (index >= numReadings) {    
            // ...wrap around to the beginning:
            index = 0;   
	    ElementTotalValue[i] = ElementTotalValue[i] - ElementVal[i][index];
	    ElementVal[i][index] = 0;
            startupAid = false;
          } 
	}


Serial.print("Slope: ");
Serial.println(ElementSlopeFS[i]);
Serial.print("Y-int: ");
Serial.println(ElementYintEx[i]);
Serial.print("ElementVal[]: ");
Serial.println(ElementVal[i][index]);
Serial.print("ElementTotalValue: ");
Serial.println(ElementTotalValue[i]);
Serial.print("ElementAvg: ");
Serial.println(ElementAvg[i]);
Serial.print("ElementCalVal: ");
Serial.println(ElementCalVal[i]);

      }
      else if (ElementFunction[i] == 2) {
	//Digital Reading Section
	ElementVal[i][counter1] = digitalRead(ElementPin[i]);
	counter1++;
	if (counter1 > 2) {
	  counter1 = 0;
	  if ((ElementVal[i][0] == 1) && (ElementVal[i][1] == 1) && (ElementVal[i][2] == 1)) {
	    ElementCalVal[i] = 1;
	  }
	  else if ((ElementVal[i][0] == 0) && (ElementVal[i][1] == 0) && (ElementVal[i][2] == 0)) {
	    ElementCalVal[i] = 0;
	  }
	  else {
	    ElementCalVal[i] = ElementCalVal[i];
	  }
	}
      }
      //Complex sensor functions such as Level and Flow will be in the sketch
    }
  }  
*/
}



void Ospom::watchDog() {  //The WatchDog Timer Function

  if (GroupCommandInt == 0) {
    //Turn Off the WatchDog Timer
    success();
  }
  else if (GroupCommandInt == 1) {
    //Turn On the WatchDog Timer
    //Have this enter Fail Safe Modes?
    //Have it try and reset the arduino
    success();
  }
  else {
    fail();
  }
  //  void software_Reset()   <--this should do a reset
  //More info here: https://github.com/WickedDevice/SoftReset
}



void Ospom::triacDimming(void) { //Function that deals with dimming 115 or 230v AC Triac Outlets
  unsigned long ZeroCrossTime;
  static unsigned long LastCrossTime;
  static unsigned long LastPulse[20];
  int TriacDelayMicro = 0; 
  boolean ZeroCross;
  unsigned long TimeNowMicro = micros();  //Read the time right now

  ZeroCross = digitalRead(2);
  if (ZeroCross) {
    ZeroCrossTime = TimeNowMicro;
  }
  if ((ZeroCross) && (ZeroCrossTime > LastCrossTime + 7000)) {  //avoids repeat ZeroCross measurements
    LastCrossTime = ZeroCrossTime;                              //There are 8300micro seconds in half a sign wave @ 60Hz
  }
	
  for (int i = 0; i < NumberOfTriacs; i++) {
    if (ElementType[i] == 'a') {   //s = sensor, a = actuator, n = nothing
      if (ElementFunction[i] == 5) {  // 5=Triac Dimming
	if (ElementYintEx[i] == 1) {
	//Do The dimming
//	TriacDelayMicro = 80 * ElementCalVal[i];  //convert to Delay:
//        TriacDelayMicro = 8000 - TriacDelayMicro;  // MilliDelay=8300-(83*%Dim)
	TriacDelayMicro = ElementTotalValue[i];
  	if ((TimeNowMicro > LastCrossTime + TriacDelayMicro) && (TimeNowMicro < LastCrossTime + 8000) && (TimeNowMicro > LastPulse[i] + 8000 - TriacDelayMicro)) {
    	  digitalWrite(ElementPin[i], HIGH);   //Turn on the Triac if it's time, and you havent already just done it     
	  LastPulse[i] = TimeNowMicro;  //the last time the particular triac was pulsed on
  	}
      }
    }
      //Turn off Triacs so they don't stick on at zero cross
    if (TimeNowMicro > LastCrossTime + 7000) {
      digitalWrite(ElementPin[i], LOW);
    }
    }
  } 
}

void Ospom::capacitiveSensing() {
  //This function does all the capacitive sensing
/*  int FillPins[] = {3,4,13,11,10,5,6,7,8,9,12};
  //Recieve 2 input integers, fill and sense
  for (int i=0; i<20; i++) {
    if (ElementType[i] == 's') {   //s = sensor, a = actuator, n = nothing
      if (ElementFunction[i] == 8) {  // 8=Capacitive Sensing
   	CapacitiveSensor   capSensor1 = CapacitiveSensor(2,pins[i]);
   	LevelSensor1.set_CS_Timeout_Millis(4000);
  	 LevelSensor1.set_CS_AutocaL_Millis(0xFFFFFFFF);

   	int CapVal1 = LevelSensor1.capacitiveSensorRaw(4);
   	Serial.print("#");
   	Serial.print(i);
   	Serial.print(": ");
   	Serial.println(CapVal1);
      }
    }
  }
*/
}


void Ospom::define(int PinNumber, char PinType, int PinFunction, String PinID) {
  //Having these written out twice as const int's saves a lot of RAM
  const int  EEPIDLoc[23] = {9,28,47,66,85,104,123,142,161,180,199,218,237,256,275,294,313,332,351,370};
	//Location of Element Type (Sensor, actuator, nothing)
  const int EEPTypLoc[23] = {17,36,55,74,93,112,131,150,169,188,207,226,245,264,283,302,321,340,359,378};
	//Location of Element Function (Analog Read, Digital Read, Analog Write, ect)
  const int EEPFuncLoc[23] = {18,37,56,75,94,113,132,151,170,189,208,227,246,265,284,303,322,341,360,379};
	//Location of Element Slope(if sensor) or Fail Safe(if actuator) value
  const int EEPSlopeFSLoc[23] = {20,39,58,77,96,115,134,153,172,191,210,229,248,267,286,305,324,343,362,381};
	//Location of Element Y-intercept(if sensor) or Extra(if actuator)
  const int EEPYintExLoc[23] = {24,43,62,81,100,119,138,157,176,195,214,233,252,271,290,309,328,347,366,385};
	Serial.println("Running ospom.define");
//#1 - Decide what Element Number We are going to use:
  boolean PinExists = false;
  for (int i=0; i<20; i++) {  //If the Pin Number is already stored, 
    if (PinNumber == ElementPin[i]) {
      ElementNumber = i;
      PinExists = true;
      break;
    }
  }
  if (PinExists == false) {  //If the Pin does not exist, assign the pin to the first empty space in the Element Array
    for (int i=0; i<20; i++) {
      if (EEPROM.read(EEPTypLoc[i]) == 'z') {  //Elements are empty if their Type is 'Z' which was loaded generically
        ElementNumber = i;
	break;
      }
    }
  }
//#2 - Fill up the element with the right info

  ElementPin[ElementNumber] = PinNumber;
  ElementType[ElementNumber] = PinType;
  EEPROM.updateInt(EEPTypLoc[ElementNumber], PinType);
  ElementFunction[ElementNumber] = PinFunction;
  EEPROM.updateInt(EEPFuncLoc[ElementNumber], PinFunction);
  char tempID[9] = "        ";
  PinID.toCharArray(tempID,9);  //Convert string to char and set the Element ID
  EEPROM.updateBlock<char>(EEPIDLoc[ElementNumber], tempID, 8);  //Store ID in EEPROM

	Serial.print("Pin Number: ");
	Serial.println(ElementPin[ElementNumber]);
	Serial.print("ElementType: ");
	Serial.println(ElementType[ElementNumber]);
	Serial.print("ElementTypeFromEEPROM: ");
	Serial.println(EEPROM.read(EEPTypLoc[ElementNumber]));
        char TempType = EEPROM.read(EEPTypLoc[ElementNumber]);
	Serial.print("TempType: ");
	Serial.print(TempType);
	Serial.print("Element ID: ");
	Serial.println(tempID);
       
}

void Ospom::read(void) {
 boolean ElementFound = false;
/*  for (int i=0; i<20; i++) {
    if (PinNumber == ElementPin[i]) {
      ElementNumber = i;	//The Element Number related to the Pin they are talking about
      ElementFound = true;
      break
    }
  }
*/
}

void Ospom::write(void) {
 boolean ElementFound = false;
/*  for (int i=0; i<20; i++) {
    if (PinNumber == ElementPin[i]) {
      ElementNumber = i;	//The Element Number related to the Pin they are talking about
      ElementFound = true;
      break
    }
  }
*/
}

void Ospom::Set(void) {

}










