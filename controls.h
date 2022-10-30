#ifndef _CONTROLS_H
#define _CONTROLS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "view.h"


#define STK_SIZE 26
#define HISTORY_MAX 32

/* low memory footprint key representation */
typedef struct {
	unsigned char sym; // SDL_Keycode but only single byte
	unsigned char mod; // SDL_keymod but only single byte
} cmdKey_t;

typedef struct {
	cmdKey_t keystack[STK_SIZE];
	bool isChanged;
	int top;	// also shows how many items are on the stack
} cmdstack_t;

// TODO: Reduce ram usage of history
typedef struct {
	struct viewstate snapshot[HISTORY_MAX];
	int index;	// -1 means no history items
} viewhistory_t;

extern cmdstack_t stack;
extern viewhistory_t history;

int push(cmdKey_t item);
cmdKey_t pop(void);
void handleKeyEvent(SDL_KeyboardEvent kbde);
void undoCommand(void);
void enqueueCurrent(void);
void executeStack(void);

#endif /* _CONTROLS_H */
