#pragma once

#include <memory>

namespace kte
{
struct ModelData;
}

template<typename T>
class gltf_shared_ptr
{
 public:
  gltf_shared_ptr() = default;

  explicit gltf_shared_ptr(T* t,
                           std::shared_ptr<kte::ModelData> model = nullptr)
      : t_(t),
        model_data_(model)
  {
    create();
  }

  ~gltf_shared_ptr()
  {
    destroy();
  }

  gltf_shared_ptr(const gltf_shared_ptr& other)
  {
    t_ = other.t_;
    model_data_ = other.model_data_;
    count_ = other.count_;

    (*count_)++;
  }

  T& operator *()
  {
    return *t_;
  }

  T* operator ->()
  {
    return t_;
  }

  explicit operator bool() const
  {
    return t_ != nullptr;
  }

  bool operator !() const
  {
    return !t_;
  }

  T* get()
  {
    return t_;
  }

  void set(T* t, std::shared_ptr<kte::ModelData> model = nullptr)
  {
    destroy();

    t_ = t;
    model_data_ = model;

    create();
  }

  template<typename... Args>
  void alloc(Args&& ... args)
  {
    set(new T(std::forward<Args>(args)...));
  }

  std::shared_ptr<kte::ModelData> model_data() const
  {
    return model_data_;
  }

 protected:
  void create()
  {
    if (!count_)
    {
      count_ = new uint32_t(1);
    }
  }

  void destroy()
  {
    if (!count_)
    {
      return;
    }

    (*count_)--;

    if (*count_ == 0)
    {
      delete count_;

      if (!model_data_ && t_)
      {
        delete t_;
      }
    }

    t_ = nullptr;
    model_data_ = nullptr;
    count_ = nullptr;
  }

  T* t_ = nullptr;

  std::shared_ptr<kte::ModelData> model_data_;

  uint32_t* count_ = nullptr;
};

template<typename T>
gltf_shared_ptr<T> make_gltf_shared_ptr(T* t,
                                        std::shared_ptr<kte::ModelData> model = nullptr)
{
  return gltf_shared_ptr<T>(t, model);
};

