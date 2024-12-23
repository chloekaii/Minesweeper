#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "window.h"
#include "board.h"
#include <cmath>

using namespace std;

Board ReadBoardConfig() {
    ifstream file("files/board_config.cfg");
    if (file.is_open()) {
        int numColumns;
        int numRows;
        int numMines;

        string temp;
        getline(file, temp);
        numColumns = stoi(temp);

        getline(file, temp);
        numRows = stoi(temp);

        getline(file, temp);
        numMines = stoi(temp);

        Board board = Board(numColumns, numRows, numMines);
        return board;
    } else {
        cout << "board_config.cfg failed to open" << endl;
        exit(0);
    }
}

void LoadAssets(map<string, sf::Texture>& assets) {
    sf::Texture texture;
    texture.loadFromFile("files/images/debug.png");
    assets.emplace("debug", texture);
    texture.loadFromFile("files/images/digits.png");
    assets.emplace("digits", texture);
    texture.loadFromFile("files/images/face_happy.png");
    assets.emplace("face_happy", texture);
    texture.loadFromFile("files/images/face_lose.png");
    assets.emplace("face_lose", texture);
    texture.loadFromFile("files/images/face_win.png");
    assets.emplace("face_win", texture);
    texture.loadFromFile("files/images/flag.png");
    assets.emplace("flag", texture);
    texture.loadFromFile("files/images/leaderboard.png");
    assets.emplace("leaderboard", texture);
    texture.loadFromFile("files/images/mine.png");
    assets.emplace("mine", texture);
    texture.loadFromFile("files/images/number_1.png");
    assets.emplace("1", texture);
    texture.loadFromFile("files/images/number_2.png");
    assets.emplace("2", texture);
    texture.loadFromFile("files/images/number_3.png");
    assets.emplace("3", texture);
    texture.loadFromFile("files/images/number_4.png");
    assets.emplace("4", texture);
    texture.loadFromFile("files/images/number_5.png");
    assets.emplace("5", texture);
    texture.loadFromFile("files/images/number_6.png");
    assets.emplace("6", texture);
    texture.loadFromFile("files/images/number_7.png");
    assets.emplace("7", texture);
    texture.loadFromFile("files/images/number_8.png");
    assets.emplace("8", texture);
    texture.loadFromFile("files/images/pause.png");
    assets.emplace("pause", texture);
    texture.loadFromFile("files/images/play.png");
    assets.emplace("play", texture);
    texture.loadFromFile("files/images/tile_hidden.png");
    assets.emplace("tile_hidden", texture);
    texture.loadFromFile("files/images/tile_revealed.png");
    assets.emplace("tile_revealed", texture);
}

void GenerateSprite(sf::Sprite& sprite, map<string, sf::Texture>& assets, const string& texture, int x, int y) {
    sprite.setTexture(assets.at(texture));
    sprite.setPosition(x, y);
}

void SetMines(vector<vector<Tile>>& tiles, int numCols, int numRows, int numMines) {
    vector<vector<int>> randIndexes;
    srand(time(nullptr));
    for (int i=0; i < numMines; i++) {
        vector<int> indexes;
        int colIndex = rand() % numCols;
        int rowIndex = rand() % numRows;
        indexes.push_back(rowIndex);
        indexes.push_back(colIndex);
        if (find(randIndexes.begin(), randIndexes.end(), indexes) == randIndexes.end()) {
            randIndexes.push_back(indexes);
        } else {
            i--;
        }
    }
    for (auto & randIndex : randIndexes) {
        tiles[randIndex[0]][randIndex[1]].SetMine();
    }
}

