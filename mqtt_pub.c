#include <stdio.h>
#include <string.h>
#include <mosquitto.h>

#define QOS 2
#define TOPIC "CHAN"

int main()
{
	int rc;
	struct mosquitto *mosq;
	char payload[128];

	mosquitto_lib_init();

	mosq = mosquitto_new("publisher-test", true, NULL);
	mosquitto_opts_set(mosq, MOSQ_OPT_TCP_NODELAY, 1);

	rc = mosquitto_connect(mosq, "192.168.219.173", 1883, 60);
	if(rc != 0){
		printf("Client could not connect to broker! Error Code: %d\n", rc);
		mosquitto_destroy(mosq);
		return -1;
	}
	printf("We are now connected to the broker!\n");
	mosquitto_loop_start(mosq);
	for(int i=0; i<1000; i++)
	{
		sprintf(payload, "HELLO: %d", i);
		mosquitto_publish(mosq, NULL, TOPIC, strlen(payload), payload, QOS, false);
		usleep(1000*10);
	}
	
	//mosquitto_loop_start(mosq);
	sleep(2);
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);

	mosquitto_lib_cleanup();
	return 0;
}
