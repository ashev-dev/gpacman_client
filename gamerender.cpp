#include "gamerender.hpp"

#include <string>

#include "assets.hpp"
GameRender::GameRender(GameModel *game) {
  m_game = game;

  Init();
}

GameRender::~GameRender() {}

bool GameRender::Init() {
  setPosition(50.f, 50.f);

  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;

  m_window.create(sf::VideoMode(width, height), "gpacman", sf::Style::Default,
                  settings);
  m_window.setFramerateLimit(60);

  m_text = sf::Text("F2 - New Game / Esc - Exit / Arrow Keys - Move / M - Mute",
                    Assets::Instance().font, 20);
  m_text.setFillColor(sf::Color::Cyan);
  m_text.setPosition(5.f, 5.f);
  return true;
}

void GameRender::Render() {
  m_window.clear();
  fps = 1.f / fps_clock.restart().asSeconds();
  m_fps = sf::Text("FPS: " + std::to_string(fps).substr(0, 5),
                   Assets::Instance().font, 20);
  m_fps.setFillColor(sf::Color::Cyan);
  m_fps.setPosition(600.f, 5.f);
  m_score =
      sf::Text("Score: " + std::to_string(m_game->GetScore()).substr(0, 5),
               Assets::Instance().font, 20);
  m_score.setFillColor(sf::Color::Cyan);
  m_score.setPosition(750.f, 5.f);
  m_steps =
      sf::Text("Steps: " + std::to_string(m_game->GetSteps()).substr(0, 5),
               Assets::Instance().font, 20);
  m_steps.setFillColor(sf::Color::Cyan);
  m_steps.setPosition(900.f, 5.f);

  m_login =
      sf::Text("Login: " + m_game->GetLogin(), Assets::Instance().font, 20);
  m_login.setFillColor(sf::Color::Cyan);
  m_login.setPosition(1050.f, 5.f);

  m_window.draw(*this);
  m_window.draw(m_text);
  m_window.draw(m_fps);
  m_window.draw(m_score);
  m_window.draw(m_steps);
  m_window.draw(m_login);
  m_window.display();
}

