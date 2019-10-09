#include <stdlib.h>
#include <stdio.h>
#include "nnet.h"
#include <err.h>

int main(int argc, char** argv){

	if (argc < 3)
		errx(1,"main: invalid number of arguments\n");

	size_t* dimensions = malloc((argc - 1) * sizeof(size_t));
	if (dimensions == NULL)
		errx(1,"main: could not allocate memory\n");

	for (int i = 0; i < argc -1; i++) {
		size_t n = strtoul(argv[i+1], NULL, 10);
		if (n == 0)
			errx(1,"main: invalid argument\n");
		dimensions[i] = n;
	}
	
	nnet* net = newNnet((size_t) argc -1, dimensions);
	printf("%f\n", net->weights[0][2]);
	writeNnet(net, "netData");
	freeNnet(net);
	net = readNnet("netData");
	printf("%f\n", net->weights[0][2]);
	
}
