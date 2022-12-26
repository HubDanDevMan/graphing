#include <SDL2/SDL.h>
#include <stdlib.h>
#include <complex.h>
#include <signal.h>
#include <pthread.h>
#include "view.h"
#include "debug.h"
#include "graphingMain.h"
#include "parallelism.h"

// set of signals used for inter-thread communication
#define WAKE_SIGNAL SIGUSR1
#define DONE_SIGNAL SIGUSR2


typedef struct {
	int threadId;
	int startLine;
	int endLine;
} task_t;

enum GraphingState {
	GRPH_RUN,
	GRPH_EXIT
};

enum GraphingState graphingState = GRPH_RUN;


// Concurrency variables
pthread_t mainThread;		// master thread
pthread_t * threads;		// worker threads
int threadCount;		// count of worker threads
task_t *args;			// args for worker threads

pthread_mutex_t graphingMutex;
sigset_t sigSetWorker;		// for worker threads
sigset_t sigSetMain;		// for master thread


/* Will be incremented until the last thread sends the signal, must be locked */
int threadDoneCtr;

void executeConcurrentGraphing(void)
{
	int sigNumber;
	unsigned int ticks = SDL_GetTicks();
	// wakes up all the sleeping threads
	for (int i = 0; i < threadCount; i++) {
		pthread_kill(threads[i], WAKE_SIGNAL);
	}
	if (graphingState != GRPH_EXIT) {
		sigwait(&sigSetMain, &sigNumber);
	}
	DEBUG("Total render time: %d ms", SDL_GetTicks() - ticks);
}


void *drawFunctionSlice(void *arg)
{
	complex double point;
	int sigNumber;
	task_t *task = (task_t *) arg;
	unsigned int ticks;

	DEBUG("[Thread%d]: Waiting for command...", task->threadId);
	// wait upon creation
	sigwait(&sigSetWorker, &sigNumber);
	do {
		DEBUG("[Thread%d]: Woken up from sleep, rendering function slice...", task->threadId);
		ticks = SDL_GetTicks();
		for (int i = task->startLine; i < task->endLine; i++){ // row
			for (int j = 0; j < WIN_WIDTH; j++){ // column
				point = cfunc(CMPLX(view.scaling*(j+view.centerx-(WIN_WIDTH/2.0L)),
					 view.scaling*(-i+view.centery+(WIN_HEIGHT/2.0))));
				numbers[i*WIN_WIDTH+j] = point;	// number buffer for faster recoloring 
				pixels[i*WIN_WIDTH+j] = colorfunc(point);
			}
		}
		DEBUG("[Thread%d]: Done calculating slice [%d...%d], took %d ms",
				task->threadId, task->startLine, task->endLine, SDL_GetTicks() - ticks); 
		// Critical section, acquire lock and set to done
		pthread_mutex_lock(&graphingMutex);
		threadDoneCtr++;
		if (threadDoneCtr == threadCount) {
			threadDoneCtr = 0;
			// wake up main thread again
			pthread_kill(mainThread, DONE_SIGNAL);
		}
		pthread_mutex_unlock(&graphingMutex);
		sigwait(&sigSetWorker, &sigNumber);
	} while (graphingState != GRPH_EXIT);
	return NULL;
}

void setupConcurrentGraphing(int numThreads)
{
	if (numThreads < 1) {
		// use at least one thread
		numThreads = 1;
	}
	threadCount = numThreads;
	mainThread = pthread_self();
	/* Initialize synchronisation mechanisms */
	sigemptyset(&sigSetWorker);
	sigemptyset(&sigSetMain);
	sigaddset(&sigSetWorker, WAKE_SIGNAL);
	sigaddset(&sigSetMain, DONE_SIGNAL);
	pthread_sigmask(SIG_BLOCK, &sigSetWorker, NULL);
	pthread_sigmask(SIG_BLOCK, &sigSetMain, NULL);
	pthread_mutex_init(&graphingMutex, NULL);

	/* create thread data structure */
	threads = malloc(sizeof(pthread_t) * numThreads);

	/* create args data structure and initialize it */
	args = malloc(sizeof(task_t) * numThreads);
	int startLine = 0;
	int endLine = WIN_HEIGHT / numThreads + WIN_HEIGHT % numThreads;

	for (int i = 0; i < numThreads; i++) {
		args[i].startLine = startLine;
		args[i].endLine	= endLine;
		args[i].threadId = i;
		if (pthread_create(&threads[i], NULL, &drawFunctionSlice, &args[i]) != 0 ) {
			perror("Unable to create thread.\n");
		}
		DEBUG("Thread %d created", i);
		startLine = endLine;
		endLine += WIN_HEIGHT / numThreads;
	}
	DEBUG("Created all threads");
}

void cleanConcurrentGraphing()
{
	DEBUG("Preparing to exit...");
	// make threads exit
	graphingState = GRPH_EXIT;
	executeConcurrentGraphing();
	
	// join all threads
	for (int i = 0; i < threadCount; i++) {
		pthread_join(threads[i], NULL);
	}
	free(threads);
	free(args);
	pthread_mutex_destroy(&graphingMutex);
	DEBUG("Joined all threads and cleaned up");
}
