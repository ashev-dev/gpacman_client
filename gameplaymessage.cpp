#include "gameplaymessage.hpp"

#include <iostream>
GamePlayMessage::GamePlayMessage(sf::Vector2u p) {
  std::string s(":loc:x:" + std::to_string(p.x) + ":y:" + std::to_string(p.y));
  message_string = s;
}

GamePlayMessage::GamePlayMessage(sf::Vector2u p, bool food_del) {
  std::string s(":fdd:x:" + std::to_string(p.x) + ":y:" + std::to_string(p.y));
  message_string = s;
}

void GamePlayMessage::from_string(std::string s) {
  std::string delimiter = ":";

  std::vector<std::string> msg;

  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    msg.push_back(token);
    s.erase(0, pos + delimiter.length());
  }
  msg.push_back(s);
  if (msg.size() == 2) {
    if (msg[1] == "start") running = true;
  }
  if (msg.size() == 4) {
    // accident
    accident = true;
    acc1 = msg[2];
    acc2 = msg[3];
  }
  if (msg.size() > 4) {
    login_ = msg[0];

    if (msg[1] == "loc") {
      coord.x = std::stoi(msg[3]);
      coord.y = std::stoi(msg[5]);

      std::cout << "From: " + login_ +
                       " type: loc x:" + std::to_string(coord.x) +
                       " y:" + std::to_string(coord.y)
                << std::endl;
      loc = true;
    } else if (msg[1] == "fda") {
      coord.x = std::stoi(msg[3]);
      coord.y = std::stoi(msg[5]);

      std::cout << "From: " + login_ +
                       " type: food x:" + std::to_string(coord.x) +
                       " y:" + std::to_string(coord.y)
                << std::endl;
      food = true;
    } else if (msg[1] == "fdd") {
      coord.x = std::stoi(msg[3]);
      coord.y = std::stoi(msg[5]);

      std::cout << "From: " + login_ +
                       " type: food x:" + std::to_string(coord.x) +
                       " y:" + std::to_string(coord.y)
                << std::endl;
      food_del = true;
    }
  }
}
