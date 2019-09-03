#include <stdint.h>
#include <math.h>

#ifdef NATIVE
#include <stdio.h>
#else
#include <printf.h>
#include <uncertain.h>
#endif

#include "MadgwickAHRS.h"

#ifdef NATIVE
typedef float approximate_float;

float unf_var(approximate_float _1) { return nan(""); }
approximate_float unf_create(approximate_float val, float _) { return val; }

approximate_float sqrtunf(approximate_float val) { return sqrtf(val); }
approximate_float atan2unf(approximate_float y, approximate_float x) { return atan2f(y, x); }
approximate_float asinunf(approximate_float val) { return asinf(val); }
#endif

void getEulerAngle(float q0, float q1, float q2, float q3, float *roll, float *pitch, float *yaw);

int main()
{
    float pi = acos(-1.0);
    float oneDegree = pi / 180;

    q0 = unf_create(0.0f, 0);
    q1 = unf_create(1.0f, 0);
    q2 = unf_create(0.0f, 0);
    q3 = unf_create(0.0f, 0);

    printf("\n");

    float timeStep = 1 / 512.0f;
    float rollRate = 45.0f * oneDegree;
    int i = 0;
    // get_covar(samples[i].Gyro_X, samples[i].Gyro_X);

    while(1) {
        float targetRoll = timeStep * i * rollRate;
        approximate_float ax = unf_create(1e-8, 0);
        approximate_float ay = unf_create(-10 * sinf(targetRoll), 1e-8);
        approximate_float az = unf_create(-10 * cosf(targetRoll), 1e-8);

        approximate_float gx = unf_create(rollRate, rollRate * rollRate * 1e-4);
        approximate_float gy = unf_create(0, 0);
        approximate_float gz = unf_create(0, 0);

        // printf("q0: %f\n", gy);

        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az);

        float roll = 0, pitch = 0, yaw = 0;
        getEulerAngle(q0, q1, q2, q3, &roll, &pitch, &yaw);
        printf("Sample: %-5d ", i);
        printf("acc: (%+05.1f ± %05.3f, %+05.1f ± %05.3f, %+05.1f ± %05.3f), gyr: (%+05.1f ± %05.3f, %+05.1f ± %05.3f, %+05.1f ± %05.3f)",
               ax, sqrtf(unf_var(ax)), ay, sqrtf(unf_var(ay)), az, sqrtf(unf_var(az)),
               gx/oneDegree, sqrtf(unf_var(gx))/oneDegree, gy/oneDegree, sqrtf(unf_var(gy))/oneDegree, gz/oneDegree, sqrtf(unf_var(gz))/oneDegree
               );
        printf("\n              ");
        printf("Angle: (%+05.1f ± %05.3f, %+05.1f ± %05.3f, %+05.1f ± %05.3f)",
               roll/oneDegree, sqrtf(unf_var(roll))/oneDegree,
               pitch/oneDegree, sqrtf(unf_var(pitch))/oneDegree,
               yaw/oneDegree, sqrtf(unf_var(yaw))/oneDegree);
        printf("\t\tq: (%+05.1f ± %05.3f, %+05.1f ± %05.3f, %+05.1f ± %05.3f, %+05.1f ± %05.3f)",
               q0, sqrtf(unf_var(q0)),
               q1, sqrtf(unf_var(q1)),
               q2, sqrtf(unf_var(q2)),
               q3, sqrtf(unf_var(q3)));


        printf("\n\n");

        if (i >= 1024) {
            break;
        }

        i++;
    }

    return 0;
}

// source: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Quaternion_to_Euler_Angles_Conversion
void getEulerAngle(float q0, float q1, float q2, float q3, float *roll, float *pitch, float *yaw)
{
    float pi = acos(-1.0);
	// roll (x-axis rotation)
	float sinr_cosp = +2.0f * (q0 * q1 + q2 * q3);
	float cosr_cosp = +1.0f - 2.0f * (q1 * q1 + q2 * q2);
	*roll = atan2unf(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = +2.0 * (q0 * q2 - q3 * q1);
	if (fabsf(sinp) >= 1)
		*pitch = copysignf(pi / 2, sinp); // use 90 degrees if out of range
	else
		*pitch = asinunf(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = +2.0f * (q0 * q3 + q1 * q2);
	float cosy_cosp = +1.0f - 2.0f * (q2 * q2 + q3 * q3);
	*yaw = atan2unf(siny_cosp, cosy_cosp);
}
