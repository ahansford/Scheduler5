/*
 * node-list-test.c
 *
 *  Created on: July, 2015
 *      Author: Alan
 */


#include "..\..\src\cross_compiler_defs.h"
#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"
#include "..\..\src\mocks\RuntimeErrorStub.h"

#include "..\..\src\node_list\node-list.h"
//#include "..\..\src\nodes\nodes-private.h"

/****************************************/
/*** Basic Object and Class Testing ****/
struct       NodeList *      myTest_NodeList  = NULL;
const struct NodeListClass * myNodeListClass_ClassPTR = NULL;
const struct NodeList *      myNodeList_ClassPTR   = NULL;

const struct ListClass * myListClass_ClassPTR = NULL;

struct Node * nodeA = NULL;
struct Node * nodeB = NULL;
struct Node * nodeC = NULL;

#ifndef MIN_LIST_BUFFER_SIZE
#define MIN_LIST_BUFFER_SIZE 4
#endif

void * nodeArray[MIN_LIST_BUFFER_SIZE];
void * otherNodeArray[MIN_LIST_BUFFER_SIZE];

void NodeList_deleteAllItems(void * _listPointer);
int deleteIndex;

/****************************************/
/*********** Start of Testing ***********/
TEST_GROUP(nodeList);

TEST_SETUP(nodeList)
{
	RuntimeErrorStub_Reset();
	//List_init();
	NodeList_init();

	myTest_NodeList          = NULL;
	myNodeListClass_ClassPTR = NodeListClass;
	myNodeList_ClassPTR      = NodeList;

	myListClass_ClassPTR = ListClass;


	//myTest_NodeList = new(NodeList, nodeArray, MIN_LIST_BUFFER_SIZE);
	myTest_NodeList = new(NodeList, nodeArray);

	nodeA = new(Node);
	nodeB = new(Node);
	nodeC = new(Node);

	//setID(myTest_NodeList, ID_UNKNOWN);
	//setState(myTest_NodeList, STATE_UNKNOWN);

	//myNodeList_Class = classOf(myTest_NodeList);
	//myNodeList_ptr  = (void *)Node;
}

TEST_TEAR_DOWN(nodeList)
{
	nodeA = safeDelete(nodeA);
	nodeB = safeDelete(nodeB);
	nodeC = safeDelete(nodeC);

	//removeAllNodes(myTest_NodeList);
	myTest_NodeList = safeDelete(myTest_NodeList);

	RuntimeErrorStub_Reset();
}

/************  NodeList Class Functions  *****************/

TEST(nodeList, create)
{

}

TEST(nodeList, NodeList_ctor_PointsTo_NodeList_ctor)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_ctor,  ((struct Class *)NodeList)->ctor );
}

TEST(nodeList, NodeList_copy_PointsTo_NodeList_copy)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_copy,  ((struct Class *)NodeList)->copy);
}

TEST(nodeList, NodeList_equal_PointsTo_NodeList_equal)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_equal,  ((struct Class *)NodeList)->equal);
}

TEST(nodeList, nodeList_puto_PointsTo_NodeList_puto)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_puto,  ((struct Class *)NodeList)->puto);
}

TEST(nodeList, NodeList_puto_PointsTo_Class_puto)
{
	// this looks like the class of the class NodeList ... which is puto for struct Class
	TEST_ASSERT_EQUAL_PTR(Class_puto,  ((struct Object *)NodeList)->class->puto );
}

TEST(nodeList, PRINT_myTest_NodeList_to_nodeList_puto_txt)
{
	char outputFilename[] = "nodeList_puto.txt";
	FILE * fp = fopen(outputFilename, "a");
	fprintf(fp, "\n** PRINTING: myTest_NodeList in %s at line: %u **\n", __FILE__, __LINE__);
	puto(myTest_NodeList, fp);
	fclose(fp);
}


/************  New NodeListClass method pointers  *****************/

TEST(nodeList, containsNodeID_PointsTo_NodeList_containsNodeID)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_containsNodeID,
			((struct NodeListClass *)myNodeList_ClassPTR)->containsNodeID  );
}

TEST(nodeList, getAvailableNodeID_PointsTo_NodeList_getAvailableNodeID)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_getAvailableNodeID,
			((struct NodeListClass *)myNodeList_ClassPTR)->getAvailableNodeID  );
}

