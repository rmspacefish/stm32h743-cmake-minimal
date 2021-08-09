#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>


#if defined STM32H7
#include <stm32h7xx_hal.h>
// STM32H743ZI green led - PD12
#define LED_PORT                GPIOB
#define LED_PIN                 GPIO_PIN_7
#define LED_PORT_CLK_ENABLE     __HAL_RCC_GPIOB_CLK_ENABLE

#elif defined STM32F3
#include <stm32f3xx_hal.h>
// STM32F3 Discovery board - Red LED
#define LED_PIN                GPIO_PIN_9
#define LED_PORT          GPIOE
#define LED_PORT_CLK_ENABLE    __HAL_RCC_GPIOE_CLK_ENABLE
#endif

static void blinky(void *arg)
{
    for(;;)
    {
        vTaskDelay(500);
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    }
}

void initGPIO()
{
    GPIO_InitTypeDef GPIO_Config;

    GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Config.Pull = GPIO_NOPULL;
    GPIO_Config.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_Config.Pin = LED_PIN;

    LED_PORT_CLK_ENABLE();
    HAL_GPIO_Init(LED_PORT, &GPIO_Config);
}

int main(void)
{
    SystemInit();
    initGPIO();
    
    xTaskCreate(blinky, "blinky", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    vTaskStartScheduler();
    for (;;);
    
    return 0;
}

extern "C" void vApplicationTickHook(void)
{
}

extern "C" void vApplicationIdleHook(void)
{
}

extern "C" void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for(;;);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    taskDISABLE_INTERRUPTS();
    for(;;);
}
