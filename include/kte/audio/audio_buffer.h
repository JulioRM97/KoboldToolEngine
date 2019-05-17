#pragma once

#include <memory>
#include <string>

namespace kte
{

/// Contains audio data.
class AudioBuffer
{
 public:
  AudioBuffer();

  ~AudioBuffer();

  /// \param filename Load this buffer data from this file.
  void loadFromFile(std::string filename);

  void* data() const;

  struct Data;

 private:
  std::unique_ptr<Data> data_;
};

}

