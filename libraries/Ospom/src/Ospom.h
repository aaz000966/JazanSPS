/* Ospom include.h File
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
 * Contact:  Staff@ReefAi.com
*/

#ifndef Ospom_h  //Fixes issues if people double define Ospom
#define Ospom_h

#include "Arduino.h"
#include <avr/pgmspace.h>

class Ospom   //Wrap up all functions and variables of Ospom in this class
{
 // friend void serialEvent();  //Serial event seems to work well as a friend

  public:  //Functions and variables available to people
    Ospom();
    void Run(int);
    void Setup();
    void define(int, char, int, String);
    void read(void);
    void write(void);
    void Set(void);
    int anaRead = 1;
    int digiRead = 2;
    int anaWrite = 3;
    int digiWrite = 4;
    int triac = 5;
    int flow = 6;
    int level = 7;
   
  private:  //Functions and variables only available to the Ospom library
	//Element Variables
    int ElementPin[20];			// but for eeprom do not add the extra one.
    char ElementType[20];  //s = sensor, a = actuator, n = nothing, z=SetBy EEPROM Loader Program
    int ElementFunction[20];  // 0=unused, 1=analogRead, 2=digitalRead, 3=analogWrite, 4=digitalWrite, 5=Triac 10=EEPLoaded
    int ElementVal[20][5];  // the readings from the analog input as an array
    float ElementTotalValue[20];       // Sensor=running total, actuator=value
    float ElementAvg[20];    // Sensor=average, actuator=
    float ElementCalVal[20];    //Sensor=Calibrated Value, Actuator=value
    float ElementSlopeFS[20];    //Sensor=Calibration Value M(slope), Actuator= Fail Safe Value
    float ElementYintEx[20];    //Sensor=Calibration Value B(Y-intercept), Actuator = Extra (*for triacs=Lockout(1/0))

	//Functions
    boolean incomingUSBmsg(void);
    void success(void);
    void fail(void);
    void clearTheMsg(void);
    int groupCommandVal(void);
    void sendGroupID(void);
    void sendSensorData(void);
    void sendActuatorData(void);
    void setActuator(void);
    void setGroupID(void);
    void readSensors(void);
    void elementParse(void);
    void watchDog(void);
    void triacDimming(void);
    void capacitiveSensing(void);

	//Variables
    boolean groupCommand;
    boolean ElementCommand;
    boolean stringcomplete;
    String InputString;
    char GroupID[9]; //*when declaring a char array, you add 1 more than the number of chars, 
    char WatchDogEnable;
    unsigned long WatchDogTime;
    char CommandCharArray[9];
    char ElementCommandChar;
    float ElementCommandFloat;
    int ElementCommandInt;
    int ElementNumber;
    int GroupCommandInt;
    int numReadings;
    boolean startupAid;
    boolean DogOnOff;
    boolean SendData;
    boolean Pause;
    boolean TriacDimming;
    boolean ZeroCross;
    byte NumberOfTriacs;
};


#endif
