#include "serialize.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

unsigned int serialize_net(NET *net, const char *path) {
	FILE *fp = fopen(path, "wb");
	if (!fp) {
		return FILE_ERROR;
	}

	
	int flag = 0x4e45;
	
	fwrite(&flag, sizeof(int), 1, fp);
	
	fwrite(&net->nlayers, sizeof(int), 1, fp);
	for (int l = 0; l < net->nlayers; l++) {
		fwrite(&net->layers[l].units, sizeof(int), 1, fp);
	}


	for (int l = 1; l < net->nlayers; l++) {
		
		for (int u = 0; u < net->layers[l].units; u++) {
				fwrite(&net->layers[l].weights[u], sizeof(float), net->layers[l-1].units + 1, fp);
		}
	}

	fclose(fp);	
	
	return NO_ERROR;
}




NET * load_net(const char *path) {
	FILE *fp = fopen(path, "rb");
	if (!fp) {
		errno = FILE_ERROR;
		return NULL;
	}

	
	int flag;
	fread(&flag, sizeof(int), 1, fp);
	
	if (flag != 0x4e45) {
		errno = INVALID_FORMAT;
		return NULL;
	}

	
	int nlayers;
	fread(&nlayers, sizeof(int), 1, fp);

	int scale[nlayers];
	fread(scale, sizeof(int), 1, fp);


	NET *net = init_network(nlayers, scale);
	for (int l = 1; l < net->nlayers; l++) {
	
		for (int u = 0; u < net->layers[l].units; u++) {
			fread(&net->layers[l].weights[u], sizeof(float), net->layers[l-1].units + 1, fp);
		}
	}

	return net;
}