TEST(nodeList, removeAllNodesWithID_PointsTo_NodeList_removeAllNodesWithID)
{
	TEST_ASSERT_EQUAL_PTR(NodeList_removeAllNodesWithID,
			((struct NodeListClass *)myNodeList_ClassPTR)->removeAllNodesWithID  );
}



/*** Unmodified NodeListClass method pointers from ListClass  ***/

TEST(nodeList, NodeListClass_add_PointsTo_List_add)
{
	TEST_ASSERT_EQUAL_PTR(List_add,      ((struct ListClass *)myNodeList_ClassPTR)->add );
}

TEST(nodeList, NodeListClass_take_PointsTo_List_take)
{
	TEST_ASSERT_EQUAL_PTR(List_take,     ((struct ListClass *)myNodeList_ClassPTR)->take );
}

TEST(nodeList, NodeListClass_contains_PointsTo_List_contains)
{
	TEST_ASSERT_EQUAL_PTR(List_contains, ((struct ListClass *)myNodeList_ClassPTR)->contains );
}


// Note Shift to Class pointer
TEST(nodeList, NodeListClass_puto_PointsTo_Class_puto)
{
	TEST_ASSERT_EQUAL_PTR(Class_puto, ((struct Object *)myNodeList_ClassPTR)->class->puto );
}

/************  NodeList object initializations  *****************/

TEST(nodeList, myTest_NodeList_NotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  myTest_NodeList);
}

TEST(nodeList, myTest_NodeList_ConstructorIs_ListClass_ctor)
{
	TEST_ASSERT_NOT_EQUAL(List_add+1,  myListClass_ClassPTR->add);
}

TEST(nodeList, nodeList_ArrayPtrNotNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  nodeArray );
}

TEST(nodeList, sizeof_nodeList_Is4xBufferSize)
{
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE*4,  sizeof(nodeArray) );
}

TEST(nodeList, myTest_NodeList_CountOfItems_ZeroOnCreate)
{
	TEST_ASSERT_EQUAL(0,  myTest_NodeList->_.countOfItems);
}

TEST(nodeList, myTest_NodeList_ArrayDimension_MIN_LIST_BUFFER_SIZE_OnCreate)
{
	TEST_ASSERT_EQUAL(MIN_LIST_BUFFER_SIZE,
			                            myTest_NodeList->_.arrayDimension);
}

/***************************************************/
/********************  add  ************************/

TEST(nodeList, nodeA_NotEqualNullOnCreate)
{
	TEST_ASSERT_NOT_EQUAL(NULL,  nodeA );
}

TEST(nodeList, add_returns_nodeA)
{
	TEST_ASSERT_EQUAL_PTR(nodeA,  add(myTest_NodeList, nodeA));
	nodeA = NULL;
}

TEST(nodeList, NodeList_contains_nodeA)
{
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL_PTR(TRUE,  contains(myTest_NodeList, nodeA));
	nodeA = NULL;
}

TEST(nodeList, NodeList_DoesNotContains_nodeB)
{
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL_PTR(FALSE,  contains(myTest_NodeList, nodeB));
	nodeA = NULL;
}

TEST(nodeList, NodeA_STATE_UNKONWN_onAdd)
{
	void * addedNodePtr = add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL(STATE_UNKNOWN,  getState(addedNodePtr) );
	nodeA = NULL;
}

TEST(nodeList, CanSetNodeA_STATE_ON)
{
	add(myTest_NodeList, nodeA);
	setState(nodeA, STATE_ON);
	TEST_ASSERT_EQUAL(STATE_ON,  getState(nodeA) );
	nodeA = NULL;
}

TEST(nodeList, NodeA_ID_UNKONWN_onAdd)
{
	void * addedNodePtr = add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	TEST_ASSERT_EQUAL(ID_UNKNOWN,  getID(addedNodePtr) );
	nodeA = NULL;
	nodeB = NULL;
}

TEST(nodeList, CanSetNodeA_ID_to34)
{
	void * addedNodePtr = add(myTest_NodeList, nodeA);
	setID(nodeA, 34);
	add(myTest_NodeList, nodeB);
	TEST_ASSERT_EQUAL(34,  getID(addedNodePtr) );
	nodeA = NULL;
	nodeB = NULL;
}

/***************************************************/
/******************  containsNodeID  ***************/

TEST(nodeList, containsNodeID_returns_DOES_NOT_CONTAIN_Create)
{
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, -2) );
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, ID_UNKNOWN) );
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 0) );
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 1) );
}

