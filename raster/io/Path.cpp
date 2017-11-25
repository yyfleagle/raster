/*
 * Copyright (C) 2017, Yeolar
 */

#include <deque>
#include <iterator>
#include <sys/types.h>
#include <unistd.h>

#include "raster/io/Path.h"
#include "raster/util/String.h"

namespace rdd {

Path& Path::operator/=(StringPiece path) {
  append(path);
  return *this;
}

Path& Path::operator+=(StringPiece path) {
  Path(path_ + path.str()).swap(*this);
  return *this;
}

Path Path::parent() const {
  return *this / "..";
}

std::string Path::name() const {
  StringPiece sp(path_);
  auto p = sp.rfind('/');
  if (p != StringPiece::npos)
    sp = sp.subpiece(p+1);
  return sp.str();
}

std::string Path::base() const {
  auto s = name();
  if (s == "." || s == "..")
    return s;
  StringPiece sp(s);
  auto p = sp.rfind('.');
  if (p != StringPiece::npos && p != 0 && p+1 != sp.size())
    sp = sp.subpiece(0, p);
  return sp.str();
}

std::string Path::ext() const {
  auto s = name();
  if (s == "." || s == "..")
    return "";
  StringPiece sp(s);
  auto p = sp.rfind('.');
  if (p != StringPiece::npos && p != 0 && p+1 != sp.size())
    sp = sp.subpiece(p);
  else
    sp = "";
  return sp.str();
}

Path Path::replaceExt(const std::string& ext) const {
  Path p = parent();
  auto b = base();
  return p / (b + ext);
}

void Path::append(StringPiece sp) {
  if (sp.empty()) return;

  std::deque<StringPiece> q1, q2;
  if (!sp.startsWith('/'))
    splitTo<StringPiece>('/', path_, std::back_inserter(q1), true);
  splitTo<StringPiece>('/', sp, std::back_inserter(q2), true);

  while (!q1.empty() && q1.front() == ".")
    q1.pop_front();
  while (!q2.empty() && q2.front() == ".")
    q2.pop_front();

  while (!q2.empty()) {
    auto p = q2.front();
    if (p == "..") {
      if (q1.empty() || q1.back() == "..") q1.push_back(p);
      else q1.pop_back();
    } else if (p != ".") {
      q1.push_back(p);
    }
    q2.pop_front();
  }

  bool abs = false;
  if (isAbsolute() || sp.startsWith('/')) {
    while (!q1.empty() && q1.front() == "..")
      q1.pop_front();
    abs = true;
  }

  if (abs) {
    path_ = "/" + join('/', q1);
  } else {
    if (q1.empty()) {
      path_ = ".";
    } else {
      path_ = join('/', q1);
    }
  }
}

bool Path::checkMode(int mode) const {
  struct stat stat;
  ::stat(path_.c_str(), &stat);
  return (stat.st_mode & S_IFMT) == mode;
}

} // namespace rdd
