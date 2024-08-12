/*******************************************************************************
 * Touch libraries:
 * FT6X36: https://github.com/strange-v/FT6X36.git
 * GT911: https://github.com/TAMCTec/gt911-arduino.git
 * XPT2046: https://github.com/PaulStoffregen/XPT2046_Touchscreen.git
 ******************************************************************************/

#define Touch_CHSC6540
#if defined(Touch_CHSC6540)
#include <Wire.h> 
#include "Arduino.h"
// #include "Touch_CHSC6540.h"
#define TOUCH_SDA  9
#define TOUCH_SCL  46
#define TOUCH_INT  8
#define TOUCH_RST  3
#define TOUCH_WIDTH  320
#define TOUCH_HEIGHT 170

#define CHSC6540_ADDR (uint8_t)(0x2E)   //写入地址，7bit-0x2E，bit-0x5C

#define CNT_FOR_POINT 		3   	//Read data at one time
#define CT_MAX_TOUCH    	1    	//The number of points supported by the capacitor screen is up to 1 points
#define CHSC6540_CONFIG_SIZE    (CNT_FOR_POINT*CT_MAX_TOUCH)

#define TP_LEVEL    	1		//触摸方向,0:竖直 1:水平
#define X_MIRRORING 	0		//X轴镜像,0:否 1:是
#define Y_MIRRORING 	1		//Y轴镜像,0:否 1:是

// CHSC6540触摸控制器的函数原型
void CHSC6540_init(void);
uint8_t CHSC6540_Scan(uint16_t* x, uint16_t* y);

bool touch_touched_flag = false;

void reset() {
  pinMode(TOUCH_INT, OUTPUT);
  pinMode(TOUCH_RST, OUTPUT);
  digitalWrite(TOUCH_INT, 0);
  digitalWrite(TOUCH_RST, 0);
  delay(10);
  digitalWrite(TOUCH_INT, 1);
  delay(1);
  digitalWrite(TOUCH_RST, 1);
  delay(5);
  digitalWrite(TOUCH_INT, 0);
  delay(50);
  pinMode(TOUCH_INT, INPUT);
}

uint8_t readData(uint8_t *buf, uint8_t size) {
  uint8_t ret = 0;
  Wire.beginTransmission((uint8_t)0x2E);
  if(Wire.endTransmission()==0){
    Wire.requestFrom(CHSC6540_ADDR, size);
    for(uint8_t i=0; i<size; i++){
      buf[i] = Wire.read();
    }
    ret = 1;
  }
  delay(50);
  return ret;
}

void CHSC6540_init(void){
  Wire.setPins(TOUCH_SDA, TOUCH_SCL);
  Wire.begin();
  reset();
}

static uint8_t times = 0;
uint8_t CHSC6540_Scan(uint16_t* x, uint16_t* y){
  uint8_t buf[CHSC6540_CONFIG_SIZE] = {0};
  uint8_t x_h8=0, y_h8=0, ret=0, ret2=0;
  uint16_t tx=0, ty=0, tpx=0, tpy=0;

  ret = readData(buf, CHSC6540_CONFIG_SIZE);
  if(ret == 1){
    y_h8 = buf[0]>>8;
    x_h8 = (buf[0]>>7);
    if(x_h8 == 1){ ty = 256 + buf[2];  }  
    else{ ty = buf[2];  }
    if(y_h8 == 1){ tx = 256 + buf[1];  }  
    else{ tx = buf[1];  }
    if(TP_LEVEL) {
        tpx = ty;
        tpy = tx;				
    }else {					
        tpx = tx;
        tpy = ty;	
    }
    if(X_MIRRORING){  *x = TOUCH_WIDTH - tpx;  }
    else{*x = tpx;}
    if(Y_MIRRORING){  *y = TOUCH_HEIGHT- tpy; }
    else{*y = tpy;}
    ret2 = 1;
    
  }
  return ret2;
}
#endif//CHSC6540

