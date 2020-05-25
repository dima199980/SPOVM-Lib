#pragma once
#include <string>

namespace Database
{
  class db
  {
  public:
  
    void init();

    void add(std::string s);

    void put(std::string s);

    void del();

    void view();

    void set(int p);

    void get();

    void next();

    void prev();

    void close();

  };
}