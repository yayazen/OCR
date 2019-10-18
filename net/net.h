#ifndef NET_H
#define NET_H

#define BI 1

struct LAYER {
    int		units;

    float	*outputs;
    float   *errors;
    float   **weights;
	float 	**delta;
};
typedef struct LAYER layer;

typedef struct {
    int		nlayers;
    
    layer	*layers;
    layer	in;
    layer   out;

    float   eta, alpha, error;
} NET ;


float randomf(float a, float b);

NET * init_network(int nlayers, int *scale);    

void init_inputs(NET *net, float *inputs);

void rand_weights(NET *net);

void free_network(NET *net);    
#endif
