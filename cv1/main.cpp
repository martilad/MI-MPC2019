#include <string>
#include <cstring>

namespace no_sso { // opt. I. - without SSO
class String {
  char* data_;
public:
  String(const char* ptr): data_(new char[strlen(ptr) + 1]) {
    strcpy(data_, ptr);
  }
  ~String() {
    delete[] data_;
  }
  const char* data() const {
    return data_;
  }
};
}

namespace sso_buf { // opt. II. - SSO with additional buffer
class String {
  char* data_;
  char buf_[8];
public:
  String(const char* ptr) {
    data_ = strlen(ptr) < 8 ? buf_ : new char[strlen(ptr) + 1];
    strcpy(data_, ptr);
  }
  ~String() {
     if (data_ != buf_) delete[] data_;
  }
  const char* data() const {
    return data_;
  }
};
}

namespace sso_buf_union { // opt. II.5. - SSO with additional buffer union
class String {
  union { char* data_; char buf_[8]; };
  bool is_short() const {
    return reinterpret_cast<uintptr_t>(data_) & 1;
    }
  char* short_ptr() {
    return &buf_[1];
    } // non-const
  const char* short_ptr() const {
    return &buf_[1];
    }

public:
  String(const char* arg = "") {
    if (strlen(arg) < 7) {
    data_ = reinterpret_cast<char*>((uintptr_t)1);
    strcpy(short_ptr(), arg);
  } else {
    data_ = new char[strlen(arg) + 1];
    strcpy(data_, arg);
  }
}
// disabled copy semantics (for the sake of simplicity as for now):
String(const String&) = delete;
String& operator=(const String&) = delete;
~String() {
  if (is_short() == false) delete[] data_;
  }
const char* data() const {
   return is_short() ? short_ptr() : data_;
   }
};
}


namespace sso_tp { // opt. III - SSO with tagged pointer
class String {
  char* data_;
  bool is_short() const { return reinterpret_cast<uintptr_t>(data_) & 1; }
  char* short_ptr() { return reinterpret_cast<char*>(&data_) + 1; }
  const char* short_ptr() const {
    return reinterpret_cast<const char*>(&data_) + 1;
    }
public:
  String(const char* arg = "") {
    if (strlen(arg) < 7) {
    data_ = reinterpret_cast<char*>((uintptr_t)1);
    strcpy(short_ptr(), arg);
  } else {
    data_ = new char[strlen(arg) + 1];
    strcpy(data_, arg);
  }
}
// disabled copy semantics (for the sake of simplicity as for now):
String(const String&) = delete;
String& operator=(const String&) = delete;
~String() { if (is_short() == false) delete[] data_; }
const char* data() const { return is_short() ? short_ptr() : data_; }
};
}

extern std::string seed;

template <typename T>
static void Short(benchmark::State& state) {
  for (auto _ : state) {
    T s(seed.c_str());
    benchmark::DoNotOptimize(s);
  }
}

BENCHMARK_TEMPLATE(Short, no_sso::String);
BENCHMARK_TEMPLATE(Short, sso_buf::String);
BENCHMARK_TEMPLATE(Short, sso_buf_union::String);
BENCHMARK_TEMPLATE(Short, sso_tp::String);

std::string seed("short");
