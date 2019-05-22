#ifndef data_h
#define data_h

#include <stdint.h>

#ifndef NATIVE
#include <uncertain.h>
#endif

#define NUM_SAMPLES 64

typedef struct
{
    float Accel_LN_X;
    float Accel_LN_Y;
    float Accel_LN_Z;
    float Accel_WR_X;
    float Accel_WR_Y;
    float Accel_WR_Z;
#ifdef NATIVE
    float Gyro_X;
    float Gyro_Y;
    float Gyro_Z;
#else
    approximate_float Gyro_X;
    approximate_float Gyro_Y;
    approximate_float Gyro_Z;
#endif
    // float Mag_X;
    // float Mag_Y;
    // float Mag_Z;
    // float Quat_Madge_9DOF_W_LN;
    // float Quat_Madge_9DOF_X_LN;
    // float Quat_Madge_9DOF_Y_LN;
    // float Quat_Madge_9DOF_Z_LN;
} imu_sample;


const imu_sample samples[30191];

#endif