TEST(nodeList, containsNodeID_DOES_CONTAIN_forKnownIDs)
{
	setID(nodeA, 23);
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 23) );

	setID(nodeA, ID_UNKNOWN);
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, ID_UNKNOWN) );

	setID(nodeA, 1567);
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 1567) );
	nodeA = NULL;
}


/***************************************************/
/**************  getAvailableNodeID  ***************/

TEST(nodeList, getAvailableNodeID_returnsZeroOnCreate)
{
	TEST_ASSERT_EQUAL(0,  getAvailableNodeID(myTest_NodeList) );
}

TEST(nodeList, getAvailableNodeID_returnsNextAvailableIndex)
{
	add(myTest_NodeList, nodeA);
	setID(nodeA, 0);
	TEST_ASSERT_EQUAL(1,  getAvailableNodeID(myTest_NodeList) );
	nodeA = NULL;
}

TEST(nodeList, getAvailableNodeID_returnsZeroIfCurrentNodeIDUnknown)
{
	add(myTest_NodeList, nodeA);
	setID(nodeA, ID_UNKNOWN);
	TEST_ASSERT_EQUAL(0,  getAvailableNodeID(myTest_NodeList) );
	nodeA = NULL;
}

TEST(nodeList, getAvailableNodeID_returnsFirstFreeIndex)
{
	setID(nodeA, 1);
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL(0,  getAvailableNodeID(myTest_NodeList) );
	nodeA = NULL;
}


/***************************************************/
/******************  removeAllNodesWithID  *******************/

TEST(nodeList, removeAllNodesWithID_returnsUnknownIDForEmptyList)
{
	TEST_ASSERT_EQUAL(ID_UNKNOWN,  NodeList_removeAllNodesWithID(myTest_NodeList, ID_UNKNOWN) );
}

TEST(nodeList, removeAllNodesWithID_removesNodeFromNodeList)
{
	setID(nodeA, 1);
	add(myTest_NodeList, nodeA);
	removeAllNodesWithID(myTest_NodeList, 1);
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 1) );
	//nodeA = NULL;
}

TEST(nodeList, removeAllNodesWithID_removesNodeFromMultiNodeList)
{
	setID(nodeA, 0);
	setID(nodeB, 1);
	setID(nodeC, 2);
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 0) );
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 1) );
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 2) );

	NodeList_removeAllNodesWithID(myTest_NodeList, 0);

	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 0) );
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 1) );
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 2) );

	NodeList_removeAllNodesWithID(myTest_NodeList, 1);

	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 0) );
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 1) );
	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 2) );
	//nodeA = NULL;
	//nodeB = NULL;
	nodeC = NULL;
}

TEST(nodeList, removeAllNodesWithID_removesMultipleNodesWithSameID)
{
	setID(nodeA, 0);
	setID(nodeB, 1);
	setID(nodeC, 1);
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);

	NodeList_removeAllNodesWithID(myTest_NodeList, 1);

	TEST_ASSERT_EQUAL(DOES_CONTAIN,  containsNodeID(myTest_NodeList, 0) );
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 1) );
	TEST_ASSERT_EQUAL(DOES_NOT_CONTAIN,  containsNodeID(myTest_NodeList, 2) );
	nodeA = NULL;
	//nodeB = NULL;
	//nodeC = NULL;
}


/***************************************************/
/******************  setNodeID  ********************/

