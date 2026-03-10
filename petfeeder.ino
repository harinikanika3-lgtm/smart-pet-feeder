//project@123
//xegama5796@bitfami.com   
#define BLYNK_TEMPLATE_ID "TMPL38lQfop0c"
#define BLYNK_TEMPLATE_NAME "pet feeder"
#define BLYNK_AUTH_TOKEN "6hlWoyx3pyh670QWjwZfosEjnxnQ1yQx"

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "pet feeder";
char pass[] = "123456789";

Servo myservo;
int servoPin = D4;

// LCD
LiquidCrystal_I2C lcd(0x27,16,2); 

// Feed sequence
int feedStep = 0;
unsigned long previousFeedMillis = 0;
const long feedInterval = 15000; // 15 seconds between feeds

int Buzzer = D5;

// Function to feed pet
void feedPet(const char* message) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(message);  
  Blynk.virtualWrite(V0, message); // send to Blynk
  
  digitalWrite(Buzzer,HIGH);
  delay(1000);
  digitalWrite(Buzzer,LOW);

  myservo.write(180);   // rotate servo
  delay(5000);          // keep it rotated for 5s
  myservo.write(0);     // back to start
}

void setup() {
  Serial.begin(9600);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer,LOW);

  // Servo init
  myservo.attach(servoPin);
  myservo.write(0);

  // LCD init
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Pet Feeder Ready");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run(); // always call this

  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000; // convert to seconds

  // Show time in LCD second row
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  lcd.print(seconds);
  lcd.print("s   "); // clear old digits

  // Handle feeding at intervals
  if(currentMillis - previousFeedMillis >= feedInterval) {
    previousFeedMillis = currentMillis;

    if(feedStep == 0) {
      feedPet("Morning feed        ");
      feedStep = 1;
    } 
    else if(feedStep == 1) {
      feedPet("Afternoon feed      ");
      feedStep = 2;
    } 
    else if(feedStep == 2) {
      feedPet("Night feed          ");
      feedStep = 0;
    }
  }
}