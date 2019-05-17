#include "kte/system/threaded_scheduler.h"
#include "kte/common.h"

#include <algorithm>
#include <cassert>

namespace kte
{
namespace system
{

////////////////////////////////////////////////////////////////
/// Used to access the current task list for threads. Not to be used for the
/// main thread task list.
thread_local TaskList* gCurrentTaskList;

////////////////////////////////////////////////////////////////
ThreadedScheduler::ThreadedScheduler()
    : main_thread_task_list_(new TaskList(*this))
{
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::startConsumers(uint32_t how_many)
{
  running_ = true;
  main_thread_id_ = std::this_thread::get_id();
  MTR_META_THREAD_NAME("main thread");

  read_threads_.reserve(how_many);
  for (auto i = read_threads_.size(); i < how_many; i++)
  {
    read_threads_.emplace_back(&ThreadedScheduler::startConsumer, this);
  }
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::startConsumer()
{
  while (running_)
  {
    std::unique_lock<std::mutex> unique_lock(active_task_lists_lock_);

    while (active_task_lists_.empty())
    {
      read_cv_.wait(unique_lock);
      if (!running_) { return; }
    }

    auto task_list = active_task_lists_.front();

    // Get next list
    if (active_task_lists_.size() > 1)
    {
      active_task_lists_.pop_front();
      active_task_lists_.push_back(task_list);
      read_cv_.notify_all();
    }

    unique_lock.unlock();

    gCurrentTaskList = task_list.get();
    task_list->runNextTask();
    gCurrentTaskList = nullptr;
  }
}

////////////////////////////////////////////////////////////////
ThreadedScheduler& ThreadedScheduler::schedule(Task& task, bool main_thread)
{
  return schedule([&]() { task.call(); }, main_thread);
}

////////////////////////////////////////////////////////////////
ThreadedScheduler& ThreadedScheduler::schedule(const std::function<void()>& task,
                                 bool main_thread)
{
  if (!running_)
  {
    printf("Scheduler not running, cannot schedule task!\n");
    return *this;
  }

  if (main_thread)
  {
    main_thread_task_list_->schedule(task);
  }
  else
  {
    if (gCurrentTaskList)
    {
      gCurrentTaskList->schedule(task);
    }
    else
    {
      auto task_list = std::make_shared<TaskList>(*this);
      task_list->schedule(task);

      std::lock_guard<std::mutex> lock_guard(active_task_lists_lock_);
      active_task_lists_.push_back(task_list);
      read_cv_.notify_one();
    }
  }

  return *this;
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::sync()
{
  assert(gCurrentTaskList != nullptr);

  if (!running_)
  {
    printf("Scheduler not running, cannot sync!\n");
    return;
  }

  gCurrentTaskList = gCurrentTaskList->sync().get();
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::finalize(uint32_t task_list_id)
{
  std::lock_guard<std::mutex> lock_guard(active_task_lists_lock_);

  auto task_list_itr = std::find_if(active_task_lists_.begin(),
                                    active_task_lists_.end(),
                                    [&](const std::shared_ptr<TaskList>& list) {
                                      return list->id_ == task_list_id;
                                    });

  if (running_ && task_list_itr != active_task_lists_.end())
  {
    auto& task_list = *task_list_itr;

    if (task_list->run_after_)
    {
      active_task_lists_.push_back(task_list->run_after_);
      task_list->run_after_ = nullptr;
      read_cv_.notify_all();
    }

    active_task_lists_.remove(task_list);
  }
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::notifyNewTasks(uint32_t count)
{
  if (count > 1)
  {
    read_cv_.notify_all();
  }
  else if (count == 1)
  {
    read_cv_.notify_one();
  }
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::flush()
{ $
  assert(is_main_thread());

  main_thread_task_list_->flush();

  wait();
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::wait()
{ $
  assert(is_main_thread());

  if (read_threads_.empty())
  {
    // No thread workers, run the tasks in the current thread
    runAllTasks();
  }
  else
  {
    // Wait for the thread workers to empty the task list
    while (!active_task_lists_.empty())
    {
      read_cv_.notify_all();
      std::this_thread::yield();
    }
  }
}

////////////////////////////////////////////////////////////////
void ThreadedScheduler::stopConsumers()
{ $
  assert(is_main_thread());

  running_ = false;

  read_cv_.notify_all();
  for (auto& thread : read_threads_)
  {
    thread.join();
  }
}


////////////////////////////////////////////////////////////////
void ThreadedScheduler::runAllTasks()
{ $
  assert(is_main_thread());
  assert(read_threads_.empty());

  while (!active_task_lists_.empty())
  {
    gCurrentTaskList = active_task_lists_.front().get();
    gCurrentTaskList->flush();
    gCurrentTaskList = nullptr;
  }
}

////////////////////////////////////////////////////////////////
bool ThreadedScheduler::is_main_thread() const
{
  return std::this_thread::get_id() == main_thread_id_;
}

} // end namespace system
} // end namespace kte

