/*
 * Copyright (C) 2017, Yeolar
 */

#pragma once

#include <string>
#include <sys/stat.h>
#include <boost/operators.hpp>

#include "raster/util/Conv.h"
#include "raster/util/Range.h"

namespace rdd {

class Path : private boost::totally_ordered<Path> {
public:
  Path() {}

  Path(const char* path) { operator/=(path); }
  Path(const std::string& path) { operator/=(path); }
  Path(StringPiece path) { operator/=(path); }

  Path& operator/=(const Path& path) { return operator/=(path.path_); }
  Path& operator/=(const char* path) { return operator/=(StringPiece(path)); }
  Path& operator/=(const std::string& p) { return operator/=(StringPiece(p)); }
  Path& operator/=(StringPiece path);

  Path& operator+=(const Path& path) { return operator+=(path.path_); }
  Path& operator+=(const char* path) { return operator+=(StringPiece(path)); }
  Path& operator+=(const std::string& p) { return operator+=(StringPiece(p)); }
  Path& operator+=(StringPiece path);

  const std::string& str() const { return path_; }
  const char* c_str() const { return path_.c_str(); }

  void swap(Path& path) {
    path_.swap(path.path_);
  }

  void clear() {
    path_.clear();
  }

  int compare(const Path& path) const {
    return StringPiece(path_).compare(StringPiece(path.path_));
  }

  bool empty() const { return path_.empty(); }
  bool isAbsolute() const { return StringPiece(path_).startsWith('/'); }
  bool isDirectory() const { return checkMode(S_IFDIR); }
  bool isFile() const { return checkMode(S_IFREG); }
  bool isLink() const { return checkMode(S_IFLNK); }
  bool isSocket() const { return checkMode(S_IFSOCK); }

  Path parent() const;
  std::string name() const;
  std::string base() const;
  std::string ext() const;

  Path replaceExt(const std::string& ext) const;

  Path(const Path&) = default;
  Path(Path&&) = default;

  Path& operator=(const Path&) & = default;
  Path& operator=(Path&&) & = default;

private:
  void append(StringPiece sp);

  bool checkMode(int mode) const;

  std::string path_;
};

inline void swap(Path& lhs, Path& rhs) { lhs.swap(rhs); }

inline Path operator/(const Path& lhs, const Path& rhs) {
  return Path(lhs) /= rhs;
}

inline Path operator+(const Path& lhs, const Path& rhs) {
  return Path(lhs) += rhs;
}

inline bool operator==(const Path& lhs, const Path& rhs) {
  return lhs.compare(rhs) == 0;
}

inline bool operator<(const Path& lhs, const Path& rhs) {
  return lhs.compare(rhs) < 0;
}

template <class Tgt>
typename std::enable_if<IsSomeString<Tgt>::value>::type
toAppend(const Path& value, Tgt* result) {
  result->append(value.str());
}

inline std::ostream& operator<<(std::ostream& os, const Path& path) {
  os << path.str();
  return os;
}

} // namespace rdd
