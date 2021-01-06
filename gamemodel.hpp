#ifndef GAMEMODEL_HPP
#define GAMEMODEL_HPP

#pragma once
#include <inttypes.h>

#include <SFML/Graphics.hpp>
#include <queue>
enum class Direction { Left = 0, Right = 1, Up = 2, Down = 3 };
enum class ObjectType { Empty = 0, Wall = 1, Pacman = 2 };
enum class GameState {
  Running = 0,
  Finish = 1,
  Init = 2,
  Login = 3,
  Connect = 4,
  Connecting = 5,
  Connect_IP = 6,
  Connect_Bttn = 7,
  Connect_Error = 8,
  Login_Name = 9,
  Login_Bttn = 10,
  Waiting_for_start = 11
};

class GameModel {
 public:
  static const uint8_t SIZE_X = 32;
  static const uint8_t SIZE_Y = 17;  // 2 по 40 пикселей под текст (о игре)
  static const uint32_t SIZE =
      SIZE_X * SIZE_Y;  // Размер игрового поля в плашках
  static const uint8_t CELL_SIZE = 40;
  sf::Clock clock;
  const float PACMAN_SPEED = 40.f;
  bool connected = false;

 protected:
  int elements[SIZE];
  int empty_index;
  bool solved;
  sf::Vector2f pacman_location;
  sf::Vector2u pacman_location_in_field;
  Direction pacman_direction;
  std::array<std::array<ObjectType, SIZE_Y>, SIZE_X> field;
  GameState state;
  std::string ip;
  std::string login;
  uint32_t steps = 0;
  uint32_t score = 0;

 public:
  GameModel();
  void Init();
  GameState CurrentState() { return state; };
  void SetState(GameState st);
  bool Check();
  void Move(Direction direction);
  bool IsSolved() { return solved; }
  int* Elements() { return elements; }
  std::array<std::array<ObjectType, SIZE_Y>, SIZE_X> Field() { return field; };
  void Update();
  sf::Vector2f PacmanLocation() { return pacman_location; }
  std::string GetIP() { return ip; };
  sf::Vector2u PacmanLoc() { return pacman_location_in_field; };
  void SetIP(std::string ip_) { ip = ip_; };

  std::string GetLogin() { return login; };
  void SetLogin(std::string login_) { login = login_; };
  void ConnectToServer();
  void ConnectToServerThread();
  void DecScore(uint32_t dec) { score = (score > dec) ? (score - dec) : 0; }
  uint32_t GetSteps() { return steps; };
  uint32_t GetScore() { return score; };
  void IncSteps() { steps++; };
  void IncScore(uint32_t sc_) { score += sc_; };
  void NewMessage(std::string s);
  std::map<std::string, sf::Vector2u> GetStrangePacmans() {
    return strange_pacmans;
  };
  std::vector<sf::Vector2u> GetFood() { return food; };
  void AddFood(sf::Vector2u s);
  void DelFood(sf::Vector2u s);

  std::queue<std::string> msgs_queue_tx;
  std::queue<std::string> msgs_queue_rx;

 private:
  void ParseField();
  std::map<std::string, sf::Vector2u> strange_pacmans;
  std::vector<sf::Vector2u> food;
};

#endif  // GAMEMODEL_HPP
