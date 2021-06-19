

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



const char *ssid = "AndroidAP2CB9";  //ENTER YOUR WIFI SETTINGS
const char *password = "807875913";

const long utcOffsetInSeconds = 19801;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//Web/Server address to read/write from 
const char *host = "34.136.219.162";   

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
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

  timeClient.begin();

  HTTPClient http;    //Declare object of class HTTPClient
  WiFiClient client;
  timeClient.update();
  String device_time = String() + timeClient.getFormattedTime();
  delay(200);
  Serial.println(device_time);
  
 
  String device_ip = String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + WiFi.localIP()[2] + "." +
  WiFi.localIP()[3];

  delay(200);
  Serial.println("Sending device ip...");
  Serial.println(device_ip);
  

  String postData, device_status;
  device_status = "online";
  delay(200);
  Serial.println("device status = ");
  Serial.println(device_status);

  //Post Data
 postData = "{\"_id=60ca73c8b11d5112a87478b&doctorid=2&deviceid=100&deviceip=" + device_ip + "&status=" + device_status + "&date=" + device_time;
  
  http.begin(client, "http://" host "/create/new");              //Specify request destination
  http.addHeader("Content-Type", "application/json");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request

  if (httpCode >= 200) {

    Serial.println(httpCode);
    String payload = http.getString();
  }
  else {

    Serial.println("failed");
  }
      //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
//  delay(5000);  //Post Data at every 5 seconds

  
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  
}





//         PostValue = PostValue + ",\"dts\":" +timestamp;
//   PostValue = PostValue +",\"data\":{\"celc\":" + t2 +",\"fahr\":" + t3 +"}"+"}";
//  postData = "{\"_id=60ca73c8b11d5112a87478b&doctorid=2&deviceid=100&deviceip=" + device_ip + "&status=" + device_status + "&date=" + device_time;
//String postData = "{"_id":"60cd9ea0dbd596001b580fc0","doctorid":"2","deviceid":"100","deviceip":"139.340.340.0","date":"2021-06-18T05:51:18.241Z","status":"up","__v":0}";
