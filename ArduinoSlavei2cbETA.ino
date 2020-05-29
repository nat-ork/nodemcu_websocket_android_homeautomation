//Auto and manaul pins  ------ Input
# define auto_man_in 3

// Manual input button pins  --- Input
# define man_btn_in_1 7
# define man_btn_in_2 6
# define man_btn_in_3 5
# define man_btn_in_4 4

// Relay controlling pins   ---- Output
# define realy_out_1 13
# define realy_out_2 12
# define realy_out_3 11
# define realy_out_4 10

//Auto and manaul LED  ------ OUTPUT
#define led_auto_man 2



#include <Wire.h>
#include <ArduinoJson.h>

String emod = "auto"; // Default mode 
int prevval=2; // Non 0 1 value 

String json="";   //value collected by the json object  
int snd = 0;     // variable to send the data stream to the master device as the amount is restricted to 32byte only at once

// Manual button inputs
int btn_1=0; //non 0 1 value
int btn_2=0;
int btn_3=0;
int btn_4=0;
int btn_mode=0;

// auto values that will be set only if the mode is auto
int autoValRel1 = 0;
int autoValRel2 = 0;
int autoValRel3 = 0;
int autoValRel4 = 0;
int mainMode=0;


int val;
void setup() {
   // in mode
  pinMode(auto_man_in,INPUT);

  // in mode
  pinMode(man_btn_in_1,INPUT);
  pinMode(man_btn_in_2,INPUT);
  pinMode(man_btn_in_3,INPUT);
  pinMode(man_btn_in_4,INPUT);

  // out mode
  pinMode(realy_out_1,OUTPUT);
  pinMode(realy_out_2,OUTPUT);
  pinMode(realy_out_3,OUTPUT);
  pinMode(realy_out_4,OUTPUT);

  // led auto man
  pinMode(led_auto_man,OUTPUT);
  
 Wire.begin(8); 
 Wire.onRequest(requestEvent); /* register request event *//* join i2c bus with address 8 */
 Wire.onReceive(receiveEvent); /* register receive event */

 Serial.begin(115200);           /* start serial for debug */
}

void loop() {
  /*
   int val = digitalRead(auto_man_in);

  if(val!=prevval)
  {
  if(val==1)//button is pressed
  {
    prevval=1;
    if(emod=="auto")
    {
      emod = "man";
    
          btn_mode = 0;
          
     
      }
      else
      {
        emod="auto";
       
          btn_mode = 1;
          
     
        }
    
    } 
  } 

  if(val==0) // button released
  {
    prevval=2;
    }

   // Serial.println(btn_mode);
  // put your main code here, to run repeatedly:
  */

  if(btn_mode==4) // manual mode ------------------------------------------------------------------------------------------------------------------------------------------------------------
  {
    digitalWrite(led_auto_man,0);
    int man_in_val1 = digitalRead(man_btn_in_1);
    int man_in_val2 = digitalRead(man_btn_in_2);
    int man_in_val3 = digitalRead(man_btn_in_3);
    int man_in_val4 = digitalRead(man_btn_in_4);
    
    // for the 1 manual button 
    if(man_in_val1==1)
    {
    digitalWrite(realy_out_1,1);
    btn_1 = digitalRead(man_btn_in_1);
    }
    else
    {
     digitalWrite(realy_out_1,0);
     btn_1 = digitalRead(man_btn_in_1);
    }



     // for the 2 manual button 
    if(man_in_val2==1)
    {
    digitalWrite(realy_out_2,1);
    btn_2 = digitalRead(man_btn_in_2);
    }
    else
    {
    
      digitalWrite(realy_out_2,0);
      btn_2 = digitalRead(man_btn_in_2);
    }


     // for the 3 manual button 
    if(man_in_val3==1)
    {
    digitalWrite(realy_out_3,1);
    btn_3 = digitalRead(man_btn_in_3);
    }
    else
    {
     
      digitalWrite(realy_out_3,0);
      btn_3 = digitalRead(man_btn_in_3);
    }


     // for the 4 manual button 
    if(man_in_val4==1)
    {
    digitalWrite(realy_out_4,1);
    btn_4 = digitalRead(man_btn_in_4);
    }
    else
    {
      
      digitalWrite(realy_out_4,0);
      btn_4 = digitalRead(man_btn_in_4);
    }

   
    
  }
  else if(btn_mode == 5)   // automatic mode ------------------------------------------------------------------------------------------------------------------------------------------------
      {
      digitalWrite(led_auto_man,1);
       // for the 1 manual button 
          if(autoValRel1==5)
          {
          digitalWrite(realy_out_1,1);
          btn_1 = digitalRead(man_btn_in_1);
          }
          else if(autoValRel1==4)
          {
           digitalWrite(realy_out_1,0);
           btn_1 = digitalRead(man_btn_in_1);
          }
      
      
      
           // for the 2 manual button 
          if(autoValRel2==5)
          {
          digitalWrite(realy_out_2,1);
          btn_2 = digitalRead(man_btn_in_2);
          }
          else if(autoValRel2==4)
          {
          
            digitalWrite(realy_out_2,0);
            btn_2 = digitalRead(man_btn_in_2);
          }
      
      
           // for the 3 manual button 
          if(autoValRel3==5)
          {
          digitalWrite(realy_out_3,1);
          btn_3 = digitalRead(man_btn_in_3);
          }
          else if(autoValRel3==4)
          {
           
            digitalWrite(realy_out_3,0);
            btn_3 = digitalRead(man_btn_in_3);
          }
      
      
          
           // for the 4 manual button 
          if(autoValRel4==5)
          {
          digitalWrite(realy_out_4,1);
          btn_4 = digitalRead(man_btn_in_4);
          }
          else if(autoValRel4==4)
          {
            
            digitalWrite(realy_out_4,0);
            btn_4 = digitalRead(man_btn_in_4);
          }
      
            
            }

      else  // packet loss state--------------------------------------------------------------------------------------------------------------------------------------------------------------
      {
        
        }

 
}

