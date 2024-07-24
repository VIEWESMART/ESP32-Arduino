
/*******************************************************************************
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#include "AiEsp32RotaryEncoder.h"
#include <Bounce2.h>

#define GFX_BL 7
Arduino_DataBus *bus = new Arduino_SWSPI(
//Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 18 /* CS */,
    13 /* SCK or SCLK */, 12 /* MOSI */, GFX_NOT_DEFINED /* MISO */);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    17 /* DE */, 3 /* VSYNC */, 46 /* HSYNC */, 9 /* PCLK */,
    40 /* R1 */, 41 /* R2 */, 42 /* R3 */, 2 /* R4 */,  1 /* R0 */,    
    21 /* G1 */, 47 /* G2 */, 48 /* G3 */, 45 /* G4 */, 38 /* G5 */, 39 /* G0 */, 
    10 /* B1 */, 11 /* B2 */, 12 /* B3 */, 13 /* B4 */, 14 /* B0 */,   
    1 /* hsync_polarity */, 40 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 20 /* hsync_back_porch */,
    1 /* vsync_polarity */, 50 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */ );
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, 8 /* RST */, gc9503v_type1_init_operations, sizeof(gc9503v_type1_init_operations));
//st7701_type1_init_operations, sizeof(st7701_type1_init_operations));//

 //Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
 //    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);
/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/

#define ROTARY_ENCODER_A_PIN 6
#define ROTARY_ENCODER_B_PIN 5
#define ROTARY_ENCODER_BUTTON_PIN 0


#define ROTARY_ENCODER_STEPS 2
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/
// 创建一个Bounce对象
Bounce debouncer = Bounce();

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

// 状态机变量
enum ButtonStatus {IDLE_STATE, PRESSED_STATE, RELEASED_STATE};
ButtonStatus buttonStatus = IDLE_STATE;

void setup()
{
  #ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, LOW);
  #endif

  gfx->begin();
  //gfx->invertDisplay(true);


  Serial.begin(115200);
  pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT);
  // Serial.setDebugOutput(true);
  // 设置去抖动对象
  debouncer.attach(ROTARY_ENCODER_BUTTON_PIN);
  debouncer.interval(50); // 设置去抖动时间为50毫秒

  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, 6, true); // Set boundaries for screen indexes
  rotaryEncoder.setAcceleration(25);
  rotaryEncoder.setEncoderValue(0); // Start from the first screen
  Serial.println("Arduino_GFX test!");
}
int currentScreen = 0;
bool isRotating = true;
unsigned long lastUpdate = 0; // 用于非阻塞定时器

void displayScreen(int screen)
{
  switch(screen) {
    case 0:
      gfx->fillScreen(CYAN);
      break;
    case 1:
      gfx->fillScreen(YELLOW);
      break;
    case 2:
      gfx->fillScreen(RED);
      break;
    case 3:
      gfx->fillScreen(BLUE);
      break;
    case 4:
      gfx->fillScreen(BLACK);
      break;
    case 5:
      gfx->fillScreen(WHITE);
      break;
    case 6:
      gfx->fillScreen(GREEN);
      break;
    default:
      gfx->fillScreen(CYAN);
      break;
  }
}

void loop()
{
  debouncer.update(); // 更新按钮状态
  switch (buttonStatus) {
    case IDLE_STATE:
      if (debouncer.fell()) {
        buttonStatus = PRESSED_STATE;
        isRotating = !isRotating;
        Serial.println("Button clicked!");
      }
      break;

    case PRESSED_STATE:
      if (debouncer.rose()) {
        buttonStatus = RELEASED_STATE;
      }
      break;

    case RELEASED_STATE:
      if (debouncer.read()) {
        buttonStatus = IDLE_STATE;
      }
      break;
  }

  // 显示当前按钮状态(检测按钮状态时调试使用)
  // Serial.print("Button State: ");
  // Serial.println(debouncer.read() ? "Released" : "Pressed");

  unsigned long currentMillis = millis(); //millis() 为非阻塞定时器 实现定时更新屏幕，避免长时间的阻塞操作，否则会阻塞检测按键状态
  if (isRotating && (currentMillis - lastUpdate >= 1000)) {
    lastUpdate = currentMillis;
    currentScreen = (currentScreen + 1) % 7;
    displayScreen(currentScreen);
  }

  if (!isRotating && rotaryEncoder.encoderChanged()) {
        currentScreen = rotaryEncoder.readEncoder();
        displayScreen(currentScreen);
    }
  // delay(50); // 50ms 延时，避免串口输出过快
}