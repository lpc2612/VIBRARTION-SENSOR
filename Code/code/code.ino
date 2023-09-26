#define BLYNK_TEMPLATE_ID "TMPL5K7R9Oz5"
#define BLYNK_DEVICE_NAME "Buzzer button"
#define BLYNK_AUTH_TOKEN "NxEzpuThq1o5tVlxo5wrhZvZrXQ-52pi"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "IphoneMT";
char pass[] = "77777777";

TaskHandle_t Task1;
TaskHandle_t Task2;

#define buzzer 4
#define led1 2
#define led2 12

int state1 = 0;
int state2 = 0;
int state;
int value = digitalRead(14);
int i = 0;
char vdata[] = " ";

SimpleTimer timer;

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  xTaskCreatePinnedToCore(
    Task1code, /* Task function. */
    "Task1",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task1,    /* Task handle to keep track of created task */
    0);        /* pin task to core 0 */
  //delay(500);

  xTaskCreatePinnedToCore(
    Task2code, /* Task function. */
    "Task2",   /* name of task. */
    10000,     /* Stack size of task */
    NULL,      /* parameter of the task */
    1,         /* priority of the task */
    &Task2,    /* Task handle to keep track of created task */
    1);        /* pin task to core 1 */
               // delay(500);
}

void loop() {
  Blynk.run();
  timer.run();
  value = digitalRead(14);
}

void Task1code(void* pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    if (state == 1) {
      if (value == 0) {
        Serial.println("Khong Rung");
        state1 = 0;
        digitalWrite(led1, state1);
        delay(0.000001);
      } else if (value == 1) {
        Serial.println("Rung");
        for (int i = 0; i < 10; i++) {
          state1 = 1;
          digitalWrite(led1, 1);
          delay(250);
          digitalWrite(led1, 0);
          delay(250);
          strcpy(vdata, "Detect Vibration\n");
          Blynk.virtualWrite(V2, vdata);
          Blynk.logEvent("vibration_detected");
          if (state1 == 0 || state == 0) {
            digitalWrite(led1, 0);
            state1 = 0;
            return;
          }
        }
      }
    } else {
      state1 = 0;
      digitalWrite(led1, state1);
    }
  }
}


void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {
    if (state == 1) {
      if (state1 == 1) {
        state2 = ~state2;
        digitalWrite(buzzer, state2);
        delay(100);
      } else if (state1 == 0) {
        state2 = 0;
        digitalWrite(buzzer, HIGH);
      }
    } else {
      state2 = 0;
      digitalWrite(buzzer, HIGH);
    }
  }
}

BLYNK_WRITE(V0) {
  state = param.asInt();
  digitalWrite(led2, state);
}
