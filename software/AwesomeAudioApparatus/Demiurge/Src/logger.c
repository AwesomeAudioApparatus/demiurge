/*
  Copyright 2019, Awesome Audio Apparatus.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
      limitations under the License.
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#define PRINT_BUFFER_SIZE 500

extern UART_HandleTypeDef *huart_log;

/** Custom printf function in order to use HAL_UART_Transmit()
 * @param *fmt String to print
 * @param argp Parameters list
 */
void HAL_printf_valist(const char *fmt, va_list argp) {
   char string[PRINT_BUFFER_SIZE];
   if( huart_log != NULL ){
      if (vsprintf(string, fmt, argp) > 0) {
         HAL_UART_Transmit(huart_log, (uint8_t*)string, strlen(string), HAL_MAX_DELAY); // send message via UART
      } else {
         HAL_UART_Transmit(huart_log, (uint8_t*)"E - Print\n", 14, HAL_MAX_DELAY);
      }
   }
}

/** Custom printf function, only translate to va_list arg HAL_UART.
 * @param *fmt String to print
 * @param ... Data
 */
void HAL_printf(const char *fmt, ...) {
   va_list argp;

   va_start(argp, fmt);
   HAL_printf_valist(fmt, argp);
   va_end(argp);
}

/** Generic LOG procedure
 * @param Log level
 * @param *fmt String to print
 * @param argp Parameters list
 */
static void log(uint8_t level, const char *fmt, va_list argp) {
   HAL_printf("%c - ", level);
   HAL_printf_valist(fmt, argp);
}

/** LOG procedure - Info
 * @param *fmt String to print
 * @param ... Parameters list
 */
void logI(const char* fmt, ...) {
   va_list argp;

   va_start(argp, fmt);
   log('I', fmt, argp);
   va_end(argp);
}

/** LOG procedure - Error
 * @param *fmt String to print
 * @param .. Parameters list
 */
void logE(const char* fmt, ...) {
   va_list argp;

   va_start(argp, fmt);
   log('E', fmt, argp);
   va_end(argp);
}