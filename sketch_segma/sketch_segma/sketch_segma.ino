/*
  MySQL Connector/Arduino Example : basic insert

 
  CREATE DATABASE test_arduino;
  CREATE TABLE test_arduino.hello_arduino (
    num integer primary key auto_increment,
    message char(40),
    recorded timestamp
  );
  
	INSERT INTO `hello_arduino` (`ID`, `availability`) VALUES ('1', '1');
	Update hello_arduino set availability = 0 where ID = 1;

  Here we see one database and a table with three fields; a primary key that
  is an auto_increment, a string, and a timestamp. This will demonstrate how
  to save a date and time of when the row was inserted, which can help you
  determine when data was recorded or updated.

  INSTRUCTIONS FOR USE

 
     the command: "SELECT * FROM test_arduino.hello_arduino" to see the data
     recorded. Note the field values and how the database handles both the
     auto_increment and timestamp fields for us. You can clear the data with
     "DELETE FROM test_arduino.hello_arduino".

  Note: The MAC address can be anything so long as it is unique on your network.

  Created by: Dr. Charles A. Bell
*/
#include <Ethernet2.h>
#include<SPI.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


IPAddress server_addr(54,247,107,148);  // IP of the MySQL *server* here 54.247.107.148   eHost : 108.167.160.65
char user[] = "sql2105329";              // MySQL user login username    sql2105329         jazanusp_aaz
char password[] = "iH1%tA7*";        // MySQL user login password  iH1%tA7*          17654321aaZ   

// Sample query
char INSERT_SQL[76];
String av="UPDATE `jazanusp_test`.`Site` SET `availability` = '1' WHERE `Site`.`id` =1";  //UPDATE `jazanusp_test`.`Site` SET `availability` = '0' WHERE `Site`.`id` =1
String nav="UPDATE `jazanusp_test`.`Site` SET `availability` = '0' WHERE `Site`.`id` =1"; //Update jazanusp_aaz.Site set availability = 1 where ID = 1;       cpses_jaFLjL3nJJ
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
long duration, distanceCm;

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  Ethernet.begin(mac_addr);
  Serial.println(av.length());
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
  else{
    Serial.println("Connection failed.");
    delay(5000);
    setup();
  }
}


void loop() {
  delay(1000);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  Serial.println(distanceCm);

  

  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  
  if (distanceCm > 50){
    for(int i=0;i<sizeof(INSERT_SQL);i++){
    INSERT_SQL[i]=av.charAt(i);
    Serial.print(INSERT_SQL[i]);
    }
    Serial.println("");
  }
    
     
  else if(distanceCm<0){
    Serial.println("distance is unknown !");
    loop();
  }
 
  else {
    for(int i=0;i<sizeof(INSERT_SQL);i++){
    INSERT_SQL[i]=nav.charAt(i);
    Serial.print(INSERT_SQL[i]);
    }
    Serial.println("");
    
  }
  
  cur_mem->execute(INSERT_SQL);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
  
}
