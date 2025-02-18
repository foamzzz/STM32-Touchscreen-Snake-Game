/**
 ****************************************************************************************************
 * @file        gtim.c
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
 * V1.0 20211216
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "array_chinese.h"
#include "./BSP/TOUCH/touch.h"
#include "./SYSTEM/delay/delay.h"
//#include "./BSP/KEY/key.h"

#include <stdlib.h>  
#include <stdio.h>




Snake snake;
Food food;

//蛇身体初始化数据
void init_snake(Snake *snake) {
    snake->length = 3; // 初始化蛇长度为3
    snake->direction = RIGHT; // 初始运动方向
    for (int i = 0; i < snake->length; i++) {
        snake->x[i] = 40 - i * SNAKE_SIZE; // 初始X位置
        snake->y[i] = 40; // 初始Y位置
    }
}

//蛇身体绘制
void draw_snake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        lcd_fill(snake->x[i], snake->y[i], snake->x[i] + SNAKE_SIZE, snake->y[i] + SNAKE_SIZE, RED);
    }
}

// 清除蛇
void clear_snake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        lcd_fill(snake->x[i], snake->y[i], snake->x[i] + SNAKE_SIZE, snake->y[i] + SNAKE_SIZE, WHITE);
    }
}
// 绘制食物
void draw_food(Food *food) {
    lcd_fill(food->x, food->y, food->x + SNAKE_SIZE, food->y + SNAKE_SIZE, GREEN);
}

// 生成食物（伪随机）
void init_food(Food *food, Snake *snake) {
	int valid_position = 0;  // 是否刷新到蛇身上
  while (!valid_position) 
	{
        // 随机生成食物位置
			food->x = rand() % (GAME_SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
			food->y = rand() % (GAME_SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;

      valid_position = 1;  // 假设不在蛇身上

        // 检查是否与蛇身体重叠
      for (int i = 0; i < snake->length; i++) 
			{
				if (food->x == snake->x[i] && food->y == snake->y[i]) 
				{
            valid_position = 0;  // 位置无效，重新生成
            break;
        }
      }
  }  
	  draw_food(food);  // 绘制食物
}


// 移动蛇
void move_snake(Snake *snake) {
    // 先清除当前的蛇
    clear_snake(snake);
    
    // 将蛇向前移动
    for (int i = snake->length - 1; i > 0; i--) {
        snake->x[i] = snake->x[i - 1];
        snake->y[i] = snake->y[i - 1];
    }

    // 检测蛇的运动方向
    if (snake->direction == UP) {
        snake->y[0] -= SNAKE_SIZE;
    } else if (snake->direction == DOWN) {
        snake->y[0] += SNAKE_SIZE;
    } else if (snake->direction == LEFT) {
        snake->x[0] -= SNAKE_SIZE;
    } else if (snake->direction == RIGHT) {
        snake->x[0] += SNAKE_SIZE;
    }

    // 绘制
    draw_snake(snake);
}

// 吃到食物时
int check_food_collision(Snake *snake, Food *food) {
    if (snake->x[0] == food->x && snake->y[0] == food->y) {
        return 1;  
    }
    return 0; 
}

// 成长事件
void grow_snake(Snake *snake) {
    snake->length++;
    // 让蛇变长
    snake->x[snake->length - 1] = snake->x[snake->length - 2];
    snake->y[snake->length - 1] = snake->y[snake->length - 2];
}

// 碰撞事件
int check_collision(Snake *snake) {
    // 检测蛇和墙壁的关系
    if (snake->x[0] < 0 || snake->x[0] >= GAME_SCREEN_WIDTH || snake->y[0] < 0 || snake->y[0] >= GAME_SCREEN_HEIGHT) {
        return 1;  
    }
		//	检测蛇自己与自己碰撞
    for (int i = 1; i < snake->length; i++) {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]) {
            return 1;  
        }
    }
    return 0; 
}


void draw_control_buttons() {
    // 按键上
    lcd_draw_rectangle(UP_X1, UP_Y1, UP_X2, UP_Y2, BLUE);
    lcd_show_string((UP_X1 + UP_X2) / 2 - 8, (UP_Y1 + UP_Y2) / 2 - 8, 16, 16, 16, "UP", RED);

    // 按键下
    lcd_draw_rectangle(DOWN_X1, DOWN_Y1, DOWN_X2, DOWN_Y2, BLUE);
    lcd_show_string((DOWN_X1 + DOWN_X2) / 2 - 16, (DOWN_Y1 + DOWN_Y2) / 2 - 8, 32, 16, 16, "DOWN", RED);

    // 按键左
    lcd_draw_rectangle(LEFT_X1, LEFT_Y1, LEFT_X2, LEFT_Y2, BLUE);
    lcd_show_string((LEFT_X1 + LEFT_X2) / 2 - 16, (LEFT_Y1 + LEFT_Y2) / 2 - 8, 32, 16, 16, "LEFT", RED);

    // 按键右
    lcd_draw_rectangle(RIGHT_X1, RIGHT_Y1, RIGHT_X2, RIGHT_Y2, BLUE);
    lcd_show_string((RIGHT_X1 + RIGHT_X2) / 2 - 16, (RIGHT_Y1 + RIGHT_Y2) / 2 - 8, 40, 16, 16, "RIGHT", RED);
		
	

}


// 检测电阻屏按键是否按下
void process_touch_input(Snake *snake) {
    uint16_t x, y;

    tp_dev.scan(0); 
    if (tp_dev.sta & TP_PRES_DOWN) 
		{  // 获取触点坐标
        x = tp_dev.x[0];
        y = tp_dev.y[0];
			//检测触点是否在按键区域
			 if (x >= UP_X1 && x <= UP_X2 && y >= UP_Y1 && y <= UP_Y2) {
				  if (snake->direction != DOWN)  // 不能反方向移动会直接与自己相撞死亡
                snake->direction = UP; 
								delay_ms(200); //防止按的过快，一下按两个方向回头撞死
					}else if (x >= DOWN_X1 && x <= DOWN_X2 && y >= DOWN_Y1 && y <= DOWN_Y2) {
							if (snake->direction != UP)  
                snake->direction = DOWN;
								delay_ms(200);
					} else if (x >= LEFT_X1 && x <= LEFT_X2 && y >= LEFT_Y1 && y <= LEFT_Y2){
							if (snake->direction != RIGHT) 
                snake->direction = LEFT;
								delay_ms(200);							
					} else if (x >= RIGHT_X1 && x <= RIGHT_X2 && y >= RIGHT_Y1 && y <= RIGHT_Y2) {
							if (snake->direction !=LEFT) 
                snake->direction = RIGHT; 
								delay_ms(200);	
					}
    }
}
#define font_dis 17
void game_explain()
{
	//贪吃蛇
	Show_Graph(65,10,(uint8_t*)tan_2,24,0);
	Show_Graph(100,10,(uint8_t*)chi_2,24,0);
	Show_Graph(135,10,(uint8_t*)she_2,24,0);
	
	//控制红色的贪吃蛇
	Show_Graph(16,50,(uint8_t*)kong,16,0);
	Show_Graph(16 + font_dis,50,(uint8_t*)zhi,16,0);
	Show_Graph(16 + 2*font_dis,50,(uint8_t*)hong,16,0);
	Show_Graph(16 + 3*font_dis,50,(uint8_t*)se,16,0);
	Show_Graph(16 + 4*font_dis,50,(uint8_t*)de_1,16,0);
	Show_Graph(16 + 5*font_dis,50,(uint8_t*)tan_1,16,0);
	Show_Graph(16 + 6*font_dis,50,(uint8_t*)chi,16,0);
	Show_Graph(16 + 7*font_dis,50,(uint8_t*)she,16,0);

	//使用屏幕下方的方向键移动
	Show_Graph(16,70,(uint8_t*)shi,16,0);
	Show_Graph(16 + font_dis,70,(uint8_t*)yong,16,0);
	Show_Graph(16 + 2*font_dis,70,(uint8_t*)ping,16,0);
	Show_Graph(16 + 3*font_dis,70,(uint8_t*)mu,16,0);
	Show_Graph(16 + 4*font_dis,70,(uint8_t*)xia,16,0);
	Show_Graph(16 + 5*font_dis,70,(uint8_t*)fang,16,0);
	Show_Graph(16 + 6*font_dis,70,(uint8_t*)de_1,16,0);
	Show_Graph(16 + 7*font_dis,70,(uint8_t*)fang,16,0);
	Show_Graph(16 + 8*font_dis,70,(uint8_t*)xiang,16,0);
	Show_Graph(16 + 9*font_dis,70,(uint8_t*)jian,16,0);
	Show_Graph(16 + 10*font_dis,70,(uint8_t*)yi,16,0);
	Show_Graph(16 + 11*font_dis,70,(uint8_t*)dong,16,0);
	
	//尽可能吃掉更多的绿色食物
	Show_Graph(16,90,(uint8_t*)jin_1,16,0);
	Show_Graph(16 + font_dis,90,(uint8_t*)ke,16,0);
	Show_Graph(16 + 2*font_dis,90,(uint8_t*)neng,16,0);
	Show_Graph(16 + 3*font_dis,90,(uint8_t*)chi,16,0);
	Show_Graph(16 + 4*font_dis,90,(uint8_t*)diao,16,0);
	Show_Graph(16 + 5*font_dis,90,(uint8_t*)geng,16,0);
	Show_Graph(16 + 6*font_dis,90,(uint8_t*)duo,16,0);
	Show_Graph(16 + 7*font_dis,90,(uint8_t*)de_1,16,0);
	Show_Graph(16 + 8*font_dis,90,(uint8_t*)lv,16,0);
	Show_Graph(16 + 9*font_dis,90,(uint8_t*)se,16,0);
	Show_Graph(16 + 10*font_dis,90,(uint8_t*)shi_1,16,0);
	Show_Graph(16 + 11*font_dis,90,(uint8_t*)wu,16,0);

	//每吃一个食物速度提升5%
	Show_Graph(16,110,(uint8_t*)mei,16,0);
	Show_Graph(16 + font_dis,110,(uint8_t*)chi,16,0);
	Show_Graph(16 + 2*font_dis,110,(uint8_t*)yi_1,16,0);
	Show_Graph(16 + 3*font_dis,110,(uint8_t*)ge,16,0);
	Show_Graph(16 + 4*font_dis,110,(uint8_t*)shi_1,16,0);
	Show_Graph(16 + 5*font_dis,110,(uint8_t*)wu,16,0);
	Show_Graph(16 + 6*font_dis,110,(uint8_t*)su,16,0);
	Show_Graph(16 + 7*font_dis,110,(uint8_t*)du,16,0);
	Show_Graph(16 + 8*font_dis,110,(uint8_t*)ti,16,0);
	Show_Graph(16 + 9*font_dis,110,(uint8_t*)sheng,16,0);
	lcd_show_string(16 + 10*font_dis,110,16,16,16,"5%",RED);
	
	//碰到墙壁或自身，游戏会结束
	Show_Graph(16,130,(uint8_t*)peng,16,0);
	Show_Graph(16 + font_dis,130,(uint8_t*)dao,16,0);
	Show_Graph(16 + 2*font_dis,130,(uint8_t*)qiang,16,0);
	Show_Graph(16 + 3*font_dis,130,(uint8_t*)bi,16,0);
	Show_Graph(16 + 4*font_dis,130,(uint8_t*)huo,16,0);
	Show_Graph(16 + 5*font_dis,130,(uint8_t*)zi,16,0);
	Show_Graph(16 + 6*font_dis,130,(uint8_t*)shen,16,0);
	lcd_show_string(16 + 7*font_dis,130,8,16,16,",",RED);
	Show_Graph(8 + 8*font_dis,130,(uint8_t*)you,16,0);
	Show_Graph(8 + 9*font_dis,130,(uint8_t*)xi,16,0);
	Show_Graph(8 + 10*font_dis,130,(uint8_t*)hui,16,0);
	Show_Graph(8 + 11*font_dis,130,(uint8_t*)jie_1,16,0);
	Show_Graph(8 + 12*font_dis,130,(uint8_t*)shu,16,0);
	
	//按下WK_UP键以开始游戏
	Show_Graph(16,170,(uint8_t*)an,16,0);
	Show_Graph(16 + font_dis,170,(uint8_t*)xia,16,0);
	lcd_show_string(16 + 2*font_dis,170,40,16,16,"WK_UP",RED);
	Show_Graph(24 + 4*font_dis,170,(uint8_t*)jian,16,0);
	Show_Graph(24 + 5*font_dis,170,(uint8_t*)yi_2,16,0);
	Show_Graph(24 + 6*font_dis,170,(uint8_t*)kai_1,16,0);
	Show_Graph(24 + 7*font_dis,170,(uint8_t*)shi_2,16,0);
	Show_Graph(24 + 8*font_dis,170,(uint8_t*)you,16,0);
	Show_Graph(24 + 9*font_dis,170,(uint8_t*)xi,16,0);
}

//游戏等级分数
struct Game
{
	uint32_t Score;//分数
	uint32_t V; //速度
}game;
//显示分数
void Show_Score()
{
	game.Score=0;
	Show_Graph(10,187,(uint8_t*)de,16,0);
	Show_Graph(25,187,(uint8_t*)fen,16,0);
	lcd_show_string(40,187,8,16,16,":",RED);
	lcd_show_num(55,187,game.Score,2,16,BLUE);
	
	//速度
	Show_Graph(70,167,(uint8_t*)su,16,0);
	Show_Graph(85,167,(uint8_t*)du,16,0);
	lcd_show_string(100,167,8,16,16,":",RED);
	game.V=(INITIAL_ARR+1)/10;
	lcd_show_num(110,167,game.V,3,16,BLUE);
	lcd_show_string(135,167,24,16,16,"ms/",RED);
	Show_Graph(156,167,(uint8_t*)bu,16,0);
}
//绘制用户界面
void draw_user()
{
	Show_Graph(10,167,(uint8_t*)tan_1,16,0);
		Show_Graph(25,167,(uint8_t*)chi,16,0);
		Show_Graph(40,167,(uint8_t*)she,16,0);
		draw_control_buttons();
		Show_Score();
			//游戏边界线
		//lcd_draw_line(0,0,240,0,BLUE);
		//lcd_draw_line(0,1,240,1,BLUE);
		//lcd_draw_line(0,2,240,2,BLUE);
		//lcd_draw_line(0,0,0,160,BLUE);
		//lcd_draw_line(1,0,1,160,BLUE);
		//lcd_draw_line(2,0,2,160,BLUE);
		
		//lcd_draw_line(238,0,238,160,BLUE);
		//lcd_draw_line(239,0,239,160,BLUE);
		//lcd_draw_line(240,0,240,160,BLUE);

		
		//操作界面边界线
		//lcd_draw_line(0,160,240,160,BLUE);
		lcd_draw_line(0,161,240,161,BLUE);
		lcd_draw_line(0,162,240,162,BLUE);
		lcd_draw_line(0,160,0,320,BLUE);
		lcd_draw_line(1,160,1,320,BLUE);
		//lcd_draw_line(2,160,2,320,BLUE);
		//lcd_draw_line(238,160,238,320,BLUE);
		lcd_draw_line(239,160,239,320,BLUE);
		lcd_draw_line(240,160,240,320,BLUE);
		//lcd_draw_line(0,318,240,318,BLUE);
		lcd_draw_line(0,319,240,319,BLUE);
		lcd_draw_line(0,320,240,320,BLUE);
		
		// KEY0:重玩
		lcd_show_string(160,187,40,16,16,"KEY0:",RED);
		Show_Graph(200,187,(uint8_t*)chong,16,0);
		Show_Graph(215,187,(uint8_t*)wan,16,0);
}
	

// 游戏运行循环
void game_loop(void) {
		gtim_timx_int_init(INITIAL_ARR, 7200 - 1); /* 10Khz的计数频率，计数5K次为500ms */
		init_snake(&snake);
		draw_user();
    while (1) {
        process_touch_input(&snake);  // ????????????
				delay_ms(5);

    }
}
//绘制死亡动画
void death_animation(Snake* snake) {

    int x_start = 0, y_start = 0;        
    int x_end = 240, y_end = 160;
		uint16_t colors[] = {BLUE, GREEN, CYAN, BROWN, YELLOW, BLACK}; // 蛇可以变化的颜色
    int color_count = sizeof(colors) / sizeof(colors[0]); // 计算颜色数量

    // 让蛇的身体变化颜色
    for (int c = 0; c < color_count; c++) 
		{
			for (int i = 0; i < snake->length; i++) 
			{
        lcd_fill(snake->x[i], snake->y[i], snake->x[i] + SNAKE_SIZE, snake->y[i] + SNAKE_SIZE, colors[c]);
				delay_ms(2); // 每次颜色变化延迟一段时间
			}
        delay_ms(100); // 每次颜色变化延迟一段时间
    }
		// 由外到内矩形填充动画
    while (x_start <= x_end && y_start <= y_end) {
        lcd_draw_line(x_start, y_start, x_end, y_start, BLACK);
        y_start++;

        lcd_draw_line(x_end, y_start, x_end, y_end, BLACK);
        x_end--;

        if (y_start <= y_end) {
            lcd_draw_line(x_end, y_end, x_start, y_end, BLACK);
            y_end--;
        }

        if (x_start <= x_end) {
            lcd_draw_line(x_start, y_end, x_start, y_start - 1, BLACK);
            x_start++;
        }

        delay_ms(50);  
    }
}
// 死亡事件
void game_over(Snake* snake) {
    //lcd_clear(WHITE);
	
		death_animation(snake);
    // "GAME OVER!"
		g_back_color=BLACK;//改变game over底色适配黑色
    lcd_show_string(40, 75, 64, 32, 32, "GAME", RED);
		lcd_show_string(120, 75, 80, 32, 32, "OVER!", RED);
		g_back_color=WHITE;
		
		//关闭定时器中断，停止更新蛇。
		HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);
    delay_ms(2000);
}

