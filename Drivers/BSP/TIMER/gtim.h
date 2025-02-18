/**
 ****************************************************************************************************
 * @file        gtim.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       通用定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200420
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/LCD/lcd.h"

/******************************************************************************************/
/* 通用定时器 定义 */

/* TIMX 中断定义 
 * 默认是针对TIM2~TIM5, TIM12~TIM17.
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM17任意一个定时器.
 */
 
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */


#define INITIAL_ARR (5000 - 1)   // 设定ARR的初始值
#define ARR_MIN 2000        // 设定ARR的最低值
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

//贪吃蛇信息
typedef struct {
    int x[100];  
    int y[100];
    int length;  
    int direction;
} Snake;

//食物信息
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
void gtim_timx_int_init(uint16_t arr, uint16_t psc);        /* 通用定时器 定时中断初始化函数 */

#endif

















