#include "stm32f0xx_hal.h"

#include <stdio.h>

#define LED_PIN GPIO_PIN_0
#define LED_PORT GPIOA
#define LED_PORT_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BUTTON_PIN GPIO_PIN_1
#define BUTTON_PORT GPIOA
#define BUTTON_IRQn EXTI0_1_IRQn

UART_HandleTypeDef huart1;

volatile uint32_t button_press_count = 0;

static void SystemClock_Config(void);
static void GPIO_Init(void);
static void Button_Init(void);
static void MX_USART1_UART_Init(void);

void Error_Handler(void);

static int i = 0;

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    GPIO_Init();
    Button_Init();
    MX_USART1_UART_Init();

    while (1)
    {
        // HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        // printf("Loop: %d, Button presses: %lu\n", ++i, button_press_count);
        // HAL_Delay(500);
    }

    return 0;
}

    static bool b = true;
/**
 * @brief GPIO EXTI callback - called when button is pressed
 */
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BUTTON_PIN)
    {
        button_press_count++;

        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

        // printf("\x1b[C");
        // printf("Loop: %d, Button presses: %lu\n", ++i, button_press_count);
        if (b)
            printf("Loop: %d, Button presses: ");
        else
            printf("%lu\n", ++i, button_press_count);

        b = !b;
    }
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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
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
 * @brief Initialize button with interrupt on PA1
 */
static void Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO port clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure button pin with interrupt on falling edge */
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Interrupt on falling edge (button press)
    GPIO_InitStruct.Pull = GPIO_PULLUP;          // Internal pull-up (button connects to GND)
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

    /* Enable EXTI interrupt in NVIC */
    HAL_NVIC_SetPriority(BUTTON_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(BUTTON_IRQn);
}

/**
 * @brief USART1 Initialization Function
 */
static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
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
/**
 * @brief EXTI line 0-1 interrupt handler (for PA1 button)
 */
extern "C" void EXTI0_1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}