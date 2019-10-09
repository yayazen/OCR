#ifndef NNET_H
#define NNET_H
#include <stddef.h>

typedef struct nnet{
	/* 	weights is a pointer to an array of matrices storing the weights of the network
		weights[i] is a pointer to a matrice of dimensions[i] * dimensions[i+1]
		
		dimensions is a pointer to an array of dimensions, dimensions[i] is the number of neurons
		in the ith layer.

		numLayers is the number of layers inside the network
	*/
	
	double** weights; 
	size_t* dimensions;
	size_t numLayers;


} nnet;

nnet* newNnet(size_t numLayers, size_t* dimensions);

nnet* newBlankNnet(size_t numLayers, size_t* dimensions);

void freeNnet(nnet* net);

void writeNnet(nnet* net, const char* filename);

nnet* readNnet(const char* filename);

#endif
