/*
 * Clock.cpp
 *
 *  Created on: 28 Oct 2011
 *      Author: blake
 */

#include "Clock.h"
#include <time.h>
#include <stdio.h>

Clock::Clock() {
}

Clock::~Clock() {
}

void Clock::initialize(void) {

	Clock::m_lastTime = clock();

}

float Clock::calculateElapsedTime(void) {

	unsigned long currentTime = clock();

	float cps = CLOCKS_PER_SEC;
	float timePassed = ((float)(currentTime - Clock::m_lastTime)) / cps;


	Clock::m_lastTime = currentTime;

	return timePassed;
}
