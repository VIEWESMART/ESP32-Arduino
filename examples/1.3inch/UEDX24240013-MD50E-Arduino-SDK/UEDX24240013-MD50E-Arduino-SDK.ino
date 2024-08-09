#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Encoder.h>
#include <Bounce2.h>

#define GFX_BL 8

Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 10 /* CS */, 1 /* SCK */, 0 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);

#define ROTARY_ENCODER_A_PIN 7
#define ROTARY_ENCODER_B_PIN 6
#define ROTARY_ENCODER_BUTTON_PIN 9

Encoder myEnc(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);
Bounce2::Button button = Bounce2::Button();


/* Change to your screen resolution */
static uint32_t screenWidth = 240;
static uint32_t screenHeight = 240;
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


/*
FM radio tunner is suposed to set frequency between 88.0 MHz and 104.0 MHz by 0.1MHz steps
Rotary encoder works with integers so we will map 88.0 to 166 and then divide by 10 to get 0.1 steps
frequency = rotaryValue / 2;
*/


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

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup started");

  #ifdef GFX_BL //Turn on the backlight
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, LOW);
  #endif
  //initializing gfx
  gfx->begin(80000000);
  Serial.println("GFX initialized"); 

  lv_init();
  delay(10);
  Serial.println("LVGL initialized");

  // Initializing buttons
  button.attach(ROTARY_ENCODER_BUTTON_PIN, INPUT);
  button.interval(5);
  Serial.println("btn initialized");

  // Initialize the rotation encoder
  myEnc.write(0); // Initialize the value of the encoder to 0

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
    //lv_demo_benchmark();
    // lv_demo_music();
    // lv_demo_stress();
 
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

