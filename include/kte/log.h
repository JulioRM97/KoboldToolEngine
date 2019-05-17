#pragma once

#include <memory>
#include <string>
#include <thread>

namespace kte
{

struct Logger
{
  Logger();
  ~Logger();

  void log(const std::string& message);

 private:
  void output();

  struct Data;

  std::unique_ptr<Data> data_;
};

}

