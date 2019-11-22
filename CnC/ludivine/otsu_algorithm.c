




int otsu_algorithm(float *histogram, unsigned long total_pixels)
{
  // initialisation
  
  double probability[256];
  double mean[256];
  double max_between;
  double between[256];
  int threshold;

  // probability = class probability
  // mean = class mean
  // between = average
   

  for(int i = 0; i < 256; i++)
    {
      probability[i] = 0.0;
      mean[i] = 0.0;
      between[i] = 0.0;
    }

  probability[0] = histogram[0];

  for(int i = 1; i < 256; i++)
    {
      probability[i] = probability[i - 1] + histogram[i];
      mean[i] = mean[i - 1] + i * histogram[i];
    }

  threshold = 0;
  max_between = 0.0;


  for(int i = 0; i < 255; i++)
    {
      if(probability[i] != 0.0 && probability[i] != 1.0)
	{
	  between[i] = pow(mean[255] * probability[i] - mean[i], 2) / (probability[i] * (1.0 - probability[i]));
	}
      else
	{
	  between[i] = 0.0;
	}
      if(between[i] > max_between)
	{
	  max_between = between[i];
	  threshold = i;
        }
    }

  return threshold;
}
