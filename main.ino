#include "DHT.h"
#include "WiFi.h"
#include "ThingSpeak.h"

WiFiClient client;

DHT dht(23,DHT11);

void setup() 
{
  Serial.begin(9600);
  Serial.println("Connecting to WiFi Network");
  WiFi.begin("Agronomy","123456789");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(50);
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());
  dht.begin();
  delay(1000);
  ThingSpeak.begin(client);
  Serial.println("Connected to ThingSpeak Cloud");
  delay(1000);
}

void loop() {
 String a;
 float hum=dht.readHumidity();
 float temp_c=dht.readTemperature();
 float soil_moi=map(analogRead(34),0,4095,0,100);
 if(isnan(hum) || isnan(temp_c))
 {
   Serial.println("No Data Received");
   return;
 }
 while(Serial.available()) {
  a=Serial.readString();
  a=getValue(a,':',1);
  a=getValue(a,',',0);  
  Serial.println(a);
 }
 Serial.print("Humidity:");
 Serial.print(hum);
 Serial.print("\t Temp(*C):");
 Serial.print(temp_c);
 Serial.print("\t Soil Moisture(%):");
 Serial.print(soil_moi);
 Serial.print("\t PH Value:");
 Serial.print(a);
 delay(2000);
 ThingSpeak.setField(1,temp_c);
 ThingSpeak.setField(2,hum);
 ThingSpeak.setField(3,soil_moi);
 ThingSpeak.setField(4,a);
 int sta=ThingSpeak.writeFields(1431927,"9XQZA0DV24NP7KS0");
 if(sta==200)
   Serial.println("Data Uploaded Successfully");
 else
   Serial.println("There is Some Error Please Check");
 delay(20000);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
