<div>
    <a href="https://github.com/VIEWESMART/ESP32-Arduino"><img src="https://img.shields.io/badge/versions-0.0.1-c32136" /></a>&emsp;
 <div>
  
# ESP32-Arduino
* [中文版本](./README_CN.md)
## explain
The ESP32-Arduino repository is an example library of the Arduino framework specifically designed for VIEWE development boards. In addition to the examples, it also includes introductions to the configuration of the development environment, FAQ files, and descriptions of version changes. We will also gradually enrich our examples (ranging from the most basic ones to those involving sensor usage). We are also looking forward to developers who use our development boards sharing your cases. The following is the structure of the repository, which will help you find the corresponding files.
## Table of Contents
* `ESP32-Arduino`
  * `docs`
    * `Board_Instructions.md`
    * `FAQ.md`
    * `How_To_Use.md`
  * `examples`
    * `1.3inch`
    * `1.9inch`
    * `2.1inch` 
    * `LVGL/v8`
    * `Panel/PanelTest`
    * `PlatformIO`
    * `SquareLine/v8`
    * `...`

## How to Use examples
Please refer to the documentation - [How to Use](./docs/How_To_Use.md).

`Note:` 1.3inch, 1.9inch, 2.1 inch please use the corresponding sample, other sizes can be used the rest of the sample

## Supported Development Boards and Drivers(only for the ESP32_Display_Panel library)
### Development Boards

Below is the list of [Supported Development Boards](docs/Board_Instructions.md):

| **Manufacturer** | **Board Model** |
| ---------------- | --------------- |
| [VIEWE](docs/Board_Instructions.md#viewe) | UEDX24320028E-WB-A-2.4, UEDX24320028E-WB-A-2.8, UEDX24320028E-WB-A-3.5-240x320, UEDX24320028E-WB-A-3.5-320x480, DX48480040E-WB-A, UEDX80480043E-WB-A-4.3-800x480, UEDX80480043E-WB-A-4.3-480x272, UEDX80480050E-WB-A, UEDX80480070E-WB-A|

For versions 1.3, 1.9 and 2.1, they haven't been added to the ESP32_Display_Panel yet. So initialization needs to be done with other libraries. There will be three special examples reserved. Besides that, the examples for the rest of the products are universal.
## FAQ
Please refer to the documentation - [FAQ](./docs/FAQ.md).
