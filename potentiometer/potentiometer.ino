#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#define WIFI_SSID "STUDBME2"
#define WIFI_PASSWORD "BME2Stud"
// const char* ssid = "STUDBME2";
// const char* password = "BME2Stud";

#define API_KEY "AIzaSyASEDaqwzVjKSWzPLYgDAp2lAXlqRxHHbU"
#define DATABASE_URL "https://monitor-fc64a-default-rtdb.firebaseio.com/"

#define POTENTIOMETER_PIN_1 36
#define POTENTIOMETER_PIN_2 39
#define POTENTIOMETER_PIN_3 34

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

#define LED_PIN 2

bool lampState;
bool pt_1_alarm=0;
bool pt_2_alarm=0;
bool pt_3_alarm=0;

void turnOnLamp() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Turning ON");
}

void turnOffLamp() {
  digitalWrite(LED_PIN, LOW);
  Serial.println("Turning OFF");
}


// Declaration of tokenStatusCallback
void tokenStatusCallback(token_info_t info);

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Authentication successful");
    signupOK = true;
  }
  else
  {
    Serial.printf("Authentication failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(LED_PIN, OUTPUT);

}


void loop()
{
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getBool(&fbdo, "LED state"))
    {
      // Successfully read the value from Firebase
      lampState = fbdo.boolData();
      if(lampState == 1)
      {
        turnOnLamp();
        Serial.println("True");
      }
      else
      {
        Serial.println("False");
        turnOffLamp();
      }
      Serial.print("Received lampState from Firebase: ");
      Serial.println(lampState);
  }
  else
  {
    // Failed to read the value from Firebase
    Serial.println("Failed to read lampState from Firebase");
    Serial.println("Reason: " + fbdo.errorReason());
  }
    // int pt_1 = analogRead(POTENTIOMETER_PIN_1);
    // int pt_2 = analogRead(POTENTIOMETER_PIN_2);
    // int pt_3 = analogRead(POTENTIOMETER_PIN_3);

    // float pt_1_value = pt_1 / 25.5;
    // float pt_2_value = pt_2 / 41.0;
    // float pt_3_value = pt_3 / 102.0;
    // Serial.println(pt_1_value);
    // Serial.println(pt_2_value);
    // Serial.println(pt_3_value);

    // if (pt_3_value > 37 || pt_3_value < 30)
    // {
    //   // Set the alarm flag to 1
    //   pt_3_alarm = 1;
    //   Serial.println("Alarm! tempreture exceeded the threshold.");
    // }
    // else
    // {
    //   // Reset the alarm flag to 0
    //   pt_3_alarm = 0;
    // }


    // if (pt_1_value > 120 || pt_1_value<80)
    // {
    //   // Set the alarm flag to 1
    //   pt_1_alarm = 1;
    //   Serial.println("Alarm! heart rate is not within the normal range.");
    // }
    // else
    // {
    //   // Reset the alarm flag to 0
    //   pt_1_alarm = 0;
    // }

    // if (pt_2_value<95)
    // {
    //   // Set the alarm flag to 1
    //   pt_2_alarm = 1;
    //   Serial.println("Alarm! oxygen consentration below the threshold.");
    // }
    // else
    // {
    //   // Reset the alarm flag to 0
    //   pt_2_alarm = 0;
     }
}

    // if (Firebase.RTDB.setFloat(&fbdo, "potentiometers/pot_1", pt_1_value) &&
    //     Firebase.RTDB.setFloat(&fbdo, "potentiometers/pot_2", pt_2_value) &&
    //     Firebase.RTDB.setFloat(&fbdo, "potentiometers/pot_3", pt_3_value) &&
    //     Firebase.RTDB.setBool(&fbdo, "Heart rate alarm", pt_1_alarm)      &&
    //     Firebase.RTDB.setBool(&fbdo, "Oxygen concentration alarm", pt_2_alarm)  &&
    //     Firebase.RTDB.setBool(&fbdo, "Tempreture alarm", pt_3_alarm)  &&
    //     Firebase.RTDB.setBool(&fbdo, "LED state",lampState)  
    //     )
    // { 
    //   Serial.println("Data sent to Firebase successfully");
    //   Serial.println("Path: " + fbdo.dataPath());
    //   Serial.println("Data type: " + fbdo.dataType());
    // }
    // else
    // {
    //   Serial.println("Failed to send data to Firebase");
    //   Serial.println("Reason: " + fbdo.errorReason());
    // }
  //}


// Implementation of tokenStatusCallback
void tokenStatusCallback(token_info_t info)
{
  int status = info.status;

  Serial.print("Token generation status: ");
  switch (status)
  {
  case 0:
    Serial.println("Token generation success");
    break;
  case -1:
    Serial.println("Token generation failed: Generic error");
    break;
  case -2:
    Serial.println("Token generation failed: Authentication error");
    break;
  case -3:
    Serial.println("Token generation failed: Network error");
    break;
  case -4:
    Serial.println("Token generation failed: Timeout");
    break;
  default:
    Serial.println("Unknown status");
    break;
  }
}