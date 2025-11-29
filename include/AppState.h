#pragma once

#include <mutex>
#include <string>

class AppState {
private:
  int counter = 1;
  std::string hash;
  std::string message;
  mutable std::mutex mtx;
public:
  int getCounter() const {
    std::lock_guard<std::mutex> lock(mtx);
    return counter;
  }
  void setCounter(int value) {
    std::lock_guard<std::mutex> lock(mtx);
    counter = value;
  }
  std::string getHash() const {
    std::lock_guard<std::mutex> lock(mtx);
    return hash;
  }
  void setHash(const std::string& h) {
    std::lock_guard<std::mutex> lock(mtx);
    hash = h;
  }

  std::string getMessage() const {
    std::lock_guard<std::mutex> lock(mtx);
    return message;
  }
  void setMessage(const std::string& m) {
    std::lock_guard<std::mutex> lock(mtx);
    message = m;
  }
};
