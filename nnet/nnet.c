#include "nnet.h"
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

nnet* newNnet(size_t numLayers, size_t* dimensions){
	// Builds a neural net from the number of layers and a dimensions array for thoses layers
	// and initialize it
	nnet* net = newBlankNnet(numLayers, dimensions);
	// extern nnet net;	
	for (size_t i = 0; i < numLayers -1; i++){
		//assigning a random number between -1.00 and +1.00 to every weight
		for (size_t j = 0; j < dimensions[i] * dimensions[i+1]; j++)
			net->weights[i][j] = ((double) (rand() % 200) - 100) / 100.0;
	}
	return net;
}

nnet* newBlankNnet(size_t numLayers, size_t* dimensions){
        // Builds a neural net from the number of layers and a dimensions array for thoses layers
	// Doesn't initialize it
        nnet* net = malloc(sizeof(nnet));

        if (net == NULL)
                errx(1,"newBlankNnet: couldn't allocate memory\n");

        net->numLayers = numLayers;
        net->dimensions = malloc(numLayers * sizeof(size_t));

        if (net->dimensions == NULL)
                errx(1,"newBlankNnet: couldn't allocate memory\n");

        for (size_t i = 0; i < numLayers; i++)
                net->dimensions[i] = dimensions[i];

        if (numLayers < 1)
                errx(1,"newBlankNnet: invalid argument, numLayers cannot be 0\n");
        net->weights = malloc((numLayers - 1) * sizeof(double*));

        if (net->weights == NULL)
                errx(1,"newBlankNnet: couldn't allocate memory\n");

        for (size_t i = 0; i < numLayers -1; i++){
                net->weights[i] = malloc(dimensions[i] * dimensions[i+1] * sizeof(double));
                if (net->weights[i] == NULL)
                        errx(1,"newBlankNnet: couldn't allocate memory\n");
        }
        return net;
}

void freeNnet(nnet* net){
	// Frees a nnet struct
	for (size_t i = 0; i < net->numLayers-1; i++){
		free(net->weights[i]);
	}
	free(net->weights);
	free(net->dimensions);
	free(net);
	return;
}

void writeNnet(nnet* net, const char* filename){
	//writes nnet object to a file
	FILE* f = fopen(filename, "wb");
	if (f == NULL)
		errx(1,"writeNnet: could not open file\n");
	fwrite(&(net->numLayers), sizeof(size_t), 1, f);
	fwrite(net->dimensions, sizeof(size_t), net->numLayers, f);
	for (size_t i = 0; i < net->numLayers - 1; i++)
		fwrite(net->weights[i], sizeof(double), net->dimensions[i] * net->dimensions[i+1], f);
	fclose(f);
}

nnet* readNnet(const char* filename){
	// reads a file and create a nnet object
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		errx(1,"readNnet: could not open file\n");
	
	size_t numLayers = 0;
	fread(&numLayers, sizeof(size_t), 1, f);
	size_t* dimensions = malloc(numLayers * sizeof(size_t));
	if (dimensions == NULL) 
		errx(1,"readNnet: could not allocate memory\n");
	
	fread(dimensions, sizeof(size_t), numLayers, f);
	nnet* net = newBlankNnet(numLayers, dimensions);
	
	for (size_t i = 0; i < numLayers -1; i++)
		fread(net->weights[i], sizeof(double), net->dimensions[i] * net->dimensions[i+1], f);
	fclose(f);
	free(dimensions);
	return net;
}
