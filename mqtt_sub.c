#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>

#define QOS 2
#define SUB_TOPIC "PARK"

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
	printf("ID: %d\n", * (int *) obj);
	if(rc) {
		printf("Error with result code: %d\n", rc);
		exit(-1);
	}
	mosquitto_subscribe(mosq, NULL, SUB_TOPIC, QOS);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
	printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
	printf("sleep 2sec....\n");
	sleep(2);
}

int main() {
	int rc, id=13;

	mosquitto_lib_init();

	struct mosquitto *mosq;

	mosq = mosquitto_new("sub-test", true, &id);
	mosquitto_opts_set(mosq, MOSQ_OPT_TCP_NODELAY, 1);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);

	rc = mosquitto_connect(mosq, "192.168.219.173", 1883, 10);
	if(rc) {
		printf("Could not connect to Broker with return code %d\n", rc);
		return -1;
	}

	mosquitto_loop_start(mosq);
	printf("Press Enter to quit...\n");
	getchar();
	mosquitto_loop_stop(mosq, true);

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}
