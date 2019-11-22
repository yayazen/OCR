#ifndef CYCLE_H
#define CYCLE_H

#define sqr(x)  ( (x) * (x) )

#include "net.h"

void init_act(NET *net);

void feed_cycle(NET *net);

void back_cycle(NET *net);

void cross_entropy(NET *net, float *target);
void mse_compute(NET *net, float *target);

void adjust(NET *net);
#endif
