#ifndef MONSTER_H
#define MONSTER_H

#include "citizen.h"

namespace {
const static char zombieName[] = "Zombie";
const static char mummyName[] = "Mummy";
const static char vampireName[] = "Vampire";
}

template <typename T, const char *name,
          typename =
              typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class Monster {
private:
  T health;
  T attackPower;

public:
  typedef T valueType;

  Monster(T health, T attackPower) : health(health), attackPower(attackPower) {}

  T getHealth() const { return health; }

  T getAttackPower() const { return attackPower; }

  void takeDamage(T damage) {
    if (health < damage)
      health -= health;
    else
      health -= damage;
  }

  const char *getName() const { return name; }
};

template <typename T> using Zombie = Monster<T, zombieName>;
template <typename T> using Vampire = Monster<T, vampireName>;
template <typename T> using Mummy = Monster<T, mummyName>;

template <typename M, typename U> void attack(M &monster, U &victim) {
  victim.takeDamage(monster.getAttackPower());
}

template <typename M, typename T> void attack(M &monster, Sheriff<T> &sheriff) {
  if (sheriff.getHealth() != 0)
    monster.takeDamage(sheriff.getAttackPower());
  sheriff.takeDamage(monster.getAttackPower());
}

#endif