void GameRender::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  // if (m_game->CurrentState() == GameState::Running) {
  sf::CircleShape shape;

  std::array<std::array<ObjectType, GameModel::SIZE_Y>, GameModel::SIZE_X>
      field = m_game->Field();
  sf::RectangleShape rect_shape;
  sf::CircleShape circ_shape;
  for (uint32_t i = 0; i < GameModel::SIZE_X; i++) {
    for (uint32_t j = 0; j < GameModel::SIZE_Y; j++) {
      switch (field[i][j]) {
        case ObjectType::Empty:
          shape.setRadius(5);
          shape.setFillColor(WAY_DOT_COLOR);
          shape.setOrigin(5, 5);
          shape.setPosition(i * 40 + 20, j * 40 + 40 + 20);
          target.draw(shape, states);
          break;
        case ObjectType::Wall:
          // draw rectangle
          rect_shape.setFillColor(WALL_COLOR);
          rect_shape.setSize(WALL_SIZE);
          rect_shape.setPosition(i * 40, j * 40 + 40);
          target.draw(rect_shape, states);

          circ_shape.setFillColor(WALL_DOT_COLOR);
          circ_shape.setRadius(10);
          circ_shape.setOrigin(10, 10);
          circ_shape.setPosition(i * 40 + 20, j * 40 + 40 + 20);
          target.draw(circ_shape, states);
          break;
        case ObjectType::Pacman:
          shape.setRadius(PACMAN_RADIUS);
          shape.setFillColor(PACMAN_COLOR);
          shape.setOrigin(0, 0);
          shape.setPosition(i * 40, j * 40 + 40);
          target.draw(shape, states);
          break;
      }
    }
  }
  // strange pacmans !)
  for (auto i : m_game->GetStrangePacmans()) {
    if (i.second == m_game->PacmanLoc()) {
      shape.setRadius(2 * PACMAN_RADIUS);
      shape.setOrigin(PACMAN_RADIUS * 2, PACMAN_RADIUS * 2);
    } else {
      shape.setRadius(PACMAN_RADIUS);
      shape.setOrigin(PACMAN_RADIUS, PACMAN_RADIUS);
    }
    shape.setFillColor(STRANGE_PACMAN_COLOR);
    // shape.setOrigin(0, 0);
    shape.setPosition(i.second.x * 40 + 20, i.second.y * 40 + 40 + 20);
    target.draw(shape, states);
  }
  for (auto i : m_game->GetFood()) {
    shape.setRadius(FOOD_RADIUS);
    shape.setFillColor(FOOD_COLOR);
    shape.setOrigin(FOOD_RADIUS, FOOD_RADIUS);
    shape.setPosition(i.x * 40 + 20, i.y * 40 + 40 + 20);
    target.draw(shape, states);
  }
  // } else
  if (m_game->CurrentState() != GameState::Running) {
    // серое заполнение для неактивных режимов игры
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(1280, 700));
    shape.setPosition(0, 40);
    shape.setFillColor(sf::Color(200, 200, 200, 200));
    target.draw(shape, states);
  }
  if (m_game->CurrentState() == GameState::Finish) {
    sf::Text txt;
    txt.setString("Game Finished:)");
    txt.setFillColor(sf::Color::Cyan);
    txt.setFont(Assets::Instance().font);
    txt.setCharacterSize(40);

    sf::FloatRect bounds(txt.getLocalBounds());
    sf::RectangleShape box_shape(sf::Vector2f(400, 70));
    box_shape.setPosition(width / 2 - 400 / 2, height / 2 - 70 / 2);
    box_shape.setFillColor(sf::Color::Transparent);
    box_shape.setOutlineColor(sf::Color::Green);
    box_shape.setOutlineThickness(5);
    txt.setPosition(box_shape.getPosition());
    const sf::Vector2f box(box_shape.getSize());
    txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                  (bounds.height - box.y) / 2 + bounds.top);

    target.draw(txt, states);
    target.draw(box_shape, states);
  }

  if (m_game->CurrentState() == GameState::Init) {
    const float tb_x = 600;
    const float tb_y = 80;
    sf::RectangleShape msg_server_addr(sf::Vector2f(tb_x, tb_y));
    msg_server_addr.setPosition(width / 2 - tb_x / 2, height / 2 - tb_y / 2);
    msg_server_addr.setFillColor(sf::Color::Transparent);
    msg_server_addr.setOutlineColor(sf::Color::Blue);
    msg_server_addr.setOutlineThickness(5);
    target.draw(msg_server_addr, states);

    sf::Text txt;
    txt.setFont(Assets::Instance().font);
    txt.setString("Press Enter to start...");
    txt.setCharacterSize(50);
    txt.setFillColor(sf::Color::Blue);
    txt.setPosition(msg_server_addr.getPosition());
    sf::FloatRect bounds(txt.getLocalBounds());
    const sf::Vector2f box(msg_server_addr.getSize());
    txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                  (bounds.height - box.y) / 2 + bounds.top);
    target.draw(txt);
  }

  if (m_game->CurrentState() == GameState::Connecting) {
    const float tb_x = 400;
    const float tb_y = 80;
    sf::RectangleShape msg_server_addr(sf::Vector2f(tb_x, tb_y));
    msg_server_addr.setPosition(width / 2 - tb_x / 2, height / 2 - tb_y / 2);
    msg_server_addr.setFillColor(sf::Color::Transparent);
    msg_server_addr.setOutlineColor(sf::Color::Green);
    msg_server_addr.setOutlineThickness(5);
    target.draw(msg_server_addr, states);

    sf::Text txt;
    txt.setFont(Assets::Instance().font);
    txt.setString("Connecting...");
    txt.setCharacterSize(50);
    txt.setFillColor(sf::Color::Green);
    txt.setPosition(msg_server_addr.getPosition());
    sf::FloatRect bounds(txt.getLocalBounds());
    const sf::Vector2f box(msg_server_addr.getSize());
    txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                  (bounds.height - box.y) / 2 + bounds.top);
    target.draw(txt);
  }
  if (m_game->CurrentState() == GameState::Waiting_for_start) {
    const float tb_x = 400;
    const float tb_y = 80;
    sf::RectangleShape msg_server_addr(sf::Vector2f(tb_x, tb_y));
    msg_server_addr.setPosition(width / 2 - tb_x / 2, height / 2 - tb_y / 2);
    msg_server_addr.setFillColor(sf::Color::Transparent);
    msg_server_addr.setOutlineColor(sf::Color::Green);
    msg_server_addr.setOutlineThickness(5);
    target.draw(msg_server_addr, states);

    sf::Text txt;
    txt.setFont(Assets::Instance().font);
    txt.setString("Waiting ...");
    txt.setCharacterSize(50);
    txt.setFillColor(sf::Color::Green);
    txt.setPosition(msg_server_addr.getPosition());
    sf::FloatRect bounds(txt.getLocalBounds());
    const sf::Vector2f box(msg_server_addr.getSize());
    txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                  (bounds.height - box.y) / 2 + bounds.top);
    target.draw(txt);
  }
  if (m_game->CurrentState() == GameState::Connect_Error) {
    const float tb_x = 400;
    const float tb_y = 80;
    sf::RectangleShape msg_server_addr(sf::Vector2f(tb_x, tb_y));
    msg_server_addr.setPosition(width / 2 - tb_x / 2, height / 2 - tb_y / 2);
    msg_server_addr.setFillColor(sf::Color::Transparent);
    msg_server_addr.setOutlineColor(sf::Color::Red);
    msg_server_addr.setOutlineThickness(5);
    target.draw(msg_server_addr, states);

    sf::Text txt;
    txt.setFont(Assets::Instance().font);
    txt.setString("Connection Error");
    txt.setCharacterSize(50);
    txt.setFillColor(sf::Color::Red);
    txt.setPosition(msg_server_addr.getPosition());
    sf::FloatRect bounds(txt.getLocalBounds());
    const sf::Vector2f box(msg_server_addr.getSize());
    txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                  (bounds.height - box.y) / 2 + bounds.top);
    target.draw(txt);
  }

  if ((m_game->CurrentState() == GameState::Connect_IP) ||
      (m_game->CurrentState() == GameState::Connect_Bttn)) {
    Connect(target, m_game->CurrentState());
  }

  if ((m_game->CurrentState() == GameState::Login_Name) ||
      (m_game->CurrentState() == GameState::Login_Bttn)) {
    Login(target, m_game->CurrentState());
  }
}

