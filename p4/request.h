#ifndef __REQUEST_H__
void requestHandle(int fd);
int threads_nums, buffers_nums;
int listenfd, port, clientlen;
pthread_mutex_t mutex;
pthread_cond_t fill;
pthread_cond_t empty;
struct sockaddr_in clientaddr;

typedef struct _buffer_
{
	int buffer_size;           // size of the buffer
	int head;                  // head index
	int tail;
	int count;                // length of the queue
} __buffer_;

__buffer_ sharedBuffer;

void *producer();
void *consumer();
void produce(int);
int consume();


#endif
