#include "assets.hpp"

void Assets::Load() {
  if (!font.loadFromFile("roboto.ttf")) throw;

  if (!buffer.loadFromFile("audio.wav")) throw;

  sound.setLoop(true);
  sound.setBuffer(buffer);
  sound.play();
}
