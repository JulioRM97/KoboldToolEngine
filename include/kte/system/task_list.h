#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>

#include "task.h"

namespace kte
{
namespace system
{

class ThreadedScheduler;

class TaskList
{
 public:
  /// Creates a new TaskList
  explicit TaskList(ThreadedScheduler& scheduler);

  TaskList(const TaskList& other) = delete;

  TaskList(TaskList&& other) noexcept = delete;

  /// \summary Schedule a task to be executed on a thread.
  ///
  /// \param task The task to be executed.
  void schedule(Task& task);

  /// \summary Schedule a task to be executed.
  ///
  /// \param task The task to be executed.
  void schedule(const std::function<void()>& task);

  /// \summary This method helps synchronizing the execution of multiple
  /// threads.
  ///
  /// \returns a TaskList that will be executed after this list is completed
  ///
  /// \related ThreadedScheduler::flush
  /// \related ThreadedScheduler::wait
  std::shared_ptr<TaskList> sync();

  /// Run the next task in the list
  bool runNextTask();

  /// Run all tasks in this list and then the lists in run_after_
  void flush();

  uint32_t id_;

  std::shared_ptr<TaskList> run_after_;

 private:
  friend class ThreadedScheduler;

  ThreadedScheduler& scheduler_;

  std::mutex tasks_lock_;

  std::deque<std::function<void()>> tasks_;

  std::atomic_uint running_tasks_count_{0};

  std::mutex run_after_lock_;

};

} // end namespace system
} // end namespace kte

