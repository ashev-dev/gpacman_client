#ifndef ASSETS_HPP
#define ASSETS_HPP

#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Assets {
 public:
  sf::Font font;
  sf::Sound sound;
  sf::SoundBuffer buffer;

 public:
  static Assets& Instance() {
    static Assets s;
    return s;
  }
  void Load();

 private:
  Assets(){};
  ~Assets(){};
  Assets(Assets const&) = delete;
  Assets& operator=(Assets const&) = delete;
};

#endif  // ASSETS_HPP
