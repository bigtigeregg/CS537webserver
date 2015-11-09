#ifndef __REQUEST_H__

void requestHandle(int fd);
// threads_nums the number of worker thread
// buffers_nums the number of request connections that can be accepted at one time.
int threads_nums, buffers_nums;




string buffer[buffers_nums];
int count;
int use;
int fill;


void *producer();
void *consumer();
void produce(int);
int consume(char *);

pthread_mutex_t Lock;
pthread_cond_t	Full, Empty;


struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    char     *argv_string;      /* From command-line argument */
};




#endif
