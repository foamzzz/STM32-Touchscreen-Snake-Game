/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       ͨ�ö�ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200420
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/LCD/lcd.h"

/******************************************************************************************/
/* ͨ�ö�ʱ�� ���� */

/* TIMX �ж϶��� 
 * Ĭ�������TIM2~TIM5, TIM12~TIM17.
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM17����һ����ʱ��.
 */
 
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 ʱ��ʹ�� */


#define INITIAL_ARR (5000 - 1)   // �趨ARR�ĳ�ʼֵ
#define ARR_MIN 2000        // �趨ARR�����ֵ
/******************************************************************************************/
#define GAME_SCREEN_WIDTH 240
#define GAME_SCREEN_HEIGHT 150
#define SNAKE_SIZE 10  


#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4


#define UP_X1 95
#define UP_Y1 202
#define UP_X2 145
#define UP_Y2 252

#define DOWN_X1 95
#define DOWN_Y1 262
#define DOWN_X2 145
#define DOWN_Y2 312

#define LEFT_X1 35
#define LEFT_Y1 262
#define LEFT_X2 85
#define LEFT_Y2 312

#define RIGHT_X1 155
#define RIGHT_Y1 262
#define RIGHT_X2 205
#define RIGHT_Y2 312

//̰������Ϣ
typedef struct {
    int x[100];  
    int y[100];
    int length;  
    int direction;
} Snake;

//ʳ����Ϣ
typedef struct {
    int x;
    int y;
} Food;

extern Snake snake;
extern Food food;

//void draw_2048_grid();
void game_explain();
void game_loop(void);
void Show_Score();
void init_snake(Snake *snake);
void init_food(Food *food, Snake *snake);
void clear_snake(Snake *snake);
void draw_user();
void gtim_timx_int_init(uint16_t arr, uint16_t psc);        /* ͨ�ö�ʱ�� ��ʱ�жϳ�ʼ������ */

#endif

















