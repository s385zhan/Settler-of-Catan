#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include <vector>
#include "observer.h"
#include "subscription.h"

class Subject;


class TextDisplay: public Observer {
  std::vector<std::vector<char>> theDisplay;
 public:
  TextDisplay();

  void notify(Subject& s, SubscriptionType t) override;

  ~TextDisplay();

  void print(); // print out board
};


#endif
