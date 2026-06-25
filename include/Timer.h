#ifndef TIMER_H
#define TIMER_H

class Timer {
public:
  Timer();
  void Update(float dt); // acumula dt 
  void Restart();        // zera
  float Get() const;     // tempo acumulado 

private:
  float time;
};

#endif
