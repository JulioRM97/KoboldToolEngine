#pragma once

namespace kte
{

/// Defines an object that can be updated.
class Updatable
{
 public:
  virtual void preUpdate() = 0;
  virtual void update() = 0;
  virtual void fixedUpdate() = 0;
  virtual void postUpdate() = 0;
};

/// Default implementation of an Updatable
class DefaultUpdatable : public Updatable
{
 public:
  void preUpdate() override {};
  void update() override {};
  void fixedUpdate() override {};
  void postUpdate() override {};
};

} // namespace kte

