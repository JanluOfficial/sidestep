#ifndef SCENE_H
#define SCENE_H

class BaseScene {
public:
  // virtual voids
  // note to self, "virtual" lets the compiler know to use the
  // derived versions (like from MenuScene, GameScene etc)
  virtual ~BaseScene() {}         // Needed for proper cleanup I guess

  virtual void Setup() = 0;       // Runs once before everything else
  virtual void Update() = 0;      // Game Logic, runs repeatedly
  virtual void Draw() = 0;        // Draw Logic, runs repeatedly
  virtual void Teardown() = 0;    // Runs when scene gets destroyed (switching to another one for example)
};

#endif // SCENE_H
