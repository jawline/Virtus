/*
 * Clock.h
 *
 *  Created on: 28 Oct 2011
 *      Author: blake
 */

#ifndef CLOCK_H_
#define CLOCK_H_

/**
 * Handles the time passing betweens frames
 */

class Clock {
private:

#ifdef _BUILD_WIN32_
	unsigned long m_lastTime;
#elif _BUILD_LINUX_
	unsigned long m_lastTime;
#else
#error "Not build config (OS) supported (Clock.h)"
#endif

public:
	Clock();
	virtual ~Clock();

	/**
	 * @brief Initialise the clock
	 */

	void initialize();

	/**
	 * @brief Returns the time that has elapsed since this was last called in seconds
	 */
	float calculateElapsedTime();
};

#endif /* CLOCK_H_ */