void ConnectAdjacentTiles(Board& board, vector<vector<Tile>>& tiles) {
    for (int i=0; i < board.numRows; i++) {
        for (int j=0; j < board.numCols; j++) {
            Tile& tile = tiles[i][j];
            if (i > 0) {
                tile.adjacentTiles.push_back(&tiles[i - 1][j]);
                if (j > 0) {
                    tile.adjacentTiles.push_back(&tiles[i - 1][j - 1]);
                }
                if (j < board.numCols - 1) {
                    tile.adjacentTiles.push_back(&tiles[i - 1][j + 1]);
                }
            }
            if (i < board.numRows - 1) {
                tile.adjacentTiles.push_back(&tiles[i + 1][j]);
                if (j > 0) {
                    tile.adjacentTiles.push_back(&tiles[i + 1][j - 1]);
                }
                if (j < board.numCols - 1) {
                    tile.adjacentTiles.push_back(&tiles[i + 1][j + 1]);
                }
            }
            if (j > 0) {
                tile.adjacentTiles.push_back(&tiles[i][j - 1]);
            }
            if (j < board.numCols - 1) {
                tile.adjacentTiles.push_back(&tiles[i][j + 1]);
            }
        }
    }
}

void LoadAllMines(vector<sf::Sprite>& hiddenMines, Board& board, map<string, sf::Texture>& assets) {
    hiddenMines.clear();
    for (int i=0; i < board.numRows; i++) {
        for (int j=0; j < board.numCols; j++) {
            Tile& tile = board.tiles[i][j];
            if (tile.TileHasMine()) {
                sf::Sprite bombSprite;
                GenerateSprite(bombSprite, assets, "mine", tile.col * 32, tile.row * 32);
                hiddenMines.push_back(bombSprite);
            }
        }
    }
}

void GenerateTiles(Board& board, map<string, sf::Texture>& assets, vector<vector<sf::Sprite>>& tileSprites) {
    tileSprites.clear();
    board.tiles.clear();
    for (int i=0; i < board.numRows; i++) {
        vector<Tile> row;
        vector<sf::Sprite> spriteRow;
        for (int j=0; j < board.numCols; j++) {
            Tile tile = Tile(i, j);
            row.push_back(tile);
            sf::Sprite tileSprite;
            GenerateSprite(tileSprite, assets, "tile_hidden", tile.col * 32, tile.row * 32);
            spriteRow.push_back(tileSprite);
        }
        board.tiles.push_back(row);
        tileSprites.push_back(spriteRow);
    }
    SetMines(board.tiles, board.numCols, board.numRows, board.numMines);
}

void LoadNumbers(Board& board, map<string, sf::Texture>& assets, vector<vector<sf::Sprite>>& numberSprites) {
    numberSprites.clear();
    for (int i=0; i < board.numRows; i++) {
        vector<sf::Sprite> numberRow;
        for (int j=0; j < board.numCols; j++) {
            sf::Sprite numSprite;
            if (board.tiles[i][j].AdjacentMines() != 0 and !board.tiles[i][j].TileHasMine()) {
                string num = to_string(board.tiles[i][j].AdjacentMines());
                GenerateSprite(numSprite, assets, num, board.tiles[i][j].xCoord, board.tiles[i][j].yCoord);
                numberRow.push_back(numSprite);
            } else {
                numberRow.push_back(numSprite);
            }
        }
        numberSprites.push_back(numberRow);
    }
}

void LoadDigits(map<string, sf::Texture>& digits, Board& board) {
    string file = "files/images/digits.png";
    for (int i=0; i < 10; i++) {
        sf::Texture texture;
        texture.loadFromFile(file, sf::IntRect(i * 21, 0, 21, 32));
        digits.emplace(to_string(i), texture);
    }
    sf::Texture texture;
    texture.loadFromFile(file, sf::IntRect(210, 0, 21, 32));
    digits.emplace("-", texture);
}

