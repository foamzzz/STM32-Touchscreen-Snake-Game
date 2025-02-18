/**
 ****************************************************************************************************
 * @file        gtim.c
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
 * V1.0 20211216
 * ��һ�η���
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

//�������ʼ������
void init_snake(Snake *snake) {
    snake->length = 3; // ��ʼ���߳���Ϊ3
    snake->direction = RIGHT; // ��ʼ�˶�����
    for (int i = 0; i < snake->length; i++) {
        snake->x[i] = 40 - i * SNAKE_SIZE; // ��ʼXλ��
        snake->y[i] = 40; // ��ʼYλ��
    }
}

//���������
void draw_snake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        lcd_fill(snake->x[i], snake->y[i], snake->x[i] + SNAKE_SIZE, snake->y[i] + SNAKE_SIZE, RED);
    }
}

// �����
void clear_snake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        lcd_fill(snake->x[i], snake->y[i], snake->x[i] + SNAKE_SIZE, snake->y[i] + SNAKE_SIZE, WHITE);
    }
}
// ����ʳ��
void draw_food(Food *food) {
    lcd_fill(food->x, food->y, food->x + SNAKE_SIZE, food->y + SNAKE_SIZE, GREEN);
}

// ����ʳ�α�����
void init_food(Food *food, Snake *snake) {
	int valid_position = 0;  // �Ƿ�ˢ�µ�������
  while (!valid_position) 
	{
        // �������ʳ��λ��
			food->x = rand() % (GAME_SCREEN_WIDTH / SNAKE_SIZE) * SNAKE_SIZE;
			food->y = rand() % (GAME_SCREEN_HEIGHT / SNAKE_SIZE) * SNAKE_SIZE;

      valid_position = 1;  // ���費��������

        // ����Ƿ����������ص�
      for (int i = 0; i < snake->length; i++) 
			{
				if (food->x == snake->x[i] && food->y == snake->y[i]) 
				{
            valid_position = 0;  // λ����Ч����������
            break;
        }
      }
  }  
	  draw_food(food);  // ����ʳ��
}


// �ƶ���
void move_snake(Snake *snake) {
    // �������ǰ����
    clear_snake(snake);
    
    // ������ǰ�ƶ�
    for (int i = snake->length - 1; i > 0; i--) {
        snake->x[i] = snake->x[i - 1];
        snake->y[i] = snake->y[i - 1];
    }

    // ����ߵ��˶�����
    if (snake->direction == UP) {
        snake->y[0] -= SNAKE_SIZE;
    } else if (snake->direction == DOWN) {
        snake->y[0] += SNAKE_SIZE;
    } else if (snake->direction == LEFT) {
        snake->x[0] -= SNAKE_SIZE;
    } else if (snake->direction == RIGHT) {
        snake->x[0] += SNAKE_SIZE;
    }

    // ����
    draw_snake(snake);
}

// �Ե�ʳ��ʱ
int check_food_collision(Snake *snake, Food *food) {
    if (snake->x[0] == food->x && snake->y[0] == food->y) {
        return 1;  
    }
    return 0; 
}

// �ɳ��¼�
void grow_snake(Snake *snake) {
    snake->length++;
    // ���߱䳤
    snake->x[snake->length - 1] = snake->x[snake->length - 2];
    snake->y[snake->length - 1] = snake->y[snake->length - 2];
}

// ��ײ�¼�
int check_collision(Snake *snake) {
    // ����ߺ�ǽ�ڵĹ�ϵ
    if (snake->x[0] < 0 || snake->x[0] >= GAME_SCREEN_WIDTH || snake->y[0] < 0 || snake->y[0] >= GAME_SCREEN_HEIGHT) {
        return 1;  
    }
		//	������Լ����Լ���ײ
    for (int i = 1; i < snake->length; i++) {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]) {
            return 1;  
        }
    }
    return 0; 
}


void draw_control_buttons() {
    // ������
    lcd_draw_rectangle(UP_X1, UP_Y1, UP_X2, UP_Y2, BLUE);
    lcd_show_string((UP_X1 + UP_X2) / 2 - 8, (UP_Y1 + UP_Y2) / 2 - 8, 16, 16, 16, "UP", RED);

    // ������
    lcd_draw_rectangle(DOWN_X1, DOWN_Y1, DOWN_X2, DOWN_Y2, BLUE);
    lcd_show_string((DOWN_X1 + DOWN_X2) / 2 - 16, (DOWN_Y1 + DOWN_Y2) / 2 - 8, 32, 16, 16, "DOWN", RED);

    // ������
    lcd_draw_rectangle(LEFT_X1, LEFT_Y1, LEFT_X2, LEFT_Y2, BLUE);
    lcd_show_string((LEFT_X1 + LEFT_X2) / 2 - 16, (LEFT_Y1 + LEFT_Y2) / 2 - 8, 32, 16, 16, "LEFT", RED);

    // ������
    lcd_draw_rectangle(RIGHT_X1, RIGHT_Y1, RIGHT_X2, RIGHT_Y2, BLUE);
    lcd_show_string((RIGHT_X1 + RIGHT_X2) / 2 - 16, (RIGHT_Y1 + RIGHT_Y2) / 2 - 8, 40, 16, 16, "RIGHT", RED);
		
	

}


// �������������Ƿ���
void process_touch_input(Snake *snake) {
    uint16_t x, y;

    tp_dev.scan(0); 
    if (tp_dev.sta & TP_PRES_DOWN) 
		{  // ��ȡ��������
        x = tp_dev.x[0];
        y = tp_dev.y[0];
			//��ⴥ���Ƿ��ڰ�������
			 if (x >= UP_X1 && x <= UP_X2 && y >= UP_Y1 && y <= UP_Y2) {
				  if (snake->direction != DOWN)  // ���ܷ������ƶ���ֱ�����Լ���ײ����
                snake->direction = UP; 
								delay_ms(200); //��ֹ���Ĺ��죬һ�°����������ͷײ��
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
	//̰����
	Show_Graph(65,10,(uint8_t*)tan_2,24,0);
	Show_Graph(100,10,(uint8_t*)chi_2,24,0);
	Show_Graph(135,10,(uint8_t*)she_2,24,0);
	
	//���ƺ�ɫ��̰����
	Show_Graph(16,50,(uint8_t*)kong,16,0);
	Show_Graph(16 + font_dis,50,(uint8_t*)zhi,16,0);
	Show_Graph(16 + 2*font_dis,50,(uint8_t*)hong,16,0);
	Show_Graph(16 + 3*font_dis,50,(uint8_t*)se,16,0);
	Show_Graph(16 + 4*font_dis,50,(uint8_t*)de_1,16,0);
	Show_Graph(16 + 5*font_dis,50,(uint8_t*)tan_1,16,0);
	Show_Graph(16 + 6*font_dis,50,(uint8_t*)chi,16,0);
	Show_Graph(16 + 7*font_dis,50,(uint8_t*)she,16,0);

	//ʹ����Ļ�·��ķ�����ƶ�
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
	
	//�����ܳԵ��������ɫʳ��
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

	//ÿ��һ��ʳ���ٶ�����5%
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
	
	//����ǽ�ڻ�������Ϸ�����
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
	
	//����WK_UP���Կ�ʼ��Ϸ
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

//��Ϸ�ȼ�����
struct Game
{
	uint32_t Score;//����
	uint32_t V; //�ٶ�
}game;
//��ʾ����
void Show_Score()
{
	game.Score=0;
	Show_Graph(10,187,(uint8_t*)de,16,0);
	Show_Graph(25,187,(uint8_t*)fen,16,0);
	lcd_show_string(40,187,8,16,16,":",RED);
	lcd_show_num(55,187,game.Score,2,16,BLUE);
	
	//�ٶ�
	Show_Graph(70,167,(uint8_t*)su,16,0);
	Show_Graph(85,167,(uint8_t*)du,16,0);
	lcd_show_string(100,167,8,16,16,":",RED);
	game.V=(INITIAL_ARR+1)/10;
	lcd_show_num(110,167,game.V,3,16,BLUE);
	lcd_show_string(135,167,24,16,16,"ms/",RED);
	Show_Graph(156,167,(uint8_t*)bu,16,0);
}
//�����û�����
void draw_user()
{
	Show_Graph(10,167,(uint8_t*)tan_1,16,0);
		Show_Graph(25,167,(uint8_t*)chi,16,0);
		Show_Graph(40,167,(uint8_t*)she,16,0);
		draw_control_buttons();
		Show_Score();
			//��Ϸ�߽���
		//lcd_draw_line(0,0,240,0,BLUE);
		//lcd_draw_line(0,1,240,1,BLUE);
		//lcd_draw_line(0,2,240,2,BLUE);
		//lcd_draw_line(0,0,0,160,BLUE);
		//lcd_draw_line(1,0,1,160,BLUE);
		//lcd_draw_line(2,0,2,160,BLUE);
		
		//lcd_draw_line(238,0,238,160,BLUE);
		//lcd_draw_line(239,0,239,160,BLUE);
		//lcd_draw_line(240,0,240,160,BLUE);

		
		//��������߽���
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
		
		// KEY0:����
		lcd_show_string(160,187,40,16,16,"KEY0:",RED);
		Show_Graph(200,187,(uint8_t*)chong,16,0);
		Show_Graph(215,187,(uint8_t*)wan,16,0);
}
	

// ��Ϸ����ѭ��
void game_loop(void) {
		gtim_timx_int_init(INITIAL_ARR, 7200 - 1); /* 10Khz�ļ���Ƶ�ʣ�����5K��Ϊ500ms */
		init_snake(&snake);
		draw_user();
    while (1) {
        process_touch_input(&snake);  // ????????????
				delay_ms(5);

    }
}
//������������
void death_animation(Snake* snake) {

    int x_start = 0, y_start = 0;        
    int x_end = 240, y_end = 160;
		uint16_t colors[] = {BLUE, GREEN, CYAN, BROWN, YELLOW, BLACK}; // �߿��Ա仯����ɫ
    int color_count = sizeof(colors) / sizeof(colors[0]); // ������ɫ����

    // ���ߵ�����仯��ɫ
    for (int c = 0; c < color_count; c++) 
		{
			for (int i = 0; i < snake->length; i++) 
			{
        lcd_fill(snake->x[i], snake->y[i], snake->x[i] + SNAKE_SIZE, snake->y[i] + SNAKE_SIZE, colors[c]);
				delay_ms(2); // ÿ����ɫ�仯�ӳ�һ��ʱ��
			}
        delay_ms(100); // ÿ����ɫ�仯�ӳ�һ��ʱ��
    }
		// ���⵽�ھ�����䶯��
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
// �����¼�
void game_over(Snake* snake) {
    //lcd_clear(WHITE);
	
		death_animation(snake);
    // "GAME OVER!"
		g_back_color=BLACK;//�ı�game over��ɫ�����ɫ
    lcd_show_string(40, 75, 64, 32, 32, "GAME", RED);
		lcd_show_string(120, 75, 80, 32, 32, "OVER!", RED);
		g_back_color=WHITE;
		
		//�رն�ʱ���жϣ�ֹͣ�����ߡ�
		HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);
    delay_ms(2000);
}

