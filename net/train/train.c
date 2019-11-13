#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <errno.h>
#include <time.h>
#include <gtk/gtk.h>

#include "net.h"
#include "train.h"
#include "cycle.h"
#include "serialize.h"

#include "mnist.h"
#include "../main.h"

NET *net;

size_t net_answer(NET *net) {
	size_t ans = 0;
	for (size_t u = 1; u < (size_t)(net->out.units); u++)
		ans = (net->out.outputs[ans] < net->out.outputs[u]) ? u : ans;

	return ans;
}


void simulate(NET *net, float *inputs, float *target, bool train) {
	init_inputs(net, inputs);

	feed_cycle(net);
	mse_compute(net, target);
	
	if (train) {
		back_cycle(net);
		adjust(net);
	}	
}


int EvalRand(NET *net, float data[][784], bool test, bool train) {
	float target[10] = {0};
	
	int rand = randi(0, 10000 - 1);	
	
	int l = (test) ? test_label[rand] : train_label[rand];
	
	target[l] = 1;

	
	simulate(net, data[rand], target, train);

	target[l] = 0;
	return l;
}


int load_gui(char *path) {
	load_mnist();

	conv(train_data, train_image, A);
	conv(test_data, test_image, A);

	// Net properties
	int scale[] = {784, 25, 25, 10};

	srand(time(NULL));
	if (!path) {
		net = init_network(4, scale);
		rand_weights(net);
		
	}
	else
		net = load_net(path);
	
	if (!net)
		return errno;

	print_gui("Success load from %s!\nPress TRAIN or TEST ...\n", path);
	return 1;
}

int train_gui(float epsilon, bool TestOnly) {
	size_t ans, count = 0;
	float err, status = 0.0f;
	bool stop;
	srand(time(NULL));
	if (!TestOnly) {
	do {
		err = 0.0f;
		stop = true;
		count ++;

		for (int t = 0; t < 20; t++) {
			EvalRand(net, test_data, true, false);

			err = MAX(err, net->error);
			stop = stop && (net->error < epsilon);
		}
		err = MAX(err, epsilon);
		status = ((epsilon / err) * 100 > status) ? (epsilon / err) * 100 : status;
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		printf("Training %0.0f%% completed ...", status);
		if (!stop) {
			for (int t = 0; t < 100; t++) {
				EvalRand(net, train_data, false, true);
			}
		}

	} while (!stop);
	}

	int l;
	size_t rate = count = 0;
	
	for (int i = 0; i < 20; i++) {
		l = EvalRand(net, test_data, true, false);
		ans = net_answer(net);
		print_gui("Test with %d found %zu (err. %f) (eval. %f) ... ", l, ans, net->error,  net->out.outputs[ans]);
		
		if (net->error < epsilon)
			rate++;
		if (l == (int) ans)
			count++;	
	}
	
	print_gui("\nSuccess %zu of 20 with %zu below %f certainty\n", count, rate, epsilon); 
	return 0;		
}

void save_training(void) {
	serialize_net(net, "gui_save.nd");
}


void print_gui(const char *fmt, ...) {
	char buf[4096];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
		
	mark = gtk_text_buffer_get_insert (buffer);
	gtk_text_buffer_get_iter_at_mark (buffer, &iter, mark);

	if (gtk_text_buffer_get_char_count (buffer))
		gtk_text_buffer_insert (buffer, &iter, "\n", 1);
	
	gtk_text_buffer_insert (buffer, &iter, buf, -1);
}
