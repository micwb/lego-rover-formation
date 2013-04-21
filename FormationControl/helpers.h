#ifndef _FORMATION_CONTROL_HELPERS_
#define _FORMATION_CONTROL_HELPERS_

extern const float MAX_ROBOT_SPEED_DPS;
extern const float MAX_ROBOT_SPEED_MPS;
extern const float MAX_ROBOT_SPEED_SPS;

#define MPS_TO_SPS(x)              ( (x * (MAX_ROBOT_SPEED_SPS/MAX_ROBOT_SPEED_MPS)) > MAX_ROBOT_SPEED_SPS ? \
                                      MAX_ROBOT_SPEED_SPS :                                                  \
                                     (x * (MAX_ROBOT_SPEED_SPS/MAX_ROBOT_SPEED_MPS)) )
#define SPS_TO_MPS(x)              ( (x * (MAX_ROBOT_SPEED_MPS/MAX_ROBOT_SPEED_SPS)) > MAX_ROBOT_SPEED_MPS ? \
                                      MAX_ROBOT_SPEED_MPS :                                                  \
                                     (x * (MAX_ROBOT_SPEED_MPS/MAX_ROBOT_SPEED_SPS)) )

#define DPS_TO_SPS(x)              ( (x * (MAX_ROBOT_SPEED_SPS/MAX_ROBOT_SPEED_DPS)) > MAX_ROBOT_SPEED_SPS ? \
                                      MAX_ROBOT_SPEED_SPS :                                                  \
                                     (x * (MAX_ROBOT_SPEED_SPS/MAX_ROBOT_SPEED_DPS)) )


void f2i(float xf, float yf, int* xi, int* yi, int width, int height);
void f2idx(float xf, int *xi, int width, int height);
void f2idy(float xy, int *xi, int width, int height);
void i2f(float* xf, float* yf, int xi, int yi, int width, int height);
void i2fdx(float* xf, int *xi, int width, int height);
void i2fdy(float* yf, int *yi, int width, int height);

#endif//_FORMATION_CONTROL_HELPERS_
