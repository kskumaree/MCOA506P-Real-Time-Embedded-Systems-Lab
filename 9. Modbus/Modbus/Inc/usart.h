

#ifndef USART_H_
#define USART_H_

void USART2_init(void);
void USART3_init(void);
void GPIO_init(void);
void USART_write(volatile uint8_t *buffer);
void USART_read (volatile uint8_t *buffer);
void USART_write_array(volatile uint8_t buffer[],uint32_t nBytes);
void USART_read_array(volatile uint8_t buffer[],uint32_t nBytes);
void msDelay(int msTime);
void configurePin(void);
#endif /* USART_H_ */
