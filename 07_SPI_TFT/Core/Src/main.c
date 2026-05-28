/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"
#include "adc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include <hagl_hal.h>
#include "hagl.h"
#include "font6x9.h"
#include "rgb565.h"
#include "metaballs.h"
#include "rotozoom.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t adc_data[3] = {0};

hagl_backend_t *display = NULL;

uint16_t adc_x, adc_y, light_raw;
//extern const uint16_t photo[];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void draw_bus_homework(void);
void draw_light_progress_bar(uint16_t adc_value);
void draw_ball(uint16_t adc_x, uint16_t adc_y);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi == &hspi2)
	{
		lcd_transfer_done();
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

  lcd_init();
  display = hagl_init();

  if (display == NULL) {
     Error_Handler();
  }
//  hagl_clear(display);
//
//  for(int i = 0; i < 8; i++){
//	  hagl_draw_rounded_rectangle(display, 2+i, 2+i, 158-i, 126-i, 8-i, rgb565(255 - (i*20), 0, i*30));
//  }
//
//  hagl_put_text(display, L"Hello World!", 40, 55, YELLOW, font6x9);
//
//  lcd_copy();
  //lcd_fill_box(0, 0, 160, 128, BLACK);

  //rotozoom_init(display);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
//	  lcd_fill_box(0, 0, 160, 16, RED);
//	  lcd_fill_box(0, 16, 160, 16, GREEN);
//	  lcd_fill_box(0, 32, 160, 16, BLUE);
//	  lcd_fill_box(0, 48, 160, 16, YELLOW);
//	  lcd_fill_box(0, 64, 160, 16, MAGENTA);
//	  lcd_fill_box(0, 80, 160, 16, CYAN);
//	  lcd_fill_box(0, 96, 160, 16, WHITE);
	  //lcd_draw_rgba_image(14, 30, 100, 100, photo);
	  //rotozoom_animate(display);

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  adc_x = HAL_ADC_GetValue(&hadc1);

	  HAL_ADC_PollForConversion(&hadc1, 10);
	  adc_y = HAL_ADC_GetValue(&hadc1);


	  HAL_ADC_PollForConversion(&hadc1, 10);
	  light_raw = HAL_ADC_GetValue(&hadc1);
	  while(lcd_is_busy()) {}

	  //hagl_clear(display);

	  hagl_fill_rectangle(display, 0, 0, 159, 127, rgb565(40, 40, 40));

	  draw_bus_homework();

	  draw_ball(adc_x, adc_y);

	  draw_light_progress_bar(light_raw);

	  //rotozoom_render(display);
	  lcd_copy();

	  HAL_Delay(16);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void draw_bus_homework(void){
	uint16_t yellow = rgb565(255, 255, 0);
	uint16_t blue = rgb565(0, 0, 255);
	uint16_t brown = rgb565(139, 69, 19);
	uint16_t black = rgb565(0, 0, 0);
	uint16_t white = rgb565(255, 255, 255);

	// Main Bus Body
	hagl_fill_rectangle(display, 45, 46, 115, 81, yellow);
	hagl_draw_rectangle(display, 45, 46, 115, 81, black);

	// Windows
	hagl_fill_rectangle(display, 50, 51, 68, 64, blue);
	hagl_fill_rectangle(display, 70, 51, 88, 64, blue);
	hagl_fill_rectangle(display, 90, 51, 108, 64, blue);

	hagl_draw_rectangle(display, 50, 51, 68, 64, black);
	hagl_draw_rectangle(display, 70, 51, 88, 64, black);
	hagl_draw_rectangle(display, 90, 51, 108, 64, black);

	//Left Wheel
	hagl_fill_circle(display, 63, 81, 7, brown);
	hagl_draw_circle(display, 63, 81, 7, black);

	// Right wheel
	hagl_fill_circle(display, 97, 81, 7, brown);
	hagl_draw_circle(display, 97, 81, 7, black);

	// Front Headlight
	hagl_fill_rectangle(display, 112, 71, 115, 75, white);
	hagl_draw_rectangle(display, 112, 71, 115, 75, black);
}

void draw_light_progress_bar(uint16_t adc_value){
	uint16_t border_color = rgb565(255, 255, 255);
	uint16_t bar_color = rgb565(0, 255, 0);
	uint16_t text_color = rgb565(255, 255, 255);
	uint16_t bg_color = rgb565(40, 40, 40);

	hagl_put_text(display, L"LIGHT:", 10, 5, text_color,  font6x9);

	int16_t x = 10;
	int16_t y = 18;
	int16_t max_width = 140;
	int16_t height = 12;

	hagl_draw_rectangle(display, x, y, x + max_width, y + height, border_color);
	hagl_fill_rectangle(display, x + 1, y + 1, x + max_width - 1, y + height - 1, bg_color);
	int16_t fill_width = (adc_value * (max_width - 2)) / 4095;

	if(fill_width > max_width - 2) {
		fill_width = max_width - 2;
	}

	if(fill_width > 0){
		hagl_fill_rectangle(display, x + 1, y + 1, x + 1 + fill_width, y + height - 1, bar_color);
	}
}

void draw_ball(uint16_t adc_x, uint16_t adc_y)
{
    static float smooth_x = 80;
    static float smooth_y = 64;

    uint16_t target_x = (adc_x * 159) / 4095;
    uint16_t target_y = (adc_y * 127) / 4095;

    smooth_x = smooth_x * 0.9f + target_x * 0.1f;
    smooth_y = smooth_y * 0.9f + target_y * 0.1f;

    uint16_t ball_x = (uint16_t)smooth_x;
    uint16_t ball_y = (uint16_t)smooth_y;

    if(ball_x < 8) ball_x = 8;
    if(ball_x > 151) ball_x = 151;

    if(ball_y < 8) ball_y = 8;
    if(ball_y > 119) ball_y = 119;

    hagl_fill_circle(display, ball_x, ball_y, 8, rgb565(255, 0, 0));
    hagl_draw_circle(display, ball_x, ball_y, 8, rgb565(255, 255, 255));
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
