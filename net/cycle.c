#include "cycle.h"

#include <math.h>


static float sig(float x) {
    return 1.0f / (1.0f + expf(-x));
}

static float dsig(const float x) {
	return x * (1.0f - x);
}






void feed(layer *from, layer *to) {
    float sum;
    for (int u = 0; u < to->units; u++) {
        sum = 0.0f;
        for (int v = 0; v <= from->units; v++) {
            sum += to->weights[u][v] * from->outputs[v];
        }
        to->outputs[u] = sig(sum);
    }
}

void feed_cycle(NET *net) {
	for (int l = 0; l < net->nlayers - 1; l++) {
		feed(&net->layers[l], &net->layers[l+1]);
	}
}




void back(layer *from, layer *to) {
	float error;
	for (int u = 0; u < to->units; u++) {
		error = 0;	
		for (int v = 0; v < from->units; v++) {
			error += from->weights[v][u] * from->errors[v];
		}
		to->errors[u] = dsig(to->outputs[u]) * error;
	}
}

void back_cycle(NET *net) {
	for (int l = net->nlayers - 1; l > 1; l--) {
		back(&net->layers[l], &net->layers[l-1]);
	}

}






void mse_compute(NET *net, float *target) {
	float e = net->error = 0.0f;
	for (int u = 0; u < net->out.units; u++) {
		e = target[u] - net->out.outputs[u];
		net->out.errors[u] = dsig(net->out.outputs[u]) * e;
		net->error += sqr(e) * 0.5f;
	}
}



void ajustWeights(NET *net) {
	float delta, value;
	for (int l = 1; l < net->nlayers; l++) {
		
		for (int u = 0; u < net->layers[l].units; u++) {
			for (int v = 0; v <= net->layers[l-1].units; v++) {
				delta = net->layers[l].delta[u][v];
				value = net->eta * net->layers[l].errors[u] * net->layers[l-1].outputs[v];
			
				net->layers[l].delta[u][v] = value;
				net->layers[l].weights[u][v] += value + net->alpha * delta;
			}
		}

	}

}

