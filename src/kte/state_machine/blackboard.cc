#include "kte/state_machine/blackboard.h"

namespace kte
{

////////////////////////////////////////////////////////////////
Blackboard::Blackboard()
{

}

////////////////////////////////////////////////////////////////
Blackboard::~Blackboard()
{

}

////////////////////////////////////////////////////////////////
template<typename T>
bool Blackboard::addValue(std::string key, T value, bool overwrite)
{
  if(values_.find(key) == values_.end() && !overwrite)
  {
    return false;
  }

  values_[key] = std::string((const char*) value);
  return true;
}

////////////////////////////////////////////////////////////////
template<typename T>
T Blackboard::getValueAsFloat(std::string key)
{
  std::string value;
  if(values_.find(key) == values_.end())
  {
    return T(0);
  }
  value =   values_[key];
  return (T)value.c_str();
}

}