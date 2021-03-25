/*This program loads an arduino (in this case ReefPOM's Core Power Bar)
 * with necessary EEPROM data for the library to function properly.
 *Run this configuration program first, then the sketch named 'ospom'
 *For more information visit BitBucket.org/ReefPOM
 

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
 *
 *
Arduino Pinout: 
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
*/
/* Core Power Bar Element List
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
**AC Phase Pin 2 can't be an element, as they arent fast enough
*/

#include <EEPROMex.h>  //For Calibration Data and ID

void setup() {
    // put your setup code here, to run once:
    //Load stock Sensor Element ID's
  EEPROM.updateBlock<char>(0,"gcp00000", 8);   //ID
      
  EEPROM.update(8, 1);  //Enable Watchdog Timer
  EEPROM.updateBlock<char>(9,"atr00001", 8);   //Element #1 ID
  EEPROM.updateBlock<char>(28,"atr00002", 8);  //Element #2 ID
  EEPROM.updateBlock<char>(47,"atr00003", 8);  //Element #3 ID
  EEPROM.updateBlock<char>(66,"atr00004", 8);  //Element #4 ID
  EEPROM.updateBlock<char>(85,"azt00001", 8);  //Element #5 ID
  EEPROM.updateBlock<char>(104,"azt00002", 8);  //Element #6 ID
  EEPROM.updateBlock<char>(123,"azt00003", 8);  //Element #7 ID
  EEPROM.updateBlock<char>(142,"azt00004", 8);  //Element #8 ID
  EEPROM.updateBlock<char>(161,"stfb0001", 8);  //Element #9 ID
  EEPROM.updateBlock<char>(180,"sph00001", 8);  //Element #10 ID
  EEPROM.updateBlock<char>(199,"sdi00001", 8);  //Element #11 ID
  EEPROM.updateBlock<char>(218,"sdi00002", 8);  //Element #12 ID
  EEPROM.updateBlock<char>(237,"sdi00003", 8);  //Element #13 ID
  EEPROM.updateBlock<char>(256,"sdi00004", 8);  //Element #14 ID
  EEPROM.updateBlock<char>(275,"zzzzzzzz", 8);  //Element #15 ID  //Fill the unused ones with z's
  EEPROM.updateBlock<char>(294,"zzzzzzzz", 8);  //Element #16 ID
  EEPROM.updateBlock<char>(313,"zzzzzzzz", 8);  //Element #17 ID
  EEPROM.updateBlock<char>(332,"zzzzzzzz", 8);  //Element #18 ID
  EEPROM.updateBlock<char>(351,"zzzzzzzz", 8);  //Element #19 ID
  EEPROM.updateBlock<char>(370,"zzzzzzzz", 8);  //Element #20 ID
  EEPROM.updateBlock<char>(389,"zzzzzzzz", 8);  //Element #21 ID
  EEPROM.updateBlock<char>(408,"zzzzzzzz", 8);  //Element #22 ID

    //write stock Element Types
        //s=sensor, a=actuator, n=nothing, z=Loaded by this program
  EEPROM.updateBlock<char>(17,"a", 1);  //Element #1 Type
  EEPROM.updateBlock<char>(36,"a", 1);  //Element #2 Type
  EEPROM.updateBlock<char>(55,"a", 1);  //Element #3 Type
  EEPROM.updateBlock<char>(74,"a", 1);  //Element #4 Type
  EEPROM.updateBlock<char>(93,"a", 1);  //Element #5 Type
  EEPROM.updateBlock<char>(112,"a", 1);  //Element #6 Type
  EEPROM.updateBlock<char>(131,"a", 1);  //Element #7 Type
  EEPROM.updateBlock<char>(150,"a", 1);  //Element #8 Type
  EEPROM.updateBlock<char>(169,"s", 1);  //Element #9 Type
  EEPROM.updateBlock<char>(188,"s", 1);  //Element #10 Type
  EEPROM.updateBlock<char>(207,"n", 1);  //Element #11 Type
  EEPROM.updateBlock<char>(226,"n", 1);  //Element #12 Type
  EEPROM.updateBlock<char>(245,"n", 1);  //Element #13 Type
  EEPROM.updateBlock<char>(264,"n", 1);  //Element #14 Type
  EEPROM.updateBlock<char>(283,"z", 1);  //Element #15 Type  //Fill the unused ones with z's
  EEPROM.updateBlock<char>(302,"z", 1);  //Element #16 Type
  EEPROM.updateBlock<char>(321,"z", 1);  //Element #17 Type
  EEPROM.updateBlock<char>(340,"z", 1);  //Element #18 Type
  EEPROM.updateBlock<char>(359,"z", 1);  //Element #19 Type
  EEPROM.updateBlock<char>(378,"z", 1);  //Element #20 Type
  EEPROM.updateBlock<char>(397,"z", 1);  //Element #21 Type
  EEPROM.updateBlock<char>(416,"z", 1);  //Element #22 Type

    //Write stock Element Functions
       //0=unused, 1=analogRead, 2=digitalRead,
       //3=analogWrite, 4=digitalWrite, 5=triac, 7=level, 10=Empty
  EEPROM.updateInt(18, 5);  //Elemenr #1 Function
  EEPROM.updateInt(37, 5);  //Elemenr #2 Function
  EEPROM.updateInt(56, 5);  //Elemenr #3 Function
  EEPROM.updateInt(75, 5);  //Elemenr #4 Function
  EEPROM.updateInt(94, 3);  //Elemenr #5 Function
  EEPROM.updateInt(113, 3);  //Elemenr #6 Function
  EEPROM.updateInt(132, 3);  //Elemenr #7 Function
  EEPROM.updateInt(151, 3);  //Elemenr #8 Function
  EEPROM.updateInt(170, 1);  //Elemenr #9 Function
  EEPROM.updateInt(189, 1);  //Elemenr #10 Function
  EEPROM.updateInt(208, 0);  //Elemenr #11 Function
  EEPROM.updateInt(227, 0);  //Elemenr #12 Function
  EEPROM.updateInt(246, 0);  //Elemenr #13 Function
  EEPROM.updateInt(265, 0);  //Elemenr #14 Function
  EEPROM.updateInt(284, 10);  //Elemenr #15 Function
  EEPROM.updateInt(303, 10);  //Elemenr #16 Function
  EEPROM.updateInt(322, 10);  //Elemenr #17 Function
  EEPROM.updateInt(341, 10);  //Elemenr #18 Function
  EEPROM.updateInt(360, 10);  //Elemenr #19 Function
  EEPROM.updateInt(379, 10);  //Elemenr #20 Function
  EEPROM.updateInt(398, 10);  //Elemenr #21 Function
  EEPROM.updateInt(417, 10);  //Elemenr #22 Function

    //Write stock Calibration data if the element is a sensor 
        //or Fail Safe Values / Extras they are actuators
  EEPROM.updateFloat(20, 1);  //Element #1 Slope / FS Val
  EEPROM.updateFloat(24, 0);  //Element #1 Y-Intercept / Extra
  EEPROM.updateFloat(39, 1);  //Element #2 Slope / FS Val
  EEPROM.updateFloat(43, 0);  //Element #2 Y-Intercept / Extra
  EEPROM.updateFloat(58, 1);  //Element #3 Slope / FS Val
  EEPROM.updateFloat(62, 0);  //Element #3 Y-Intercept / Extra
  EEPROM.updateFloat(77, 1);  //Element #4 Slope / FS Val
  EEPROM.updateFloat(81, 0);  //Element #4 Y-Intercept / Extra
  EEPROM.updateFloat(96, 1);  //Element #5 Slope / FS Val
  EEPROM.updateFloat(100, 0);  //Element #5 Y-Intercept / Extra
  EEPROM.updateFloat(115, 1);  //Element #6 Slope / FS Val
  EEPROM.updateFloat(119, 0);  //Element #6 Y-Intercept / Extra
  EEPROM.updateFloat(134, 1);  //Element #7 Slope / FS Val
  EEPROM.updateFloat(138, 0);  //Element #7 Y-Intercept / Extra
  EEPROM.updateFloat(153, 1);  //Element #8 Slope / FS Val
  EEPROM.updateFloat(157, 0);  //Element #8 Y-Intercept / Extra
  EEPROM.updateFloat(172, 1);  //Element #9 Slope / FS Val
  EEPROM.updateFloat(176, 0);  //Element #9 Y-Intercept / Extra
  EEPROM.updateFloat(191, 1);  //Element #10 Slope / FS Val
  EEPROM.updateFloat(195, 0);  //Element #10 Y-Intercept / Extra
  EEPROM.updateFloat(210, 1);  //Element #11 Slope / FS Val
  EEPROM.updateFloat(214, 0);  //Element #11 Y-Intercept / Extra
  EEPROM.updateFloat(229, 1);  //Element #12 Slope / FS Val
  EEPROM.updateFloat(233, 0);  //Element #12 Y-Intercept / Extra
  EEPROM.updateFloat(248, 1);  //Element #13 Slope / FS Val
  EEPROM.updateFloat(252, 0);  //Element #13 Y-Intercept / Extra
  EEPROM.updateFloat(267, 1);  //Element #14 Slope / FS Val
  EEPROM.updateFloat(271, 0);  //Element #14 Y-Intercept / Extra
  EEPROM.updateFloat(286, 1);  //Element #15 Slope / FS Val
  EEPROM.updateFloat(290, 0);  //Element #15 Y-Intercept / Extra
  EEPROM.updateFloat(305, 1);  //Element #16 Slope / FS Val
  EEPROM.updateFloat(309, 0);  //Element #16 Y-Intercept / Extra
  EEPROM.updateFloat(324, 1);  //Element #17 Slope / FS Val
  EEPROM.updateFloat(328, 0);  //Element #17 Y-Intercept / Extra
  EEPROM.updateFloat(343, 1);  //Element #18 Slope / FS Val
  EEPROM.updateFloat(347, 0);  //Element #18 Y-Intercept / Extra
  EEPROM.updateFloat(362, 1);  //Element #19 Slope / FS Val
  EEPROM.updateFloat(366, 0);  //Element #19 Y-Intercept / Extra
  EEPROM.updateFloat(381, 1);  //Element #20 Slope / FS Val
  EEPROM.updateFloat(385, 0);  //Element #20 Y-Intercept / Extra
  EEPROM.updateFloat(400, 1);  //Element #21 Slope / FS Val
  EEPROM.updateFloat(404, 0);  //Element #21 Y-Intercept / Extra
  EEPROM.updateFloat(419, 1);  //Element #22 Slope / FS Val
  EEPROM.updateFloat(423, 0);  //Element #22 Y-Intercept / Extra
  
  //Element Pins
  EEPROM.updateInt(389, 11);  //Element #1 Pin
  EEPROM.updateInt(391, 3);  //Element #2 Pin
  EEPROM.updateInt(393, 4);  //Element #3 Pin
  EEPROM.updateInt(395, 7);  //Element #4 Pin
  EEPROM.updateInt(397, 5);  //Element #5 Pin
  EEPROM.updateInt(399, 9);  //Element #6 Pin
  EEPROM.updateInt(401, 10);  //Element #7 Pin
  EEPROM.updateInt(403, 6);  //Element #8 Pin
  EEPROM.updateInt(405, 20);  //Element #9 Pin
  EEPROM.updateInt(407, 21);  //Element #10 Pin
  EEPROM.updateInt(409, 19);  //Element #11 Pin
  EEPROM.updateInt(411, 18);  //Element #12 Pin
  EEPROM.updateInt(413, 12);  //Element #13 Pin
  EEPROM.updateInt(415, 13);  //Element #14 Pin
  EEPROM.updateInt(417, 15);  //Element #15 Pin
  EEPROM.updateInt(419, 99);  //Element #16 Pin
  EEPROM.updateInt(421, 99);  //Element #17 Pin
  EEPROM.updateInt(423, 99);  //Element #18 Pin
  EEPROM.updateInt(425, 99);  //Element #19 Pin
  EEPROM.updateInt(427, 99);  //Element #20 Pin
}

void loop() {
  // put your main code here, to run repeatedly:

}