void GameRender::Connect(sf::RenderTarget &target, GameState state) const {
  const float tb_x = 400;
  const float tb_y = 80;

  sf::RectangleShape msg_server_addr(sf::Vector2f(tb_x, tb_y));
  msg_server_addr.setPosition(width / 2 - tb_x / 2, height / 8 * 2 - tb_y / 2);
  msg_server_addr.setFillColor(sf::Color::Transparent);
  msg_server_addr.setOutlineColor(sf::Color::Green);
  msg_server_addr.setOutlineThickness(5);
  target.draw(msg_server_addr);

  sf::Text txt;
  txt.setFont(Assets::Instance().font);
  txt.setString("Server IP");
  txt.setCharacterSize(50);
  txt.setFillColor(sf::Color::Green);
  txt.setPosition(msg_server_addr.getPosition());
  sf::FloatRect bounds(txt.getLocalBounds());
  const sf::Vector2f box(msg_server_addr.getSize());
  txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                (bounds.height - box.y) / 2 + bounds.top);
  target.draw(txt);

  sf::RectangleShape inp_server_addr(sf::Vector2f(tb_x, tb_y));
  inp_server_addr.setPosition(width / 2 - tb_x / 2, height / 8 * 4 - tb_y / 2);
  if (state == GameState::Connect_IP)
    inp_server_addr.setFillColor(sf::Color(255, 0, 0, 128));
  else
    inp_server_addr.setFillColor(sf::Color::Transparent);
  inp_server_addr.setOutlineColor(sf::Color::Red);
  inp_server_addr.setOutlineThickness(5);
  target.draw(inp_server_addr);

  // sf::Text txt;
  txt.setFont(Assets::Instance().font);
  txt.setString(m_game->GetIP());
  txt.setCharacterSize(50);
  txt.setFillColor(sf::Color::Yellow);
  txt.setPosition(inp_server_addr.getPosition());
  bounds = txt.getLocalBounds();
  // const sf::Vector2f box(msg_server_addr.getSize());
  txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                (bounds.height - box.y) / 2 + bounds.top);
  target.draw(txt);

  sf::RectangleShape btt_server_addr(sf::Vector2f(tb_x, tb_y));
  btt_server_addr.setPosition(width / 2 - tb_x / 2, height / 8 * 6 - tb_y / 2);
  if (state == GameState::Connect_Bttn)
    btt_server_addr.setFillColor(sf::Color(0, 0, 255, 128));
  else

    btt_server_addr.setFillColor(sf::Color::Transparent);
  btt_server_addr.setOutlineColor(sf::Color::Blue);
  btt_server_addr.setOutlineThickness(5);
  target.draw(btt_server_addr);

  txt.setFont(Assets::Instance().font);
  txt.setString("Connect");
  txt.setCharacterSize(50);
  txt.setFillColor(sf::Color::Green);
  txt.setPosition(btt_server_addr.getPosition());
  bounds = txt.getLocalBounds();
  // const sf::Vector2f box(msg_server_addr.getSize());
  txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                (bounds.height - box.y) / 2 + bounds.top);
  target.draw(txt);
}