void SetCounter(int minesLeft, vector<sf::Sprite>& counter, map<string, sf::Texture>& digitTextures, Board& board) {
    std::stringstream temp;
    temp << minesLeft;
    counter.clear();
    sf::Sprite sprite1;
    sf::Sprite sprite2;
    sf::Sprite sprite3;
    int dig = temp.str()[0] - '0';

    if (minesLeft < 100) {
        GenerateSprite(sprite1, digitTextures, "0", 33, 32 * ((board.numRows) + 0.5f) + 16);
        counter.push_back(sprite1);
        if (minesLeft < 10) {
            GenerateSprite(sprite2, digitTextures, "0", 33 + 21, 32 * ((board.numRows) + 0.5f) + 16);
            counter.push_back(sprite2);
            if (minesLeft == 0) {
                GenerateSprite(sprite3, digitTextures, "0", 33 + 42, 32 * ((board.numRows) + 0.5f) + 16);
                counter.push_back(sprite3);
            } else {
                string str = to_string(dig);
                GenerateSprite(sprite3, digitTextures, str, 33 + 42, 32 * ((board.numRows) + 0.5f) + 16);
                counter.push_back(sprite3);
            }
        } else {
            string str = to_string(dig);
            GenerateSprite(sprite2, digitTextures, str, 33 + 21, 32 * ((board.numRows) + 0.5f) + 16);
            counter.push_back(sprite2);

            dig = temp.str()[1] - '0';
            str = to_string(dig);
            GenerateSprite(sprite3, digitTextures, str, 33 + 42, 32 * ((board.numRows) + 0.5f) + 16);
            counter.push_back(sprite3);
        }
    } else {
        string str = to_string(dig);
        GenerateSprite(sprite1, digitTextures, str, 33, 32 * ((board.numRows) + 0.5f) + 16);
        counter.push_back(sprite1);

        dig = temp.str()[1] - '0';
        str = to_string(dig);
        GenerateSprite(sprite2, digitTextures, str, 33 + 21, 32 * ((board.numRows) + 0.5f) + 16);
        counter.push_back(sprite2);

        dig = temp.str()[2] - '0';
        str = to_string(dig);
        GenerateSprite(sprite3, digitTextures, str, 33 + 42, 32 * ((board.numRows) + 0.5f) + 16);
        counter.push_back(sprite3);
    }
}

bool CheckWin(vector<vector<Tile>>& tiles) {
    for (int i=0; i < tiles.size(); i++) {
        for (int j=0; j < tiles[i].size(); j++) {
            if (tiles[i][j].IsFlagged() and !tiles[i][j].TileHasMine()) {
                return false;
            } else if (!tiles[i][j].TileIsRevealed() and !tiles[i][j].TileHasMine()) {
                return false;
            }
        }
    }
    return true;
}

sf::Text CreateText(sf::Font& font, string str, unsigned int size) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    return text;
}

void CenterText(sf::Text& text, Window& window, int y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(window.width / 2.0f, (window.height / 2.0f) + y));
}

void SetTimer(int seconds, vector<sf::Sprite>& timer, map<string, sf::Texture>& digitTextures, Board& board) {
    int minutes = floor(seconds / 60);
    int temp = floor(seconds);
    int newSeconds = temp % 60;

    int min1val;
    int min2val;
    if (minutes > 0) {
        min2val = minutes % 10;
        min1val = floor(minutes / 10);
    } else {
        min2val = 0;
        min1val = 0;
    }

    int sec1val;
    int sec2val;
    if (newSeconds > 0) {
        sec1val = floor(newSeconds / 10);
        sec2val = newSeconds % 10;
    } else {
        sec1val = 0;
        sec2val = 0;
    }

    sf::Sprite min1;
    GenerateSprite(min1, digitTextures, to_string(min1val), (board.numCols * 32) - 97, 32 * (board.numRows + 0.5f) + 16);
    sf::Sprite min2;
    GenerateSprite(min2, digitTextures, to_string(min2val), (board.numCols * 32) - 76, 32 * (board.numRows + 0.5f) + 16);
    sf::Sprite sec1;
    GenerateSprite(sec1, digitTextures, to_string(sec1val), (board.numCols * 32) - 54, 32 * (board.numRows + 0.5f) + 16);
    sf::Sprite sec2;
    GenerateSprite(sec2, digitTextures, to_string(sec2val), (board.numCols * 32) - 33, 32 * (board.numRows + 0.5f) + 16);
    timer[0] = min1;
    timer[1] = min2;
    timer[2] = sec1;
    timer[3] = sec2;
}

void ReadLeaderboardFile(map<int, map<string, int>>& currentEntries, fstream& file) {
    currentEntries.clear();
    string line;
    int entry = 1;
    while (getline(file, line)) {
        int mins;
        int secs;
        string name;

        istringstream stream(line);
        string token;
        getline(stream, token, ':');
        if (token[0] != '0') {
            mins = stoi(token);
        } else {
            token = token[1];
            mins = stoi(token);
        }

        getline(stream, token, ',');
        if (token[0] != '0') {
            secs = stoi(token);
        } else {
            token = token[1];
            secs = stoi(token);
        }

        getline(stream, name);
        int totalSecs = (mins * 60) + secs;
        map<string, int> player;
        player.emplace(name, totalSecs);
        currentEntries.emplace(entry, player);
        entry++;
    }
}

