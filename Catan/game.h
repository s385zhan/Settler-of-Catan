#ifndef __GAME_H__
#define __GAME_H__
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "input.h"

class Dice;
class Player;
class Tile;
class Path;
class Address;
class TextDisplay;


class Game {
    int numTurn;
    TextDisplay* td;
    int seed;
    bool seeded;
    std::map<std::string,Player*> players;
    std::vector<Tile*> tiles; // sorted
    std::vector<Path*> paths; // sorted
    std::vector<Address*> addresses; // sorted
    
    Dice * rollDice();
    bool eachPlay(Player* p);
    Dice * rollLoadedDice();
    bool beginPlay(Player *);
    void readin(std::istream& in, InputType i);
    
public:
    Game& operator=(Game& other); // move assignment operator
    // which does the same as move assignment operator
    Game& operator=(Game&& other);// move assignment operator
    void clear();
    void geese(Tile* t, Player* p);
    std::ostream& save (std::ostream &out);
    void printStatus();// Print the status of each palyer.
    Player* getPlayer(const std::string& s);
    Game (std::istream& in,InputType i,const int& seed,const bool& seeded);
    ~Game();
    bool round ();// Return true if someone won.
    bool beginning ();
    void restart(std::istream& in);// Restart the game.
    void print(); // Text display the board.
};


class GameFailure{
public:
    GameFailure();
}; 
#endif  
