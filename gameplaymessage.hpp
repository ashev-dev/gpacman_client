#ifndef GAMEPLAYMESSAGE_HPP
#define GAMEPLAYMESSAGE_HPP

#include <SFML/Graphics.hpp>

class GamePlayMessage {
 public:
  GamePlayMessage(sf::Vector2u p);
  GamePlayMessage(sf::Vector2u p, bool food_del);
  GamePlayMessage(){};
  std::string to_string() { return login_ + message_string; };
  void from_string(std::string s);
  void SetLogin(std::string login) { login_ = login; };
  bool loc = false;
  bool food = false;
  bool food_del = false;
  bool running = false;
  bool accident = false;
  std::string acc1 = "";
  std::string acc2 = "";
  std::string GetLogin() { return login_; };
  sf::Vector2u GetLoc() { return coord; };

 private:
  std::string message_string;
  std::string login_;
  sf::Vector2u coord;
};

#endif  // GAMEPLAYMESSAGE_HPP
