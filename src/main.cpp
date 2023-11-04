#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "Tile.h"

void setupTiles(std::vector<std::vector<Tile>>& tile, float xLimit, double yLimit, double numOfBombs){
    for(int i = 0; i < xLimit; i++){
        std::vector<Tile> vec;
        for (int j = 0; j < yLimit; j++) {
            Tile newTile;
            newTile.sprite.setPosition(i * (1024 / xLimit), j * (1024 / yLimit) / (xLimit / yLimit));
            vec.push_back(newTile);
        }
        tile.push_back(vec);
    }
}

void setBombs(std::vector<std::vector<Tile>>& tile, int xLimit, int yLimit, int numOfBombs){
    srand(time(NULL));
    int bombCount = 0;
    while (bombCount < numOfBombs) {
        int x = rand() % xLimit;
        int y = rand() % yLimit;

        if (!tile[x][y].isBomb) {
            tile[x][y].isBomb = true;
            bombCount++;
        }
    }
}

void checkForBombs(std::vector<std::vector<Tile>>& tile, int x, int y){

    if(tile[x][y].isBomb){
        tile[x][y].state = 9;
        return;
    }

    int bombCount = 0;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(x + i >= 0 && x + i < tile.size() && y + j >= 0 && y + j < tile[0].size()){
                if(tile[i + x][j + y].isBomb){
                    bombCount++;
                }
            }
        }
    }
    tile[x][y].state = bombCount;
}


int main(){

    unsigned int SIZE_X = 16;
    unsigned int SIZE_Y = 10;
    unsigned int NUM_OF_BOMBS = 30;
    
    std::cout << "choose difficulty: 1 - easy, 2 - normal, 3 - hard\n";
    char x;
    std::cin >> x;
    switch(x){
    case '1':
        SIZE_X = 9;
        SIZE_Y = 9;
        NUM_OF_BOMBS = 10;
        break;
    case '2':
        SIZE_X = 16;
        SIZE_Y = 16;
        NUM_OF_BOMBS = 40;
        break;
    case '3':
        SIZE_X = 30;
        SIZE_Y = 16;
        NUM_OF_BOMBS = 99;
        break;
    }

    sf::RenderWindow window;
    window.setFramerateLimit(25);
    window.create(sf::VideoMode(1024, 1024), "okno");

    //textures---
    sf::Texture Blank, Flag, overlay, winTexture, screenTexture, loseTexture;
    sf::Texture numbers[10];
    Blank.loadFromFile("Blank.png");
    Flag.loadFromFile("Flag.png");
    overlay.loadFromFile("outline.png");
    winTexture.loadFromFile("win.png");
    loseTexture.loadFromFile("lost.png");
    screenTexture.loadFromFile("screen.png");

    sf::Sprite win, lose, screen;
    win.setTexture(winTexture);
    win.setScale(8, 8);
    win.setPosition(256, 256);
    lose.setTexture(loseTexture);
    lose.setScale(8, 8);
    lose.setPosition(256, 256);
    screen.setTexture(screenTexture);
    screen.setScale(8, 8);
    screen.setPosition(256, 256);

    for(int i = 0; i < 10; i++){
        std::string name = std::to_string(i) + ".png";
        numbers[i].loadFromFile(name);
    }
    //-----------

    std::vector<std::vector<Tile>> tile;
    setupTiles(tile, SIZE_X, SIZE_Y, NUM_OF_BOMBS);
    
    setBombs(tile, SIZE_X, SIZE_Y, NUM_OF_BOMBS);

    for(int i = 0; i < SIZE_X; i++){
        for(int j = 0; j < SIZE_Y; j++){
            checkForBombs(tile, i, j);
        }
    }
    for(int i = 0; i < SIZE_X; i++){
        for(int j = 0; j < SIZE_Y; j++){
            tile[i][j].setTexture(&Blank, &overlay, &Flag, &numbers[tile[i][j].state], SIZE_X);
        }
    }
    int winState = 0;
    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
        window.clear();
        for(int i = 0; i < SIZE_X; i++){
            for(int j = 0; j < SIZE_Y; j++){
                if(winState == 0){
                    winState = tile[i][j].setOpenTexture(tile, i, j, event, window, SIZE_X, SIZE_Y, NUM_OF_BOMBS);
                }
                window.draw(tile[i][j].sprite);
                tile[i][j].hovering(event, window);
            }
        }
        if(winState == 1){
            window.draw(screen);
            window.draw(win);
        }
        else if(winState == 2){
            window.draw(screen);
            window.draw(lose);
        }
        window.display();
        sf::sleep(sf::milliseconds(16));
    }
    return 0;
}