// function that executes whenever data is received from master

void receiveEvent(int howMany) {

  String check="";
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
            /* print the character */
    json.concat(c);
    check.concat(c);
  }
  if(check=="}") // End of the json incomming data, data is loaded in the json String
  {
   //Serial.print(json);

        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& object = jsonBuffer.parseObject(json);   // Parsing the incomming data to the json object
        String world = object["trans"];                      // cheking if the data is asking for more data or not, 0 means main data has been send
        val = world.toInt();  
        if(val==0)      // this is the main data
        {
          String lmode = object["mode"];
          String lrel1 = object["rel1"];
          String lrel2 = object["rel2"];
          String lrel3 = object["rel3"];
          String lrel4 = object["rel4"];
         
          btn_mode = lmode.toInt();
          autoValRel1 = lrel1.toInt();
          autoValRel2 = lrel2.toInt();
          autoValRel3 = lrel3.toInt();
          autoValRel4 = lrel4.toInt();
          Serial.print(btn_mode);
          Serial.print(autoValRel1);
          Serial.print(autoValRel2);
          Serial.print(autoValRel3);
          Serial.print(autoValRel4);
          Serial.println("");
          /*
          String lrel1 = object["rel1"];
          String lrel2 = object["rel2"];
          String lrel3 = object["rel3"];
          String lrel4 = object["rel4"];
*/
          /*
          btn_mode = lmode.toInt();
          autoValRel1 = lrel1.toInt();
          autoValRel2 = lrel2.toInt();
          autoValRel3 = lrel3.toInt();
          autoValRel4 = lrel4.toInt();
          Serial.print(lmode);
          Serial.print(autoValRel1);
          Serial.print(autoValRel2);
          Serial.print(autoValRel3);
          Serial.print(autoValRel4);//---------------------------------------------------------------------------------------------------------------------------------------------------------------
          Serial.println("");
          */
        }
          
           
        json=""; // All the data has been received, setting the json val to original state
        check="";
   
  }
  
}

// function that executes whenever data is requested from master
void requestEvent() {
if(val==1)
{
  String mode  = String(btn_mode);
  String btn1 = String(btn_4);
  String a = "{\"mode\":\""+mode+"\",\"rel1\":\""+btn1+"\",";
  Wire.write(a.c_str());  /*send string on request */
  
}else if(val==2)
{
  
  String btn2 = String(btn_3);
  String btn3 = String(btn_2);
  String b = "\"reel2\":\""+btn2+"\",\"rel3\":\""+btn3+"\",";
 Wire.write(b.c_str()); 
}
else if(val==3)
{
  
  String btn4 = String(btn_1);
  String b = "\"reel4\":\""+btn4+"\",\"data\":\"1\"}"; 
  Wire.write(b.c_str()); 
  }


}



char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}