TEST(nodeList, getIncrementReadToRun_returnsNullOnFail)
{
	setRunCount(nodeA, 0);
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL_PTR(NULL,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
}

TEST(nodeList, getIncrementReadToRun_FindsOne)

{
	setRunCount(nodeA, 1);
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL_PTR(nodeA,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
}

TEST(nodeList, getIncrementReadToRun_FindsOnlyOne)

{
	setRunCount(nodeA, 1);
	add(myTest_NodeList, nodeA);
	incrementGetReadyToRun(myTest_NodeList);
	decrementRunCount(nodeA);
	TEST_ASSERT_EQUAL_PTR(NULL,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
}

TEST(nodeList, getIncrementReadToRun_FindsMultiple)
{
	setRunCount(nodeA, 1);
	setRunCount(nodeB, 1);
	setRunCount(nodeC, 1);
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);
	resetIndex(myTest_NodeList);  // nodeA

	TEST_ASSERT_EQUAL_PTR(nodeB,  incrementGetReadyToRun(myTest_NodeList) );
	TEST_ASSERT_EQUAL_PTR(nodeC,  incrementGetReadyToRun(myTest_NodeList) );
	TEST_ASSERT_EQUAL_PTR(nodeA,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
	nodeB = NULL;
	nodeC = NULL;
}

TEST(nodeList, getIncrementReadToRun_searchesFullCountOfItems)
{
	setRunCount(nodeA, 0);
	setRunCount(nodeB, 0);
	setRunCount(nodeC, 1);
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);

	TEST_ASSERT_EQUAL_PTR(nodeC,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
	nodeB = NULL;
	nodeC = NULL;
}

TEST(nodeList, getIncrementReadToRun_wrapsToBeginningOfList)
{
	setRunCount(nodeA, 1);
	setRunCount(nodeB, 0);
	setRunCount(nodeC, 0);
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);
	resetIndex(myTest_NodeList);
	incrementGet(myTest_NodeList);
	incrementGet(myTest_NodeList);


	TEST_ASSERT_EQUAL_PTR(nodeA,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
	nodeB = NULL;
	nodeC = NULL;
}

TEST(nodeList, getIncrementReadToRun_cyclesListIfItemHasMultiple)
{
	setRunCount(nodeA, 5);
	setRunCount(nodeB, 1);
	setRunCount(nodeC, 1);
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);
	resetIndex(myTest_NodeList);  // nodeA

	TEST_ASSERT_EQUAL_PTR(nodeB,  incrementGetReadyToRun(myTest_NodeList) );
	TEST_ASSERT_EQUAL_PTR(nodeC,  incrementGetReadyToRun(myTest_NodeList) );
	TEST_ASSERT_EQUAL_PTR(nodeA,  incrementGetReadyToRun(myTest_NodeList) );
	nodeA = NULL;
	nodeB = NULL;
	nodeC = NULL;
}



//****  NodeList_copy  ********************

TEST(nodeList, copy_NewListEqualsOriginal)
{
	// create new node, set node values
	struct Node * otherNodeA = new(Node);
	setID(otherNodeA, 45);
	setState(otherNodeA, STATE_ON);

	// create new list, add new node to new list
	struct NodeList * otherNodeList = new(NodeList, otherNodeArray);
	add(otherNodeList, otherNodeA);

	// copy List
	copy(myTest_NodeList, otherNodeList);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_NodeList, otherNodeList));
	removeAllNodes(otherNodeList);
	otherNodeList = safeDelete(otherNodeList);
}

TEST(nodeList, copy_ReturnsNewListOnSuccess)
{
	// create new node
	struct Node * otherNodeA = new(Node);

	// create new list, add new node to new list
	struct NodeList * otherNodeList = new(NodeList, otherNodeArray);
	add(otherNodeList, otherNodeA);

	TEST_ASSERT_EQUAL(myTest_NodeList, copy(myTest_NodeList, otherNodeList));
	removeAllNodes(otherNodeList);
	delete(otherNodeList);
	//otherNodeList = safeDelete(otherNodeList);
}

TEST(nodeList, copy_returnsNullOnNullCopyToList)
{
	// create new node
	struct Node * otherNodeA = new(Node);

	// create new list, add new node to new list
	struct NodeList * otherNodeList = new(NodeList, otherNodeArray);
	add(otherNodeList, otherNodeA);

	TEST_ASSERT_EQUAL(NULL, copy(NULL, otherNodeList));
	removeAllNodes(otherNodeList);
	otherNodeList = safeDelete(otherNodeList);
}

TEST(nodeList, copy_returnsNullOnNullCopyFromNodeList)
{
	TEST_ASSERT_EQUAL(NULL, copy(myTest_NodeList, NULL));
}

//****  NodeList_equal  ********************
/**/
TEST(nodeList, equal_EmptyNodeList_NotEqualTo_Null)
{
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(myTest_NodeList, NULL));
	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(NULL, myTest_NodeList));
}

TEST(nodeList, equal_myTest_NodeList_NotequalTo_NullList)
{
	struct NodeList * other_NodeList = new(NodeList, otherNodeArray);
	add(myTest_NodeList, nodeA);

	TEST_ASSERT_EQUAL(OBJECT_UNEQUAL, equal(other_NodeList, myTest_NodeList));
	nodeA = NULL;
	other_NodeList = safeDelete(other_NodeList);
}

TEST(nodeList, equal_EmptyLists_ReturnEqual)
{
	struct NodeList * other_NodeList = new(NodeList, otherNodeArray);
	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(other_NodeList, myTest_NodeList));
	other_NodeList = safeDelete(other_NodeList);
}

