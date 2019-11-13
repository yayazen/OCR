#ifndef TRAIN_H
#define TRAIN_H

#define bool unsigned int
#define true 1
#define false 0

#include "net.h"
#include <stdlib.h>

#define A 10000
#define B 784


float train_data[A][B];
float test_data[A][B];

void simulate(NET *net, float *inputs, float *target, bool train);
int EvalRand(NET *net, float data[][784], bool test, bool train);
size_t net_answer(NET *net);

int load_gui(char *path);
int train_gui(float epsilon, bool TestOnly);
void save_training(void);
void print_gui(const char *fmt, ...);
#endif
