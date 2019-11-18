
#include <SPI.h>
//#include <WiFi.h>
#include <WiFi101.h>





#define ARDUINO_PLATFORM
#define M2X_ENABLE_READER
#include <jsonlite.h>
#include "M2XStreamClient.h"

/*
 //4006
char deviceId[] = "78b26bc133b5c13dbb469b979c94dc33"; // Device you want to push to
char m2xKey[] = "6a69b382d3daedcf85f674fcf4114234"; // Your M2X access key
*/

char deviceId[] = "52a77b2a3f14d6c405fbf3ad237041cc"; // Device you want to push to
char m2xKey[] = "8fbbd73e2331abf05616a6ef7d10e974"; // Your M2X access key //5699

       
WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);


void on_data_point_found(const char* at, m2x_value value, int index, void* context, int type) {
  Serial.print("Found a data point, index:");
  Serial.print(index);
  /*
  Serial.print("Type:");
  Serial.println(type);
  Serial.print("At:");
  Serial.println(at);
  */
  Serial.print("Value:");
  Serial.println(value.value_string);
}

#include "C:\Projects\MKR1000_M2X\Common.h"
#include "C:\Projects\MKR1000_M2X\MKR1000.h"

//Uart Serial1();
// Timer Interrupt
void TC5_Handler (void) {
    /*
    if(IntCounter == 100)LoopProcess1 = ON;
    if(IntCounter == 5000)LoopProcess2 = ON;
    if(IntCounter == 10000)LoopProcess3 = ON;

    
    */

  IntCounter++;
  if( IntCounter > 1000){

  //  EnableLoop = ON;
    
    IntCounter = 0;
      counter++;
      digitalWrite(greenled, !digitalRead(greenled));   // Toggle led

    }      
        TC5->COUNT16.INTFLAG.bit.MC0 = 1; //don't change this, it's part of the timer code
}
void setup() {


    Serial.begin(115200);   
     pinMode(greenled, OUTPUT);
      pinMode(yellowled, OUTPUT);
      delay(10);
      /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  */
    digitalWrite(yellowled, 1);   // Toggle
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    for(int i = 0; i < 100; i++){
      digitalWrite(yellowled, !digitalRead(yellowled));   // Toggle led
      delay(100);
    
    }
   // delay(10000);
  }
  digitalWrite(yellowled, 0);   // Toggle led 
  Serial.println("Connected to wifi");
  printWifiStatus();
 
  TimerSetup();
    for(int i=0; i< 16;i++){
      ReceiveArray[i] = '5';// buffer not to start with 0 for crc wrong calculation
    }
     Serial1.begin(115200);
       while (!Serial1) {
            ; // wait for serial port to connect. Needed for Leonardo only
      } 
}


/*
byte rx_byte = 0;        // stores received byte
void SerialFloatPrint(float f) {
  byte * b = (byte *) &f;
  //Serial1.write(b[3]);
  Serial1.write(b, 4);
}
*/
/*
float FloatSerialConvert(byte * b){
  float *f;
  f = (float *)b;
  return  *f;
}
*/


void loop() {
       Serial1.write((byte*)ReqArray, 6);
       delay(3000);
 
            //  Serial1.println("SERIAL1__");

        //SerialFloatPrint(2.34);
          //  Serial.println(2.34); 
          //   delay(15);
          // 9600 baud 1 byte 1 msec
          // 9600 baud 11 bytes 11 mmsec
            //115200 baud 9600*12
  /*
      for (int i= 0; i<64; i++){
        ReceiveArray[i]=0 ;       
      } 
      */  
      
    // Serial.println("Receive Start");
     Serial.print("Rx Buffer Size1:");Serial.println(Serial1.available());

     ReceiveCounter = 0;
     while (Serial1.available() > 0) {
         ReceiveArray[ReceiveCounter] = Serial1.read();
         ReceiveCounter++;
         if(ReceiveCounter >= 64)break; // do not overflow the array              
     }
     while (Serial1.available() > 0) {
         Temp = Serial1.read(); // clean all reamining rx buffer
     }
     
              Serial.print("Rx Buffer Size2:");Serial.println(Serial1.available());     
              Serial.println("Receive End");
              ArraySize =  ReceiveArray[4];   
              CRC_Calculate = calcrc((char*)ReceiveArray, ArraySize-2);
              CRC_Received = ReceiveArray[ArraySize-1];  
              CRC_Received <<= 8;
              CRC_Received += ReceiveArray[ArraySize-2];

                        GetValues(5);
                        
              Serial.print("CRC: ");    
            Serial.println(CRC_Calculate);
            Serial.print("CRC.Err: ");    
            Serial.println(CRC_Error);
            PrintValues();              
            if ( CRC_Received != CRC_Calculate) CRC_Error++;
            else { // send and update data
                digitalWrite(yellowled, 1);   // Toggle led
                  UpdateM2XStream();
                  M2X_SendStream(); 
                  digitalWrite(yellowled, 0);   // Toggle led                 
            }
        //      M2X_ReceiveStream(); 

                    printWifiStatus();           
                    ControlM2XCounter++;
   
}
