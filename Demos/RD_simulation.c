#include "bmp.h"
#include "proc.h"

#include "net.h"
#include "cycle.h"
#include "serialize.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#define bool unsigned int
#define true 1
#define false 0


char                  Pattern[10][7][5] = { 	  { " OOO ",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    "O   O",
                                                    " OOO "  },

                                                  { "  O  ",
                                                    " OO  ",
                                                    "O O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  ",
                                                    "  O  "  },

                                                  { " OOO ",
                                                    "O   O",
                                                    "    O",
                                                    "   O ",
                                                    "  O  ",
                                                    " O   ",
                                                    "OOOOO"  },

                                                  { " OOO ",
                                                    "O   O",
                                                    "    O",
                                                    " OOO ",
                                                    "    O",
                                                    "O   O",
                                                    " OOO "  },

                                                  { "   O ",
                                                    "  OO ",
                                                    " O O ",
                                                    "O  O ",
                                                    "OOOOO",
                                                    "   O ",
                                                    "   O "  },

                                                  { "OOOOO",
                                                    "O    ",
                                                    "O    ",
                                                    "OOOO ",
                                                    "    O",
                                                    "O   O",
                                                    " OOO "  },

                                                  { " OOO ",
                                                    "O   O",
                                                    "O    ",
                                                    "OOOO ",
                                                    "O   O",
                                                    "O   O",
                                                    " OOO "  },

                                                  { "OOOOO",
                                                    "    O",
                                                    "    O",
                                                    "   O ",
                                                    "  O  ",
                                                    " O   ",
                                                    "O    "  },

                                                  { " OOO ",
                                                    "O   O",
                                                    "O   O",
                                                    " OOO ",
                                                    "O   O",
                                                    "O   O",
                                                    " OOO "  },

                                                  { " OOO ",
                                                    "O   O",
                                                    "O   O",
                                                    " OOOO",
                                                    "    O",
                                                    "O   O",
                                                    " OOO "  } };





int net_answer(NET *net) {
	int ans = 0;
	for (int u = 1; u < net->out.units; u++)
		ans = (net->out.outputs[ans] < net->out.outputs[u]) ? u : ans;

	return ans;
}

/*
void sampleXOR(NET *net, int id) {
	int u0 = (int) net->in.outputs[0], u1 = (int) net->in.outputs[1];
	int ans = net_answer(net);

	printf(" Simulation #%d - Errors : (unit %f, net %f)\n\n\
		\t[ (eval. 0 -> %f | 1 -> %f) / %s \\ %d xor %d = %d ]\n\n",
				id, net->out.errors[ans], net->error,
				net->out.outputs[0], net->out.outputs[1], 
			(ans == (u0 ^ u1)) ? "True" : "False", u0, u1, ans);
}
*/


void simulate(NET *net, float *inputs, float *target, bool train) {
	init_inputs(net, inputs);
	
	feed_cycle(net);
	mse_compute(net, target);

	if (train) {
		back_cycle(net);	
		ajustWeights(net);
	}	
} 


int main(int argc, char **argv) {
	unsigned long epoch;
	
	if (argc < 2 || (epoch = strtoul(argv[1], NULL, 10)) == 0)
	{
		fprintf(stderr, "usage: RD_simulation (epoch) (file.nd)\n");
		return 1;
	}


	
	// Simulation data
	float data[5 * 7];
	float target[10] = {0};
	
	// Net properties
	int scale[] = {10, 100, 100, 10};
	NET *net;
	
	if (argc != 3) {
		net = init_network(4, scale);

		srand(time(NULL));
		rand_weights(net);
	}
	else
		net = load_net(argv[2]);
	
	if (!net)
		return errno;

	
	for (unsigned long s = 0UL; s < epoch; s++) {
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 5; j++) {
				data[5 * i + j] = Pattern[s % 10][i][j];
			}
		}
		target[s % 10] = 1;
		simulate(net, data, target, epoch > 10);
		int ans = net_answer(net);
		printf("Simu %lu | %ld data got %d %s (err. %f, eval %f)  \n\n", s, s % 10, ans, (ans == (int) s % 10) ? "True" : "False", net->out.errors[ans], net->out.outputs[ans]);
		target[s % 10] = 0;
	}
	if (epoch > 10)
		serialize_net(net, "save.nd");
	
	/*
	// Sampling / Training
	for (unsigned long s = 0UL; s < epoch; s++) {
		simulate(net, data[s % 4], target[s % 4], epoch > 4);
		sampleXOR(net,s);		
	}

	if (epoch > 4)
		serialize_net(net, "save.nd");
	*/
			
	free_network(net);
	return 0;
}
