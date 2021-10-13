#ifndef _APPLICATION_H
#define _APPLICATION_H

#ifndef VERSION
#define VERSION "vdev"
#endif

#include <bcl.h>
#include <twr.h>

#define _TWR_RADIO_PUB_BUFFER_SIZE_ACCELERATION (1 + sizeof(float) + sizeof(float) + sizeof(float))


bool twr_radio_pub_acceleration_hold(float *x_axis, float *y_axis, float *z_axis);

#endif
