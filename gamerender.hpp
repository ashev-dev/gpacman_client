#ifndef GAMERENDER_HPP
#define GAMERENDER_HPP

#pragma once
#include <SFML/Graphics.hpp>

#include "gamemodel.hpp"

class GameRender : public sf::Drawable, public sf::Transformable {
  GameModel* m_game;
  sf::RenderWindow m_window;
  sf::Text m_text;
  sf::Text m_score;
  sf::Text m_steps;
  sf::Text m_fps;

  sf::Text m_login;

 public:
  GameRender(GameModel* game);
  ~GameRender();
  sf::RenderWindow& window() { return m_window; };
  bool Init();
  void Render();

 public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

 private:
  const uint32_t width = 1280;
  const uint32_t height = 720;
  const float PACMAN_RADIUS = 20.f;
  const sf::Color PACMAN_COLOR = sf::Color(0xC307FAff);
  const sf::Color STRANGE_PACMAN_COLOR = sf::Color(0xFF0000FF);
  const sf::Vector2f WALL_SIZE = sf::Vector2f(40.f, 40.f);
  const sf::Color WALL_COLOR = sf::Color(0x2360FAFF);
  const sf::Color WAY_DOT_COLOR = sf::Color(0x11FA8D55);
  const sf::Color WALL_DOT_COLOR = sf::Color(0xFABE2Aff);  // FABE2A

  const float FOOD_RADIUS = 15.f;
  const sf::Color FOOD_COLOR = sf::Color(0x00FF0055);
  float fps;
  sf::Clock fps_clock;
  void Connect(sf::RenderTarget& target, GameState state) const;
  void Login(sf::RenderTarget& target, GameState state) const;
};
#endif  // GAMERENDER_HPP