void GameRender::Login(sf::RenderTarget &target, GameState state) const {
  const float tb_x = 400;
  const float tb_y = 80;

  sf::RectangleShape msg_server_addr(sf::Vector2f(tb_x, tb_y));
  msg_server_addr.setPosition(width / 2 - tb_x / 2, height / 8 * 2 - tb_y / 2);
  msg_server_addr.setFillColor(sf::Color::Transparent);
  msg_server_addr.setOutlineColor(sf::Color::Green);
  msg_server_addr.setOutlineThickness(5);
  target.draw(msg_server_addr);

  sf::Text txt;
  txt.setFont(Assets::Instance().font);
  txt.setString("Login");
  txt.setCharacterSize(50);
  txt.setFillColor(sf::Color::Green);
  txt.setPosition(msg_server_addr.getPosition());
  sf::FloatRect bounds(txt.getLocalBounds());
  const sf::Vector2f box(msg_server_addr.getSize());
  txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                (bounds.height - box.y) / 2 + bounds.top);
  target.draw(txt);

  sf::RectangleShape inp_server_addr(sf::Vector2f(tb_x, tb_y));
  inp_server_addr.setPosition(width / 2 - tb_x / 2, height / 8 * 4 - tb_y / 2);
  if (state == GameState::Login_Name)
    inp_server_addr.setFillColor(sf::Color(255, 0, 0, 128));
  else
    inp_server_addr.setFillColor(sf::Color::Transparent);
  inp_server_addr.setOutlineColor(sf::Color::Red);
  inp_server_addr.setOutlineThickness(5);
  target.draw(inp_server_addr);

  // sf::Text txt;
  txt.setFont(Assets::Instance().font);
  txt.setString(m_game->GetLogin());
  txt.setCharacterSize(50);
  txt.setFillColor(sf::Color::Yellow);
  txt.setPosition(inp_server_addr.getPosition());
  bounds = txt.getLocalBounds();
  // const sf::Vector2f box(msg_server_addr.getSize());
  txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                (bounds.height - box.y) / 2 + bounds.top);
  target.draw(txt);

  sf::RectangleShape btt_server_addr(sf::Vector2f(tb_x, tb_y));
  btt_server_addr.setPosition(width / 2 - tb_x / 2, height / 8 * 6 - tb_y / 2);
  if (state == GameState::Login_Bttn)
    btt_server_addr.setFillColor(sf::Color(0, 0, 255, 128));
  else

    btt_server_addr.setFillColor(sf::Color::Transparent);
  btt_server_addr.setOutlineColor(sf::Color::Blue);
  btt_server_addr.setOutlineThickness(5);
  target.draw(btt_server_addr);

  txt.setFont(Assets::Instance().font);
  txt.setString("Join");
  txt.setCharacterSize(50);
  txt.setFillColor(sf::Color::Green);
  txt.setPosition(btt_server_addr.getPosition());
  bounds = txt.getLocalBounds();
  // const sf::Vector2f box(msg_server_addr.getSize());
  txt.setOrigin((bounds.width - box.x) / 2 + bounds.left,
                (bounds.height - box.y) / 2 + bounds.top);
  target.draw(txt);
}
