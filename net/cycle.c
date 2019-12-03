#include "cycle.h"

#include <math.h>



static float sig(float x) {
    return 1.0f / (1.0f + expf(-x));
}

static float dsig(float x) {
	return x * (1.0f - x);
}

/*
static float relu(float x) {
	return fmaxf(0, x);
}

static float drelu(float x) {
	return (x > 0) ? 1 : 0; 
}
*/

void init_act(NET *net) {
	net->layers[1]->act = sig;
	net->layers[1]->dact = dsig;
	
	net->layers[2]->act = sig;
	net->layers[2]->dact = dsig;

	net->layers[3]->act = expf;
	net->layers[3]->dact = dsig;
}


void softmax(layer *out) {
	float sum = 0.0f;
	for (int u = 0; u < out->units; u++) {
		sum += out->outputs[u];
	}

	for (int u = 0; u < out->units; u++) {
		out->outputs[u] = out->outputs[u] / sum;
	}
}




void feed(layer *from, layer *to) {
	float sum;
    for (int u = 0; u < to->units; u++) {
        sum = 0.0f;
        
		for (int v = 0; v <= from->units; v++) {
            sum += to->weights[u][v] * from->outputs[v];
        }
        to->outputs[u] = to->act(sum);
    }
}

void feed_cycle(NET *net) {
	/*
	from in to out (exclude)
	*/
	for (int l = 0; l < net->nlayers - 1; l++) {
		feed(net->layers[l], net->layers[l+1]);
	}
	softmax(net->out);
}




void back(layer *from, layer *to) {
	float err;
	for (int u = 0; u < to->units; u++) {
		err = 0;	
		for (int v = 0; v < from->units; v++) {
			err += from->weights[v][u] * from->errors[v];
		}
		to->errors[u] = to->dact(to->outputs[u]) * err;
	}
}


void back_cycle(NET *net) {
	/*
	from out to input layer (exclude)
	*/
	for (int l = net->nlayers - 1; l > 1; l--) {
		back(net->layers[l], net->layers[l-1]);
	}

}



void cross_entropy(NET *net, float *target) {
	float y, out;
	net->error = 0.0f;
	for (int u = 0; u < net->out->units; u++) {
		out = net->out->outputs[u];
		y = target[u];

		net->out->errors[u] = (y - out); 
	//	net->error -= (y == 0.0f) ? log10f(1.0f - out) : log10f(out);
		net->error += sqr(y - out) * 0.5f;
	}
}


void mse_compute(NET *net, float *target) {
	float e;
	net->error = 0.0f;
	for (int u = 0; u < net->out->units; u++) {
		e = target[u] - net->out->outputs[u];
		
		net->out->errors[u] = net->out->dact(net->out->outputs[u]) * e;
		net->error += sqr(e) / net->out->units;
	}
}



void adjust(NET *net) {
	float delta, val;
	
	for (int l = 1; l < net->nlayers; l++) {
		
		for (int u = 0; u < net->layers[l]->units; u++) {
			for (int v = 0; v <= net->layers[l-1]->units; v++) {
				val = net->eta * net->layers[l]->errors[u] * net->layers[l-1]->outputs[v];
				
				delta = net->layers[l]->delta[u][v];
				net->layers[l]->delta[u][v] = val;
	
				net->layers[l]->weights[u][v] += val + net->alpha * delta;
			}
		}
	}
}

