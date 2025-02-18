/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ͨ�ö�ʱ���ж� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� MiniSTM32 V4������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* ����ʱ��, 72Mhz */
    delay_init(72);                         /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                                         /* ��ʼ��LCD */
		tp_dev.init();                      /* ��������ʼ�� */
	  extix_init();                          /* ��ʼ���ⲿ�ж����� */
    g_point_color = RED;
    //sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id);  /* ��LCD ID��ӡ��lcd_id���� */
		show_picture(0,0,224,205,(uint16_t *)gImage_1);
		Show_Graph(50,250,(uint8_t*)lin,16,0);
		delay_ms(2000);//����У�պ�����2��
		lcd_clear(WHITE);
		//draw_2048_grid();
	  game_explain();

    while (1)
    {		

			  
    }
}
