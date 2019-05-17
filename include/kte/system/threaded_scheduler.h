#pragma once

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <list>
#include <thread>
#include <vector>

#include "kte/system/task_list.h"

namespace kte
{
namespace system
{

/// \summary A multi threaded scheduler that can handle task synchronization.
///
/// Task are submitted to be executed on the main thread or on a pool of worker
/// threads.
class ThreadedScheduler
{
 public:
  /// Construct a new ThreadedScheduler.
  ThreadedScheduler();

  /// Increases the number of consumers to be at least the number passed.
  /// This method should be called from the main thread.
  void startConsumers(uint32_t how_many);

 private:
  /// Start one consumer on the current thread.
  void startConsumer();

 public:
  /// \summary Schedule a task to be executed on a thread.
  ///
  /// \param task The task to be executed.
  /// \param main_thread true if should be executed on the main thread.
  ThreadedScheduler& schedule(Task& task, bool main_thread = false);

  /// \summary Schedule a task to be executed.
  ///
  /// \param task The task to be executed.
  /// \param main_thread true if should be executed on the main thread.
  ThreadedScheduler& schedule(const std::function<void()>& task, bool main_thread = false);

  /// Syncs the execution to this point in time.
  void sync();

  /// Removes a task list from the queue and adds the "run after" task list if
  /// available.
  void finalize(uint32_t task_list_id);

  /// Notify the scheduler that there are new tasks
  void notifyNewTasks(uint32_t count);

  /// Execute the tasks for the main thread and wait for the tasks .
  void flush();

 private:
  /// Wait for all task lists to finish.
  void wait();

 public:
  /// Stops all the consumers.
  void stopConsumers();

 private:
  void runAllTasks();

  inline bool is_main_thread() const;

  bool running_ = false;

  std::thread::id main_thread_id_;

  // Worker threads

  std::condition_variable read_cv_;

  std::vector<std::thread> read_threads_;

  // Main thread lists

  std::shared_ptr<TaskList> main_thread_task_list_;

  // Worker thread lists

  std::mutex active_task_lists_lock_;

  std::list<std::shared_ptr<TaskList>> active_task_lists_;

};

} // end namespace system
} // end namespace kte