TEST(nodeList, equal_equalNodeListsReturnEqual)
{
	//  create new node list and add nodes
	struct NodeList * other_NodeList = new(NodeList, otherNodeArray);
	add(other_NodeList, nodeA);
	add(other_NodeList, nodeB);
	add(other_NodeList, nodeC);

	// copy list
	copy(myTest_NodeList, other_NodeList);

	TEST_ASSERT_EQUAL(OBJECT_EQUAL, equal(myTest_NodeList, other_NodeList));
	nodeA = NULL;
	nodeB = NULL;
	nodeC = NULL;
	other_NodeList = safeDelete(other_NodeList);

}

/***********  nodeConfig  **************/
/*
TEST(nodeList, nodeConfig_SelfSelf_returnsUnknownID)
{
	struct Node * master = new(Node);
	setID(master, 2);
	add(myTest_NodeList, master);
	TEST_ASSERT_EQUAL(ID_UNKNOWN, nodeConfig(myTest_NodeList, 2, 2));
}

TEST(nodeList, nodeConfig_CopiesMasterDataToNewNode)
{
	struct Node * master = new(Node);
	id_t masterID = 101;
	setID   (master, masterID);
	setType (master, 1223);
	setState(master, STATE_ON);
	setValue(master, 5632);
	add(myTest_NodeList, master);

	id_t targetNodeID = 77;
	setID(nodeA, targetNodeID);
	add(myTest_NodeList, nodeA);

	NodeList_nodeConfig(myTest_NodeList, targetNodeID, masterID);

	TEST_ASSERT_EQUAL(77,       getID   (nodeA));
	TEST_ASSERT_EQUAL(1223,     getType (nodeA));
	TEST_ASSERT_EQUAL(STATE_ON, getState(nodeA));
	TEST_ASSERT_EQUAL(5632,     getValue(nodeA));
}

TEST(nodeList, nodeConfig_AssignsNextAvailableID_IfMasterID_isUnknown)
{
	struct Node * master = new(Node);
	setID   (master, 0);
	add(myTest_NodeList, master);

	add(myTest_NodeList, nodeA);

	TEST_ASSERT_EQUAL(1, nodeConfig(myTest_NodeList, ID_UNKNOWN, 0));
}

TEST(nodeList, nodeConfig_AssignsMultipleIndexesToMultipleUnknownIDNodes)
{
	struct Node * master = new(Node);
	setID   (master, 0);
	add(myTest_NodeList, master);

	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);

	TEST_ASSERT_EQUAL(1, nodeConfig(myTest_NodeList, ID_UNKNOWN, 0));
	TEST_ASSERT_EQUAL(2, nodeConfig(myTest_NodeList, ID_UNKNOWN, 0));
	TEST_ASSERT_EQUAL(3, nodeConfig(myTest_NodeList, ID_UNKNOWN, 0));
}

TEST(nodeList, nodeConfig_ReturnOriginalID_onSuccess)
{
	struct Node * master = new(Node);
	setID(master, 4);
	add(myTest_NodeList, master);

	setID(nodeA, 6);
	add(myTest_NodeList, nodeA);

	TEST_ASSERT_EQUAL(6, nodeConfig(myTest_NodeList, 6, 4));
}
*/
/**************************************/
TEST(nodeList, ClassPointersUnchangedAfterMultiInitCalls)
{
	const void * originalListNode = NodeList;
	const void * originalNodeListClass = NodeListClass;
	Node_init();
	Node_init();
	Node_init();
	TEST_ASSERT_EQUAL_PTR(NodeList,      originalListNode);
	TEST_ASSERT_EQUAL_PTR(NodeListClass, originalNodeListClass);
}

