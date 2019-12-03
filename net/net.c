#include "net.h"

#include <stdlib.h>



NET * init_network(int nlayers, int *scale) {
    static NET net;
    
    net.nlayers = nlayers;
    net.layers = calloc(nlayers, sizeof(layer *));
    
    if (!net.layers)
        return NULL;

    for (int l = 0; l < nlayers; l++) {
        net.layers[l] = malloc(sizeof(layer));

        net.layers[l]->units = scale[l];
        
        net.layers[l]->outputs = calloc(scale[l] + 1, sizeof(float));
        net.layers[l]->outputs[scale[l]] = BI;
        
        net.layers[l]->errors = calloc(scale[l], sizeof(float));
        net.layers[l]->weights = calloc(scale[l], sizeof(float *));
        
        net.layers[l]->delta = calloc(scale[l], sizeof(float *));

        if (l > 0) {
            for (int u = 0; u < scale[l]; u++) {
                net.layers[l]->weights[u] = calloc(scale[l - 1] + 1, sizeof(float));
                net.layers[l]->delta[u] = calloc(scale[l - 1] + 1, sizeof(float));
            }
        }
    }

    net.in = net.layers[0];
    net.out = net.layers[nlayers - 1];

    return &net;
}




float randomf(float a, float b) {
    return ((float) rand() / RAND_MAX) * (b - a) + a;
}

int randint(int a, int b) {
    return rand() % (b - a + 1) + a;
}




void init_inputs(NET *net, float *inputs) {
    for (int u = 0; u < net->in->units; u++) {
        net->in->outputs[u] = inputs[u];
    }
}



void rand_weights(NET *net) {
    for (int l = 1; l < net->nlayers; l++) {
    
        for (int u = 0; u < net->layers[l]->units; u++) {
            for (int v = 0; v <= net->layers[l - 1]->units; v++) {
                
                net->layers[l]->weights[u][v] = randomf(-0.5, 0.5);
            }
        }
    }
}





void free_network(NET *net) {
    for (int l = 0; l < net->nlayers; l++) {
        free(net->layers[l]->outputs);
        free(net->layers[l]->errors);
    
        for (int u = 0; u < net->layers[l]->units; u++) {
            free(net->layers[l]->weights[u]);
            free(net->layers[l]->delta[u]);
        }
        free(net->layers[l]->weights);
        free(net->layers[l]->delta);
        free(net->layers[l]);
    }
    free(net->layers);
    net = NULL;
}
