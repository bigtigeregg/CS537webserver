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
void getargs(int *port,int *threadnums, int *buffernums, int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s <port> <threadnums> <buffernums> \n", argv[0]);
	*threadnums = argv[1];
	*buffernums = argv[2];
	exit(1);
    }
    *port = atoi(argv[1]);
}


void *producer(void *arg){
	int i;
	for(i = 0; i < loops; i++){
		Pthread_mutex_lock(&mutex);
		while(count == MAX)
			Pthread_cond_wait(&empty, &mutex);
		put(i);
		Pthread_cond_signal(&fill);
		Pthread_mutex_unlock(&mutex);
	}
}

void produce(){

}


void *consumer(void *arg){
	int i;
	for(i = 0; i < loops; i++){
		pthread_mutex_lock(&mutex);
		while(count == 0)
			Pthread_cond_wait(&fill ,&mutex);
		int tmp = get();
		Pthread_cond_signal(&empty);
		Pthread_mutex_unlock(&mutex);
	}
}

int consume(){
	int tmp = buffer[use];
	use = (use + 1) % buffers_nums;
	count--;
	return tmp;
}



int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    struct thread_info *tinfo;

    getargs(&port,&threadnums, &buffers_nums, argc, argv);



    // 
    // CS537: Create some threads...
    //
    pthread_t *threads;

    // tinfo = calloc(num_threads, sizeof(struct thread_info));
    // 	if (tinfo == NULL)
    // 		 handle_error("calloc");

    threads = (pthread_t *)malloc(sizeof(pthread_t)*threads_nums);
    for(int i = 0; i < threadnums;i++){
    	
    	pthread_create(&threads[i],NULL,consumer,NULL);
    }

    listenfd = Open_listenfd(port);

    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
	// 
	// CS537: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. However, for SFF, you may have to do a little work
	// here (e.g., a stat() on the filename) ...
	// 
	requestHandle(connfd);
	Close(connfd);
    }
}


    


 
