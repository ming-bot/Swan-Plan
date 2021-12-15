#ifndef CON_TROL_GET_H
#define CON_TROL_GET_H

#include "struct_typedef.h"
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"

void QuaternionToAngleAxis(const float * quaternion, float* angle_axis);

extern void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

float invSqrt(float x);

#endif