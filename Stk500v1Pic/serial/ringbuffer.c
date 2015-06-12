#include "ringbuffer.h"
#include <util/atomic.h>

void ring_buffer_reset(ringbuffer* buffer) {
	buffer->head = 0;
	buffer->tail = 0;
	buffer->size = 0;
}

void ring_buffer_queue(ringbuffer* buffer, unsigned char val) {
	while (1) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			if (buffer->size < RING_BUFFER_SIZE) {
				//wait until we have space for data
				buffer->data[buffer->tail] = val;
				buffer->tail++;
				if (buffer->tail == RING_BUFFER_SIZE) {
					buffer->tail = 0;
				}
				buffer->size++;
				return;
			}
		}
	}
}

unsigned char ring_buffer_dequeue(ringbuffer* buffer) {
	while (1) {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			while (buffer->size > 0) {
				//wait until we have data
				unsigned char res = buffer->data[buffer->head];
				buffer->head++;
				if (buffer->head == RING_BUFFER_SIZE) {
					buffer->head = 0;
				}
				buffer->size--;
				return res;
			}
		}
	}
}