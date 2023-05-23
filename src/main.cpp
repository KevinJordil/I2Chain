#include "mbed.h"

#if !DEVICE_I2CSLAVE
#error[NOT_SUPPORTED] I2C Slave is not supported
#endif

#define SLAVE_ADDR (0x60)

DigitalOut led1(LED1);
I2CSlave slave(PB_9, PB_6);
UnbufferedSerial pc(USBTX, USBRX, 9600);

void enable_i2c_interrupt(); // this function use NVIC to enable interrupt on the I2C
void i2c_irq_handler(); // this function is called when an interrupt is triggered

char buf[10];
char msg[] = "1";

int main()
{
    printf("Initialize...\n");
    slave.frequency(400000);
    slave.address(SLAVE_ADDR << 1);

    enable_i2c_interrupt();

    led1 = 0;
    printf("Ready !\n");

    printf("Deep sleep allowed: %i\r\n", sleep_manager_can_deep_sleep());

    while (true)
    {
        led1 = !led1;
        ThisThread::sleep_for(500ms);
    }
}

void i2c_irq_handler()
{
    int i = slave.receive();
    switch (i)
    {
    case I2CSlave::ReadAddressed:
        slave.write(msg, 1);
        break;
    case I2CSlave::WriteGeneral:
        slave.read(buf, 10);
        break;
    case I2CSlave::WriteAddressed:
        slave.read(buf, 10);
        break;
    }
    for (int i = 0; i < 10; i++)
    {
        buf[i] = 0;
    }

    // Clear interrupt address matched flag
    I2C1->ICR |= I2C_ICR_ADDRCF;
    // Clear pending IRQ
    NVIC_ClearPendingIRQ(I2C1_IRQn);
}

/*
    This function as to be change if you are not on a cortex M4
    because the I2C_EV_IRQn depend of your vector table
 */
void enable_i2c_interrupt()
{
    NVIC_SetVector(I2C1_IRQn, (uint32_t)(&i2c_irq_handler));

    I2C1->CR1 = 0;
    I2C1->CR1 |= I2C_CR1_PE;
    I2C1->CR1 |= I2C_CR1_ADDRIE;

    // Configure I2C_OAR1
    I2C1->OAR1 = 0;
    I2C1->OAR1 |= I2C_OAR1_OA1EN | (SLAVE_ADDR << 1);

    NVIC_SetPriority(I2C1_IRQn, 0);

    NVIC_EnableIRQ(I2C1_IRQn);   
}
