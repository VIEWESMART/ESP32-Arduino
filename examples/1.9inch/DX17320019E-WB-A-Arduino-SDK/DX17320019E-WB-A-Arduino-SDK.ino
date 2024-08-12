
/*******************************************************************************
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
// #include "Touch_CHSC6540.h"
// 定义触摸屏的I2C引脚
#define TP_SDA 9
#define TP_SCL 46
#define TP_INT_PIN 8
#define TP_RST_PIN 3

// 触摸屏分辨率
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 170
#define TFT_BL 38
#define GFX_BL DF_GFX_BL

Arduino_DataBus *bus = new Arduino_ESP32SPI(11 /* DC */, 10 /* CS */, 12 /* SCK */, 13 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
//Arduino_GFX *gfx = new Arduino_ST7789(bus, 1 /* RST */, 1 /* rotation */, false /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 1 /* RST */, 1 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);

/*******************************************************************************
 * Please config the touch panel in touch.h
 ******************************************************************************/
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
#include <lvgl.h>
#include "touch.h"
//#include <demos\lv_demos.h>

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
lv_disp_draw_buf_t draw_buf;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;
lv_disp_drv_t disp_drv;
lv_event_code_t code;
static uint16_t x, y;
static uint8_t ret;

static uint8_t last_ret = 0;

SemaphoreHandle_t xMutex;  // 定义一个互斥锁

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
   if (xSemaphoreTake(xMutex, pdMS_TO_TICKS(10)) == pdTRUE) {  // 获取互斥锁
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
  xSemaphoreGive(xMutex); // 释放互斥锁
  } else {
    Serial.println("my_touchpad_read: Failed to obtain mutex");
  }
}


void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  if (xSemaphoreTake(xMutex, pdMS_TO_TICKS(10)) == pdTRUE) {  // 获取互斥锁
  ret = CHSC6540_Scan(&x, &y);
  if (ret == 1 && last_ret == 1) {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = x;
    data->point.y = y;
    Serial.println("Touch detected");
  } else {
    data->state = LV_INDEV_STATE_REL;
    Serial.println("No touch detected");
  }
  last_ret = ret;
  xSemaphoreGive(xMutex); // 释放互斥锁
  } else {
    Serial.println("my_touchpad_read: Failed to obtain mutex");
  }
}

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

// lvgl -ui -------------------------------------------------------------------------------

static void event_handler(lv_event_t *e) {
  code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    LV_LOG_USER("Clicked");
    Serial.println("Test is Clicked!");
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    LV_LOG_USER("Toggled");
    Serial.println("Toggled is Clicked!");
  }
}

void lv_example_btn_1(void) {
  lv_obj_t *label;

  lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
  lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);

  label = lv_label_create(btn1);
  lv_label_set_text(label, "test");
  lv_obj_center(label);
  lv_obj_add_flag(btn1, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);

}

// lvgl -ui end-----------------------------------------------------------------------------

void lvglTask(void *pvParameters) {
    while (1) {
        lv_timer_handler(); // 让GUI完成工作
        vTaskDelay(pdMS_TO_TICKS(8)); // 延迟8ms
    }
}


void setup() {

  Serial.begin(115200);

  Serial.println("LVGL Widgets Demo");
  
  delay(20);
 

  // Init Display
  gfx->begin(80000000);
  gfx->invertDisplay(true);
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif

  CHSC6540_init();
  Serial.println("Touch screen initialized");

  lv_init();
  delay(10);



  screenWidth = gfx->width();
  screenHeight = gfx->height();

  
  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 8, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  if (!disp_draw_buf1 && !disp_draw_buf2) {
    Serial.println("LVGL disp_draw_buf allocate failed!");
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

        /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);
    
    lv_example_btn_1();
    //lv_demo_benchmark();
    // lv_demo_music();
    // lv_demo_stress();


    Serial.println("Setup done");

    xMutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(
      lvglTask,     // 任务函数
      "LVGL Task",  // 任务名称
      4096*2,         // 堆栈大小（字节）
      NULL,         // 任务参数
      1,            // 优先级
      NULL,         // 任务句柄
      1             // 运行在Core 1上
    );

  }
}


void loop()
{

  //lvgl loop
  // lv_timer_handler(); /* let the GUI do its work */
 // delay(10);

}