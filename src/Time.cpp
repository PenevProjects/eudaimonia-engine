#include "Time.h"
#include <sstream>
#include <iostream>

//Referenced from "Acquiring high resolution time stamps". https://docs.microsoft.com/en-us/windows/win32/sysinfo/acquiring-high-resolution-time-stamps

LARGE_INTEGER Time::starting_time_{ 0 };
LARGE_INTEGER Time::ending_time_{ 0 };
LARGE_INTEGER Time::elapsed_ms_{ 0 };
LARGE_INTEGER Time::frequency_{ 0 };
double Time::delta_time_{ 0.007 };
int Time::frames_{ 0 };
float Time::avg_FPS_{ 0.0f };

void Time::update()
{
	QueryPerformanceFrequency(&frequency_);
	QueryPerformanceCounter(&starting_time_);
}

void Time::reset()
{
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//
	QueryPerformanceCounter(&ending_time_);
	elapsed_ms_.QuadPart = ending_time_.QuadPart - starting_time_.QuadPart;
	elapsed_ms_.QuadPart *= 1000000;
	elapsed_ms_.QuadPart /= frequency_.QuadPart;
	delta_time_ = (double)elapsed_ms_.QuadPart / 1000000.0;
	frames_++;
	avg_FPS_ += 1.0f / (float)delta_time_;
}
void Time::displayFPSinWindowTitle(SDL_Window* _window, std::string _title)
{
	//use average fps over 10 frames
	if (frames_ > 10)
	{
		std::stringstream ss;
		ss << _title << avg_FPS_/frames_;
		SDL_SetWindowTitle(_window, ss.str().c_str());
		avg_FPS_ = 0;
		frames_ = 0;
	}
}


