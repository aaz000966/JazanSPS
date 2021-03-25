/*  Reef POM Example Sketch
 *     For more information visit BitBucket.org/ReefPOM
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
 *
 *
Example Arduino Pinout (Reef POM Core Power Bar): 
2 AC Phase Input
11 Traic #1 (new)
3 Triac #2
4 Triac #3
5 0-10v #1
6 0-10v #4
7 Triac #4
8 Bottom LED
9 0-10v #2
10 0-10v #3
ADC6 Thermisor (new)
ADC7 pH Reading
12  DIY Pin #3
13  DIY Pin #4
14 Right LED
15 Center LED
16 Left LED
17 Top LED
18 DIY Pin #2 (SDA)
19 DIY Pin #1 (SCL)

Example Element List (Core Power Bar)
Element, Function, Pin#
1  ,  Triac#1  ,  11
2  ,  Triac#2  ,  3
3  ,  Triac#3  ,  4
4  ,  Triac#4  ,  7
5  ,  0-10v#1  ,  5
6  ,  0-10v#2  ,  9
7  ,  0-10v#3  ,  10
8  ,  0-10v#4  ,  6
9 ,  PCBtemp  ,  ADC6 (20)
10 ,  pH       ,  ADC7 (21)
11 ,  DIY#1    ,  19 (SCL)  
12 ,  DIY#2    ,  18 (SDA)
13 ,  DIY#3    ,  12
14 ,  DIY#4    ,  13
15 ,  cntrLED  ,  15
*/

#include <Ospom.h>  //include the ospom library
#include <EEPROMex.h>  //This must be included for Ospom to work
Ospom ospom;  // instantiate ospom, an instance of Ospom Library  Don't put () if it isn't getting a variable

//**If doing Triac Dimming, set phase pin in Ospom.cpp at line 682**  ToDo: make this setable here & internet

void setup() {  // put your setup code here, to run once:
  ospom.Setup();  //ospom.Setup initilizes ospom 
}

void loop() {
  ospom.Run(333);  //Activates the ospom library every time the loop runs **This is required
        //(333 = Read Sensors 3 times a second, 1000 = 1 time a second, )
    /*
    Put any standard arduino code in here that you want to run over and over again.
    OSPOM Functions:
    ospom.define(Pin#, Type, Function, ID);  = Define OSPOM Internet Dashboard accessable Sensor or Actuator Elements
               example: ospom.define(7, 's', ospom.anaRead, "stfw0000");
    ospom.read(Pin#, ReadType);  = Read OSPOM Sensors or Actuators and recieve the result
    ospom.write(Pin#, WriteType);  = Write OSPOM Sensors or Actuators using this function
    ospom.webSet(Pin#, Value); = Sets the value of the Element with the Pin# specified, so the
  					Dashboard will get the new data
    	**Writing a pin that is part of the OSPOM dashboard without using this function will cause
           the dashboard to be confused
    **Dont forget to Write your own OSPOM Configuation Sketch to setup your arduino, 
       or run the OSPOM Generic Configuration sketch at least once before calling any ospom functions
    */
}
