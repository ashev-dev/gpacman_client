#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <boost/asio.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <queue>

#include "gamemodel.hpp"
using boost::asio::ip::tcp;
#include <iostream>

class chat_message {
 public:
  enum { header_length = 4 };
  enum { max_body_length = 512 };

  chat_message() : body_length_(0) {}

  chat_message(std::string s) : body_length_(0) {
    this->body_length(std::strlen(s.c_str()));
    std::memcpy(body(), s.c_str(), body_length());
    this->encode_header();
  }

  const char* data() const { return data_; }

  char* data() { return data_; }

  std::size_t length() const { return header_length + body_length_; }

  const char* body() const { return data_ + header_length; }

  char* body() { return data_ + header_length; }

  void body_str(std::string s) {
    std::memcpy(this->body(), s.c_str(), this->body_length());
  }

  std::size_t body_length() const { return body_length_; }

  void body_length(std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length) body_length_ = max_body_length;
  }

  bool decode_header() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length) {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
  }

 private:
  char data_[header_length + max_body_length];
  std::size_t body_length_;
};

typedef std::deque<chat_message> chat_message_queue;

class chat_client {
 public:
  bool is_connected = false;

  chat_client(boost::asio::io_context& io_context,
              const tcp::resolver::results_type& endpoints, GameModel* gm)
      : io_context_(io_context), socket_(io_context) {
    do_connect(endpoints);
    m_game = gm;
  }

  void write(const chat_message& msg) {
    boost::asio::post(io_context_, [this, msg]() {
      bool write_in_progress = !write_msgs_.empty();
      write_msgs_.push_back(msg);
      if (!write_in_progress) {
        do_write();
      }
    });
  }

  void close() {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }

 private:
  GameModel* m_game;
  void do_connect(const tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(
        socket_, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint) {
          if (!ec) {
            is_connected = true;
            do_read_header();
          } else {
            std::cout << ec.message() << std::endl;
            is_connected = false;
          }
        });
  }

  void do_read_header() {
    boost::asio::async_read(
        socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec && read_msg_.decode_header()) {
            do_read_body();
          } else {
            socket_.close();
          }
        });
  }

  void do_read_body() {
    boost::asio::async_read(
        socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            std::string s(read_msg_.body(), read_msg_.body_length());
            m_game->NewMessage(s);
            do_read_header();
          } else {
            socket_.close();
          }
        });
  }

  void do_write() {
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(write_msgs_.front().data(),
                            write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            write_msgs_.pop_front();
            if (!write_msgs_.empty()) {
              do_write();
            }
          } else {
            socket_.close();
          }
        });
  }

 private:
  boost::asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};
#endif  // CHAT_MESSAGE_HPP
