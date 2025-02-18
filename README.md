stm32触屏贪吃蛇
介绍
使用stm32f103RCT6, 2.8寸tft屏幕 CSDN文章​​https://blog.csdn.net/foamz/article/details/145566223
四个按键使用外部中断，定时器控制游戏更新，触屏操作改变蛇移动前进方向。
使用说明
主要文件 main.c 主循环
gtim.c 贪吃蛇运行大部分函数放在里面 gtim.h
lcd.c 汉字显示，图片显示函数放在里面 lcd.h
exti.c 外部中断函数
array_chinese.c 存放图片和字模数组。 array_chinese.h extern声明


Using STM32F103RCT6, 2.8-inch TFT screen. CSDN article: https://blog.csdn.net/foamz/article/details/145566223.

Four buttons use external interrupts, and a timer controls the game updates. The touchscreen is used to change the snake’s movement direction.

Usage Instructions:

main.c: Main loop of the program.
gtim.c: Contains most of the functions for running the Snake game. gtim.h.
lcd.c: Contains functions for displaying Chinese characters and images. lcd.h.
exti.c: External interrupt functions.
array_chinese.c: Stores image and character arrays. array_chinese.h for extern declarations.
