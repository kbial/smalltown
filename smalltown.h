#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>

#include "monster.h"

namespace fibonacciNumbers {

namespace {

template <typename U, size_t N> struct number {
  static const U take = number<U, N - 1>::take + number<U, N - 2>::take;
};

template <typename U> struct number<U, 1> { static const U take = 1; };

template <typename U> struct number<U, 0> { static const U take = 1; };

template <typename U, size_t N, size_t index = 0, U... C> struct array {
  static constexpr typename std::array<U, N> make() {
    return array<U, N, index + 1, C..., number<U, index>::take>::make();
  }
};

template <typename U, size_t N, U... C> struct array<U, N, N, C...> {
  static constexpr typename std::array<U, N> make() {
    return std::array<U, N>{{C...}};
  }
};
}

template <typename U, U t1, size_t N = 3>
constexpr typename std::enable_if<t1 <= number<U, N - 1>::take, size_t>::type
indexOfMaxFibonacciNumber() {
  return N;
}

template <typename U, U t1, size_t N = 3>
    constexpr typename std::enable_if <
    number<U, N - 1>::take<t1, size_t>::type indexOfMaxFibonacciNumber() {
  return indexOfMaxFibonacciNumber<U, t1, N + 1>();
}

template <typename U, size_t size>
static constexpr std::array<U, size> takeFibonacciNumbersArray() {
  return array<U, size>::make();
}
}

template <typename M, typename U, U t0, U t1, typename... C> class SmallTown {
private:
  M monster;
  U currentTime = t0;
  const U endTime = t1;
  std::tuple<C...> _citizens;
  size_t livingCitizens = 0;

  static_assert(t0 <= t1 && t0 >= 0, "INCORRECT STARTING OR ENDING TIME");

  template <std::size_t I = 0, typename... Tp>
  inline typename std::enable_if<I == sizeof...(Tp), void>::type
  iterateTuple(std::tuple<Tp...> &t, bool ifAttackTime) {}

  template <std::size_t I = 0, typename... Tp>
      inline typename std::enable_if <
      I<sizeof...(Tp), void>::type iterateTuple(std::tuple<Tp...> &t,
                                                bool ifAttackTime) {
    if (ifAttackTime)
      attack(monster, std::get<I>(t));
    if (std::get<I>(t).getHealth() > 0)
      livingCitizens++;
    iterateTuple<I + 1, Tp...>(t, ifAttackTime);
  }

  static bool isAttackTime(const U &time) {
    static constexpr auto fibonacciNumbersArray =
        fibonacciNumbers::takeFibonacciNumbersArray<
            U, fibonacciNumbers::indexOfMaxFibonacciNumber<U, t1>()>();
    auto foundLower = std::lower_bound(fibonacciNumbersArray.begin(),
                                       fibonacciNumbersArray.end(), time);
    return time == *foundLower;
  }

public:
  SmallTown(M monster, C... citizens)
      : monster(monster), _citizens(std::make_tuple(citizens...)) {
    iterateTuple(_citizens, false);
  }

  std::tuple<std::string, typename M::valueType, size_t> getStatus() const {
    return std::make_tuple(monster.getName(), monster.getHealth(),
                           livingCitizens);
  }

  void tick(const U &timeStep) {
    if (monster.getHealth() == 0 && livingCitizens == 0)
      std::cout << "DRAW" << std::endl;
    else if (monster.getHealth() == 0)
      std::cout << "CITIZENS WON" << std::endl;
    else if (livingCitizens == 0)
      std::cout << "MONSTER WON" << std::endl;
    else {
      if (isAttackTime(currentTime)) {
        livingCitizens = 0;
        iterateTuple(_citizens, true);
      }
      currentTime = currentTime + timeStep;
      if (currentTime > endTime)
        currentTime = currentTime % (endTime + 1);
    }
  }
};

#endif
