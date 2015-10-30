/*
 * stacks-test-runner.c
 *
 *  Created on: April 28, 2015
 *      Author: Alan
 */

#include "..\unity\unity_fixture.h"
#include "..\unity\unity.h"

TEST_GROUP_RUNNER(stacks_init)
{
	/**/


}

TEST_GROUP_RUNNER(stacks)
{
	/**/
	RUN_TEST_CASE(stacks, Create);
	RUN_TEST_CASE(stacks, StackClass_is_registered_after_Stack_init);
	RUN_TEST_CASE(stacks, Stack_is_registered_after_Stack_init);
	RUN_TEST_CASE(stacks, Add_returns_self_pointer);
	RUN_TEST_CASE(stacks, take_returns_added_pointer);
	RUN_TEST_CASE(stacks, Can_add_four_items);
	RUN_TEST_CASE(stacks, Can_take_four_items_in_LIFO_order);
	RUN_TEST_CASE(stacks, Taking_item_with_count_zero_returns_NULL);
	RUN_TEST_CASE(stacks, Taking_extra_item_returns_NULL);
	RUN_TEST_CASE(stacks, Add_and_take_ten_items_in_LIFO_order);
	RUN_TEST_CASE(stacks, contain_does_not_reorder_stack);
	RUN_TEST_CASE(stacks, Multiple_calls_to_Stack_init_do_not_change_ptr);
	RUN_TEST_CASE(stacks, New_createsAnEmptyStack);

}
