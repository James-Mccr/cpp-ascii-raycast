#pragma once

class Frame
{
public:
    Frame(long fps);
    
    void limit();
    long get_delta_time_ms();
    long get_milliseconds_per_frame();

private:
    long milliseconds_per_frame{};
    long limit_timestamp_ms{};
    long delta_time_ms{};

    long get_timestamp_ms();
};

extern Frame frame;