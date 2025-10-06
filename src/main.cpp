/*
  -----------------------
  ElegantOTA - Demo Example
  -----------------------

  Skill Level: Beginner

  This example provides with a bare minimal app with ElegantOTA functionality.

  Github: https://github.com/ayushsharma82/ElegantOTA
  WiKi: https://docs.elegantota.pro

  Works with following hardware:
  - ESP8266
  - ESP32
  - RP2040 (with WiFi) (Example: Raspberry Pi Pico W)


  Important note for RP2040 users:
  - RP2040 requires LittleFS partition for the OTA updates to work. Without LittleFS partition, OTA updates will fail.
    Make sure to select Tools > Flash Size > "2MB (Sketch 1MB, FS 1MB)" option.
  - If using bare RP2040, it requires WiFi module like Pico W for ElegantOTA to work.

  -------------------------------

  Upgrade to ElegantOTA Pro: https://elegantota.pro

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ElegantOTA.h>
#include <NeoPixelBus.h>
#include <AsyncWiFiManager.h>
#include <EEPROM.h>

#include "webpage.h"
#include "wififage.h"

// const char *ssid = "KN*2.42";
// const char *password = "123#123#";

// AP credentials
const char *ap_ssid = "ESP8266-OTA";
const char *ap_password = "88888888";

// Fixed AP IP
IPAddress apIP(8, 8, 8, 8);
IPAddress gateway(8, 8, 8, 8);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

#define EEPROM_SIZE 96 // đủ để lưu SSID + PASS (32 + 64)

String ssid, pass;

unsigned long ota_progress_millis = 0;

#define colorSaturation 128

#define numLedTia 20
#define numLedCot 100

#define DelayLedCot 25
#define DelayLedTia 30

int ledIndex = 0;
uint32_t lastLedUpdate = 0;
bool finished = 0;
uint32_t Delayabc = 0;
uint8_t colorLed = 0; // Màu hiện tại của LED
uint8_t randompoint = 0;
uint8_t switcheffect = 0; // Biến để theo dõi trạng thái của LED

NeoPixelBus<NeoGrbFeature, NeoEsp8266DmaWs2812xMethod> strip(numLedCot + numLedTia);

const RgbColor colorList[20] = {
    RgbColor(255, 0, 0),     // Red - Đỏ
    RgbColor(0, 255, 0),     // Green - Xanh lá
    RgbColor(0, 0, 255),     // Blue - Xanh dương
    RgbColor(255, 255, 0),   // Yellow - Vàng
    RgbColor(255, 165, 0),   // Orange - Cam
    RgbColor(255, 0, 255),   // Magenta - Hồng đậm
    RgbColor(0, 255, 255),   // Cyan - Xanh biển
    RgbColor(128, 0, 128),   // Purple - Tím
    RgbColor(0, 128, 128),   // Teal - Xanh ngọc
    RgbColor(128, 128, 0),   // Olive - Vàng xanh
    RgbColor(255, 192, 203), // Pink - Hồng nhạt
    RgbColor(173, 216, 230), // Light Blue - Xanh da trời
    RgbColor(144, 238, 144), // Light Green - Xanh lá nhạt
    RgbColor(255, 105, 180), // Hot Pink - Hồng tươi
    RgbColor(210, 105, 30),  // Chocolate - Nâu
    RgbColor(255, 255, 255), // White - Trắng
    RgbColor(192, 192, 192), // Silver - Xám nhạt
    RgbColor(128, 128, 128), // Gray - Xám
    RgbColor(70, 130, 180),  // Steel Blue
    RgbColor(0, 100, 0)      // Dark Green - Xanh rêu
};

void onOTAStart()
{
  // Log when OTA has started
  // Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final)
{
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000)
  {
    ota_progress_millis = millis();
    // Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success)
{
  // Log when OTA has finished
  if (success)
  {
    // Serial.println("OTA update finished successfully!");
  }
  else
  {
    // Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void ledChaseFade_nonBlocking(int &index,
                              uint8_t interval, uint8_t interval1,
                              uint8_t numpoint,
                              uint32_t &lastUpdateTime,
                              const RgbColor &chaseColor)
{
  uint32_t now = millis();
  if (index > numLedCot)
  {
    if (now - lastUpdateTime < interval1)
      return;
  }
  else
  {
    if (now - lastUpdateTime < interval)
      return;
  }

  lastUpdateTime = now;

  const uint8_t fadeAmount = 30;
  uint16_t count = strip.PixelCount();

  // Làm mờ tất cả LED
  for (uint16_t i = 0; i < count; i++)
  {
    RgbColor c = strip.GetPixelColor(i);
    uint8_t r = (c.R > fadeAmount) ? c.R - fadeAmount : 0;
    uint8_t g = (c.G > fadeAmount) ? c.G - fadeAmount : 0;
    uint8_t b = (c.B > fadeAmount) ? c.B - fadeAmount : 0;
    strip.SetPixelColor(i, RgbColor(r, g, b));
  }
  // for (uint8_t i = 0; i < numpoint; i++)
  // {
  //   int chaseIndex = (index - 20 * i + count) % count;
  //   strip.SetPixelColor(chaseIndex, chaseColor);
  // }
  strip.SetPixelColor(index, chaseColor);
  for (uint8_t i = 1; i < numpoint; i++)
  {
    int chaseIndex = (index - 20 * i + count) % count;
    RgbColor randColor(250, random(256), random(256)); // màu ngẫu nhiên
    strip.SetPixelColor(chaseIndex, randColor);
  }

  strip.Show();
  index = (index + 1) % count;
}

bool ledChaseFade_RunOnce(int &index,
                          uint8_t interval, uint8_t interval1,
                          uint8_t numpoint,
                          uint32_t &lastUpdateTime,
                          uint8_t color)
{

  uint32_t now = millis();
  uint8_t delayTime = (index > numLedCot) ? interval1 : interval;

  if (now - lastUpdateTime < delayTime)
    return false;

  lastUpdateTime = now;

  uint16_t count = strip.PixelCount();
  const uint8_t fadeAmount = 60; // Giảm độ sáng mỗi lần

  // Làm mờ toàn bộ
  for (uint16_t i = 0; i < count; i++)
  {
    RgbColor c = strip.GetPixelColor(i);
    strip.SetPixelColor(i, RgbColor(
                               (c.R * fadeAmount) / 100,
                               (c.G * fadeAmount) / 100,
                               (c.B * fadeAmount) / 100));
  }

  // Tô điểm chase
  for (uint8_t i = 0; i < numpoint; i++)
  {
    uint8_t colorLed = (color + i * 3) % 20; // Lấy màu từ danh sách màu
    uint16_t chaseIndex = (index - 20 * i);
    if ((chaseIndex < count - 1))
      strip.SetPixelColor(chaseIndex, colorList[colorLed]);
  }

  strip.Show();
  uint16_t countled = ((count + numpoint * 20) - 1);
  if (index >= countled)
  {
    return true;
  }
  index++;
  return false;
}

bool blinkLedGroups_RunOnce(uint8_t color,                // màu nhấp nháy
                            uint8_t repeatMax,            // số lần lặp lại hiệu ứng
                            uint16_t blinkInterval = 200) // thời gian ON/OFF (ms)
{
  const uint8_t group2[] = {103, 110, 118, 97, 90, 82};
  const uint8_t group1[] = {104, 107, 96, 93};
  const uint8_t blinkCountMax = 4;

  static uint8_t currentGroup = 0; // 0: group1, 1: group2
  static uint8_t blinkCount = 0;   // lần nhấp nháy trong nhóm
  static bool isOn = true;         // trạng thái nhấp nháy
  static uint32_t lastBlinkTime = 0;
  static uint8_t repeatCount = 0;

  uint32_t now = millis();
  if (now - lastBlinkTime < blinkInterval)
    return false;
  lastBlinkTime = now;

  // Chọn nhóm hiện tại
  const uint8_t *group = (currentGroup == 0) ? group1 : group2;
  uint8_t groupSize = (currentGroup == 0) ? sizeof(group1) : sizeof(group2);

  // Gán màu cho nhóm
  for (uint8_t i = 0; i < groupSize; i++)
  {
    strip.SetPixelColor(group[i], isOn ? colorList[color] : RgbColor(0, 0, 0));
  }
  strip.Show();

  // Sau mỗi lần OFF thì tính là xong 1 nhấp nháy
  if (!isOn)
  {
    blinkCount++;
    if (blinkCount >= blinkCountMax)
    {
      blinkCount = 0;
      currentGroup++;
      if (currentGroup > 1)
      {
        currentGroup = 0;
        repeatCount++;
        if (repeatCount >= repeatMax)
        {
          // Reset toàn bộ sau khi hoàn tất
          repeatCount = 0;
          return true; // hiệu ứng xong
        }
      }
    }
  }
  isOn = !isOn;
  return false; // hiệu ứng đang chạy
}

String wifiOptions = "<option value=''>Click Rescan to search WiFi</option>";

void handleScan()
{
  int n = WiFi.scanNetworks();
  if (n == 0)
  {
    wifiOptions = "<option value=''>No networks found</option>";
  }
  else
  {
    wifiOptions = "";
    for (int i = 0; i < n; i++)
    {
      String ssid = WiFi.SSID(i);
      int rssi = WiFi.RSSI(i);
      int quality = 0;
      if (rssi <= -100)
        quality = 0;
      else if (rssi >= -50)
        quality = 100;
      else
        quality = 2 * (rssi + 100);

      wifiOptions += "<option value='" + ssid + "'>" + ssid + " (" + String(quality) + "%)</option>";
    }
  }
  server.sendHeader("Location", "/wifi");
  server.send(303); // Redirect về /wifi
}

void saveWiFi(String s, String p)
{
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.write(0, s.length());
  for (int i = 0; i < s.length(); i++)
  {
    EEPROM.write(i + 1, s[i]);
  }
  EEPROM.write(33, p.length());
  for (int i = 0; i < p.length(); i++)
  {
    EEPROM.write(i + 34, p[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}

void loadWiFi()
{
  EEPROM.begin(EEPROM_SIZE);
  int ssidLen = EEPROM.read(0);
  ssid = "";
  for (int i = 0; i < ssidLen; i++)
  {
    ssid += char(EEPROM.read(i + 1));
  }
  int passLen = EEPROM.read(33);
  pass = "";
  for (int i = 0; i < passLen; i++)
  {
    pass += char(EEPROM.read(i + 34));
  }
  EEPROM.end();
  Serial.println("Loaded WiFi credentials:");
  Serial.println("SSID: " + ssid);
  Serial.println("PASS: " + pass);
}

void setup(void)
{
  Serial.begin(115200);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  loadWiFi();
  if (ssid != "")
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
    // Serial.println("Connecting to saved WiFi: " + ssid);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
    {
      delay(500);
      Serial.print(".");
    }
  }

  if (WiFi.status() != WL_CONNECTED)

  {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, gateway, subnet);
    WiFi.softAP(ap_ssid, ap_password);
  }

  // server.on("/", []()
  //           { server.send(200, "text/plain", "Hi! This is ElegantOTA Demo."); });
  server.on("/", []()
            { server.send_P(200, "text/html", MAIN_page); });
  server.on("/wifi", []()
            { server.send(200, "text/html", generateWiFiPage()); });
  server.on("/scan", handleScan);

  server.on("/restart", []()
            {

  server.send(200, "text/html", html_rs);
  delay(500);
  ESP.restart(); });

  server.on("/setwifi", HTTP_POST, []()
            {
    String newSSID = server.arg("ssid");
    String newPASS = server.arg("password");

    // Serial.println("=== New WiFi Credentials ===");
    // Serial.println("SSID: " + newSSID);
    // Serial.println("PASS: " + newPASS);
    saveWiFi(newSSID, newPASS);
     
    server.send(200, "text/html", html_rs);
    delay(500);
    ESP.restart(); });

  ElegantOTA.begin(&server); // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);       // Called when OTA starts
  ElegantOTA.onProgress(onOTAProgress); // Called during OTA progress
  ElegantOTA.onEnd(onOTAEnd);           // Called when OTA ends

  server.begin();
  // Serial.println("HTTP server started");

  strip.Begin();
  strip.Show();
}

void loop(void)
{
  server.handleClient();
  ElegantOTA.loop();
  switch (switcheffect)
  {
  case 0: // Hiệu ứng đuổi theo LED
    if (finished)
    {
      uint32_t now = millis();
      if (now - Delayabc > 100)
      {
        Delayabc = now;
        finished = false;
        ledIndex = 0;
        colorLed = random(20);      // Chọn màu ngẫu nhiên từ 0 đến 19
        randompoint = random(1, 5); // Số điểm ngẫu nhiên từ 1 đến 3
        switcheffect = 1;           // Chuyển sang hiệu ứng nhấp nháy LED
      }
    }
    else
      finished = ledChaseFade_RunOnce(ledIndex, DelayLedCot, DelayLedTia, randompoint, lastLedUpdate, colorLed);
    break;
  case 1: // Hiệu ứng nhấp nháy LED
    if (finished)
    {
      uint32_t now = millis();
      if (now - Delayabc > 1000)
      {
        Delayabc = now;
        finished = false;
        colorLed = random(20); // Chọn màu ngẫu nhiên từ 0 đến 19
        switcheffect = 0;      // Chuyển sang hiệu ứng đuổi theo LED
      }
    }
    else
      finished = blinkLedGroups_RunOnce(colorLed, 1, 100);
  }
}
