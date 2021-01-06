#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#pragma once
#include <SFML/Graphics.hpp>

#include "gamerender.hpp"

class GameController {
  GameModel *m_game;
  GameRender *m_render;

 public:
  GameController(GameModel *game, GameRender *render);
  ~GameController();
  void Run();
};

#endif  // GAMECONTROLLER_HPP
