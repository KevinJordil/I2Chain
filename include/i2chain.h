#ifndef I2CHAIN_H
#define I2CHAIN_H

#include "mbed.h"

#if !DEVICE_I2CSLAVE
#error[NOT_SUPPORTED] I2C Slave is not supported
#endif

class I2Chain{

public:
    I2Chain(PinName sda, PinName scl, int address);

    /**
     * @brief Enable I2C interrupt
     * @param function to be called when interrupt is triggered
     *     
    */
    void enable_i2c_interrupt(void (*i2c_irq_handler)());

    /**
     * @brief Acknowledge interrupt and clear ISR
     * 
     */
    void clear_i2c_interrupt(void);

    /**
     * @brief I2C slave receive function
     * 
     */
    int receive(void);

    /**
     * @brief I2C slave write function
     * 
     */
    void write(const char *data, int length);

    /**
     * @brief I2C slave read function
     * 
     */
    void read(char *data, int length);
    
private:

    I2CSlave slave;

    char buf[10];
    char msg[2];

};

#endif //I2CHAIN_H