TEST(nodeList, memory_leak)
{
	// new structure location will move based on other processes
	// create and destroy a large number of objects
	// then look for the base address within an additional 10 new objects

	struct NodeList * new_NodeList = new(NodeList, otherNodeArray);
	struct Node *     new_NodeItem = new(Node);
	struct Node * originalNodePointer = new_NodeItem;

	if ( add(new_NodeList, new_NodeItem) == NULL ) { printf("ERROR1: failed to add node\n"); }
	TEST_ASSERT_EQUAL(1, getItemCount(new_NodeList));
	TEST_ASSERT_EQUAL_PTR(new_NodeList, cast(NodeList, new_NodeList) );

	removeAllNodes(new_NodeList);
	TEST_ASSERT_EQUAL(0, getItemCount(new_NodeList));
	new_NodeList = safeDelete(new_NodeList);
	new_NodeList = new(NodeList, otherNodeArray);

	// create and destroy a large number of objects to cause a memory leak
	// TODO:  does not like creating and destroying nodelists in the range of 898 or above
	int j;
	for ( j = 0; j < 1000; j++ ) {

		//new_NodeList = new(NodeList, otherNodeArray);
		new_NodeItem = new(Node);
		if ( add(new_NodeList, new_NodeItem) == NULL ) { printf("ERROR3: failed to add node\n"); }
		//printf("new_NodeItem: %p, j: %i\n", new_NodeItem, j);

		removeAllNodes(new_NodeList);
		//new_NodeList = safeDelete(new_NodeList);
	}

	//printf("\nOriginal new_NodeItem target: %p\n", originalNodePointer);

	// check for original object location within a small number of additional objects
	int originalPtrCount = 0;
	for ( j = 0; j < 100; j++ ) {

		//new_NodeList = new(NodeList, otherNodeArray);
		new_NodeItem = new(Node);
		if ( add(new_NodeList, new_NodeItem) == NULL ) { printf("ERROR4: failed to add node\n"); }

		//printf("new_NodeItem: %p, j: %i\n",new_NodeItem, j);

		if ( originalNodePointer  == new_NodeItem )  { originalPtrCount++; }

		removeAllNodes(new_NodeList);
		TEST_ASSERT_EQUAL(0, getItemCount(new_NodeList));
		//new_NodeList = safeDelete(new_NodeList);

		}

	printf("\nNodeList-memory-leak-test, originalPtrCount: %i, out of %i trials\n", originalPtrCount, 100);
	TEST_ASSERT_TRUE(originalPtrCount);
	new_NodeList = safeDelete(new_NodeList);
}

/****************************************/
/****************************************/
/****************************************/
/*** Start of Initialization Testing ****/


/***************************************************/
/***************  delete NodeList  *****************/

// WARNING:  If a delete() is added to the tear down,
// then these tests should be moved to their own runner
// to avoid calling delete twice.


TEST_GROUP(nodeLists_init);

TEST_SETUP(nodeLists_init)
{
	RuntimeErrorStub_Reset();
	//List_init();
	NodeList_init();
	myTest_NodeList = new(NodeList, nodeArray);

	nodeA = new(Node);
	nodeB = new(Node);
	nodeC = new(Node);

}

TEST_TEAR_DOWN(nodeLists_init)
{
	nodeA = safeDelete(nodeA);
	nodeB = safeDelete(nodeB);
	nodeC = safeDelete(nodeC);

	//removeAllNodes(myTest_NodeList);
	myTest_NodeList = safeDelete(myTest_NodeList);

	RuntimeErrorStub_Reset();
}

TEST(nodeLists_init, create)
{

}

TEST(nodeLists_init, delete_returns_myTest_NodeList_OnEmptyList)
{
	TEST_ASSERT_EQUAL_PTR(myTest_NodeList,  delete(myTest_NodeList) );
	myTest_NodeList = NULL;
}

TEST(nodeLists_init, delete_returns_myTest_NodeList_OnSingleItemList)
{
	add(myTest_NodeList, nodeA);
	TEST_ASSERT_EQUAL_PTR(myTest_NodeList,  delete(myTest_NodeList) );
	nodeA = NULL;
	myTest_NodeList = NULL;
}

TEST(nodeLists_init, delete_returns_myTest_NodeList_OnMultiItemList)
{
	add(myTest_NodeList, nodeA);
	add(myTest_NodeList, nodeB);
	add(myTest_NodeList, nodeC);

	TEST_ASSERT_EQUAL_PTR(myTest_NodeList,  delete(myTest_NodeList) );
	nodeA = NULL;
	nodeB = NULL;
	nodeC = NULL;
	myTest_NodeList = NULL;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void NodeList_deleteAllItems(void * _listPointer)
{
	void * localPTR;
	int localItemCount = getItemCount(_listPointer);
	for ( deleteIndex = 0; deleteIndex < localItemCount; deleteIndex++) {
		// WARNING:  deleting non-objects is not allowed
		localPTR = take(_listPointer);
		//printf("deleting pointer: %p\n", localPTR);
		delete(localPTR);
	}
	return;
}
