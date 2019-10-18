#include "net.h"
#include "cycle.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX(x,y)	((x) > (y) ? (x) : (y))

void Simulate(NET *net, float *inputs, float *target) {
	init_inputs(net, inputs);
	
	feed_cycle(net);
	compute_errors(net, target);
	back_cycle(net);

	ajustWeights(net);
} 


void SampleOutput(NET *net, unsigned long id_sim, int ans) {
	int x = (int) net->in.outputs[0], y = (int) net->in.outputs[1];
	
	printf("Simu.%lu - got [%d, %d] and answered with %d (%.3f %%); \
		NET error -> %f\n", id_sim, x, y, ans, net->out.outputs[ans], net->error);

}


int main(int argc, char **argv) {
	unsigned long count, sfreq;
	
	if (argc != 3 || (count = strtoul(argv[1], NULL, 10)) == 0 
			|| (sfreq = strtoul(argv[2], NULL, 10)) == 0)
	{	
		fprintf(stderr, "usage: XOR_simulation (count) (sample_frequence)\n");
		return 1;
	}



	srand(time(NULL));
	
	float data[4][2] = {{1, 0}, {0, 0}, {0, 1}, {1, 1}};
	float target[2][2] = {{1, 0}, {0, 1}};

	int scale[2] = {2, 2};

	NET *net = init_network(2, scale);
	if (!net)
		return 255;

	rand_weights(net);	
	
	for (unsigned long i = 1UL; i < count; i++) {
		Simulate(net, data[i % 4], target[i % 2]);

		if (i % sfreq == 0) {
			int ans = (net->out.outputs[0] > net->out.outputs[1]) ? 0 : 1;
			SampleOutput(net, i, ans);
		}
	}
	
	free_network(net);
}
