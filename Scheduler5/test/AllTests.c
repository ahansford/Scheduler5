/*
 * AllTests.c
 *
 *  Created on: Oct 11, 2014
 *      Author: Alan
 */

#include "unity\unity_fixture.h"
#include "..\src\objects\objects.h"
#include "..\src\lists\lists.h"
#include "..\src\stacks\stacks.h"
#include "..\src\nodes\nodes.h"
#include "..\src\nodes_X10\nodes-X10.h"
#include "..\src\events\events.h"
#include "..\src\event_controller\event-controller.h"
#include "..\src\button\button.h"
#include "..\src\sensors\sensors.h"
#include "..\src\io\io.h"
#include "..\src\access_mem\access-mem.h"

#ifndef MIN_LIST_BUFFER_SIZE
#define MIN_LIST_BUFFER_SIZE 4
#endif

void * allTestArray[MIN_LIST_BUFFER_SIZE];

static void RunAllTests(void)
{
	RUN_TEST_GROUP(scheduler);
	RUN_TEST_GROUP(objects);
	RUN_TEST_GROUP(points);
	RUN_TEST_GROUP(lists_init);
	RUN_TEST_GROUP(lists);
	RUN_TEST_GROUP(stacks_init);
	RUN_TEST_GROUP(stacks);
	RUN_TEST_GROUP(nodes);
	RUN_TEST_GROUP(nodesX10);
	RUN_TEST_GROUP(nodeList);
	RUN_TEST_GROUP(nodeLists_init);
	RUN_TEST_GROUP(times);
	RUN_TEST_GROUP(events);
	RUN_TEST_GROUP(eventController); // come back to check this one
	RUN_TEST_GROUP(button);
	//RUN_TEST_GROUP(sensor);
	//RUN_TEST_GROUP(io);
	RUN_TEST_GROUP(accessMem);

}


int clearFile(char * _fileName);

int main(int argc, char* argv[])
{
	Node_init();
	Time_init();
	Event_init();
	List_init();
	NodeList_init();
	EventController_init(UPDATE_SECONDS);
	Stack_init();
	Button_init(100);
	Sensor_init();
	Node_X10_init();
	IO_init(NULL);
	Access_init();

	clearFile("scheduler_puto.txt");
	clearFile("object_puto.txt");
	clearFile("scheduler_puto.txt");
	clearFile("nodes_puto.txt");
	clearFile("nodes_X10_puto.txt");
	clearFile("nodeList_puto.txt");
	clearFile("list_puto.txt");
	clearFile("times_puto.txt");
	clearFile("events_puto.txt");
	clearFile("button_puto.txt");
	clearFile("sensor_puto.txt");
	clearFile("access_mem_puto.txt");

	return UnityMain(argc, argv, RunAllTests);
}

int clearFile(char * _fileName)
{
	FILE *data;
	//data = fopen("node_list_puto.txt", "w");
	data = fopen(_fileName, "w");
	if(!data) {
		printf("Error opening file %s in main\n", _fileName);
		return 1;
	}
	fprintf(data, " ");
	fclose(data);
	return 0;
}

