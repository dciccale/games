#include "Ship.h"

Ship::Ship(void) {
  // set the health (at least)
  health = 100;
}

Ship* Ship::createShip(const char* pszFilename) {
  Ship *sprite = new Ship();

  if (sprite && sprite->initWithFile(pszFilename)) {
    sprite->autorelease();
    return sprite;
  }
  CC_SAFE_DELETE(sprite);
  return NULL;
}