int EditLeaderboard(map<int, map<string, int>>& currentEntries, map<string, int>& currentGame) {
    map<int, map<string, int>> temp = currentEntries;
    int lineReplaced = 10; //placeholder value
    for (auto i=currentEntries.begin(); i != currentEntries.end(); i++) {
        if (currentGame.begin()->second < i->second.begin()->second) {
            currentEntries[i->first] = currentGame;
            lineReplaced = i->first;
            break;
        }
    }
    if (lineReplaced != 10) {
        for (int i=lineReplaced; i < 5; i++) {
            currentEntries[i + 1] = temp.at(i);
        }
        ofstream file;
        file.open("files/leaderboard.txt");
        for (auto & currentEntry : currentEntries) {
            string str;
            int mins;
            int secs;

            mins = floor(currentEntry.second.begin()->second / 60);
            secs = currentEntry.second.begin()->second % 60;

            string minStr;
            string secStr;
            if (mins < 10) {
                minStr = "0" + to_string(mins);
            } else {
                minStr = to_string(mins);
            }
            if (secs < 10) {
                secStr = "0" + to_string(secs);
            } else {
                secStr = to_string(secs);
            }

            str = minStr + ":" + secStr + "," + currentEntry.second.begin()->first;
            file << str << "\n";
        }
    }
    return lineReplaced;
}