TIM_HandleTypeDef g_timx_handle;  /* 定时器x句柄 */

/**
 * @brief       通用定时器TIMX定时中断初始化函数
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();                                 /* 使能TIMx时钟 */

    g_timx_handle.Instance = GTIM_TIMX_INT;                     /* 通用定时器x */
    g_timx_handle.Init.Prescaler = psc;                         /* 预分频系数 */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    g_timx_handle.Init.Period = arr;                            /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 3);             /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                     /* 开启ITMx中断 */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                      /* 使能定时器x和定时器x更新中断 */
}


/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_INT_IRQHandler(void)
{
    /* 以下代码没有使用定时器HAL库共用处理函数来处理，而是直接通过判断中断标志位的方式 */
    if(__HAL_TIM_GET_FLAG(&g_timx_handle, TIM_FLAG_UPDATE) != RESET)
    {
				move_snake(&snake);  //移动蛇

        if (check_food_collision(&snake, &food)) {  // 看有没有吃到食物
            grow_snake(&snake);
            init_food(&food,&snake); 
						game.Score++;
						lcd_show_num(55,187,game.Score,2,16,BLUE);
						uint16_t new_arr = __HAL_TIM_GET_AUTORELOAD(&g_timx_handle) * 95 / 100; // 让 ARR的装填值减少5%，提升5%的行进速度
            if (new_arr < ARR_MIN) {
                new_arr = ARR_MIN;  //设定最低值，防止速度过快。
            }
						game.V=(new_arr+1)/10;
						lcd_show_num(110,167,game.V,3,16,BLUE);
					  draw_food(&food);  // 绘制食物
            __HAL_TIM_SET_AUTORELOAD(&g_timx_handle, new_arr); // ???? ARR ?
        }

        if (check_collision(&snake)) { 
            game_over(&snake); 
        }

        __HAL_TIM_CLEAR_IT(&g_timx_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
    }
}
