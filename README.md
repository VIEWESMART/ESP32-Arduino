# ESP32-Arduino
* [中文版本](./README_CN.md)
## explain
This repository mainly introduces the usage of ESP32 in Arduino, covering various aspects such as examples, environment configuration, frequently asked questions (FAQ), and driver libraries. The following is the structure of the repository, which will help you find the corresponding files.
## Contents
* `ESP32-Arduino`
  * `doc`
  * `examples`
    * `1.3inch`
    * `1.9inch`
    * `...` 
  * `general example`
    * `LVGL/v8`
    * `Panel/PanelTest`
    * `PlatformIO`
    * `SquareLine/v8`
## examples
 It mainly contains some display driver examples with LVGL added for different models and sizes
 ## general example
 It is mainly applicable to touch-enabled display screens,But the 1.9-inch ones are not included for the time being
 ## doc
Super detailed Arduino environment configuration

<table>  
<tr><td>
 
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
<tr><td>
  <table>
