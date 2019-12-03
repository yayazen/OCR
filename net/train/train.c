#define USE_MNIST_LOADER
#define MNIST_FLOAT

#include "train.h"
#include "serialize.h"
#include "cycle.h"

#include <math.h>
#include <time.h>
#include <errno.h>  

#include <gtk/gtk.h>
#include "../../main.h"



/* NIST balanced mapping */
const char labels[47] = { 48, 49, 50, 51, 52,

				53, 54, 55, 56, 57, 65,

				66, 67, 68, 69, 70, 71,

				72, 73, 74, 75, 76, 77,

				78, 79, 80, 81, 82, 83,

				84, 85, 86, 87, 88, 89, 

				90, 97, 98, 100, 101, 102,
				
				103, 104, 110, 113, 114, 116 };






NET	*net;

mnist_data	*train_data; unsigned int train_cnt;
mnist_data	*test_data;  unsigned int test_cnt;

float	target[N] = {0};


size_t net_answer() {
	size_t ans = 0;
	for (size_t u = 1; u < (size_t)(net->out->units); u++)
		ans = (net->out->outputs[ans] < net->out->outputs[u]) ? u : ans;

	return ans;
}


void simulate(float *inputs, float *target, bool train) {
	init_inputs(net, inputs);

	feed_cycle(net);
	
	cross_entropy(net, target);	
	//mse_compute(net, target);

	if (train) {
		back_cycle(net);
		adjust(net);
	}	
}


int EvalRand(bool train, bool show) {
	mnist_data *set = (train) ? train_data : test_data;

	
	int rand = randint(0, ((train) ? train_cnt : test_cnt) - 1);	
	
	int l = set[rand].label;
	target[l] = 1;
	

	if (show)
		print2D(set[rand].data);

	simulate(set[rand].data, target, train);

	target[l] = 0;


	return l;
}









int load_gui(char *path, bool trsp) {
	srand(time(NULL));
	int ret;

	if ((ret = mnist_load(train_image_path, train_label_path, &train_data, &train_cnt)) 
		|| (ret = mnist_load(test_image_path, test_label_path, &test_data, &test_cnt))) {
		print_gui(NN.buffer, "Dataset load fail !\n");
		return ret;
	}

	
	// Net properties
	int scale[] = {784, 25, 25, N};

	if (!path) {
		net = init_network(4, scale);
		rand_weights(net);		
	}

	else
		net = load_net(path);
	
	if (!net)
		return errno;


	init_act(net);

		
	for (unsigned int i = 0;trsp && i < train_cnt; i++) { 
		transpose(train_data[i].data);
		if (i < test_cnt)
			transpose(test_data[i].data);
	}

	net->eta = LR;
	net->alpha = 0.2;

	print_gui(NN.buffer, "Success load from %s!\nData collection holds %d trains and %d tests ...\n", path, train_cnt, test_cnt);
	return 0;
}


void save_training(void) {
	serialize_net(net, "save.nd");
	free_network(net);
}






gboolean train_gui(gpointer data) {	

    if (data != NULL)
        return FALSE;

    srand(time(NULL));

	bool stop;
	float err = 0.0f, min = +HUGE_VAL;
	int count = 0;


	if (NN.train == 1) {
		do {
            while (gtk_events_pending())
                gtk_main_iteration();

			err = 0.0f;
			stop = true;
			for (int it = 0; it < 20; it++) {
				EvalRand(false, false);
				err = fmaxf(err, net->error);
				stop = stop && ((net->error) < NN.goal);
			}

			err = fmaxf(err, NN.goal);
			min = fminf(min, err);

			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
			printf("[ep. %d] --> Training %0.0f%%, err. %f", count, (NN.goal / min) * 100, min);

			if (!stop) {
				for (int it = 0; it < 100; it++) {
					EvalRand(true, false);
				}
			}
		} while(!stop && NN.train != 2 && ++count < EPOCHS); 
	}
		
	// TEST;
    if (NN.train > 0) {
    	print_gui(NN.buffer, "Epochs count %zu \n\nSamples,", count);
    }
	int l;
	size_t rate = count = 0;
	size_t ans;

	for (int i = 0; i < 100; i++) {
		l = EvalRand(false, (i % SHOW == 0));
		ans = net_answer(net);

		if (i % SHOW == 0) 
			print_gui(NN.buffer, "Test with %c found %c (err. %f) (eval. %f) ... ", labels[l], labels[ans], net->error,  net->out->outputs[ans]);

		if (net->error < NN.goal)
			rate++;

		if (l == (int) ans)
			count++;	
	}
		
	print_gui(NN.buffer, "\n[Terminate at %s] Success %zu%% with %zu%% below %f certainty\n", time_handler(), count, rate, NN.goal);

    NN.event = 0;
    gtk_button_set_label (GTK_BUTTON (TrainButton), "TRAIN");

	return FALSE;
}	
