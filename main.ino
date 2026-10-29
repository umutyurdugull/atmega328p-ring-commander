#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define SET_BIT(REG,BIT)	  ((REG) |= (1U << (BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1U << (BIT)))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1U << BIT))
#define READ_BIT(REG, BIT)    (((REG) >> (BIT)) & 1U)



#define BUFFER_SIZE 8

typedef struct 
{
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t count; 
}RingBuffer;




bool push(RingBuffer *rb, uint8_t data){
	if(rb->count == BUFFER_SIZE){
		return false;
	}

	rb->head = (rb->head + 1) & (BUFFER_SIZE - 1);

	rb->count++;
	return true;
}

bool pop(RingBuffer *rb, uint8_t *read){
	if(rb->count == 0){
		return false;
	}

	*read = rb->buffer[rb->tail];
	rb->tail = (rb->tail + 1) & (BUFFER_SIZE - 1);
	rb->count --;
	return true;
}


volatile uint8_t *ddr = (volatile uint8_t *) 0x24;
volatile uint8_t *port = (volatile uint8_t *) 0x25;




void setup(){

}


void loop(){
	
}