int main() {
    Board board = ReadBoardConfig();
    Window welcome = Window((board.numCols * 32), board.numRows * 32 + 100);
    sf::RenderWindow welcomeWindow(sf::VideoMode(welcome.width, welcome.height), "Minesweeper");

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    sf::Text welcomeText = CreateText(font, "WELCOME TO MINESWEEPER!", 24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    CenterText(welcomeText, welcome, -150);

    sf::Text inputPromptText = CreateText(font, "Enter your name:", 20);
    CenterText(inputPromptText, welcome, -75);

    string playerInput = "|";
    sf::Text playerName = CreateText(font, playerInput, 18);
    playerName.setFillColor(sf::Color::Yellow);
    playerName.setStyle(sf::Text::Bold);
    CenterText(playerName, welcome, -45);

    bool displayGame = false;

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        welcomeWindow.setFramerateLimit(60);

        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (isalpha(static_cast<char>(event.text.unicode)) and playerInput.length() < 11) {
                        playerInput.insert(playerInput.length() - 1, 1, static_cast<char>(event.text.unicode));
                        playerName.setString(playerInput);
                        CenterText(playerName, welcome, -45);
                    } else if (event.text.unicode == '\b' and playerInput.size() > 1) {
                        playerInput.erase(playerInput.size() - 2, 1);
                        playerName.setString(playerInput);
                        CenterText(playerName, welcome, -45);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Return and playerInput.size() > 1) {
                    string temp;
                    for (unsigned int i=0; i < playerInput.size() - 1; i++) {
                        if (i > 0) {
                            temp.push_back(tolower(playerInput.at(i)));
                        } else {
                            temp.push_back(toupper(playerInput.at(i)));
                        }
                    }
                    playerInput = temp;
                    displayGame = true;
                    welcomeWindow.close();
                }
            }
            welcomeWindow.clear(sf::Color::Blue);

            welcomeWindow.draw(welcomeText);
            welcomeWindow.draw(inputPromptText);
            welcomeWindow.draw(playerName);

            welcomeWindow.display();
        }
    }

    if (displayGame) {
        bool win = false;
        bool lose = false;
        bool showLeaderboard = false;
        bool leaderboardEdited = false;
        bool timing = true;
        int seconds;
        int editedEntry = 10;

        sf::RenderWindow gameWindow(sf::VideoMode(welcome.width, welcome.height), "Minesweeper");
        gameWindow.setFramerateLimit(30);

        map<string, sf::Texture> assets;
        LoadAssets(assets);
        vector<vector<sf::Sprite>> tileSprites;
        map<int, sf::Sprite> flags;
        vector<sf::Sprite> hiddenMines;
        vector<vector<sf::Sprite>> numberSprites;
        map<string, sf::Texture> digitTextures;
        vector<sf::Sprite> counter;
        vector<sf::Sprite> timer;
        map<int, map<string, int>> currentEntries;

        GenerateTiles(board, assets, tileSprites);
        LoadAllMines(hiddenMines, board, assets);
        ConnectAdjacentTiles(board, board.tiles);
        LoadNumbers(board, assets, numberSprites);
        LoadDigits(digitTextures, board);

        vector<vector<Tile>>& tiles = board.tiles;  //here to declutter the rest of main()
        int minesLeft = board.numMines;

        sf::Sprite dig1;
        GenerateSprite(dig1, digitTextures, "0", (board.numCols * 32) - 97, 32 * (board.numRows + 0.5f) + 16);
        sf::Sprite dig2;
        GenerateSprite(dig2, digitTextures, "0", (board.numCols * 32) - 76, 32 * (board.numRows + 0.5f) + 16);
        sf::Sprite dig3;
        GenerateSprite(dig3, digitTextures, "0", (board.numCols * 32) - 54, 32 * (board.numRows + 0.5f) + 16);
        sf::Sprite dig4;
        GenerateSprite(dig4, digitTextures, "0", (board.numCols * 32) - 33, 32 * (board.numRows + 0.5f) + 16);
        timer.push_back(dig1);
        timer.push_back(dig2);
        timer.push_back(dig3);
        timer.push_back(dig4);

        Button debugButton = Button((board.numCols * 32) - 304, 32 * (board.numRows + 0.5f));
        sf::Sprite debugSprite;
        GenerateSprite(debugSprite, assets, "debug", debugButton.xCoord, debugButton.yCoord);

        Button pauseButton = Button((board.numCols * 32) - 240, 32 * (board.numRows + 0.5f));
        sf::Sprite pauseButtonSprite;
        GenerateSprite(pauseButtonSprite, assets, "pause", pauseButton.xCoord, pauseButton.yCoord);

        Button leaderboardButton = Button((board.numCols * 32) - 176, 32 * (board.numRows + 0.5f));
        sf::Sprite showLeaderboardSprite;
        GenerateSprite(showLeaderboardSprite, assets, "leaderboard", leaderboardButton.xCoord, leaderboardButton.yCoord);

        Button resetButton = Button(((board.numCols / 2) * 32) - 32,  32 * (board.numRows + 0.5f));
        sf::Sprite resetButtonSprite;
        GenerateSprite(resetButtonSprite, assets, "face_happy", resetButton.xCoord, resetButton.yCoord);

        sf::Clock clock;
        sf::Clock pauseClock;
        sf::Clock leaderboardClock;
        float tempTime = 0;
        float pausedTime = 0;
        float leaderboardTime = 0;

        while (gameWindow.isOpen()) {
            sf::Event event;

            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                } else if (event.type == sf::Event::MouseButtonReleased or event.type == sf::Event::KeyPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                        if (debugButton.ButtonWasClicked(mousePos.x, mousePos.y) and !pauseButton.clicked
                            and !win and !lose) {
                            debugButton.OnClick();
                        } else if (leaderboardButton.ButtonWasClicked(mousePos.x,  mousePos.y)) {
                            leaderboardClock.restart();
                            showLeaderboard = true;
                            leaderboardButton.clicked = true;
                        } else if (resetButton.ButtonWasClicked(mousePos.x, mousePos.y)) {
                            clock.restart();
                            GenerateTiles(board, assets, tileSprites);
                            LoadAllMines(hiddenMines, board, assets);
                            ConnectAdjacentTiles(board, board.tiles);
                            LoadNumbers(board, assets, numberSprites);
                            minesLeft = board.numMines;
                            tiles = board.tiles;

                            pauseButton.clicked = false;
                            pausedTime = 0;
                            tempTime = 0;
                            leaderboardTime = 0;
                            win = false;
                            lose = false;
                            leaderboardEdited = false;
                            editedEntry = 10;
                            for (auto i=flags.begin();  i != flags.end(); i++) {
                                flags.erase(i);
                            }
                            resetButtonSprite.setTexture(assets.at("face_happy"));
                        } else if (pauseButton.ButtonWasClicked(mousePos.x, mousePos.y) and !win and !lose) {
                            pauseButton.OnClick();
                            if (pauseButton.clicked) {
                                pauseButtonSprite.setTexture(assets.at("play"));
                                tempTime += pausedTime;
                                pauseClock.restart();
                            } else {
                                pauseButtonSprite.setTexture(assets.at("pause"));
                            }
                        } else if (!win and !lose) {
                            for (int i=0; i < board.numRows; i ++) {
                                for (int j=0; j < board.numCols; j++) {
                                    Tile& tile = tiles[i][j];
                                    if (tile.TileWasClicked(mousePos.x, mousePos.y) and !pauseButton.clicked) {
                                        tile.OnClick();
                                    }
                                }
                            }
                        }
                    }

                    if ((event.mouseButton.button == sf::Mouse::Right or sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                        and !pauseButton.clicked and !win and !lose) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                        for (int i=0; i < board.numRows; i++) {
                            for (int j=0; j < board.numCols; j++) {
                                Tile& tile = tiles[i][j];
                                if (tile.TileWasClicked(mousePos.x, mousePos.y) and !tile.TileIsRevealed()) {
                                    tile.OnRightClick();
                                    if (tile.IsFlagged()) {
                                        sf::Sprite flag;
                                        GenerateSprite(flag, assets, "flag", tile.xCoord, tile.yCoord);
                                        flags.emplace((i * board.numCols) + j, flag);
                                        minesLeft--;
                                    } else {
                                        flags.erase((i * board.numCols) + j);
                                        minesLeft++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (int i=0; i < board.numRows; i++) {
                for (int j=0; j < board.numCols; j++) {
                    Tile& tile = tiles[i][j];
                    if (tile.TileIsRevealed()) {
                        tileSprites[i][j].setTexture(assets.at("tile_revealed"));
                        if (tile.TileHasMine()) {
                            lose = true;
                            resetButtonSprite.setTexture(assets.at("face_lose"));
                        }
                    }
                }
            }

            if (minesLeft >= 0) {
                SetCounter(minesLeft, counter, digitTextures, board);
            } else {
                SetCounter(minesLeft * -1, counter, digitTextures, board);
                sf::Sprite negSymbol;
                GenerateSprite(negSymbol, digitTextures, "-", 12, 32 * ((board.numRows) + 0.5f) + 16);
                counter.insert(counter.begin(), negSymbol);
            }

            win = CheckWin(tiles);
            if (win) {
                fstream file;
                file.open("files/leaderboard.txt");
                ReadLeaderboardFile(currentEntries, file);
                map<string, int> currentGame;
                currentGame.emplace(playerInput, seconds);
                if (!leaderboardEdited) {
                    editedEntry = EditLeaderboard(currentEntries, currentGame);
                    leaderboardEdited = true;
                }
                resetButtonSprite.setTexture(assets.at("face_win"));
                for (int i=0; i < board.numRows; i++) {
                    for (int j=0; j < board.numCols; j++) {
                        if (tiles[i][j].TileHasMine() and !tiles[i][j].IsFlagged()) {
                            sf::Sprite flag;
                            GenerateSprite(flag, assets, "flag", tiles[i][j].xCoord, tiles[i][j].yCoord);
                            flags.emplace((i * board.numCols) + j, flag);
                            minesLeft = 0;
                        }
                    }
                }
            }

            if (pauseButton.clicked) {
                pausedTime = pauseClock.getElapsedTime().asSeconds();
            }
            if (win or lose) {
                SetTimer(seconds, timer, digitTextures, board);
            } else {
                sf::Time time_passed = clock.getElapsedTime();
                seconds = floor(time_passed.asSeconds() - pausedTime - tempTime);
                SetTimer(seconds, timer, digitTextures, board);
            }

            gameWindow.clear();

            if (pauseButton.clicked or leaderboardButton.clicked) {
                for (int i=0; i < board.numRows; i++) {
                    for (int j=0; j < board.numCols; j++) {
                        sf::Sprite revealedTile;
                        GenerateSprite(revealedTile, assets, "tile_revealed", j * 32, i * 32);
                        gameWindow.draw(revealedTile);
                    }
                }
            } else {
                for (int i=0; i < board.numRows; i++) {
                    for (int j=0; j < board.numCols; j++) {
                        gameWindow.draw(tileSprites[i][j]);
                        if (tiles[i][j].TileIsRevealed()) {
                            gameWindow.draw(numberSprites[i][j]);
                        }
                    }
                }
                for (auto i = flags.begin(); i != flags.end(); i++) {
                    gameWindow.draw(i->second);
                }
                if (lose or debugButton.clicked) {
                    for (int i=0; i < hiddenMines.size(); i++) {
                        gameWindow.draw(hiddenMines.at(i));
                    }
                }
            }
            for (int i=0; i < counter.size(); i++) {
                gameWindow.draw(counter[i]);
            }
            gameWindow.draw(debugSprite);
            gameWindow.draw(pauseButtonSprite);
            gameWindow.draw(resetButtonSprite);
            gameWindow.draw(showLeaderboardSprite);

            //timer test
            for (const auto & i : timer) {
                gameWindow.draw(i);
            }

            gameWindow.display();

            if (showLeaderboard) {

                Window leaderboard = Window(welcome.width / 2, welcome.height / 2 + 50);
                sf::RenderWindow leaderboardWin(sf::VideoMode(welcome.width / 2, welcome.height / 2 + 50),
                                                "Leaderboard");
                leaderboardWin.setFramerateLimit(30);

                //reads leaderboard file
                fstream file;
                file.open("files/leaderboard.txt");
                ReadLeaderboardFile(currentEntries, file);

                sf::Text leaderboardText = CreateText(font, "LEADERBOARD", 20);
                leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
                CenterText(leaderboardText, leaderboard, -120);

                sf::Text currentRecordsText = CreateText(font, "", 18);
                currentRecordsText.setStyle(sf::Text::Bold);
                string str;
                for (int i = 1; i <= 5; i++) {
                    string tempStr;
                    int mins = floor(currentEntries[i].begin()->second / 60);
                    int secs = currentEntries[i].begin()->second % 60;

                    int pos1;
                    int pos2;
                    if (mins >= 10) {
                        pos1 = floor(mins / 10);
                        pos2 = mins % 10;
                    } else {
                        pos1 = 0;
                        pos2 = mins % 10;
                    }

                    int pos3;
                    int pos4;
                    if (secs >= 10) {
                        pos3 = floor(secs / 10);
                        pos4 = secs % 10;
                    } else {
                        pos3 = 0;
                        pos4 = secs % 10;
                    }

                    if (editedEntry == i) {
                        tempStr = to_string(i) + ". " + "\t" + to_string(pos1) + to_string(pos2) + ":" + to_string(pos3) +
                                  to_string(pos4) + "\t" + currentEntries[i].begin()->first + "*" + "\n\n";
                    } else {
                        tempStr = to_string(i) + ". " + "\t" + to_string(pos1) + to_string(pos2) + ":" + to_string(pos3) +
                                  to_string(pos4) + "\t" + currentEntries[i].begin()->first + "\n\n";
                    }
                    str += tempStr;
                }
                currentRecordsText.setString(str);
                CenterText(currentRecordsText, leaderboard, 20);

                while (leaderboardWin.isOpen()) {

                    sf::Event leaderboardEvent;
                    leaderboardTime = leaderboardClock.getElapsedTime().asSeconds();

                    while (leaderboardWin.pollEvent(leaderboardEvent)) {
                        if (leaderboardEvent.type == sf::Event::Closed) {
                            leaderboardWin.close();
                            showLeaderboard = false;
                            leaderboardButton.clicked = false;
                            if (!pauseButton.clicked) {
                                tempTime += leaderboardTime;
                            }
                        }
                    }
                    leaderboardWin.clear(sf::Color::Blue);
                    leaderboardWin.draw(leaderboardText);
                    leaderboardWin.draw(currentRecordsText);

                    leaderboardWin.display();
                }
            }

        }
    }

    return 0;
}