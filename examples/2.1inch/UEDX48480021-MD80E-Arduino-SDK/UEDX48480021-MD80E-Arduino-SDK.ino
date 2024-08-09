
/*******************************************************************************
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Encoder.h>
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


Encoder myEnc(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
Bounce2::Button button = Bounce2::Button();

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;
lv_disp_drv_t disp_drv;
lv_group_t *group; // Create a group object

static int32_t last_encoder_value = 0; // The last encoder value

lv_obj_t *page1, *page2, *page3;
int currentPage = 1;
bool autoScroll = true; // Automatic cycle flag

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

static void my_button_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {

}


static void my_encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {

}

void lv_example_page(void) {
  page1 = lv_obj_create(NULL);
  lv_obj_t *label1 = lv_label_create(page1);
  lv_label_set_text(label1, "This is Page 1");
  lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

  page2 = lv_obj_create(NULL);
  lv_obj_t *label2 = lv_label_create(page2);
  lv_label_set_text(label2, "This is Page 2");
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);

  page3 = lv_obj_create(NULL);
  lv_obj_t *label3 = lv_label_create(page3);
  lv_label_set_text(label3, "This is Page 3");
  lv_obj_align(label3, LV_ALIGN_CENTER, 0, 0);

  lv_scr_load(page1); // Loading the initial page
}
// // 状态机变量
// enum ButtonStatus {IDLE_STATE, PRESSED_STATE, RELEASED_STATE};
// ButtonStatus buttonStatus = IDLE_STATE;

void setup()
{
  #ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, LOW);
  #endif

  gfx->begin();
  //gfx->invertDisplay(true);
  Serial.begin(115200);

  lv_init();
  delay(10);
  Serial.println("LVGL initialized");

  // Initializing buttons
  button.attach(ROTARY_ENCODER_BUTTON_PIN, INPUT);
  button.interval(5);
  Serial.println("btn initialized");

  // Initialize the rotation encoder
  myEnc.write(0); // Initialize the value of the encoder to 0

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  if (!disp_draw_buf1 && !disp_draw_buf2) {
    Serial.println("LVGL disp_draw_buf allocate failed!");
    return;
  } else {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf1, disp_draw_buf2, screenWidth * screenHeight / 8);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    Serial.println("Display initialized");

    
    /* Initialize the (dummy) input device driver */
    group = lv_group_create();
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = my_encoder_read;
    lv_indev_t *indev_encoder = lv_indev_drv_register(&indev_drv);
    lv_indev_set_group(indev_encoder, group);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_BUTTON;
    indev_drv.read_cb = my_button_read;
    lv_indev_t *indev_button = lv_indev_drv_register(&indev_drv);
    lv_indev_set_group(indev_button, group);
    Serial.println("Input devices initialized");

    lv_example_page();
    Serial.println("Example page loaded");
  }

  xTaskCreatePinnedToCore(
    lvglTask,        // Task function
    "lvglTask",      // Task name
    8192,            // Stack size
    NULL,            // Parameters
    2,               // Priority
    NULL,            // Task handle
    1                // Run on core 1
  );

  xTaskCreatePinnedToCore(
    inputTask,       // Task function
    "inputTask",     // Task name
    8192,            // Stack size
    NULL,            // Parameters
    2,               // Priority
    NULL,            // Task handle
    1                // Run on core 1
  );

  Serial.println("Setup done");

}

void loop()
{
  
}

// Task handler function
void lvglTask(void *pvParameters) {
  Serial.println("lvglTask started");
  while (true) {
    lv_timer_handler(); // Handle LVGL tasks
    vTaskDelay(pdMS_TO_TICKS(5)); // delay 5ms
  }
}

// Input processing tasks
void inputTask(void *pvParameters) {
  Serial.println("inputTask started");
  static unsigned long lastTime = 0;
  unsigned long currentTime;

  while (true) {
    currentTime = millis();

    // Automatically cycle through pages
    if (autoScroll && (currentTime - lastTime >= 2000)) { // It switches every 2s
      Serial.print("Switching to page: ");
      Serial.println(currentPage + 1); // Debugging output
      currentPage++;
      if (currentPage > 3) {
          currentPage = 1;
      }
      lv_scr_load(currentPage == 1 ? page1 : (currentPage == 2 ? page2 : page3));
      lastTime = currentTime;
    }

    // Each loop reads the input device state
    button.update();
    if (button.pressed()) {
        Serial.println("Button pressed");
        autoScroll = !autoScroll; // Toggle the auto cycling state
    }

    int32_t current_encoder_value = myEnc.read() / 2;
    int32_t encoder_diff = current_encoder_value - last_encoder_value;

    if (!autoScroll && encoder_diff != 0) {
      Serial.print("Encoder diff: ");
      Serial.println(encoder_diff);
      currentPage += encoder_diff;
      if (currentPage < 1) {
          currentPage = 3;
      } else if (currentPage > 3) {
          currentPage = 1;
      }
      lv_scr_load(currentPage == 1 ? page1 : (currentPage == 2 ? page2 : page3));
      last_encoder_value = current_encoder_value;
    }

    vTaskDelay(pdMS_TO_TICKS(50)); // delay is 50 milliseconds
  }
}

void app_main() {
  setup(); // initial setup

    // Creating tasks
    // xTaskCreate(lvglTask, "lvglTask", 8192, NULL, 2, NULL);
    // xTaskCreate(inputTask, "inputTask", 8192, NULL, 2, NULL);
  xTaskCreatePinnedToCore(
  lvglTask, "lvglTask", 8192*2, NULL, 2, NULL, 1
  );
  xTaskCreatePinnedToCore(
    inputTask, "inputTask", 8192*2, NULL, 2, NULL, 1
  );
  Serial.println("Tasks created");
}
