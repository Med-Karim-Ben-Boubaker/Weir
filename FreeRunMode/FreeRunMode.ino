
#define BLYNK_TEMPLATE_ID "TMPL2-lOSi8z_"
#define BLYNK_TEMPLATE_NAME "Weir"
#define BLYNK_AUTH_TOKEN "ttM8jnEMAK6kaDeMdRz1ywVShc8CdnLG"

#include "OneWire.h"
#include "DallasTemperature.h"
#include <math.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Orange-82DC";
char pass[] = "EUROBOT2023RAS";
BlynkTimer timer;

// Average Hand Tempeture

#define HANDTEMP 36.5
#define Kp 20
#define Kd 3 
  
// PWM 

#define PWM_Res   8
#define PWM_Freq  1000

// Vibrator1 PWM caracteristics

#define VIB1_GPIO   15
#define PWM1_Ch    0


// Vibrator2 PWM caracteristics

#define VIB2_GPIO   2
#define PWM2_Ch    1


// Vibrator3 PWM caracteristics


#define VIB3_GPIO   4
#define PWM3_Ch    2


// Vibrator4 PWM caracteristics

#define VIB4_GPIO   5
#define PWM4_Ch    3


// Vibrator5 PWM caracteristics

#define VIB5_GPIO   18
#define PWM5_Ch    4

// Echant Period

#define ECHANT 0.05
float currentTime = 0.0;
float prevError = 0.0;

// GPIO where the DS18B20 is connected to
const int oneWireBus_1 = 15;
const int oneWireBus_2 = 2;
const int oneWireBus_3 = 4;

// Vibrators DutyCycle
int PWM_DutyCycle = 0;

// Tempeture Vars

float temperatureC_1;
float temperatureC_2;
float temperatureC_3;
float moy_temperatureC = 28;

// Setup a oneWire instance to+- communicate with any OneWire devices
OneWire oneWire_1(oneWireBus_1);
OneWire oneWire_2(oneWireBus_2);
OneWire oneWire_3(oneWireBus_3);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensor_1(&oneWire_1);
DallasTemperature sensor_2(&oneWire_2);
DallasTemperature sensor_3(&oneWire_3);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  // Setting PWMs zith Pins
  ledcAttachPin(VIB5_GPIO, PWM5_Ch);
  ledcSetup(PWM5_Ch, PWM_Freq, PWM_Res);
  ledcAttachPin(VIB4_GPIO, PWM4_Ch);
  ledcSetup(PWM4_Ch, PWM_Freq, PWM_Res);
  ledcAttachPin(VIB3_GPIO, PWM3_Ch);
  ledcSetup(PWM3_Ch, PWM_Freq, PWM_Res);
  ledcAttachPin(VIB2_GPIO, PWM2_Ch);
  ledcSetup(PWM2_Ch, PWM_Freq, PWM_Res);
  ledcAttachPin(VIB1_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM_Freq, PWM_Res);
  
  // Start the DS18B20 sensor
  sensor_1.begin();
  sensor_2.begin();
  sensor_3.begin();
}


void loop() {
  Blynk.run();
  timer.run();
  delay(2000);
  if( millis() - currentTime > ECHANT )
  {
    sensor_1.requestTemperatures();
    sensor_2.requestTemperatures();
    sensor_3.requestTemperatures();
  
    temperatureC_1 = sensor_1.getTempCByIndex(0);
    temperatureC_2 = sensor_2.getTempCByIndex(0);
    temperatureC_3 = sensor_3.getTempCByIndex(0);
    moy_temperatureC = (temperatureC_1 + temperatureC_2 + temperatureC_3 ) / 3;

    setPWM ( moy_temperatureC );
    
    currentTime = millis();
  }
 
}

void myTimerEvent()
{
  Blynk.virtualWrite(V1,moy_temperatureC);
}
BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}
// PID: Closed loop control for vibration frequency control
void setPWM ( float moy )
{
  int command;
  float error = abs(HANDTEMP - moy);
  command = int ( Kp * error + Kd * ( error - prevError ) ); 
  prevError = error;
  ledcWrite(PWM1_Ch, command);
  ledcWrite(PWM2_Ch, command);
  ledcWrite(PWM3_Ch, command);
  ledcWrite(PWM4_Ch, command);
  ledcWrite(PWM5_Ch, command);
}
