/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       通用定时器中断 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 MiniSTM32 V4开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/TOUCH/touch.h"
#include "./BSP/EXTI/exti.h"
#include "array_chinese.h"


int main(void)
{
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    led_init();                             /* 初始化LED */
    lcd_init();                                         /* 初始化LCD */
		tp_dev.init();                      /* 触摸屏初始化 */
	  extix_init();                          /* 初始化外部中断输入 */
    g_point_color = RED;
    //sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* 将LCD ID打印到lcd_id数组 */
		show_picture(0,0,224,205,(uint16_t *)gImage_1);
		Show_Graph(50,250,(uint8_t*)lin,16,0);
		delay_ms(2000);//保持校徽和人名2秒
		lcd_clear(WHITE);
		//draw_2048_grid();
	  game_explain();

    while (1)
    {		

			  
    }
}
