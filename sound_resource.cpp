#include "sound_resource.h"
#include <raylib.h>

#ifndef PLATFORM_WEB
void PlaySoundResource(Sound sound) {
  PlaySound(sound);
  return;
}

void StopSoundResource(Sound sound) {
  StopSound(sound);
  return;
}
#endif
