#pragma once

#include <SFML/Graphics.hpp>

class Tile{
public:
    void setTexture(sf::Texture* texture, sf::Texture* overlay, sf::Texture* flagTexture, sf::Texture* openTexture, double xLimit){
        this->texture = texture;
        this->openTexture = openTexture;
        this->flagTexture = flagTexture;
        this->overlay = overlay;
        this->sprite.setTexture(*this->texture);
        this->sOverlay.setTexture(*this->overlay);
        this->sprite.setScale(1024 / xLimit / 16, 1024 / xLimit / 16);
        this->sOverlay.setScale(1024 / xLimit / 16, 1024 / xLimit / 16);
    }
    sf::Texture* texture;
    sf::Texture* overlay;
    sf::Texture* flagTexture;
    sf::Sprite sprite, sOverlay;
    sf::Texture* openTexture;
    bool isBomb = false;
    int state = 0; // 0 - nothing, 1 - 1, 2 - 2, 3 - 3, 4 - 4, 5 - 5, 6 - 6, 7 - 7, 8 - 8, 9 - bomb
    bool isOpen = false;
    bool hasFlag = false;
    bool wasPressed = false;
    
    int setOpenTexture(std::vector<std::vector<Tile>>& tile, int x, int y,sf::Event event, sf::RenderWindow& window, int sizeX, int sizeY, int numOfBombs){
        if(event.type == sf::Event::MouseButtonPressed){
            wasPressed = true;
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
                    this->isOpen = true;
                    this->sprite.setTexture(*this->openTexture);
                    openNeighbors(tile, x, y, x, y, event, window);
                    if(tile[x][y].isBomb){
                        for(int k = 0; k < sizeX; k++){
                            for(int l = 0; l < sizeY; l++){
                                if(tile[k][l].isBomb){
                                    tile[k][l].sprite.setTexture(*tile[k][l].openTexture);
                                }
                            }
                        }
                        return 2;
                    }
                    int unopendCount = 0;
                    for(int i = 0; i < sizeX; i++){
                        for(int j = 0; j < sizeY; j++){
                            if(!tile[i][j].isOpen){
                                unopendCount++;
                            }
                        }
                    }
                    if(unopendCount == numOfBombs)
                        return 1;
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Right && wasPressed == true){
                if(sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
                    if(!isOpen && !hasFlag){
                        this->hasFlag = true;
                        this->sprite.setTexture(*this->flagTexture);
                    }
                    else if(!isOpen){
                        this->hasFlag = false;
                        this->sprite.setTexture(*this->texture);
                    }
                }
            }
            wasPressed = false;
        }
        return 0;
    }

    void openNeighbors(std::vector<std::vector<Tile>>& tile, int x, int y, int parentX, int parentY, sf::Event event, sf::RenderWindow& window){
        if(tile[x][y].state != 0)
            return;

        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                if(x + i >= 0 && x + i < tile.size() && y + j >= 0 && y + j < tile[0].size() && !tile[i + x][j + y].isOpen){
                    tile[i + x][j + y].sprite.setTexture(*tile[i + x][j + y].openTexture);
                    tile[i + x][j + y].isOpen = true;
                    openNeighbors(tile, i + x, j + y, x, y, event, window);
                }
            }
        }   
    }

    void hovering(sf::Event event, sf::RenderWindow& window){
        if(sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))){
            this->sOverlay.setPosition(this->sprite.getPosition());
            window.draw(sOverlay);
        }
    }

};
