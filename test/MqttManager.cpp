#include <iostream>
#include <cstring>
#include <unistd.h>
#include <mosquitto.h>
#include <nlohmann/json.hpp>
#include <FAS_EziMOTIONPlusE.h>

#define QOS 2
#define SUB_TOPIC "device/motion/control"
#define PUB_TOPIC "device/motion/result"

using namespace std;
using json = nlohmann::json;

static void onConnect(struct mosquitto *mosq, void *obj, int rc)
{
    printf("ID: %d\n", *(int *)obj);
    if (rc)
    {
        printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, SUB_TOPIC, QOS);
}

static void onMessage(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    // "cmd": 0, # 0: normal, 1: initialize, 2: check connection
    // "device": 0, # 0: X Motor, 1: Y Motor, 2: Z Motor
    // "param: 0"

    printf("New message with topic %s: %s\n", msg->topic, (char *)msg->payload);
}

class MqttManager
{
private:
    int rc;
    int id = 12;
    struct mosquitto *mosq;

public:
    int initMqtt();
    int deinitMqtt();
    int publish(string topic, string message);
};

int MqttManager::initMqtt()
{
    mosquitto_lib_init();
    mosq = mosquitto_new("pubsub-test", true, &id);
    // Disable Nagle Algorithm
    mosquitto_opts_set(mosq, MOSQ_OPT_TCP_NODELAY, (void *)1);
    // Set Connection Callback Function
    mosquitto_connect_callback_set(mosq, onConnect);
    // Set Message Callback Function
    mosquitto_message_callback_set(mosq, onMessage);
    // Connect Mqtt broker
    rc = mosquitto_connect(mosq, "192.168.219.173", 1883, 10);
    if (rc)
    {
        printf("Could not connect to Broker with return code %d\n", rc);
        return -1;
    }
    // Start Mqtt
    mosquitto_loop_start(mosq);
    return 0;
}

int MqttManager::deinitMqtt()
{
    mosquitto_loop_stop(mosq, true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}

int MqttManager::publish(string topic, string message)
{
    return mosquitto_publish(mosq, NULL, topic.c_str(), message.length(), message.c_str(), QOS, false);
}

int main()
{
    MqttManager *mqttManager = new MqttManager();
    mqttManager->initMqtt();
    for (int i = 0; i < 100; i++)
    {
        json jsonObj;
        jsonObj["name"] = "John Doe";
        jsonObj["age"] = 30;
        jsonObj["is_student"] = false;
        jsonObj["address"] = {
            {"street", "123 Main St"},
            {"city", "Anytown"},
            {"zip", "12345"}};
        jsonObj["hobbies"] = {"reading", "traveling", "swimming"};
        if (mqttManager->publish(PUB_TOPIC, jsonObj.dump()) != MOSQ_ERR_SUCCESS)
        {
            printf("error!\n");
        }
        sleep(1);
    }
    mqttManager->deinitMqtt();
    return 0;
}
