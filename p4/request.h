#ifndef __REQUEST_H__
#include <pthread.h>
void requestHandle(int fd);
// threads_nums the number of worker thread
// buffers_nums the number of request connections that can be accepted at one time.
int threads_nums, buffers_nums;
int listenfd, connfd, port, clientlen;
int getconnfd;
pthread_mutex_t mutex;
pthread_cond_t fill;
pthread_cond_t empty;
struct sockaddr_in clientaddr;
//int empty;
//int fill;

typedef struct _buffer_
{
	int buffer_size;           // size of the buffer
	int head;                  // head index 
	int count;                // length of the queue
	int* connfd;              // request queue
} __buffer_;

__buffer_ sharedBuffer;

void *producer();
void *consumer();
void produce(int);
int consume();




#endif
