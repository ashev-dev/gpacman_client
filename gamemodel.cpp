#include "gamemodel.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "gamenetclient.hpp"
#include "gameplaymessage.hpp"
GameModel::GameModel() { Init(); }

void GameModel::Init() {
  // static const sf::Vector2f PACMAN_INITIAL_POSITION = {40, 120};
  // pacman_location = PACMAN_INITIAL_POSITION;
  // pacman_location_in_field = sf::Vector2u(1, 1);
  for (auto &i : field)
    for (auto &j : i) {
      j = ObjectType::Empty;
    }
  /*
    for (uint32_t i = 0; i < SIZE_X; i++) {
      field[i][0] = ObjectType::Wall;
      field[i][SIZE_Y - 1] = ObjectType::Wall;
    }
    for (uint32_t i = 0; i < SIZE_Y; i++) {
      field[0][i] = ObjectType::Wall;
      field[SIZE_X - 1][i] = ObjectType::Wall;
    }

    field[pacman_location_in_field.x][pacman_location_in_field.y] =
        ObjectType::Pacman;
       */
  ParseField();
  ip = "127.0.0.1";
  state = GameState::Init;
}

void GameModel::SetState(GameState st) { state = st; }

bool GameModel::Check() { return true; }

void GameModel::Move(Direction direction) { pacman_direction = direction; }

void GameModel::Update() {
  float time = (steps < 900) ? 500 - steps / 2 : 100;
  if (clock.getElapsedTime().asSeconds() > (time / 1000)) {
    const float elapsedSeconds = clock.restart().asSeconds();
    sf::Vector2u new_location = pacman_location_in_field;
    switch (pacman_direction) {
      case Direction::Down:
        new_location.y += 1;
        break;
      case Direction::Up:
        new_location.y -= 1;
        break;
      case Direction::Left:
        new_location.x -= 1;
        break;
      case Direction::Right:
        new_location.x += 1;
        break;
    }
    if (field[new_location.x][new_location.y] == ObjectType::Empty) {
      field[pacman_location_in_field.x][pacman_location_in_field.y] =
          ObjectType::Empty;
      field[new_location.x][new_location.y] = ObjectType::Pacman;
      // переместили пакмана
      pacman_location_in_field = new_location;
      IncSteps();
      GamePlayMessage q(pacman_location_in_field);
      q.SetLogin(login);

      msgs_queue_tx.push(q.to_string());

      // проверели сожрал ли он что то

      auto it = find(food.begin(), food.end(), pacman_location_in_field);
      while (it != food.end()) {
        food.erase(it);
        this->IncScore(100);
        GamePlayMessage q(pacman_location_in_field, true);
        q.SetLogin(login);
        q.food_del = 1;
        msgs_queue_tx.push(q.to_string());
        it = find(food.begin(), food.end(), pacman_location_in_field);
      }
    }
    // rx messages parser
    while (!msgs_queue_rx.empty()) {
      std::string msg = msgs_queue_rx.front();
      std::cout << "Msg from queue: " << msg << std::endl;
      GamePlayMessage m;
      m.from_string(msg);
      if (m.loc & (m.GetLogin() != this->login)) {
        // add pacman on field
        std::cout << "Add pacman" << std::endl;
        strange_pacmans[m.GetLogin()] = m.GetLoc();
      }
      if (m.food) {
        AddFood(m.GetLoc());
      }
      if (m.food_del) {
        DelFood(m.GetLoc());
      }
      if (m.running) {
        this->SetState(GameState::Running);
        m.running = false;
      }
      if (m.accident) {
        if ((m.acc1 == this->login) | (m.acc2 == this->login)) {
          this->DecScore(100);
        }
      }
      msgs_queue_rx.pop();
    }
  }
}

void GameModel::ConnectToServer() {
  std::cout << "Connecting... " << std::endl;
  std::thread thr(&GameModel::ConnectToServerThread, this);
  thr.detach();
}
#include <boost/asio.hpp>
void GameModel::ConnectToServerThread() {
  auto t1 = std::chrono::high_resolution_clock::now();
  std::cout << "Thread started: " << std::this_thread::get_id() << std::endl;
  /*
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << "Thread duration: " << duration << std::endl;1000
  if (duration > 500) {
    this->SetState(GameState::Connect_Error);
  } else {
    this->SetState(GameState::Login_Name);
  }
  while (this->CurrentState() != GameState::Login) {
  }
  std::cout << "Login state" << std::endl;
  std::cout << "Login: " << this->GetLogin() << std::endl;
  this->SetState(GameState::Running);
*/
  // Connection to server!)

  try {
    std::cout << "Section connection" << std::endl;
    boost::asio::io_context io_context;
    using boost::asio::ip::tcp;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(this->GetIP(), "8081");
    chat_client c(io_context, endpoints, this);

    std::thread t([&io_context]() { io_context.run(); });

    char line[chat_message::max_body_length + 1];
    while (c.is_connected != true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Connected successful" << std::endl;
    this->SetState(GameState::Login_Name);

    while (this->CurrentState() != GameState::Login) {
    }
    std::cout << "Login state" << std::endl;
    std::cout << "Login: " << this->GetLogin() << std::endl;
    this->SetState(GameState::Waiting_for_start);

    std::string s = "login:" + this->GetLogin();
    chat_message msg;
    msg.body_length(s.length());
    msg.body_str(s);
    msg.encode_header();
    c.write(msg);

    while (1) {
      if (!msgs_queue_tx.empty()) {
        std::string s(msgs_queue_tx.front());
        c.write(chat_message(s));
        msgs_queue_tx.pop();
      }
      // msgs_queue_tx.push(s);
    };
    while (std::cin.getline(line, chat_message::max_body_length + 1)) {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

void GameModel::NewMessage(std::string s) { msgs_queue_rx.push(s); }

void GameModel::AddFood(sf::Vector2u s) {
  if (food.size() < 10)
    if (field[s.x][s.y] != ObjectType::Wall) {
      food.push_back(s);
    }
}
void GameModel::DelFood(sf::Vector2u s) {
  auto it = std::find(food.begin(), food.end(), s);
  if (it != food.end()) {
    food.erase(it);
  }
}
void GameModel::ParseField() {
  const std::string field_ =
      "################################"
      "#p............#..#.............#"
      "#.#########...#..#...#########.#"
      "#.#...........#..#...........#.#"
      "#.#.##........#..#........##.#.#"
      "#.#.....###...#..#...###.....#.#"
      "#.#...........#..#...........#.#"
      "#.#...#######.#..#.#######...#.#"
      "#.#...#.....#......#.....#...#.#"
      "#.#.........#......#.........#.#"
      "#.#...#.....#......#.....#...#.#"
      "#.#...#.....#......#.....#...#.#"
      "#.....#.....#......#.....#.....#"
      "#.....#.....#......#.....#.....#"
      "#.....#.....#......#.....#.....#"
      "#.....#.....#......#.....#.....#"
      "################################";

  uint32_t row = 0;
  uint32_t col = 0;
  for (auto i : field_) {
    // std::cout << i << std::endl;

    switch (i) {
      case '#':
        field[col][row] = ObjectType::Wall;
        break;
      case '.':
        field[col][row] = ObjectType::Empty;
        break;
      case 'p':
        field[col][row] = ObjectType::Pacman;
        pacman_location_in_field = sf::Vector2u(row, col);
        break;
      default:
        break;
    }
    col++;
    if (col == SIZE_X) {
      row++;
      col = 0;
    }
  }
}
