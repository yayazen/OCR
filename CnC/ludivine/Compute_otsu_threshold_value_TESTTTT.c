public static int computeOtsuThresholdLevel(int image[], int histogram[], int numOfLevels)
{

    // Total number of pixels [N]
  size_t len = sizeof(image);

    // Allocate space for the histogram
    //int[] histogram = new int[numOfLevels];        
    // Compute the histogram of the grayscale input
    for (size_t i = 0; i < total; i++)
      {
        histogram[image[i]]++; // [ni]
      }

    int meanTotal = 0; // [muT]

    
    // Compute mean value for the overall histogram
    for (int i = 0; i < numOfLevels; i++)
      {
        meanTotal += i * histogram[i];
      }

    int meanCurrent = 0;
    // [mu(k)]

    
    // The probability of the first class occurrence
    int omega0 = 0;
    
    // The probability of the second class occurrence
    int omega1 = 0;

    float maximumVariance = 0;
    int thresholdValue = 0;

    for (int k = 0; k < numOfLevels; k++)
      {
        // Compute omega0 for current k
        omega0 += histogram[k]; 
        if (omega0 == 0)
	  continue;
      }

        // Compute omega1 for current k
        omega1 = total - omega0;
	
        if (omega1 == 0)
	  break;

        meanCurrent +=  (k * histogram[k]);

        // Mean value for class 0 [mu0]
        float mean0 = (float)meanCurrent / (float)omega0;
	
        // Mean value for class 1 [mu1]
        float mean1 = ((float)meanTotal - (float)meanCurrent) / (float)omega1;

	
        // Calculate the between-class variance [sigmasquareB]
        float varianceBetween = (float) omega0 * (float) omega1 * (mean1 - mean0) * (mean1 - mean0);

        // Check if new maximum found
        if (varianceBetween > maximumVariance)
	  {
            maximumVariance = varianceBetween;
            thresholdValue = k;
	  }
    }

    return thresholdValue;
}
