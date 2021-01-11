#include "gamecontroller.hpp"

#include "assets.hpp"
#include "gamenetclient.hpp"
GameController::GameController(GameModel *game, GameRender *render) {
  m_game = game;
  m_render = render;
}

GameController::~GameController() {}

void GameController::Run() {
  sf::Event event;

  while (m_render->window().isOpen()) {
    while (m_render->window().pollEvent(event)) {
      if (event.type == sf::Event::Closed) m_render->window().close();
      if (event.type == sf::Event::KeyPressed) {
        // Получаем нажатую клавишу - выполняем соответствующее действие
        if (event.key.code == sf::Keyboard::Escape) m_render->window().close();
        if (event.key.code == sf::Keyboard::M) {
          if (Assets::Instance().sound.getStatus() == sf::Sound::Playing)
            Assets::Instance().sound.pause();
          else
            Assets::Instance().sound.play();
        }
        if (m_game->CurrentState() == GameState::Running) {
          if (event.key.code == sf::Keyboard::Left)
            m_game->Move(Direction::Left);
          if (event.key.code == sf::Keyboard::Right)
            m_game->Move(Direction::Right);
          if (event.key.code == sf::Keyboard::Up) m_game->Move(Direction::Up);
          if (event.key.code == sf::Keyboard::Down)
            m_game->Move(Direction::Down);
        }
        switch (m_game->CurrentState()) {
          case GameState::Init:
            if (event.key.code == sf::Keyboard::Return)
              m_game->SetState(GameState::Connect_IP);
            break;
          default:
            break;
        }
        // Новая игра
        if (event.key.code == sf::Keyboard::F2) {
          m_game->Init();
        }
      }

      // connect events

      if (m_game->CurrentState() == GameState::Connect_IP) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          m_game->SetState(GameState::Connect_Bttn);
        std::string ip = m_game->GetIP();
        if (event.type == sf::Event::TextEntered) {
          if (event.text.unicode < 128) {
            if (event.text.unicode > 32) {
              if (ip.length() < 15)
                ip.push_back(static_cast<char>(event.text.unicode));
            } else if (event.text.unicode == 8)
              if (ip.length() > 0) ip.pop_back();
          }
        }
        m_game->SetIP(ip);
      }

      if (m_game->CurrentState() == GameState::Connect_Bttn) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          m_game->SetState(GameState::Connect_IP);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
          m_game->SetState(GameState::Connecting);
          m_game->ConnectToServer();
        }
      }

      // TODO: добавить обработку поля с логином и паролем для поля ввода

      if (m_game->CurrentState() == GameState::Login_Name) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))

          m_game->SetState(GameState::Login_Bttn);
        std::string ip = m_game->GetLogin();
        if (event.type == sf::Event::TextEntered) {
          if (event.text.unicode < 128) {
            if (event.text.unicode > 32) {
              if (ip.length() < 15)
                ip.push_back(static_cast<char>(event.text.unicode));
            } else if (event.text.unicode == 8)
              if (ip.length() > 0) ip.pop_back();
          }
        }
        m_game->SetLogin(ip);
      }

      if (m_game->CurrentState() == GameState::Login_Bttn) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          m_game->SetState(GameState::Login_Name);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
          m_game->SetState(GameState::Login);
      }
    }
    // TODO: попробовать прицепить тестовый пример от буста для сообщения

    // Выполняем необходимые действия по отрисовке
    m_game->Update();
    m_render->Render();
  }
}
