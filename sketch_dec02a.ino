
//esp32s3 4x4
//A为车头，B为车尾。
//A1、B1电机为反装，A2、B2电机为正装。减速箱 在前为正装。


#include <WiFi.h>
const char* ssid     = "Wangnapao";
const char* password = "1234567890";


unsigned int MotorA1_IN1=3;       //控制A1电机的PWM引脚 
unsigned int MotorA1_IN2=4;      
unsigned int MotorA2_IN1=5;       //控制A2电机的PWM引脚
unsigned int MotorA2_IN2=6;   
unsigned int MotorB1_IN1=7;       //控制B1电机的PWM引脚
unsigned int MotorB1_IN2=8;   
unsigned int MotorB2_IN1=13;      //控制B2电机的PWM引脚
unsigned int MotorB2_IN2=15;    
char cmd;                //定义一个字符型变量存储串口输入命令

#define TURN_SPEED 120
#define LINE_SPEED 250    


WiFiServer server(80);

void setup() 
{
  Serial.begin(115200);           //打开串口
  Serial.println("start now");
  
  pinMode(MotorA1_IN1,OUTPUT);   //设置两个驱动引脚为输出模式
  pinMode(MotorA1_IN2,OUTPUT); 
  pinMode(MotorA2_IN1,OUTPUT);   //设置两个驱动引脚为输出模式
  pinMode(MotorA2_IN2,OUTPUT); 
  pinMode(MotorB1_IN1,OUTPUT);   //设置两个驱动引脚为输出模式
  pinMode(MotorB1_IN2,OUTPUT); 
  pinMode(MotorB2_IN1,OUTPUT);   //设置两个驱动引脚为输出模式
  pinMode(MotorB2_IN2,OUTPUT); 
    delay(10);
    
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

    
}


void loop() 
{

   WiFiClient client = server.available();   // listen for incoming clients
 if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html; charset=utf-8");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<div style='font-size:40px;text-align:center;margin:20 auto;'>------<a href=\"/F\">前进</a>------<br/><br/>");
            client.print("---<a href=\"/L\">左</a>------");
            client.print("<a href=\"/R\">右</a>---<br><br/>");

            client.print("<a href=\"/B\">------后退------</a><br/><br/>");
            client.print("<a href=\"/B\">------启动------</a><br/><br/>");
            client.print("<a href=\"/S\">------停车------</a><br/><br/></div>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /F")) {
          movefront();               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /B")) {
          moveback();
        }
        if (currentLine.endsWith("GET /R")) {
          moveright();               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          moveleft();
        }
        if (currentLine.endsWith("GET /S")) {
         movestop();
        }
        if (currentLine.endsWith("GET /T")) {
         fandb();
        }
        
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

  void movefront(){
          analogWrite(MotorA1_IN1,LINE_SPEED); 
         analogWrite(MotorA1_IN2,0);
         analogWrite(MotorA2_IN1,LINE_SPEED); 
         analogWrite(MotorA2_IN2,0);
         analogWrite(MotorB1_IN1,0); 
         analogWrite(MotorB1_IN2,LINE_SPEED);
         analogWrite(MotorB2_IN1,0); 
         analogWrite(MotorB2_IN2,LINE_SPEED);
  }
  void moveback(){
           analogWrite(MotorA1_IN1,0); 
         analogWrite(MotorA1_IN2,LINE_SPEED);
         analogWrite(MotorA2_IN1,0); 
         analogWrite(MotorA2_IN2,LINE_SPEED);
         analogWrite(MotorB1_IN1,LINE_SPEED); 
         analogWrite(MotorB1_IN2,0);
         analogWrite(MotorB2_IN1,LINE_SPEED); 
         analogWrite(MotorB2_IN2,0);
  }
  void moveleft(){
         analogWrite(MotorA1_IN1,TURN_SPEED); 
         analogWrite(MotorA1_IN2,0);
         analogWrite(MotorA2_IN1,TURN_SPEED); 
         analogWrite(MotorA2_IN2,0);
         analogWrite(MotorB1_IN1,TURN_SPEED); 
         analogWrite(MotorB1_IN2,0);
         analogWrite(MotorB2_IN1,TURN_SPEED); 
         analogWrite(MotorB2_IN2,0);
  }
  void moveright(){
         analogWrite(MotorA1_IN1,0); 
         analogWrite(MotorA1_IN2,TURN_SPEED);
         analogWrite(MotorA2_IN1,0); 
         analogWrite(MotorA2_IN2,TURN_SPEED);
         analogWrite(MotorB1_IN1,0); 
         analogWrite(MotorB1_IN2,TURN_SPEED);
         analogWrite(MotorB2_IN1,0); 
         analogWrite(MotorB2_IN2,TURN_SPEED);
  }
  void movestop(){
           analogWrite(MotorA1_IN1,0); 
         analogWrite(MotorA1_IN2,0);
         analogWrite(MotorA2_IN1,0); 
         analogWrite(MotorA2_IN2,0);
         analogWrite(MotorB1_IN1,0); 
         analogWrite(MotorB1_IN2,0);
         analogWrite(MotorB2_IN1,0); 
         analogWrite(MotorB2_IN2,0);
  }

  void fandb(){
            analogWrite(MotorA1_IN1,LINE_SPEED); 
         analogWrite(MotorA1_IN2,0);
         analogWrite(MotorA2_IN1,LINE_SPEED); 
         analogWrite(MotorA2_IN2,0);
         analogWrite(MotorB1_IN1,0); 
         analogWrite(MotorB1_IN2,LINE_SPEED);
         analogWrite(MotorB2_IN1,0); 
         analogWrite(MotorB2_IN2,LINE_SPEED);

         delay(5000);

           analogWrite(MotorA1_IN1,0); 
         analogWrite(MotorA1_IN2,0);
         analogWrite(MotorA2_IN1,0); 
         analogWrite(MotorA2_IN2,0);
         analogWrite(MotorB1_IN1,0); 
         analogWrite(MotorB1_IN2,0);
         analogWrite(MotorB2_IN1,0); 
         analogWrite(MotorB2_IN2,0);

         delay(2000);

           analogWrite(MotorA1_IN1,0); 
         analogWrite(MotorA1_IN2,LINE_SPEED);
         analogWrite(MotorA2_IN1,0); 
         analogWrite(MotorA2_IN2,LINE_SPEED);
         analogWrite(MotorB1_IN1,LINE_SPEED); 
         analogWrite(MotorB1_IN2,0);
         analogWrite(MotorB2_IN1,LINE_SPEED); 
         analogWrite(MotorB2_IN2,0);

         delay(5000);

           analogWrite(MotorA1_IN1,0); 
         analogWrite(MotorA1_IN2,0);
         analogWrite(MotorA2_IN1,0); 
         analogWrite(MotorA2_IN2,0);
         analogWrite(MotorB1_IN1,0); 
         analogWrite(MotorB1_IN2,0);
         analogWrite(MotorB2_IN1,0); 
         analogWrite(MotorB2_IN2,0);
  }
