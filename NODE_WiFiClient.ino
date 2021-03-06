
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DMDESP.h>
#include <fonts/ElektronMart6x8.h>
#include <fonts/Mono5x7.h>

#include "index.h" 





const char* ssid = "Bsnl";
const char* password = "*****";

ESP8266WebServer server(80); //Server on port 80


#define DISPLAYS_WIDE 1 
#define DISPLAYS_HIGH 1
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  
//----------------------------------------


char *Text[] = {"SEEKMY INC"}; 
String Incoming_Text = ""; 

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}


void handle_Incoming_Text(){
  Incoming_Text = server.arg("TextContents");

  server.send(200, "text/plane", "");

  Process_Incoming_Text();
}

void Process_Incoming_Text(){

  delay(500);
  Serial.println("Incoming Text : ");
  Serial.println(Incoming_Text);
  Serial.println();
  int str_len = Incoming_Text.length() + 1;
  char char_array[str_len];
  Incoming_Text.toCharArray(char_array, str_len);
  strcpy(Text[0], char_array);
  
  Incoming_Text = "";
}

void setup(){
  Serial.begin(115200);
  delay(500);
   //----------------------------------------DMDESP Setup
  Disp.start(); //--> Run the DMDESP library
  Disp.setBrightness(50); //--> Brightness level
  Disp.setFont(Mono5x7); //--> Determine the font used
  //----------------------------------------
  WiFi.begin(ssid, password);     //Connect to WiFi router
  Serial.println("hello");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/setText", handle_Incoming_Text);  //--> Routine to handle handle_Incoming_Text Subroutines
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}


  
void loop(void){
 
  server.handleClient();          //Handle client requests

  Disp.loop(); //--> Run "Disp.loop" to refresh the LED
  Disp.drawText(4, 0, "ZHETON"); //--> Display text "Disp.drawText(x position, y position, text)"
  Scrolling_Text(9, 50); //--> Show running text "Scrolling_Text(y position, speed);"
}

 
void Scrolling_Text(int y, uint8_t scrolling_speed) {
  static uint32_t pM;
  static uint32_t x;
  int width = Disp.width();
  Disp.setFont(Mono5x7);
  int fullScroll = Disp.textWidth(Text[0]) + width;
  if((millis() - pM) > scrolling_speed) { 
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, Text[0]);
  }  
}
