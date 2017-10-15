#ifndef CITIZEN_H
#define CITIZEN_H

#include <cassert>
#include <type_traits>

template <typename T, int minAge, int maxAge, bool canAttack,
          typename =
              typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Citizen {
private:
  T health;
  T age;
  T attackPower;

public:
  template <typename U = T>
  Citizen(T health, T age,
          typename std::enable_if<canAttack == false, U>::type * = 0)
      : health(health), age(age) {
    assert(minAge <= age && age <= maxAge);
  }

  template <typename U = T>
  Citizen(T health, T age, T attackPower,
          typename std::enable_if<canAttack == true, U>::type * = 0)
      : health(health), age(age), attackPower(attackPower) {
    assert(minAge <= age && age <= maxAge);
  }

  T getHealth() const { return health; }
  T getAge() const { return age; }

  void takeDamage(T damage) {
    if (health <= damage)
      health -= health;
    else
      health -= damage;
  }

  template <typename U = T>
  typename std::enable_if<canAttack, U>::type getAttackPower() const {
    return attackPower;
  }
};

template <typename T> using Adult = Citizen<T, 18, 100, false>;
template <typename T> using Teenager = Citizen<T, 11, 17, false>;
template <typename T> using Sheriff = Citizen<T, 18, 100, true>;

#endif
