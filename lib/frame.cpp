#include "frame.h"
#include <bits/chrono.h>
#include <chrono>
#include <thread>

Frame::Frame(long fps) 
{
    milliseconds_per_frame = (1000 / fps);
    limit_timestamp_ms = get_timestamp_ms();
    delta_time_ms = 0;
}

long Frame::get_delta_time_ms()
{
    return delta_time_ms;
}

long Frame::get_milliseconds_per_frame()
{
    return milliseconds_per_frame;
}

void Frame::limit()
{
    long current_timestamp_ms = get_timestamp_ms();
    delta_time_ms = current_timestamp_ms - limit_timestamp_ms;
    if (delta_time_ms < milliseconds_per_frame)
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds_per_frame-delta_time_ms));
    limit_timestamp_ms = get_timestamp_ms();
}

long Frame::get_timestamp_ms()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Frame frame {30};