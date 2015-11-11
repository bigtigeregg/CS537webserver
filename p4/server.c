#include "cs537.h"
#include "request.h"
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
int *bufferconnd;
void getargs(int *port,int *threadnums, int *buffernums, int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <port> <threadnums> <buffernums> \n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *threadnums = atoi(argv[2]);
    *buffernums = atoi(argv[3]);
}


void *producer(void *arg){
	int connfd;
	while(1){
		clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
		pthread_mutex_lock(&mutex);
		while(sharedBuffer.count == buffers_nums)
			pthread_cond_wait(&empty, &mutex);
		produce(connfd);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&mutex);
	}
}

void produce(int connfd){
    bufferconnd[sharedBuffer.tail] = connfd;
    sharedBuffer.tail = (sharedBuffer.tail + 1) % sharedBuffer.buffer_size;
    sharedBuffer.count = sharedBuffer.count + 1;
}


void *consumer(void *arg){
	int getconnfd;
	while(1){
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
	//int tmp = sharedBuffer.connfd[sharedBuffer.head];
    int tmp = bufferconnd[sharedBuffer.head];
	sharedBuffer.head = (sharedBuffer.head + 1) % sharedBuffer.buffer_size;
	sharedBuffer.count = sharedBuffer.count - 1;
	return tmp;
}


int main(int argc, char *argv[])
{
    int i;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&fill, NULL);
    pthread_cond_init(&empty, NULL);
    getargs(&port,&threads_nums, &buffers_nums, argc, argv);
    bufferconnd = (int *)malloc(sizeof(int)*buffers_nums);

    sharedBuffer.buffer_size = buffers_nums;
    sharedBuffer.count = 0;
    sharedBuffer.head = 0;
    sharedBuffer.tail = 0;
    // 
    // CS537: Create some threads...
    //
    pthread_t *threads;
    pthread_t pid;
    listenfd = Open_listenfd(port);
    pthread_create(&pid, NULL, producer, NULL);
    threads = (pthread_t *)malloc(sizeof(pthread_t)*threads_nums);
    for(i = 0; i < threads_nums;i++){
    	pthread_create(&threads[i],NULL,consumer,NULL);
    }
	pthread_join(&pid,NULL);
}





