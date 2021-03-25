
#include <Ethernet2.h>
#include<SPI.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


IPAddress server_addr(54,247,107,148); 
char user[] = "root";             
char password[] = "";     

char INSERT_SQL[74];
String av="UPDATE `sql2105932`.`Site` SET `availability` = '1' WHERE `Site`.`ID` =1;";
String nav="UPDATE `sql2105932`.`Site` SET `availability` = '0' WHERE `Site`.`ID` =1;";
const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
long duration, distanceCm,Nduration, NdistanceCm;

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  Ethernet.begin(mac_addr);
  Serial.print("IP = ");
  Serial.println(Ethernet.localIP());
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
  delay(3000);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  Serial.println(distanceCm);

  

 
  
  
    
  
  if (distanceCm > 50){
    Serial.println("distance value is more then 50 !");
    for(int i=0;i<sizeof(INSERT_SQL);i++){
    INSERT_SQL[i]=av.charAt(i);
    Serial.print(INSERT_SQL[i]);
    }
    Serial.println("");
  }
   
     
  else if(distanceCm<0){
    Serial.println("distance is in negative value !");
    for(int i=0;i<sizeof(INSERT_SQL);i++){
    INSERT_SQL[i]=av.charAt(i);
    Serial.print(INSERT_SQL[i]);
    }
    Serial.println("");
    
  }
 
  else {
   if(distanceCm==5||distanceCm==6){
     
     digitalWrite(TRIG_PIN, LOW);
     delayMicroseconds(2);
     digitalWrite(TRIG_PIN, HIGH);
     delayMicroseconds(10);
     digitalWrite(TRIG_PIN, LOW);
     duration = pulseIn(ECHO_PIN,HIGH);
 
     // convert the time into a distance
     distanceCm = duration / 29.1 / 2 ;
     Serial.println(distanceCm);
     if(distanceCm>3000){
       
      Serial.println("distance value is more then 50 !");
      for(int i=0;i<sizeof(INSERT_SQL);i++){
      INSERT_SQL[i]=av.charAt(i);
      Serial.print(INSERT_SQL[i]);
      }
      Serial.println("");
       
     }
     else{
     Serial.println("park is busy");
    for(int i=0;i<sizeof(INSERT_SQL);i++){
    INSERT_SQL[i]=nav.charAt(i);
    Serial.print(INSERT_SQL[i]);
    }
    Serial.println("");
     }
     
     
     
   }
   else{
     Serial.println("park is busy");
    for(int i=0;i<sizeof(INSERT_SQL);i++){
    INSERT_SQL[i]=nav.charAt(i);
    Serial.print(INSERT_SQL[i]);
    }
    Serial.println("");
   }
   
    
  }
   MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL);
  delete cur_mem;
  
}
