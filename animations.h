#ifndef _ANIMATIONS_H
#define _ANIMATIONS_H
#include "stdbool.h"
#include "coloring.h"


enum AnimationType {
	ANM_NONE = 0x00,
	ANM_DONE = 0x00,
	ANM_COLOR = 0x01 << 0,
	ANM_SHIFT = 0x01 << 1,
	ANM_SPREAD = 0x01 << 2,
	ANM_OSC = 0x01 << 3
};


typedef struct {
	enum AnimationType type;
	float deltas[FLUID_COLOR_COUNT];
	double shiftChangePerStep;
	double spreadChangePerStep;
	double amp;	// oscillation amplitude
	float freq;	// oscillation frequency
	int currentStep;
	int maxStep;
	int dps; 	// duration per step
} animation_t;

bool createColorAnimation(int, int, float *);

bool createShiftAnimation(int, int, float);

bool createSpreadAnimation(int, int, float);

bool createSpreadAnimationOsc(int steps, int durationPerStep, int oscCount, float amp);

bool createShiftAnimationOsc(int steps, int durationPerStep, int oscCount, float amp);

bool createFullAnimation(int steps, int durationPerStep, float * endColor, double spreadEndVal, double shiftEndVal);

int playAnimation(void);

#endif /* _ANIMATIONS_H */
