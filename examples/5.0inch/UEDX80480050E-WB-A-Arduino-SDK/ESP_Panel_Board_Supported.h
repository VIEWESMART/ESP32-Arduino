/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/* Set to 1 if using a supported board */
#define ESP_PANEL_USE_SUPPORTED_BOARD       (1)         // 0/1

#if ESP_PANEL_USE_SUPPORTED_BOARD
/**
 * Uncomment one of the following macros to select an supported development board. If multiple macros are uncommented
 * at the same time, an error will be prompted during compilation.
 *
 */

/*
 * viewe Supported Boards :
 *UEDX80480043E-WB-A (BOARD_VIEWE_ESP_S3_Touch_LCD_43)
 *UEDX80480050E-WB-A (BOARD_VIEWE_ESP_S3_Touch_GT911_LCD_50 and // #define BOARD_VIEWE_ESP_S3_Touch_FT5x06_LCD_50)
 *UEDX80480070E-WB-A (BOARD_VIEWE_ESP_S3_Touch_LCD_70)
 *
 */


/*viewe*/
// #define BOARD_VIEWE_ESP_S3_Touch_LCD_40
// #define BOARD_VIEWE_ESP_S3_Touch_LCD_43
#define BOARD_VIEWE_ESP_S3_Touch_GT911_LCD_50
// #define BOARD_VIEWE_ESP_S3_Touch_FT5x06_LCD_50
// #define BOARD_VIEWE_ESP_S3_Touch_LCD_70
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////// File Version ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Do not change the following versions, they are used to check if the configurations in this file are compatible with
 * the current version of `ESP_Panel_Board_Supported.h` in the library. The detailed rules are as follows:
 *
 *   1. If the major version is not consistent, then the configurations in this file are incompatible with the library
 *      and must be replaced with the file from the library.
 *   2. If the minor version is not consistent, this file might be missing some new configurations, which will be set to
 *      default values. It is recommended to replace it with the file from the library.
 *   3. If the patch version is not consistent, it will not affect normal functionality.
 *
 */
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MAJOR 0
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_MINOR 3
#define ESP_PANEL_BOARD_SUPPORTED_FILE_VERSION_PATCH 0

#endif
