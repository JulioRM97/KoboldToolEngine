#include "kte/system/task_list.h"
#include "kte/common.h"

#include "kte/system/threaded_scheduler.h"

namespace kte
{
namespace system
{

////////////////////////////////////////////////////////////////
TaskList::TaskList(ThreadedScheduler& scheduler)
    : scheduler_(scheduler)
{
  static uint32_t next_id = 1;
  id_ = next_id++;
}

////////////////////////////////////////////////////////////////
void TaskList::schedule(Task& task)
{
  schedule([&]() { task.call(); });
}

////////////////////////////////////////////////////////////////
void TaskList::schedule(const std::function<void()>& task)
{
  std::lock_guard<std::mutex> lock_guard(tasks_lock_);
  tasks_.push_back(task);
  scheduler_.notifyNewTasks(1);
}

////////////////////////////////////////////////////////////////
std::shared_ptr<TaskList> TaskList::sync()
{
  std::lock_guard<std::mutex> lock_guard(run_after_lock_);

  if (run_after_)
  {
    return run_after_;
  }

  run_after_ = std::make_shared<TaskList>(scheduler_);

  return run_after_;
}

////////////////////////////////////////////////////////////////
bool TaskList::runNextTask()
{ $
  std::unique_lock<std::mutex> unique_lock(tasks_lock_);

  if (!tasks_.empty())
  {
    auto task = tasks_.front();
    running_tasks_count_++;
    tasks_.pop_front();

    unique_lock.unlock();

    {
      $
      task();
      running_tasks_count_--;
    }

    unique_lock.lock();
  }

  if (tasks_.empty() && running_tasks_count_ == 0)
  {
    scheduler_.finalize(this->id_);
  }

  return true;
}

////////////////////////////////////////////////////////////////
void TaskList::flush()
{ $
  // Run at least once to be able to finalize the task list.
  do
  {
    runNextTask();
  }
  while (!tasks_.empty());
}

} // end namespace system
} // end namespace kte

