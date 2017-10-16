// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FLATDICTTEST_RDD_FBS_H_
#define FLATBUFFERS_GENERATED_FLATDICTTEST_RDD_FBS_H_

#include "flatbuffers/flatbuffers.h"

namespace rdd {
namespace fbs {

struct Person;

struct Person FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_CITY = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *city() const {
    return GetPointer<const flatbuffers::String *>(VT_CITY);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyOffset(verifier, VT_CITY) &&
           verifier.Verify(city()) &&
           verifier.EndTable();
  }
};

struct PersonBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Person::VT_NAME, name);
  }
  void add_city(flatbuffers::Offset<flatbuffers::String> city) {
    fbb_.AddOffset(Person::VT_CITY, city);
  }
  PersonBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  PersonBuilder &operator=(const PersonBuilder &);
  flatbuffers::Offset<Person> Finish() {
    const auto end = fbb_.EndTable(start_, 2);
    auto o = flatbuffers::Offset<Person>(end);
    return o;
  }
};

inline flatbuffers::Offset<Person> CreatePerson(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> city = 0) {
  PersonBuilder builder_(_fbb);
  builder_.add_city(city);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Person> CreatePersonDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *city = nullptr) {
  return rdd::fbs::CreatePerson(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      city ? _fbb.CreateString(city) : 0);
}

}  // namespace fbs
}  // namespace rdd

#endif  // FLATBUFFERS_GENERATED_FLATDICTTEST_RDD_FBS_H_