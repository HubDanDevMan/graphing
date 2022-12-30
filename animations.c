#include <string.h>
#include <math.h>
#include "view.h"
#include "stdbool.h"
#include "debug.h"
#include "coloring.h"
#include "animations.h"

#define ANM_QUEUE_SIZE 16

static int findQueueEntry(void);
static void clearQueue(void);

animation_t animationQueue[ANM_QUEUE_SIZE];

/**
 * Will set up an animation in the queue
 * Extend with duration option in future
 * steps: number of iterations until animation is complete
 */

bool createFullAnimation(int steps, int durationPerStep, float * endColor, double spreadEndVal, double shiftEndVal)
{
	int index = findQueueEntry();
	if (index < 0) {
		return false;
	}
	animationQueue[index].type = ANM_COLOR | ANM_SPREAD | ANM_SHIFT;
	animationQueue[index].currentStep = 0;
	animationQueue[index].maxStep = steps;
	animationQueue[index].dps = durationPerStep;

	// calculate delta color steps
	for (int j = 0; j < FLUID_COLOR_COUNT; j++) {
		animationQueue[index].deltas[j] = (endColor[j] - fluidColors[j])/steps; 
	}
	// Calculate shift
	animationQueue[index].shiftChangePerStep = (shiftEndVal - view.shift) / (double)steps;
	// Calculate spread
	animationQueue[index].spreadChangePerStep = (spreadEndVal - view.spread) / (double)steps;
	DEBUG("Full animation has been added to queue");
	return true;
}
/**
 * steps = frames per wave cycle
 * oscCount = total wave cycle count
 * amp = signed value of max range, min range
 */
bool createSpreadAnimationOsc(int steps, int durationPerStep, int oscCount, float amp)
{
	int index = findQueueEntry();
	if (index < 0) {
		return false;
	}
	animationQueue[index].type = ANM_SPREAD | ANM_OSC;
	animationQueue[index].currentStep = 0;
	animationQueue[index].maxStep = steps * oscCount;
	animationQueue[index].dps = durationPerStep / steps;
	animationQueue[index].freq = (float) oscCount/(float)(steps * durationPerStep);
	animationQueue[index].amp = exp(amp);	// use exp to smoothly approach values close to 0 and because its unsigned
	// Spread Change per step should be interpreted as the base line of the oscillation, i.e. view's spread value
	animationQueue[index].spreadChangePerStep = view.spread;
	DEBUG("Oscillating spread animation has been added to queue");
	DEBUG("Oscillating spread animation with:\n%d Steps\n%dms total duration\n%fhz freq\n%f amp",steps,durationPerStep*steps,animationQueue[0].freq,amp);
	return true;
}
/**
 * steps = frames per wave cycle
 * durationPerStep = duration per frame
 * oscCount = total wave cycle count
 * amp = signed value of max range, min range
 */
bool createShiftAnimationOsc(int steps, int durationPerStep, int oscCount, float amp)
{
	int index = findQueueEntry();
	if (index < 0) {
		return false;
	}
	animationQueue[index].type = ANM_SHIFT | ANM_OSC;
	animationQueue[index].currentStep = 0;
	animationQueue[index].maxStep = steps * oscCount;
	animationQueue[index].dps = durationPerStep / steps;
	animationQueue[index].freq = (float) oscCount/(float)(steps * durationPerStep);
	animationQueue[index].amp = exp(amp);
	// Shift Change per step should be interpreted as the base line of the oscillation, i.e. view's shift value
	animationQueue[index].shiftChangePerStep = view.shift;
	DEBUG("Oscillating shift animation has been added to queue");
	DEBUG("Oscillating shift animation with:\n%d Steps\n%dms total duration\n%fhz freq\n%f amp",steps,durationPerStep*steps,animationQueue[0].freq,amp);
	return true;
}


