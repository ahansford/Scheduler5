/*
 * node-list-test-runner.c
 *
 *  Created on: July, 2015
 *      Author: Alan
 */


#include "..\unity\unity_fixture.h"
//#include "..\unity\unity.h"

TEST_GROUP_RUNNER(nodeLists_init)
{
	/***************************************************/
	/***************  delete NodeList  *****************/
	/**/
	RUN_TEST_CASE(nodeLists_init, create);
	RUN_TEST_CASE(nodeLists_init, delete_returns_myTest_NodeList_OnEmptyList);
	RUN_TEST_CASE(nodeLists_init, delete_returns_myTest_NodeList_OnSingleItemList);
	RUN_TEST_CASE(nodeLists_init, delete_returns_myTest_NodeList_OnMultiItemList);

	}


TEST_GROUP_RUNNER(nodeList)
{
	/************  NodeList Class Functions  *****************/

	RUN_TEST_CASE(nodeList, create);
	/**/
	RUN_TEST_CASE(nodeList, NodeList_ctor_PointsTo_NodeList_ctor);
	RUN_TEST_CASE(nodeList, NodeList_copy_PointsTo_NodeList_copy);
	RUN_TEST_CASE(nodeList, NodeList_equal_PointsTo_NodeList_equal);
	RUN_TEST_CASE(nodeList, nodeList_puto_PointsTo_NodeList_puto);
	RUN_TEST_CASE(nodeList, NodeList_puto_PointsTo_Class_puto);
	RUN_TEST_CASE(nodeList, PRINT_myTest_NodeList_to_nodeList_puto_txt);


	/************  New NodeListClass method pointers  *****************/
	/**/
	RUN_TEST_CASE(nodeList, containsNodeID_PointsTo_NodeList_containsNodeID);
	RUN_TEST_CASE(nodeList, getAvailableNodeID_PointsTo_NodeList_getAvailableNodeID);
	RUN_TEST_CASE(nodeList, removeAllNodesWithID_PointsTo_NodeList_removeAllNodesWithID);


	/*** Unmodified NodeListClass method pointers from ListClass  ***/
	/**/
	RUN_TEST_CASE(nodeList, NodeListClass_add_PointsTo_List_add);
	RUN_TEST_CASE(nodeList, NodeListClass_take_PointsTo_List_take);
	RUN_TEST_CASE(nodeList, NodeListClass_contains_PointsTo_List_contains);
	RUN_TEST_CASE(nodeList, NodeListClass_puto_PointsTo_Class_puto);

	/************  NodeList object initializations  *****************/
	/**/
	RUN_TEST_CASE(nodeList, myTest_NodeList_NotNullOnCreate);
	RUN_TEST_CASE(nodeList, myTest_NodeList_ConstructorIs_ListClass_ctor);
	RUN_TEST_CASE(nodeList, nodeList_ArrayPtrNotNullOnCreate);
	RUN_TEST_CASE(nodeList, sizeof_nodeList_Is4xBufferSize);
	RUN_TEST_CASE(nodeList, myTest_NodeList_CountOfItems_ZeroOnCreate);
	RUN_TEST_CASE(nodeList, myTest_NodeList_ArrayDimension_MIN_LIST_BUFFER_SIZE_OnCreate);

	/***************************************************/
	/********************  add  ************************/
	/**/
	RUN_TEST_CASE(nodeList, nodeA_NotEqualNullOnCreate);

	RUN_TEST_CASE(nodeList, add_returns_nodeA);
	RUN_TEST_CASE(nodeList, NodeList_contains_nodeA);
	RUN_TEST_CASE(nodeList, NodeList_DoesNotContains_nodeB);
	RUN_TEST_CASE(nodeList, NodeA_STATE_UNKONWN_onAdd);
	RUN_TEST_CASE(nodeList, CanSetNodeA_STATE_ON);
	RUN_TEST_CASE(nodeList, NodeA_ID_UNKONWN_onAdd);
	RUN_TEST_CASE(nodeList, CanSetNodeA_ID_to34);

	/***************************************************/
	/******************  containsNodeID  ***************/
	/**/
	RUN_TEST_CASE(nodeList, containsNodeID_returns_DOES_NOT_CONTAIN_Create);
	RUN_TEST_CASE(nodeList, containsNodeID_DOES_CONTAIN_forKnownIDs);

	/***************************************************/
	/**************  getAvailableNodeID  ***************/
	/**/
	RUN_TEST_CASE(nodeList, getAvailableNodeID_returnsZeroOnCreate);
	RUN_TEST_CASE(nodeList, getAvailableNodeID_returnsNextAvailableIndex);
	RUN_TEST_CASE(nodeList, getAvailableNodeID_returnsZeroIfCurrentNodeIDUnknown);
	RUN_TEST_CASE(nodeList, getAvailableNodeID_returnsFirstFreeIndex);

	/***************************************************/
	/**************  removeAllNodesWithID  *************/
	/**/
	RUN_TEST_CASE(nodeList, removeAllNodesWithID_returnsUnknownIDForEmptyList);
	RUN_TEST_CASE(nodeList, removeAllNodesWithID_removesNodeFromNodeList);
	RUN_TEST_CASE(nodeList, removeAllNodesWithID_removesNodeFromMultiNodeList);
	RUN_TEST_CASE(nodeList, removeAllNodesWithID_removesMultipleNodesWithSameID);

	/***************************************************/
	/******************  IncrementReadToRun  ********************/
	/**/
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_returnsNullOnFail);
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_FindsOne);
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_FindsOnlyOne);
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_FindsMultiple);
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_searchesFullCountOfItems);
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_wrapsToBeginningOfList);
	RUN_TEST_CASE(nodeList, getIncrementReadToRun_cyclesListIfItemHasMultiple);


	/***************************************************/
	//***************  NodeList_copy  ******************/
	/**/
	RUN_TEST_CASE(nodeList, copy_NewListEqualsOriginal);
	RUN_TEST_CASE(nodeList, copy_ReturnsNewListOnSuccess);
	RUN_TEST_CASE(nodeList, copy_returnsNullOnNullCopyToList);
	RUN_TEST_CASE(nodeList, copy_returnsNullOnNullCopyFromNodeList);


	/***************************************************/
	//************  NodeList_equal  ********************/
	/**/
	RUN_TEST_CASE(nodeList, equal_EmptyNodeList_NotEqualTo_Null);
	RUN_TEST_CASE(nodeList, equal_myTest_NodeList_NotequalTo_NullList);
	RUN_TEST_CASE(nodeList, equal_EmptyLists_ReturnEqual);
	RUN_TEST_CASE(nodeList, equal_equalNodeListsReturnEqual);


	/***********  NodeList_config  **************/
	/**/
			//RUN_TEST_CASE(nodeList, nodeConfig_SelfSelf_returnsUnknownID);
			//RUN_TEST_CASE(nodeList, nodeConfig_CopiesMasterDataToNewNode);
			//RUN_TEST_CASE(nodeList, nodeConfig_AssignsNextAvailableID_IfMasterID_isUnknown);
			//RUN_TEST_CASE(nodeList, nodeConfig_AssignsMultipleIndexesToMultipleUnknownIDNodes);
			//RUN_TEST_CASE(nodeList, nodeConfig_ReturnOriginalID_onSuccess);


	RUN_TEST_CASE(nodeList, ClassPointersUnchangedAfterMultiInitCalls);
	//RUN_TEST_CASE(nodeList, memory_leak);

}

