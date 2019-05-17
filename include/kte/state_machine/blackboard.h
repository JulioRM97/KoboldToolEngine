#pragma once

#include <map>
#include <string>

namespace kte
{

class Blackboard
{
 public:
  Blackboard();

  virtual ~Blackboard();

  template<typename T>
  bool addValue(std::string key, T value, bool overwrite = false);

  template<typename T>
  T getValueAsFloat(std::string key);

 private:
  std::map<std::string, std::string> values_;
};

} // namespace kte


