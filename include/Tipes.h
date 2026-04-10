#pragma once
struct ASMR_Entry
{
    union
    {
        uint8_t raw;

        uint8_t cyc_type : 2;

        struct
        {
            uint8_t cyc_type : 2;
            uint8_t stidle_mode : 6;
        } stidle;
        
        struct
        {
            uint8_t cyc_type : 2;
            uint8_t forw_mode : 1;
            uint8_t forw_dist : 5;
        } forw;

        struct
        {
            uint8_t cyc_type : 2;
            uint8_t turn_mode : 2;
            uint8_t turn_source : 1;
            uint8_t turn_angle : 2;
            uint8_t turn_dir : 1;
        } turn;
    };
};

struct SensorData 
{
    float odom_S;
    float odom_theta;
    float time;
    int dist_left;
    int dist_right;
    int dist_fleft;
    int dist_fright;
    bool is_wall_left;
    bool is_wall_right;
    bool is_wall_fleft;
    bool is_wall_fright;
};

struct CyclogramOutput
{
    float theta_i0;
    float v_0;
    bool is_completed;
};

enum ASMR_CYC : uint8_t
{
    STOP = 0b00000000,
    IDLE = 0b00000001,

    SWD05 = 0b01000001,
    SWD1 = 0b01000010,
    SS90SEL = 0b10010010,
    SS90SER = 0b10010011,
    TURN_CYC = 0b10000000,

    FROM_STRAIGHT = 0,
    FROM_DIAG = 0b00001000,

    SHORTEST = 0,
    EXPLORE = 0b00010000,
    IN_PLACE = 0b00100000,

    T45 = 0,
    T90 = 0b00000010,
    T135 = 0b00000100,
    T180 = 0b00000110,

    TURN_LEFT = 0b00000000,
    TURN_RIGHT = 0b00000001,
};

