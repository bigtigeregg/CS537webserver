#include "cs537.h"
#include "request.h"
#include <pthread.h>
// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// CS537: Parse the new arguments too
void getargs(int *port,int *threadnums, int *buffernums, int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <port> <threadnums> <buffernums> \n", argv[0]);

	exit(1);
    }
    *threadnums = atoi(argv[1]);
	*buffernums = atoi(argv[2]);
    *port = atoi(argv[1]);
}


void *producer(void *arg){
	
	listenfd = Open_listenfd(port);
	while(1){
		clientlen = sizeof(clientaddr);
		pthread_mutex_lock(&mutex);
		connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
		while(sharedBuffer.count == buffers_nums)
			pthread_cond_wait(&empty, &mutex);
		produce(connfd);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
	}
}

void produce(int connfd){
	
	sharedBuffer.connfd[sharedBuffer.head] = connfd;
	sharedBuffer.count = sharedBuffer.count + 1;
}


void *consumer(void *arg){
	while(1){
	//	printf("consumer\n");
		pthread_mutex_lock(&mutex);
		while(sharedBuffer.count == 0)
			pthread_cond_wait(&fill ,&mutex);
		getconnfd = consume();
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
		requestHandle(getconnfd);
		Close(getconnfd);
	}
}

int consume(){
	printf("consume\n");
	int tmp = sharedBuffer.connfd[sharedBuffer.head];
	sharedBuffer.head = (sharedBuffer.head + 1) % sharedBuffer.buffer_size;
	sharedBuffer.count = sharedBuffer.count - 1;
	return tmp;
}



int main(int argc, char *argv[])
{
    
   // struct sockaddr_in clientaddr;
    
    
	// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	// pthread_cond_t fill = PTHREAD_COND_INITIALIZER;
	// pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

    pthread_mutex_init(&mutex, NULL);
    getargs(&port,&threads_nums, &buffers_nums, argc, argv);
    
    sharedBuffer.buffer_size = buffers_nums;
    sharedBuffer.count = 0;
    sharedBuffer.head = 0;
    sharedBuffer.connfd = NULL;
    // 
    // CS537: Create some threads...
    //
    pthread_t *threads;
    pthread_t pid;
    pthread_create(&pid, NULL, producer, NULL);

    // tinfo = calloc(num_threads, sizeof(struct thread_info));
    // 	if (tinfo == NULL)
    // 		 handle_error("calloc");

    threads = (pthread_t *)malloc(sizeof(pthread_t)*threads_nums);
    for(int i = 0; i < threads_nums;i++){
    	pthread_create(&threads[i],NULL,consumer,NULL);
    }




    //listenfd = Open_listenfd(port);
 	// while (1) {
	// clientlen = sizeof(clientaddr);
	// connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	// // 
	// // CS537: In general, don't handle the request in the main thread.
	// // Save the relevant info in a buffer and have one of the worker threads 
	// // do the work. However, for SFF, you may have to do a little work
	// // here (e.g., a stat() on the filename) ...
	// // 
	// requestHandle(connfd);
	// Close(connfd);
 //    }
}





