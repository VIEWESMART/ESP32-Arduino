# 如何在Arduino IDE中安装esp32

* [English Version](./How_To_Configure_Arduino-esp32.md)

## 目录

- [如何在Arduino IDE中安装esp32](#如何在Arduino-IDE中安装esp32)
  - [安装Arduino IDE](#安装Arduino-IDE)
  - [安装esp32](#安装esp32)

  ## 安装Arduino IDE
  * [`点击`](https://www.arduino.cc/en/software) 这去进行下载
      *  找到 `Downloads` 并根据自己的系统选择对应的版本
      ![image](https://github.com/user-attachments/assets/7b2e1bde-566a-45b8-a1b6-027e4b473356)
      *  点击 `Just Download` 
      ![image](https://github.com/user-attachments/assets/84290f8a-55b1-4d0a-8373-4375d6fe45aa)
      ![image](https://github.com/user-attachments/assets/34f7f218-c5db-4d8f-a195-5a8c65501d78)
      * 下载完成之后安装即可。

  ## 安装esp32
  * 打开Arduino IDE
      ![image](https://github.com/user-attachments/assets/cb15d47b-ee2b-4fd7-b1c1-14518b545d35)
  * 如果出现如下画面点击安装即可
      ![image](https://github.com/user-attachments/assets/c6a3cb21-55d3-4aa1-8c5e-4ba4845acb96)
  * 设置语言
      * 点击 `文件- > 首选项`
      ![image](https://github.com/user-attachments/assets/628614e3-5151-4f2e-91f8-394ddb67a3ce)
      ![image](https://github.com/user-attachments/assets/45ba4791-4ef4-40a9-b7d4-5c1223ed9c11)
  * 设置 `其他开发板管理器地址`: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
      ![image](https://github.com/user-attachments/assets/14b6cdcd-3487-48f9-bb0d-5d0184e18ab1)

      * `注意`:
          * 您可以在不设置附加板管理器 URL 的情况下下载 esp32，但提供的版本不完整，需要精确控制版本时不太方便
          
  * 打开 arduino 并在第二个侧边栏中搜索 “esp32” 以安装第二个。安装时间可能很长，请耐心等待
    ![image](https://github.com/user-attachments/assets/a1d597df-0410-439c-aa5e-089a0c3bdef7)
     * 如果下载失败，重置其他开发板管理器地址:
          *   https://espressif.github.io/arduino-esp32/package_esp32_index.json
          *   https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json
      * 还有一种情况你无法下载成功，你的网络无法访问GitHub（即不能访问外网，无法翻墙），不过无需担心，这里也将给你提供一种方法。点击链接去下载对应的版本即可。（还在整理中，给你带来不便抱歉） 




 


  

