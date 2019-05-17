#pragma once

#include "kte/game/updatable.h"

#include <memory>
#include <list>
#include <unordered_map>

#include "kte/audio/audio_buffer.h"
#include "kte/audio/audio_listener.h"

namespace kte
{

class AudioSource;

/// Holds information about fading in and out of a source
struct Fade
{
  std::shared_ptr<AudioSource> source;

  float target_gain = 1.0f;

  float speed = 1.0f;
};

/// The main audio manager. Handles fading sounds in and out as well as
/// crossfading in tracks.
class AudioManager : public DefaultUpdatable
{
 public:
  AudioManager();

  ~AudioManager();

  /// Gets a buffer by filename. Loads from disk if not previously loaded.
  const AudioBuffer& getBuffer(std::string filename);

  /// Updates all the running sounds, applying fades.
  void update() override;

  /// Fade a source.
  ///
  /// \param source The source to fade.
  /// \param target_gain The target gain after fading.
  /// \param duration The duration of the fade.
  void fadeTo(std::shared_ptr<AudioSource> source,
              float target_gain,
              float duration);

  /// Play a source.
  ///
  /// \param source The source to play.
  /// \param track The track in which to play. If < 0, will play in no track.
  void play(std::shared_ptr<AudioSource> source,
            int track = -1);

  /// Play a source fading in.
  ///
  /// \param source The source to fade in.
  /// \param duration The duration of the fade.
  /// \param track The track in which to play. If < 0, will fade in no track.
  void playFadeIn(std::shared_ptr<AudioSource> source,
                  float duration,
                  int track = -1);

  /// Play a source fading in while fading out any current source on the
  /// specified track.
  ///
  /// \param source The source to crossfade to.
  /// \param duration The duration of the fade.
  /// \param track The track in which to crossfade. If < 0, will cross fade in
  ///              no track.
  void crossfadeTo(std::shared_ptr<AudioSource> source,
                   float duration,
                   int track = -1);

  /// Fade out a source.
  ///
  /// \param source The source to fade out.
  /// \param duration The duration of the fade.
  void fadeOut(std::shared_ptr<AudioSource> source,
               float duration);

  /// Fade a source playing on a track.
  ///
  /// \param duration The duration of the fade.
  /// \param track The track in which to play. If < 0, will not do anything.
  void fadeOut(float duration,
               int track = -1);

  /// Stop a playing source.
  ///
  /// \param source The source to stop.
  void stop(std::shared_ptr<AudioSource> source);

  /// Stop a playing source on a track.
  ///
  /// \param track The track in which to stop. If < 0, will not do anything.
  void stop(int track = 0);

  /// A map of tracks and the current playing source.
  std::unordered_map<int, std::shared_ptr<AudioSource>> tracks_;

  /// A singleton audio listener.
  AudioListener audio_listener_;

  struct Data;

 private:
  std::unique_ptr<Data> data_;

  /// A map of audio buffers by name.
  std::unordered_map<std::string, AudioBuffer> buffers_;

  /// A map of fades by the pointer of the source.
  std::unordered_map<AudioSource*, Fade> fades_;
};

}

