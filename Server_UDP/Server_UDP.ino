// /////////////////////////////////////////////////////////////////
// /*
//   ESP32CAM | Sending images via UDP?
//   Video Tutorial: https://youtu.be/1kHxd5FOUEU
//   Created by Eric N. (ThatProject)
// */
// /////////////////////////////////////////////////////////////////

// #define LGFX_USE_V1
// #include <LovyanGFX.hpp>
// #include "WiFi.h"
// #include "AsyncUDP.h"
// #include <vector>
// #define CHUNK_LENGTH 1460

// class LGFX : public lgfx::LGFX_Device {

//   lgfx::Panel_ST7796 _panel_instance;
//   lgfx::Bus_SPI _bus_instance;
//   lgfx::Light_PWM _light_instance;

// public:
//   LGFX(void) {
//     {
//       auto cfg = _bus_instance.config();
//       cfg.spi_host = VSPI_HOST;
//       cfg.spi_mode = 0;
//       cfg.freq_write = 40000000;
//       cfg.freq_read = 20000000;
//       cfg.spi_3wire = false;
//       cfg.use_lock = true;
//       cfg.dma_channel = 1;
//       cfg.pin_sclk = 18;
//       cfg.pin_mosi = 19;
//       cfg.pin_miso = 23;
//       cfg.pin_dc = 27;
//       _bus_instance.config(cfg);
//       _panel_instance.setBus(&_bus_instance);
//     }
//     {
//       auto cfg = _panel_instance.config();
//       cfg.pin_cs = 5;
//       cfg.pin_rst = -1;
//       cfg.pin_busy = -1;
//       cfg.memory_width = 320;
//       cfg.memory_height = 480;
//       cfg.panel_width = 320;
//       cfg.panel_height = 480;
//       cfg.offset_x = 0;
//       cfg.offset_y = 0;
//       cfg.offset_rotation = 0;
//       cfg.dummy_read_pixel = 8;
//       cfg.dummy_read_bits = 1;
//       cfg.readable = true;
//       cfg.invert = false;
//       cfg.rgb_order = false;
//       cfg.dlen_16bit = false;
//       cfg.bus_shared = true;
//       _panel_instance.config(cfg);
//     }
//     {
//       auto cfg = _light_instance.config();
//       cfg.pin_bl = 12;
//       cfg.invert = false;
//       cfg.freq = 44100;
//       cfg.pwm_channel = 7;
//       _light_instance.config(cfg);
//       _panel_instance.setLight(&_light_instance);
//     }

//     setPanel(&_panel_instance);
//   }
// };

// LGFX tft;
// std::vector<uint8_t> byte_vector;

// const char* ssid = "Etisalat-HjHC";
// const char* password = "missarahmed@246";
// const int udpPort = 8080;

// AsyncUDP udp;

// uint32_t fpsLastTime = 0;
// int nbFrames = 0;

// void setup() {
//   Serial.begin(115200);

//   tft.begin();
//   tft.setRotation(1);
//   tft.setBrightness(255);

//   tft.fillScreen(TFT_BLACK);
//   tft.setFont(&fonts::Orbitron_Light_24);
//   tft.setTextColor(TFT_GREEN);

//   WiFi.softAP(ssid, password);

//   // WiFi.begin(ssid, password);
//   // WiFi.setSleep(false);

//   // while (WiFi.status() != WL_CONNECTED) {
//   //   delay(500);
//   //   Serial.print(".");
//   // }
//   // Serial.println("");
//   // Serial.println("WiFi connected");
//   IPAddress myIP = WiFi.softAPIP();
//   tft.drawString(myIP.toString(), tft.width() / 2, tft.height() / 2);

//   if (udp.listen(udpPort)) {
//     Serial.print("UDP Listening on IP: ");
//     Serial.println(WiFi.localIP());

//     udp.onPacket([](AsyncUDPPacket packet) {
//       Serial.printf("packet Length: %d\n", packet.length());

//       std::vector<uint8_t> temp_vector(&packet.data()[0], &packet.data()[packet.length()]);

//       if (packet.length() == CHUNK_LENGTH && 
//       packet.data()[0] == 255 && 
//       packet.data()[1] == 216 &&
//       packet.data()[2] == 255) { // FF D8 FF
//         byte_vector.clear();
//       }

//       byte_vector.insert(byte_vector.end(), temp_vector.begin(), temp_vector.end());

//       if (packet.length() != CHUNK_LENGTH && 
//       packet.data()[packet.length() - 2] == 255 && 
//       packet.data()[packet.length() - 1] == 217) { // FF D9
//         uint8_t* jpgData = byte_vector.data();
//         tft.drawJpg(jpgData, byte_vector.size(), 40, 12);
      
