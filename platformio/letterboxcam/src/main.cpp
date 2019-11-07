
#include <esp_camera.h>
#include <WiFi.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include "driver/rtc_io.h"
#define XSTR(x) #x
#define STR(x) XSTR(x)

int capture_interval_us = 10e+6; // 10 seconds between captures
const char* wifi_ssid = STR(SSID_NAME);
const char* wifi_psk = STR(SSID_PASSWORD);

bool internet_connected = false;
WiFiClient net;
PubSubClient client(net);

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

bool connectWifi() 
{
  int connAttempts = 0;
  Serial.println("\r\nConnecting to: " + String(SSID_NAME));
  WiFi.begin(wifi_ssid, wifi_psk);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print("x");
    if (connAttempts > 10) return false;
    connAttempts++;
  }

  return true;
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("letterbox")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  client.setServer("192.168.1.175", 1883);

  if (connectWifi()) { // Connected to WiFi
    internet_connected = true;
    Serial.println("Internet connected");
  }


  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;  

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

static esp_err_t take_send_photo()
{
  Serial.println("Flash on...");
  rtc_gpio_hold_dis(GPIO_NUM_4);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH); 

  Serial.println("Taking picture...");
  camera_fb_t * fb = NULL;

  fb = esp_camera_fb_get();

  Serial.println("Flash off...");
  digitalWrite(4, LOW);
  rtc_gpio_hold_en(GPIO_NUM_4);

  if (!fb) {
    Serial.println("Camera capture failed");
    return ESP_FAIL;
  }

  Serial.printf("Camera captured: %d bytes", fb->len);

  client.publish("/cameras/letterbox/snapshot", fb->buf, fb->len);

  esp_camera_fb_return(fb);
}

void loop() {
  if (!internet_connected) connectWifi();
  connectMQTT();

  client.loop();
  take_send_photo();

  sleep(10);

  ESP.deepSleep(capture_interval_us);
}