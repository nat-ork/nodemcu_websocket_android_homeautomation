#include <Wire.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
 
#define btn_input 13

 
AsyncWebServer server(80);

String json="";
int mod=0;
int val;
int prevval=2;
String emod = "auto"; // Default mode 

// Manual relay variable coming out of the arduino
int ManRel_1=0; //non 0 1 value
int ManRel_2=0;
int ManRel_3=0;
int ManRel_4=0; 
// Auto variable coming out of the server
int AutoRel_1=4; //non 0 1 value  // 4 is off, 5 is on;
int AutoRel_2=4;
int AutoRel_3=4;
int AutoRel_4=4; 
int btn_mode=5;    // 5, 4 (auto, manual)

void setup() {
 Serial.begin(9600); /* begin serial for debug */
 Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
 pinMode(btn_input,INPUT);

 //------------------------------------------------------- Server programming -------------------------------------------------------------------------------------------------------

  
  //setting up the access point 
  Serial.print("Setting soft-AP ... ");
  boolean result = WiFi.softAP("NewTest", "123456789");
  if(result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }

  while(WiFi.softAPgetStationNum()==0)
  {
    loop();
    
  }

    
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("NewTest");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

 
  server.on("/led1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", emod);
    if(emod=="man")
    {
      btn_mode = 5;  // auto
      emod="auto";
      }
      else
      {
         btn_mode = 4; // man
         emod="man";
        }
    
    
  });

   server.on("/rel1on", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", emod);
   
    AutoRel_1 = 5;
  
    
  });

   server.on("/rel1off", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", emod);
   
    AutoRel_1 = 4;
  
    
  });
 
  server.begin();


 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! End of server programming !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void loop() {


  int val = digitalRead(btn_input);

  if(val!=prevval)
  {
  if(val==1)//button is pressed
  {
    prevval=1;
    if(emod=="auto")
    {
      emod = "man";
    
          btn_mode = 4;
         
     
      }
      else
      {
        emod="auto";
       
          btn_mode = 5;
          
     
        }
    
    } 
  } 

  if(val==0) // button released
  {
    prevval=2;
    }

Serial.print(btn_mode);
Serial.print(ManRel_1);
Serial.print(ManRel_2);
Serial.print(ManRel_3);
Serial.print(ManRel_4);
Serial.println("");

SendAll();   // Sending the data collected in the global varibales in to the arduino 
//String data = GetData();
GetData();

//StaticJsonBuffer<200> jsonBuffer;
//JsonObject& object = jsonBuffer.parseObject(data);
//String world = object["hello"];
//Serial.println(world);
 
}

void SendAll()
{
SendData("{");  
SendData("\"mode\":");
SendData("\"");  
SendData(string2char(String(btn_mode))); // data zone
SendData("\",");
SendData("\"rel1\":");
SendData("\"");  
SendData(string2char(String(AutoRel_1))); // data zone
SendData("\",");
SendData("\"rel2\":");
SendData("\"");  
SendData(string2char(String(AutoRel_2))); // data zone
SendData("\",");
SendData("\"rel3\":");
SendData("\"");  
SendData(string2char(String(AutoRel_3))); // data zone
SendData("\",");
SendData("\"rel4\":");
SendData("\"");  
SendData(string2char(String(AutoRel_4))); // data zone
SendData("\"");
SendData("}"); 
  }

void SendData(char* data)
{
 Wire.beginTransmission(8); /* begin with device address 8 */
 Wire.write(data);  /* sends hello string */
 Wire.endTransmission();    /* stop transmitting */
 }




void GetData()
 {

    String check="";
      for(int i =1;i<=3;i++)
      {
      
          String val = String(i);
          SendData("{");  
          SendData("\"trans\":");
          SendData("\"");  
          SendData(string2char(val)); // data zone
          SendData("\"");
          SendData("}");
        
         
         Wire.requestFrom(8, 23); /* request & read data of size 13 from slave */
         while(Wire.available())
           {
                 char c = Wire.read();
                 
                 //Serial.print(c);
                 json.concat(c);
                 check = c;
                
           }
          
          
      
      }
      if(check=="}")
      {
        //Serial.println(json);
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& object = jsonBuffer.parseObject(json);   // Parsing the incomming data to the json object
        String lmode = object["mode"]; 
        String lrel1 = object["rel1"]; 
        String lrel2 = object["reel2"]; 
        String lrel3 = object["rel3"]; 
        String lrel4 = object["reel4"]; // cheking if the data is asking for more data or not, 0 means main data has been send
       
        ManRel_1 = lrel1.toInt();
        ManRel_2 = lrel2.toInt();
        ManRel_3 = lrel3.toInt();
        ManRel_4 = lrel4.toInt(); 
        //Serial.println(mod);
        json="";
        check="";
        
        }

   
  }

  

char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}


                             
  
  
