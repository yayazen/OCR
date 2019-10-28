#ifndef CYCLE_H
#define CYCLE_H

#define sqr(x)  ( (x) * (x) )

#include "net.h"


void feed(layer *from, layer *to);
void feed_cycle(NET *net);

void back(layer *from, layer *to);
void back_cycle(NET *net);

void mse_compute(NET *net, float *target);

void ajustWeights(NET *net);
#endif