TIM_HandleTypeDef g_timx_handle;  /* ��ʱ��x��� */

/**
 * @brief       ͨ�ö�ʱ��TIMX��ʱ�жϳ�ʼ������
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ36M, ���Զ�ʱ��ʱ�� = 72Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();                                 /* ʹ��TIMxʱ�� */

    g_timx_handle.Instance = GTIM_TIMX_INT;                     /* ͨ�ö�ʱ��x */
    g_timx_handle.Init.Prescaler = psc;                         /* Ԥ��Ƶϵ�� */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ��������ģʽ */
    g_timx_handle.Init.Period = arr;                            /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 3);             /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                     /* ����ITMx�ж� */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                      /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}


/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_INT_IRQHandler(void)
{
    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ */
    if(__HAL_TIM_GET_FLAG(&g_timx_handle, TIM_FLAG_UPDATE) != RESET)
    {
				move_snake(&snake);  //�ƶ���

        if (check_food_collision(&snake, &food)) {  // ����û�гԵ�ʳ��
            grow_snake(&snake);
            init_food(&food,&snake); 
						game.Score++;
						lcd_show_num(55,187,game.Score,2,16,BLUE);
						uint16_t new_arr = __HAL_TIM_GET_AUTORELOAD(&g_timx_handle) * 95 / 100; // �� ARR��װ��ֵ����5%������5%���н��ٶ�
            if (new_arr < ARR_MIN) {
                new_arr = ARR_MIN;  //�趨���ֵ����ֹ�ٶȹ��졣
            }
						game.V=(new_arr+1)/10;
						lcd_show_num(110,167,game.V,3,16,BLUE);
					  draw_food(&food);  // ����ʳ��
            __HAL_TIM_SET_AUTORELOAD(&g_timx_handle, new_arr); // ???? ARR ?
        }

        if (check_collision(&snake)) { 
            game_over(&snake); 
        }

        __HAL_TIM_CLEAR_IT(&g_timx_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
    }
}
