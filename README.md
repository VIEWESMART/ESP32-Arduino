# ESP32-Arduino
This repository mainly introduces the usage of ESP32 in Arduino, covering various aspects such as examples, environment configuration, frequently asked questions (FAQ), and driver libraries. The following is the structure of the repository, which will help you find the corresponding files:
* `ESP32-Arduino`
  * `doc`
  * `examples`(It mainly contains some display driver examples with LVGL added for different models and sizes)
    * `1.3inch`
    * `1.9inch`
    * `...` 
  * `general example`(It is mainly applicable to touch-enabled display screens,But the 1.9-inch ones are not included for the time being)
    * `LVGL/v8`
    * `Panel/PanelTest`
    * `PlatformIO`
    * `SquareLine/v8`
## examples
 This warehouse mainly provides you with the Arduino demo of VIEWE's smart screen development board. Among them, the display screen, the knob encoder and the button of the knob smart screen have been driven, and a simple LVGL example has been added. For touch screens we also drive the display, touch and add a simple example of LVGL. So you can use it for free development.
 ## doc
Super detailed Arduino environment configuration

# WHO WE ARE
### We are Display Expert
  ####
 VIEWE(www.viewedisplay.com) focus on IOT Smart GUI and display & touch total solutions. VIEWE is a high-tech enterprise that integrates R&D, production and sales service.
 ####   Adhering to the core values of professionalism, dedication, innovation and ambition, the company is customer-centric and innovation-driven, is determined to become the most valuable and respected leading enterprise in the field of Display & GUI.

#### Total solutions of Display Screen, Touch Screen and Full Lamination
#### Smart Display: Uart Display, IOT Display, Raspberry Pi Display, Arduino Display…
#### Innovative Display: Ecolity Display, Sunreadable Display, NearEye Display..

# WIKI:
## www.display-wiki.com

 # The following is the display of various models of smart screen development boards:
  ### The image below is just a rendering, and each screen size is not the same.
 ###          1.3inch:
![1.3Primary](https://github.com/VIEWESMART/image/blob/main/1.3inch_Primary.png)![1.3secondary](https://github.com/VIEWESMART/image/blob/main/1.3inch_Secondary.png)
 
###           2.1inch:
![2.1Primary](https://github.com/VIEWESMART/image/blob/main/2.1inch_Primary.png)![2.1secondary](https://github.com/VIEWESMART/image/blob/main/2.1inch_Secondary.png)

###          4.0inch:
![4.0Primary](https://github.com/VIEWESMART/image/blob/main/4.0inch_P.png)![4.0secondary](https://github.com/VIEWESMART/image/blob/main/4.0inch_S.png)

###          4.3inch:
![4.3Primary](https://github.com/VIEWESMART/image/blob/main/4.3inch_P.png)![4.3secondary](https://github.com/VIEWESMART/image/blob/main/4.3inch_S.png)
 
###           5.0inch:
![5.0Primary](https://github.com/VIEWESMART/image/blob/main/5.0inch_P.png)![5.0secondary](https://github.com/VIEWESMART/image/blob/main/5.0inch_S.png)

###          7.0inch:
![7.0Primary](https://github.com/VIEWESMART/image/blob/main/7.0inch_P.png)![7.0secondary](https://github.com/VIEWESMART/image/blob/main/7.0inch_S.png)

### VIEWE Store
### www.aliexpress.com/store/1103793083
# For detailed Arduino configuration, please refer to:
## https://github.com/VIEWESMART/ESP32-Arduino/tree/main/doc


# Here's a quick overview of the Arduino configuration:
## Download and install Arduino IDE.
## Once installed, proceed to the following steps
### (1)Open the Arduino IDE.
### (2)Click on the File menu on the top menu bar.
### (3)Click on the Preferences menu item. This will open a Preferences dialog box.
### (4)You should be on the Settings tab in the Preferences dialog box by default.
### (5) Look for the textbox labeled “Additional Boards Manager URLs ”.
### (6)If there is already text in this box add a coma at the end of it, then follow the next step.
### (7)Paste the following link into the text box  ： 
### Stable release link:
### https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json 
### Development release link:
### https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json 
### (8)Click the OK button to save the setting.
### (9)In the Arduino I DE click on the Tools menu on the top menu bar.
### (10)Scroll down to the Board: entry
### (11)A sub menu will open when you highlight the Board: entry.
### (12)At the top of the sub menu is Boards Manager. Click on it to open the Boards Manager dialog box.
### (13)In the search box in the Boards Manager enter “ESP32 ”.
### You should see an entry for “esp32 by Espressif Systems ”. Highlight this entry and click on the Install button.
### (15)Click tools and then click “Board”
### (16)Let's go to “esp32”
### (17)Select the appropriate development board here select “ESP32S3 Dev Module”
### (18)CPU, Flash Mode, Flash Size, Partition Scheme, PSRAM.(For these configurations, please check the corresponding specifications of the product. If there are no special instructions, please follow this configuration)
### (19)Set and In the programmer entry of the same tab, we choose “esptool ”.
## Library Installation

