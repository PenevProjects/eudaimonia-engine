#ifndef _TIME_H
#define _TIME_H

#include <windows.h>
#include <SDL2/SDL.h>
#include <string>


class Time
{
private:
	static LARGE_INTEGER starting_time_;
	static LARGE_INTEGER ending_time_;
	static LARGE_INTEGER elapsed_ms_;
	static LARGE_INTEGER frequency_;
	static double delta_time_;
	static int frames_;
	static float avg_FPS_;
public:
	static double delta_time() { return delta_time_; }
	static double fps() { return 1.0 / delta_time_; }
	static void update();
	static void reset();
	static void displayFPSinWindowTitle(SDL_Window* _window, std::string _title);
};



#endif