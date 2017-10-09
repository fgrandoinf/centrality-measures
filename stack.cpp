#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void StackInit(stack *stackP, int maxSize) {
	stackElement *newContents;

	/* Allocate a new array to hold the contents. */

	newContents = (stackElement *) malloc(sizeof(stackElement) * maxSize);

	if (newContents == NULL) {
		fprintf(stderr, "Insufficient memory to initialize stack.\n");
		exit(1); /* Exit, returning error code. */
	}

	stackP->contents = newContents;
	stackP->maxSize = maxSize;
	stackP->top = -1; /* I.e., empty */
}

void StackDestroy(stack *stackP) {
	free(stackP->contents);
}

void StackPush(stack *stackP, stackElement element) {
	if (StackIsFull(stackP)) {
		fprintf(stderr, "Can't push element on stack: stack is full.\n");
		exit(1); /* Exit, returning error code. */
	}

	/* Put information in array; update top. */

	stackP->contents[++stackP->top] = element;
}

stackElement StackPop(stack *stackP) {
	if (StackIsEmpty(stackP)) {
		fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
		exit(1); /* Exit, returning error code. */
	}

	return stackP->contents[stackP->top--];
}

int StackIsEmpty(stack *stackP) {
	return stackP->top < 0;
}

int StackIsFull(stack *stackP) {
	return stackP->top >= stackP->maxSize - 1;
}
