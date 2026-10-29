#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#define SET_BIT(REG,BIT)	  ((REG) |= (1U << (BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1U << (BIT)))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1U << BIT))
#define READ_BIT(REG, BIT)    (((REG) >> (BIT)) & 1U)

#define BUFFER_SIZE 8

volatile uint8_t *ddr = (volatile uint8_t *) 0x24;
volatile uint8_t *port = (volatile uint8_t *) 0x25;


typedef struct 
{
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t count; 
}RingBuffer;

void rb_init(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}


RingBuffer tail;


bool push(RingBuffer *rb, uint8_t data){
	if(rb->count == BUFFER_SIZE){
		return false;
	}

	rb->buffer[rb->head] = data;
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





void applycommand(char command){


	static int commandsapplied = 0;
	SET_BIT(*ddr,5);
	switch(command) {
        case 'A':
            SET_BIT(*port, 5);
            break; 
        case 'K':
            CLEAR_BIT(*port, 5);
            break;
        case 'T':
            TOGGLE_BIT(*port, 5);
            break;
        default:
            return; 
    }
	
    commandsapplied++;
}



void setup(){


	Serial.begin(9600);
	rb_init(&tail);
}


void loop(){


	if(Serial.available() > 0){
		char read = Serial.read();
		if(read != '\n' && read != '\t'){
			push(tail,read);
		}
	}

	static uint32_t time = 0;
	if(millis() - time >= 500){
		time = millis();
		uint8_t nextcommand;
		if(pop(&tail,&nextcommand) == true){
			applycommand(nextcommand);
		}
	}


}