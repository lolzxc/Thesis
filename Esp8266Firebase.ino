#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "HUAWEI-2.4G-U22c"
#define WIFI_PASSWORD "43gFPUbS"
#define API_KEY "AIzaSyBfb0mPdUEWWRwMx1ChF3qq4eZHQPU7peA"
#define DATABASE_URL "thesis-4530c-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "testuser@mail.com"
#define USER_PASSWORD "test123"

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

// Timezone
const long utcOffsetInSeconds = 28800;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // initLoRaWan();
  initWifi();

  timeClient.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  

  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();

  struct tm *ptm = gmtime ((time_t *)&epochTime);
  // Get Current Date
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);

  int count = Firebase.getInt(fbdo, ("/" + currentDate + "/Count/"));
  Serial.println(count);

  if (count == 0) {
    Firebase.setInt(fbdo, ("/" + currentDate + "/Count/"), count++);
  }

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    Firebase.setFloat(fbdo, ("/" + currentDate + "/WaterQualityData/" + "/PH/" + count), getPh());
    Firebase.setFloat(fbdo, ("/" + currentDate + "/WaterQualityData/" + "/Temperature/" + count), getTemperature());
    Firebase.setFloat(fbdo, ("/" + currentDate + "/WaterQualityData/" + "/Depth/" + count), getDepth());
    Firebase.setFloat(fbdo, ("/" + currentDate + "/FishCounterData/" + "/Fishes/" + count), getFishes());
    Firebase.setInt(fbdo, ("/" + currentDate + "/Count/"), count++);

  }
  
  // If LoraWan and Firebase is Ready
  // Pass Water Quality Data to Firebase
  // Firebase.set(/currentDate/WaterQualityData/count/PH)
  // Firebase.set(/currentDate/WaterQualityData/count/Depth)
  // Firebase.set(/currentDate/WaterQualityData/count/Temperature)
  // Pass Fish Counter Data to Firebase
  // Firebase.set(/currentDate/FishCounterData/count/Fishes)
  // Increment Count
  // Else
  // Error
  // End
  

}

float getPh() {
  // TODO
  int a = 0;
  return a++;
}

float getTemperature() {
  // TODO
  int a = 0;
  return a++;
}

float getDepth() {
  // TODO
  int a = 0;
  return a++;
}

float getFishes() {
  // TODO
  int a = 0;
  return a++;
}

void initLoRaWan() {

}

void initWifi() {
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  unsigned long ms = millis();
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected");
  Serial.println();

    /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");


}

void getWaterQualityData() {

}

void getFishCounterData() {

}