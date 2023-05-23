#include "i2chain.h"

I2Chain::I2Chain(PinName sda, PinName scl, int address) : slave(sda, scl){
    slave.address(address << 1);
    slave.frequency(400000);
    msg[0] = 0x42;
    msg[1] = 0;
}

void I2Chain::enable_i2c_interrupt(void (*i2c_irq_handler)()){
    NVIC_SetVector(I2C1_IRQn, (uint32_t)(i2c_irq_handler));

    I2C1->CR1 |= I2C_CR1_ADDRIE;
    NVIC_SetPriority(I2C1_IRQn, 0);

    NVIC_EnableIRQ(I2C1_IRQn);
}

void I2Chain::clear_i2c_interrupt(void){
    I2C1->ICR |= I2C_ICR_ADDRCF;
    NVIC_ClearPendingIRQ(I2C1_IRQn);
}

int I2Chain::receive(void){
    return slave.receive();
}

void I2Chain::write(const char *data, int length){
    slave.write(data, length);
}

void I2Chain::read(char *data, int length){
    slave.read(data, length);
}