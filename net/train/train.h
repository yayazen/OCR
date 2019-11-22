#ifndef TRAIN_H
#define TRAIN_H

#include "net.h"
#include "mnist.h"
#include <stdlib.h>



#define N 47


#define	bool unsigned int
#define true 	1
#define false 	0


#define train_image_path  "./data/train-images-idx3-ubyte"
#define train_label_path  "./data/train-labels-idx1-ubyte"
#define test_image_path   "./data/test-images-idx3-ubyte"
#define test_label_path   "./data/test-labels-idx1-ubyte"


#define EPOCHS 		10000
#define GOAL		0.001

#define LR			0.15
#define LR_INC		1.2
#define	LR_DEC		0.7

#define	PERF_INC	1.2
#define	MIN_GRAD	0.00001

#define SHOW		25
#define	CMDLINE		false
#define GUI			true
	


int load_gui(char *path, bool trsp);


int train_gui(bool TestOnly);

void save_training(void);



void print_gui(const char *fmt, ...);
#endif
