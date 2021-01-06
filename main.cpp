#include <iostream>

#include "assets.hpp"
#include "gamecontroller.hpp"
#include "gamemodel.hpp"
#include "gamerender.hpp"
using namespace std;

int main() {
  Assets::Instance().Load();
  GameModel game;            // Создаем модель игры
  GameRender render(&game);  // Создаем представление
  GameController controller(&game, &render);  // Создаем контроллер
  controller.Run();                           // Запускаем игру
  return 0;
  while (1)
    ;
  cout << "Hello World!" << endl;
  return 0;
}
