#include <iostream>
#include "MathLibrary.h"

using namespace Database;
int main()
{
  db db;
  db.init();
  std::cout << "add elem 1-9" << std::endl;
  db.add("1");
  db.add("2");
  db.add("3");
  db.add("4");
  db.add("5");
  db.add("6");
  db.add("7");
  db.add("8");
  db.add("9");
  db.view();
  std::cout << "delete first elem" << std::endl;
  db.del();
  db.view();
  std::cout << "delete elem pos 4" << std::endl;
  db.set(4);
  db.del();
  db.view();
  std::cout << "next elem after pos 4" << std::endl;
  db.next();
  std::cout << "next elem after pos 5" << std::endl;
  db.next();
  db.set(3);
  std::cout << "prev elem before pos 3" << std::endl;
  db.prev();
  std::cout << "prev elem before pos 2" << std::endl;
  db.prev();
  std::cout << "set pos 5 and put elem 10" << std::endl;
  db.set(5);
  db.put("10");
  db.view();
  db.set(1);
 
  system("PAUSE");

  return 0;
}
