STM32 Touchscreen Snake Game
Introduction
This project implements a Snake game on the STM32F103RCT6 using a 2.8-inch TFT screen.

CSDN article: https://blog.csdn.net/foamz/article/details/145566223

Key Features:
External Interrupts: Four buttons are used with external interrupts.
Timer-Controlled Updates: A timer controls the game updates.
Touchscreen Input: The touchscreen is used to change the snake’s movement direction.
Usage Instructions
main.c: Main loop of the program.
gtim.c: Contains most of the functions for running the Snake game. Refer to gtim.h.
lcd.c: Contains functions for displaying Chinese characters and images. Refer to lcd.h.
exti.c: External interrupt functions.
array_chinese.c: Stores image and character arrays.
array_chinese.h: Extern declarations.
