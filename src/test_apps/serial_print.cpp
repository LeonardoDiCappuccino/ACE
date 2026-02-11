#include "stm32f0xx_hal.h"

/**
 * Blinky program for STM32F042x6
 * Toggles an LED on a GPIO pin
 * Configure LED_PIN below to match your hardware
 */

/* User Configuration */
#define LED_PIN                GPIO_PIN_0
#define LED_PORT               GPIOA
#define LED_PORT_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

/* Function prototypes */
static void SystemClock_Config(void);
static void GPIO_Init(void);
void Error_Handler(void);

int main(void)
{
    HAL_Init();
    
    SystemClock_Config();
    
    GPIO_Init();
    
    while (1)
    {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        HAL_Delay(500);
    }
    
    return 0;
}

/**
 * @brief Configure the system clock
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    
    /* Initializes the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        /* Initialization error */
        Error_Handler();
    }
    
    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        /* Initialization error */
        Error_Handler();
    }
}

/**
 * @brief Initialize GPIO for LED output
 */
static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* Enable GPIO port clock */
    LED_PORT_CLK_ENABLE();
    
    /* Configure GPIO pin */
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
    
    /* Initialize LED as OFF */
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Error handler - called on HAL errors
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
        /* Infinite loop */
    }
}

/**
 * @brief System tick hook - called by HAL_Init()
 */
extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

/**
 * @brief Hard Fault handler
 */
extern "C" void HardFault_Handler(void)
{
    while (1)
    {
        /* Infinite loop */
    }
}
