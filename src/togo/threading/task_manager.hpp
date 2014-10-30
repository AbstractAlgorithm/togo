#line 2 "togo/threading/task_manager.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief TaskManager interface.
@ingroup threading
@ingroup task_manager
*/

#pragma once

#include <togo/config.hpp>
#include <togo/types.hpp>
#include <togo/threading/types.hpp>

namespace togo {
namespace task_manager {

/**
	@addtogroup task_manager
	@{
*/

/// Construct an empty TaskWork.
inline TaskWork task_work_empty() {
	return {nullptr, nullptr};
}

/// Add task with no children.
///
/// The task may start immediately.
TaskID add(TaskManager& tm, TaskWork const& work, u16 priority = 0);

/// Add task with a hold on its execution.
TaskID add_hold(TaskManager& tm, TaskWork const& work, u16 priority = 0);

/// Add an empty task with a hold on its completion.
inline TaskID add_hold_empty(TaskManager& tm, u16 priority = 0) {
	return add_hold(tm, task_work_empty(), priority);
}

/// Set task parent.
///
/// To prevent race conditions, this should only be used with a parent
/// that was created with add_hold().
void set_parent(TaskManager& tm, TaskID const child_id, TaskID const parent_id);

/// End the hold on a task.
void end_hold(TaskManager& tm, TaskID const id);

/// Wait for a task to complete.
///
/// This function will execute any available tasks while id is incomplete.
void wait(TaskManager& tm, TaskID const id);

/** @} */ // end of doc-group task_manager

} // namespace task_manager
} // namespace togo
