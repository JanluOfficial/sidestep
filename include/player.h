#ifndef PLAYER_H
#define PLAYER_H

class Player {
  public:
    int x;
    int score;
    Player() {
      x = 3; // Leftmost position = 0, Center position = 3, Rightmost position = 6
      score = 0;
    }
};

#endif // PLAYER_H