//         nbFrames++;
//         if (millis() - fpsLastTime >= 1000) {
//           drawingFPSText(nbFrames);
//           nbFrames = 0;
//           fpsLastTime += 1000;
//         }
//       }
//     });
//   }
// }

// void loop() {
// }

// void drawingFPSText(int fps) {
//   tft.fillRect(0, 0, 40, 40, TFT_BLUE);
//   tft.setCursor(8, 4);
//   tft.printf("%d", fps);
// }

// //#include <Arduino.h>
// #include <WiFi.h>
// //#include <ESPAsyncWebServer.h>
// #include <ESPAsyncWebSrv.h>
// #include <AsyncUDP.h>
// #include <vector>

// #define CHUNK_LENGTH 1460

// std::vector<uint8_t> byte_vector;

// const char *ssid = "Etisalat-HjHC";
// const char *password = "missarahmed@246";
// const int udpPort = 8080;

// AsyncUDP udp;

// const char *html = "<html><body><img src=\"/image\"></body></html>";

// void handleRoot(AsyncWebServerRequest *request) {
//   request->send(200, "text/html", html);
// }

// void handleImage(AsyncWebServerRequest *request) {
//   if (byte_vector.size() > 0) {
//     request->send_P(200, "image/jpeg", byte_vector.data(), byte_vector.size());
//     byte_vector.clear();
//   } else {
//     request->send(404);
//   }
// }

// void setup() {
//   Serial.begin(115200);

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }

//   Serial.println("Connected to WiFi");

//   if (udp.listen(udpPort)) {
//     Serial.print("UDP Listening on IP: ");
//     Serial.println(WiFi.localIP());

//     udp.onPacket([](AsyncUDPPacket packet) {
//       Serial.printf("packet Length: %d\n", packet.length());

//       std::vector<uint8_t> temp_vector(&packet.data()[0], &packet.data()[packet.length()]);

//       if (packet.length() == CHUNK_LENGTH &&
//           packet.data()[0] == 255 &&
//           packet.data()[1] == 216 &&
//           packet.data()[2] == 255) { // FF D8 FF
//         byte_vector.clear();
//       }

//       byte_vector.insert(byte_vector.end(), temp_vector.begin(), temp_vector.end());

//       if (packet.length() != CHUNK_LENGTH &&
//           packet.data()[packet.length() - 2] == 255 &&
//           packet.data()[packet.length() - 1] == 217) { // FF D9
//         // Do nothing here, the image will be sent when requested through HTTP
//       }
//     });
//   }

//   // Setup the web server
//   AsyncWebServer server(80);
//   server.on("/", HTTP_GET, handleRoot);
//   server.on("/image", HTTP_GET, handleImage);

//   server.begin();
// }

// void loop() {
//   // Handle UDP packets, if any
//   while (udp.parsePacket()) {
//     // Process incoming UDP packets
//     udp.handlePacket();
//   }
// }

#include <Arduino.h>
#include <WiFi.h>
//#include <ESPAsyncWebServer.h>
#include <ESPAsyncWebSrv.h>
#include <AsyncUDP.h>
#include <vector>

#define CHUNK_LENGTH 1460

std::vector<uint8_t> byte_vector;

// const char *ssid = "STUDBME2";
// const char *password = "BME2Stud";
const char *ssid = "Etisalat-HjHC";
const char *password = "missarahmed@246";
const int udpPort = 8080;

AsyncUDP udp;

const char *html = "<html><body><img src=\"/image\"></body></html>";

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", html);
}

void handleImage(AsyncWebServerRequest *request) {
  if (byte_vector.size() > 0) {
    request->send_P(200, "image/jpeg", byte_vector.data(), byte_vector.size());
    byte_vector.clear();
  } else {
    request->send(404);
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  if (udp.listen(udpPort)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());

    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.printf("packet Length: %d\n", packet.length());
      //Serial.println("packet: ");
      //Serial.print(packet.data()[0]);

      std::vector<uint8_t> temp_vector(&packet.data()[0], &packet.data()[packet.length()]);

      if (packet.length() == CHUNK_LENGTH &&
          packet.data()[0] == 255 &&
          packet.data()[1] == 216 &&
          packet.data()[2] == 255) { // FF D8 FF
        byte_vector.clear();
      }

      byte_vector.insert(byte_vector.end(), temp_vector.begin(), temp_vector.end());

      if (packet.length() != CHUNK_LENGTH &&
          packet.data()[packet.length() - 2] == 255 &&
          packet.data()[packet.length() - 1] == 217) { // FF D9
        // Do nothing here, the image will be sent when requested through HTTP
      }
    });
  }

  // Setup the web server
  AsyncWebServer server(80);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/image", HTTP_GET, handleImage);

  server.begin();
}

void loop() {
  // No need for loop to handle UDP packets in this case
}

