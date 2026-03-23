#ifndef PLAYER_H
#define PLAYER_H

class Player {
  public:
    int x = 3;
    int score = 0;
    int rempen = 3; // Remaining penalties
    float rempen_cooldown = 0;
};

#endif // PLAYER_H
