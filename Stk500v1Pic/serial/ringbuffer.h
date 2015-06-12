#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#define RING_BUFFER_SIZE 256

typedef struct {
	unsigned char data[RING_BUFFER_SIZE];
	unsigned int tail;
	unsigned int head;
	unsigned int size;
} ringbuffer;


void ring_buffer_reset(ringbuffer* buffer);

void ring_buffer_queue(ringbuffer* buffer, unsigned char val);

unsigned char ring_buffer_dequeue(ringbuffer* buffer);

#endif