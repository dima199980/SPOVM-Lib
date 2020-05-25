#include "MathLibrary.h"
#include <iostream>

#include <experimental/filesystem>
#include <cstring>

#include "db_cxx.h"
#include "stl\dbstl_map.h"
#include "stl\dbstl_vector.h"

#include <algorithm>
#include <iterator>

const std::string ENV_FOLDER = "sample-queue-multithread-usage-dir";

namespace fs = std::experimental::filesystem;

void* save_str(const std::string& str, void* dest)
{
  auto size = str.length();
  memcpy(dest, &size, sizeof(size));
  char* charDest = static_cast<char*>(dest) + sizeof(size);
  memcpy(charDest, str.data(), str.length());
  return charDest + str.length();
}

const void* restore_str(std::string& str, const void* src)
{
  std::string::size_type size;
  memcpy(&size, src, sizeof(size));
  str.reserve(size);
  const char* strSrc = static_cast<const char*>(src) + sizeof(size);
  str.insert(0, strSrc, size);
  return strSrc + size;
}

//stored element type
struct a
{
  std::string str;
};

std::ostream& operator<<(std::ostream& os, const a& el) {
  os << el.str;
  return os;
}

//marshall/unmarshal class
class TestMarshaller {
public:
  static void restore(a& elem, const void* src)
  {
    src = restore_str(elem.str, src);
  }
  static u_int32_t size(const a& element)
  {
    u_int32_t size = 0;
    size += sizeof(std::string::size_type) + element.str.length();
    return size;
  }
  static void store(void* dest, const a& elem)
  {
    dest = save_str(elem.str, dest);
  }
};

dbstl::db_vector<a> container;
int pos = 1;
namespace Database
{
  void db::init()
  {
    if (!fs::exists(ENV_FOLDER) && !fs::create_directory(ENV_FOLDER)) {
      std::cerr << "Failed to create env directory: " << ENV_FOLDER << std::endl;
      //return 1;
    }

    dbstl::dbstl_startup();

    auto penv = dbstl::open_env(ENV_FOLDER.c_str(), 0u, DB_INIT_MPOOL | DB_CREATE | DB_INIT_LOCK | DB_THREAD);

    // open an environment and the database
    auto db = new Db(nullptr, DB_CXX_NO_EXCEPTIONS);

    db->set_flags(DB_RENUMBER);

    if (0 != db->open(nullptr, "sample-queue-multithread-usage.db", nullptr, DB_RECNO, DB_CREATE | DB_THREAD, 0600)) {
      std::cerr << "Failed to open DB in path " << "sample-map-usage.db" << std::endl;
      delete db;
      // return 1;
    }

    dbstl::register_db(db);

    //cleare current database data
    db->truncate(nullptr, nullptr, 0u);

    // register callbacks for marshalling/unmarshalling stored element
    auto inst = dbstl::DbstlElemTraits<a>::instance();
    inst->set_size_function(&TestMarshaller::size);
    inst->set_copy_function(&TestMarshaller::store);
    inst->set_restore_function(&TestMarshaller::restore);

    dbstl::db_vector<a> ontainer{ db, penv };
    container = ontainer;
  }

  void db::add(std::string s)
  {
    container.push_back( { s } );
  }
  void db::put(std::string s)
  {
    container[pos] = { s };
  }
  void db::del()
  {
    container.erase(container.begin() + pos - 1);
  }
  void db::view()
  {
    for (const auto& val : container) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }
  void db::set(int p)
  {
    pos = p - 1;
  }
  void db::get()
  {
    std::cout << container[pos] << std::endl;
  }
  void db::next()
  {
    pos++;
    std::cout << container[pos] << std::endl;
  }
  void db::prev()
  {
    --pos;
    std::cout << container[pos] << std::endl;
  }
  void db::close()
  {
    dbstl::dbstl_exit();
  }
}