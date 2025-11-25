#pragma once

#include <mutex>
#include <string>

class AppState {
private:
  int counter = 1;
  std::string hash;
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
};