bool createColorAnimation(int steps, int durationPerStep, float * endColor)
{
	int index = findQueueEntry();
	if (index < 0) {
		return false;
	}
	animationQueue[index].type = ANM_COLOR;
	animationQueue[index].currentStep = 0;
	animationQueue[index].maxStep = steps;
	animationQueue[index].dps = durationPerStep;
	// get current colors
	for (int j = 0; j < FLUID_COLOR_COUNT; j++) {
		// calculate delta color steps
		animationQueue[index].deltas[j] = (endColor[j] - fluidColors[j])/steps; 
	}
	DEBUG("Color animation has been added to queue");
	return true;
}

bool createShiftAnimation(int steps, int durationPerStep, float shiftEndVal)
{
	int index = findQueueEntry();
	if (index < 0) {
		return false;
	}
	animationQueue[index].type = ANM_SHIFT;
	animationQueue[index].currentStep = 0;
	animationQueue[index].maxStep = steps;
	animationQueue[index].dps = durationPerStep;
	// get current colors
	animationQueue[index].shiftChangePerStep = (shiftEndVal - view.shift) / (double)steps;
	DEBUG("Shift animation has been added to queue");
	return true;
}

bool createSpreadAnimation(int steps, int durationPerStep, float spreadEndVal)
{
	int index = findQueueEntry();
	if (index < 0) {
		return false;
	}
	animationQueue[index].type = ANM_SPREAD;
	animationQueue[index].currentStep = 0;
	animationQueue[index].maxStep = steps;
	animationQueue[index].dps = durationPerStep;
	// get current colors
	animationQueue[index].spreadChangePerStep = (spreadEndVal - view.spread) / (double)steps;
	DEBUG("Spread animation has been added to queue");
	return true;
}

/**
 * Writes the changes to the view
 * returns the appropriate amount of time to sleep in ms
 */
int playAnimation()
{
	if (animationQueue[0].type == ANM_NONE) {
		// No animation to be played
		return 0;
	}
	if (animationQueue[0].type & ANM_COLOR) {
		DEBUG("Playing Color animation");
		for (int i = 0; i < FLUID_COLOR_COUNT; i++) {
			fluidColors[i] += animationQueue[0].deltas[i];
		}
	}
	if (animationQueue[0].type & ANM_SHIFT) {
		if (animationQueue[0].type & ANM_OSC) {
			view.shift = animationQueue[0].shiftChangePerStep
				+ sin(animationQueue[0].currentStep + 2.0L
				* M_PI / animationQueue[0].freq) * animationQueue[0].amp;
			DEBUG("Osc-Shift: shift=%lf", view.shift);
		} else {
			view.shift += animationQueue[0].shiftChangePerStep;
			DEBUG("Playing Shift animation");
		}
	}
	if (animationQueue[0].type & ANM_SPREAD) {
		if (animationQueue[0].type & ANM_OSC) {
			view.spread = animationQueue[0].spreadChangePerStep
				+ sin(animationQueue[0].currentStep + 2.0L
				* M_PI / animationQueue[0].freq) * animationQueue[0].amp;
			DEBUG("Osc-Spread: spread=%lf", view.spread);
		} else {
			view.spread += animationQueue[0].spreadChangePerStep;
			DEBUG("Playing Spread animation");
		}
	}
	redoFlag = REDRAW;
	animationQueue[0].currentStep++;
	// Clear up animation queue if done
	if (animationQueue[0].currentStep == animationQueue[0].maxStep) {
		animationQueue[0].type = ANM_DONE;
		DEBUG("Done playing animation");
		clearQueue();
		return 0;
	}
	return animationQueue[0].dps;

}

/**
 * Finds a free spot in the animation queue
 */
static int findQueueEntry(void)
{
	int i;
	for (i = 0; i < ANM_QUEUE_SIZE; i++) {
		if (animationQueue[i].type == ANM_NONE) {
			return i;
		}
	}
	DEBUG("Animation was NOT added to queue");
	return -1;
}

/**
 * Clears the queue and moves the queued animations to the front
 *
 */
static void clearQueue(void)
{
	for (int i = 0; i < ANM_QUEUE_SIZE ; i++) {
		if (animationQueue[i].type != ANM_DONE) {
			animationQueue[i-1] = animationQueue[i];
		} 
		memset(&animationQueue[i], 0x00, sizeof(animationQueue[0]));
	}
}


