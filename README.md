# OCR S3 Epita

## Final version

### Requirements
	gtk+-3.0
	gmodule-2.0
	
    You must extract sets from train.tar.gz in order to train the network.
    Testing images can be found in data/

### Tools
    file : tools/uconv
    Convert image sets into a lighter binary file, 0s and 1s mainly.

### Run (GUI only)
	(build) make main
	or  ./start

### Useful files
	~/data/save.nd - Testing network data
