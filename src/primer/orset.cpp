#include "primer/orset.h"
#include <algorithm>
#include <string>
#include <vector>
#include "common/exception.h"
#include "fmt/format.h"

namespace bustub {

template <typename T>
auto ORSet<T>::Contains(const T &elem) const -> bool {
  // TODO(student): Implement this
  bool b = 0;
  for (std::vector<int>::size_type i = 0; i < E.size(); i++) {
    if (E[i] == elem) {
      b = 1;
      break;
    }
  }
  return b;
  // throw NotImplementedException("ORSet<T>::Contains is not implemented");
}

template <typename T>
void ORSet<T>::Add(const T &elem, uid_t uid) {
  E.push_back(elem);
  Ve.push_back(uid);
  // TODO(student): Implement this
  // throw NotImplementedException("ORSet<T>::Add is not implemented");
}

template <typename T>
void ORSet<T>::Remove(const T &elem) {
  // TODO(student): Implement this
  std::vector<int>::size_type i = 0;
  T temp;
  for (i = 0; i < E.size(); i++) {
    temp = E[i];
    if (temp == elem) {
      D.push_back(temp);
      Vd.push_back(Ve[i]);
      E.erase(E.begin() + i);
      Ve.erase(Ve.begin() + i);
    }
  }
  // throw NotImplementedException("ORSet<T>::Remove is not implemented");
}
template <typename T>
int ORSet<T>::ContainE(const T &elem, uid_t uid) {
  std::vector<int>::size_type k = 0;
  for (k = 0; k < E.size(); k++) {
    if (E[k] == elem && Ve[k] == uid) {
      return k;
      break;
    }
  }
  return -1;
}

template <typename T>
int ORSet<T>::ContainD(const T &elem, uid_t uid) {
  std::vector<int>::size_type k = 0;
  for (k = 0; k < D.size(); k++) {
    if (D[k] == elem && Vd[k] == uid) {
      return k;
      break;
    }
  }
  return -1;
}

template <typename T>
void ORSet<T>::Merge(const ORSet<T> &other) {
  // TODO(student): Implement this
  for (std::vector<int>::size_type i = 0; i < other.E.size(); i++) {
    T temp = other.E[i];
    uid_t uid = other.Ve[i];
    if (ContainE(temp, uid) == -1 && ContainD(temp, uid) == -1) {
      E.push_back(temp);
      Ve.push_back(uid);
    }
  }  // 去除other的E中与E和D的重复值
  for (std::vector<int>::size_type i = 0; i < other.D.size(); i++) {
    T temp = other.D[i];
    uid_t uid = other.Vd[i];
    int j = ContainE(temp, uid);
    int k = ContainD(temp, uid);
    if (j != -1) {
      E.erase(E.begin() + j);
      Ve.erase(Ve.begin() + j);
    }
    if (k == -1) {
      D.push_back(temp);
      Vd.push_back(uid);
    }
  }  // 去除E中与other.D的重复值，以及去除D中与other.D的重复值
  // throw NotImplementedException("ORSet<T>::Merge is not implemented");
}

template <typename T>
auto ORSet<T>::Elements() const -> std::vector<T> {
  // TODO(student): Implement this
  return E;
  // throw NotImplementedException("ORSet<T>::Elements is not implemented");
}

template <typename T>
auto ORSet<T>::ToString() const -> std::string {
  auto elements = Elements();
  std::sort(elements.begin(), elements.end());
  return fmt::format("{{{}}}", fmt::join(elements, ", "));
}

template class ORSet<int>;
template class ORSet<std::string>;

}  // namespace bustub
