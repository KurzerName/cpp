#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;
using namespace sf;

const float WIDTH_GAME_WINDOW = 700;
const float HEIGHT_GAME_WINDOW = 700;
const float CENTER_WIDTH_GAME_WINDOW = WIDTH_GAME_WINDOW / 2;
const float CENTER_HEIGHT_GAME_WINDOW = HEIGHT_GAME_WINDOW / 2;

class Draw;
/**
 * Дефолтный класс элемента
 */
class Element {
protected:
    RectangleShape rectangle;

    Text text;

    Color fontColor;

    Font font;

    string innerText;
    string fontPath = "./media/fonts/font2.ttf";

    int elementId = 0;

    float width = 0;
    float height = 0;
    float top = 0;
    float left = 0;
    float fontSize;
public:
    Element(
            float width = 200,
            float height = 100,
            float top = 0,
            float left = 0,
            int elementId = 0,
            Color fillColor = Color::White
    ){
        this->width = width;
        this->height = height;
        this->top = top;
        this->left = left;
        this->elementId = elementId;

        this->setSize(width, height);
        this->setFillColor(fillColor);
        this->setPosition(top, left);
    }

    void setFillColor(Color color) {
        this->rectangle.setFillColor(color);
    }

    void setPosition(float top, float left) {
        this->rectangle.setPosition(left, top);
    }

    void setSize(float width, float height) {
        this->rectangle.setSize(Vector2f(width, height));
    }

    void setText(
            string innerText = "Test",
            float fontSize = 16,
            Color color = Color::Black) {
        this->font.loadFromFile(fontPath);
        this->innerText = innerText;
        this->fontColor = color;
        this->fontSize = fontSize;
    }

    void setMarginText(float marginLeft = 0, float marginTop = 0) {
        this->text.setPosition(this->left + marginLeft, this->top + marginTop);
    }

    int getTop() {
        return this->top;
    }

    int getLeft() {
        return this->left;
    }

    int getBottom() {
        return this->top + this->height;
    }

    int getRight() {
        return this->left + this->width;
    }

    int getElementId() {
        return this->elementId;
    }

    bool isInElement(int X, int Y) {
        return (
                (X > this->getLeft() && X < this->getRight())
                &&
                (Y > this->getTop()) && Y < this->getBottom()
        );
    }

    void render(RenderWindow& window) {
        this->text.setFont(this->font);
        this->text.setString(L"" + this->innerText);
        this->text.setCharacterSize(this->fontSize);
        this->text.setFillColor(this->fontColor);

        window.draw(this->rectangle);
        window.draw(this->text);
    }
};

/**
 * Класс для созданий кнопок
 */
class MenuButton : public Element {
public:
    MenuButton(
            float width = 200,
            float height = 100,
            float top = 0,
            float left = 0,
            int elementId = 0,
            Color fillColor = Color::White
    ){
        this->width = width;
        this->height = height;
        this->top = top;
        this->left = left;
        this->elementId = elementId;

        this->setSize(width, height);
        this->setFillColor(fillColor);
        this->setPosition(top, left);
    }

    void setBorder(float width = 0, Color color = Color::Black) {
        this->rectangle.setOutlineThickness(width);
        this->rectangle.setOutlineColor(color);
    }
};

/**
 * Класс для создания игровой ячейки
 */
class GameCell : public Element {
    Texture texture;

    bool haveSprite = false;
    bool clicked    = false;

    const string IMGS_PATH = "./media/imgs/";
public:
    GameCell(
            float width = 200,
            float height = 100,
            float top = 0,
            float left = 0,
            int elementId = 0
    ){
        this->width = width;
        this->height = height;
        this->top = top;
        this->left = left;
        this->elementId = elementId;

        this->setSize(width, height);
        this->setPosition(top, left);
    }

    void select(Texture texture) {
        this->clicked = true;
        this->texture = texture;
    }

    void unSelect() {
        this->clicked = false;
    }

    bool isSelect() {
        return this->clicked;
    }

    void renderImg(RenderWindow& window) {
        Sprite sprite;
        sprite.setTexture(this->texture);
        sprite.setPosition(this->left, this->top);
        sprite.setScale(Vector2f(0.225, 0.225));

        window.draw(sprite);
    }

    void render(RenderWindow& window) {
        if (this->clicked) {
            this->renderImg(window);

            return;
        }

       window.draw(this->rectangle);
    }
};

enum MENU_BUTTONS {
    playerVersus   = 0,
    computerVersus = 1
};

enum GAME_BUTTONS {
    retryButton = 0,
    backMenuButton
};

enum GAME_ELEMENTS {
    whoseTurn   = 0
};

/**
 * Класс игры в крестики-нолики
 */
class TicTacToe
{
    RenderWindow &window;

    Texture crossTexture;
    Texture roundTexture;

    vector <Element> menuElements;
    vector <Element> gameElements;
    vector <MenuButton> menuButtons;
    vector <MenuButton> gameButtons;
    vector <vector<GameCell>> gameCells;

    vector <vector<int>> selectedGameCells;
    vector <vector<int>> turnPrioritet;
    vector <vector<int>> randomTurn;

    const int CROSS_TURN = 1;
    const int ROUND_TURN = 2;
    const int ELEMENT_WIDTH_HEIGHT = 50;
    const int MARGIN = 52;
    const int PLAYER_ENEMY_TYPE = 1;
    const int COMPUTER_ENEMY_TYPE = 2;
    const int COUNT_SELECTED_FOR_WIN = 3;
    const int FIELD_COUNT_ROWS = 4;
    const int FIELD_COUNT_COLUMNS = 4;
    const int FIELD_CELL_HEIGHT = 60;
    const int FIELD_CELL_WIDTH = 60;
    const int FIELD_START_POSITION = 30;
    const int FIELD_END_POSITION = 75 * FIELD_COUNT_ROWS;

    const int MAX_ITERATION = 100;
    int iteration = 0;

    const float GAME_CELL_WIDTH = 50;
    const float GAME_CELL_HEIGHT = 50;

    const string IMGS_PATH = "./media/imgs/";

    bool showAlert = false;
    bool clickableGameCell = true;
    bool gameOn = false;

    int enemyType = 0;
    int playerTurn = CROSS_TURN;

    string winner = "";


    void check(int playerTurn) {
        for (int row = 0; row < selectedGameCells.size(); row++) {
            for (int column = 0; column < selectedGameCells[row].size(); column++) {
                int selectedValue = selectedGameCells[row][column];

                if (selectedValue == -1) {
                    continue;
                }

                int colMod = column;
                int rowMod = row;
                int steps = 1;

                while (steps > 0) {
                    int countToWin = 1;
                    // Проверка справа
                    for (int i = 0; i < COUNT_SELECTED_FOR_WIN; i++) {
                        colMod++;

                        if (colMod > selectedGameCells[row].size() - 1) {
                            colMod--;

                            break;
                        }

                        int nextValue = selectedGameCells[rowMod][colMod];

                        if (selectedValue == nextValue) {
                            countToWin++;
                        } else {
                            steps--;

                            break;
                        }

                        if (countToWin == COUNT_SELECTED_FOR_WIN) {
                            this->winGame(playerTurn);

                            return;
                        }
                    }

                    countToWin = 1;
                    colMod = column;

                    // Проверка по низу
                    for (int i = 0; i < COUNT_SELECTED_FOR_WIN; i++) {
                        rowMod++;

                        if (rowMod > selectedGameCells.size() - 1) {
                            rowMod--;

                            break;
                        }

                        int nextValue = selectedGameCells[rowMod][colMod];

                        if (selectedValue == nextValue) {
                            countToWin++;
                        } else {
                            steps--;

                            break;
                        }

                        if (countToWin == COUNT_SELECTED_FOR_WIN) {
                            this->winGame(playerTurn);

                            return;
                        }
                    }

                    countToWin = 1;
                    rowMod = column;
                    colMod = row;

                    // Диагональ права
                    for (int i = 0; i < COUNT_SELECTED_FOR_WIN; i++) {
                        colMod++;
                        rowMod++;

                        if (colMod > selectedGameCells[row].size() - 1) {
                            colMod--;

                            break;
                        }

                        if (rowMod > selectedGameCells.size() - 1) {
                            rowMod--;

                            break;
                        }

                        int nextValue = selectedGameCells[rowMod][colMod];

                        if (selectedValue == nextValue) {
                            countToWin++;
                        } else {
                            steps--;

                            break;
                        }

                        if (countToWin == COUNT_SELECTED_FOR_WIN) {
                            this->winGame(playerTurn);

                            return;
                        }
                    }

                    countToWin = 1;
                    rowMod = row;
                    colMod = column;

                    // Диагональ лево
                    for (int i = 0; i < COUNT_SELECTED_FOR_WIN; i++) {
                        colMod--;
                        rowMod++;

                        if (colMod < 0) {
                            break;
                        }

                        if (rowMod > selectedGameCells.size() - 1) {
                            rowMod--;

                            steps--;

                            break;
                        }

                        int nextValue = selectedGameCells[rowMod][colMod];

                        if (selectedValue == nextValue) {
                            countToWin++;
                        } else {
                            steps--;

                            break;
                        }

                        if (countToWin == COUNT_SELECTED_FOR_WIN) {
                            this->winGame(playerTurn);
                            return;
                        }
                    }
                }
            }
        }
    }

    void onClick(Event event) {
        int X = event.mouseButton.x;
        int Y = event.mouseButton.y;

        if (!this->gameOn) {
            this->menuClick(X, Y);

            return;
        }

        this->gameClick(X, Y);
    }

    void selectCell(int row, int column) {
        if (selectedGameCells[row][column] > -1) {
            cout << "Поле уже выбрано. Поле выбрал " << this->playerTurn << endl;

            return;
        }

        cout << "Поле выбрал " << this->playerTurn << endl;

        Texture texture = this->playerTurn == CROSS_TURN ? this->crossTexture : this->roundTexture;

        selectedGameCells[row][column] = this->playerTurn;

        this->gameCells[row][column].select(texture);

        this->check(this->playerTurn);

        this->playerTurn = this->playerTurn == CROSS_TURN ? ROUND_TURN : CROSS_TURN;

        if (this->enemyType == COMPUTER_ENEMY_TYPE && this->playerTurn == CROSS_TURN) {
            this->computerTurn();
        }
    }

    void gameCellClick(int X, int Y) {
        for (int row = 0; row < this->gameCells.size(); row++) {
            for (int column = 0; column < this->gameCells[row].size(); column++) {
                if (this->gameCells[row][column].isSelect()) {
                    continue;
                }

                if (this->gameCells[row][column].isInElement(X, Y)) {
                    this->selectCell(row, column);

                    break;
                }
            }
        }
    }

    void gameClick(int X, int Y) {
        if (this->isInGameField(X, Y) && this->clickableGameCell) {
            this->gameCellClick(X, Y);
        } else if (this->gameButtons[GAME_BUTTONS::retryButton].isInElement(X, Y)) {
            this->retryGame();
        } else if (this->gameButtons[GAME_BUTTONS::backMenuButton].isInElement(X, Y)) {
            this->stopGame();
        }
    }

    void menuClick(int X, int Y) {
        if (
                !this->menuButtons[MENU_BUTTONS::playerVersus].isInElement(X, Y)
                &&
                !this->menuButtons[MENU_BUTTONS::computerVersus].isInElement(X, Y)
                ) {
            return;
        }

        for (int i = 0; i < menuButtons.size(); i++) {
            int buttonRight = menuButtons[i].getRight();
            int buttonLeft = menuButtons[i].getLeft();

            if (X > buttonLeft && X < buttonRight) {
                int id = menuButtons[i].getElementId();

                this->startGame(id);
            }
        }
    }

    void stopGame() {
        this->gameOn = false;
        this->showAlert = false;

        this->clearSelectedGameCells();
    }

    void retryGame() {
        this->clearSelectedGameCells();

        this->showAlert = false;

        if (this->enemyType == COMPUTER_ENEMY_TYPE) {
            this->computerTurn();
        }
    }

    void winGame(int playerTurn) {
        this->winner = playerTurn == CROSS_TURN ? "cross" : "round";
        this->showAlert = true;
        this->clickableGameCell = false;
    }

    void startGame(int id) {
        this->enemyType = id;
        this->gameOn = true;

        if (this->enemyType == COMPUTER_ENEMY_TYPE) {
            this->computerTurn();
        }
    }

    void clearSelectedGameCells() {
        this->selectedGameCells.clear();
        this->turnPrioritet.clear();

        for (int i = 0; i < this->gameCells.size(); i++) {
            vector<int> selectedRow;

            for (int j = 0; j < this->gameCells[i].size(); j++) {
                this->gameCells[i][j].unSelect();

                selectedRow.push_back(-1);
            }

            this->selectedGameCells.push_back(selectedRow);
            this->turnPrioritet.push_back(selectedRow);
        }

        this->playerTurn = CROSS_TURN;
        this->clickableGameCell = true;
    }

    /**
     * Метод инициализации элементов меню
     */
    void initMenuElements() {
        Element title(240, 50, 10, CENTER_WIDTH_GAME_WINDOW - 120);
        title.setText("Welcome to TicTacToe!", 20);
        title.setMarginText(7, 10);

        Element chooseText(240, 50, 60, CENTER_WIDTH_GAME_WINDOW - 120);
        chooseText.setText("Before you start, please, \n\t choose game mode!", 16);
        chooseText.setMarginText(35);

        MenuButton twoPlayersButton(118, 25, 112, CENTER_WIDTH_GAME_WINDOW - 118, PLAYER_ENEMY_TYPE);
        twoPlayersButton.setText("1 vs 1", 13);
        twoPlayersButton.setMarginText(45, 2);
        twoPlayersButton.setBorder(2);

        MenuButton computerButton(116, 25, 112, CENTER_WIDTH_GAME_WINDOW + 2, COMPUTER_ENEMY_TYPE);
        computerButton.setText("versus computer", 13);
        computerButton.setMarginText(7, 3);
        computerButton.setBorder(2);

        menuElements.push_back(title);
        menuElements.push_back(chooseText);
        menuButtons.push_back(twoPlayersButton);
        menuButtons.push_back(computerButton);
    }

    /**
     * Метод инициализации игровых элементов
     */
    void initGameElements() {
        int cellIndex = 0;

        for (int row = 1; row < FIELD_COUNT_ROWS + 1; row++) {
            int top = MARGIN * row;

            vector <GameCell> rowCell;
            vector<int> selectedRow;

            for (int column = 1; column < FIELD_COUNT_COLUMNS + 1; column++) {
                int left = MARGIN * column;

                GameCell gameColumn(
                        GAME_CELL_WIDTH,
                        GAME_CELL_HEIGHT,
                        top,
                        left,
                        cellIndex
                );

                rowCell.push_back(gameColumn);
                selectedRow.push_back(-1);

                cellIndex++;
            }

            this->gameCells.push_back(rowCell);
            this->selectedGameCells.push_back(selectedRow);
            this->turnPrioritet.push_back(selectedRow);
        }

        Element whoseTurn(240, 50, 10, CENTER_WIDTH_GAME_WINDOW);
        whoseTurn.setMarginText(75, 10);

        MenuButton retryButton(240, 27, 60, CENTER_WIDTH_GAME_WINDOW);
        retryButton.setText("retry");
        retryButton.setFillColor(Color::Green);
        retryButton.setMarginText(100, 3);

        MenuButton backMenuButton(240, 27, 87, CENTER_WIDTH_GAME_WINDOW);
        backMenuButton.setText("Back to menu");
        backMenuButton.setFillColor(Color::Yellow);
        backMenuButton.setMarginText(70, 3);

        this->gameElements.push_back(whoseTurn);
        this->gameButtons.push_back(retryButton);
        this->gameButtons.push_back(backMenuButton);
    }

    /**
     * Метод инициализации картинок
     */
    void initImgs() {
        Image crossImg;

        if (!crossImg.loadFromFile(IMGS_PATH + "cross.png")) {
            cout << "Ошибка загрузки изображения крестика" << endl;

            return;
        }

        Image roundImg;

        if (!roundImg.loadFromFile(IMGS_PATH + "round.png")) {
            cout << "Ошибка загрузки изображения нолика" << endl;

            return;
        }

        this->crossTexture.loadFromImage(crossImg);
        this->roundTexture.loadFromImage(roundImg);
    }

    /**
     * Метод инициализации элементов для игры
     */
    void init() {
        this->initMenuElements();
        this->initGameElements();
        this->initImgs();
    }

    void setListeners(Event event) {
        switch (event.type) {
            case Event::Closed:
                this->window.close();

                break;
            case Event::MouseButtonReleased:
                this->onClick(event);

                break;
        }
    }

    void menuDraw() {
        for (int i = 0; i < this->menuElements.size(); i++) {
            this->menuElements[i].render(this->window);
        }

        for (int i = 0; i < this->menuButtons.size(); i++) {
            this->menuButtons[i].render(this->window);
        }
    }

    void gameMenuDraw() {
        for (int i = 0; i < this->gameCells.size(); i++) {
            for (int j = 0; j < this->gameCells[i].size(); j++) {
                this->gameCells[i][j].render(this->window);
            }
        }

        string whoTurn = this->playerTurn == this->CROSS_TURN ? "cross turn" : "round turn";

        this->gameElements[GAME_ELEMENTS::whoseTurn].setText(whoTurn, 20);

        for (int i = 0; i < this->gameElements.size(); i++) {
            this->gameElements[i].render(this->window);
        }

        for (int i = 0; i < this->gameButtons.size(); i++) {
            this->gameButtons[i].render(this->window);
        }
    }

    void gameFieldDraw() {
        for (int i = 1; i <= this->FIELD_COUNT_ROWS + 1; i++) {
            Vertex line[] = {
                    Vertex(Vector2f(this->MARGIN * i, this->FIELD_START_POSITION), Color::Red),
                    Vertex(Vector2f(this->MARGIN * i, this->FIELD_END_POSITION), Color::Red),
                    Vertex(Vector2f(this->FIELD_START_POSITION, this->MARGIN * i), Color::Red),
                    Vertex(Vector2f(this->FIELD_END_POSITION, this->MARGIN * i), Color::Red)
            };

            this->window.draw(line, 4, Lines);
        }
    }

    void alertDraw() {
        Element alert(240, 215, 20, CENTER_WIDTH_GAME_WINDOW - 240);
        alert.setText("Before you start, please, \n\t choose game mode!", 16, Color::White);
        alert.setMarginText(30, 80);
        alert.setFillColor(Color::Red);
        alert.render(this->window);
    }

    void winnerDraw() {
        Element winAlert(200, 35, 240, CENTER_WIDTH_GAME_WINDOW - 80);
        winAlert.setText(this->winner + " win!", 20, Color::Black);
        winAlert.setMarginText(50, 5);
        winAlert.setFillColor(Color::Green);
        winAlert.render(this->window);
    }

    void computerTurn() {
        iteration++;

        if (iteration >= MAX_ITERATION) {
            cout << "Достигли максимума" << endl;

            return;
        }

        cout << "Компьютер сделал ход" << endl;

        // Сначала используем стандартный паттерн - между крестиками
        for(int row = 0; row < FIELD_COUNT_ROWS; row++){
            for(int column = 0; column < FIELD_COUNT_COLUMNS; column++) {
                int nextRow        = row + 1;
                int nextColumn     = column + 1;
                int nextNextRow    = row + 2;
                int nextNextColumn = column + 2;

                if (nextNextColumn >= FIELD_COUNT_COLUMNS) {
                    nextNextColumn = FIELD_COUNT_COLUMNS - 1;
                }

                if (nextNextRow >= FIELD_COUNT_ROWS) {
                    nextNextRow = FIELD_COUNT_ROWS - 1;
                }

                if (nextColumn >= FIELD_COUNT_COLUMNS) {
                    nextColumn = FIELD_COUNT_COLUMNS - 1;
                }

                if (nextRow >= FIELD_COUNT_ROWS) {
                    nextRow = FIELD_COUNT_ROWS - 1;
                }

                if (selectedGameCells[row][column] == CROSS_TURN) {
                    bool basicTurn = this->basicComputerPatterns(
                            row,
                            nextRow,
                            nextNextRow,
                            column,
                            nextColumn,
                            nextNextColumn
                    );

                    if (basicTurn) {
                        return;
                    }
                }

                if (selectedGameCells[row][column] == ROUND_TURN) {
                    bool advancedTurn = this->advancedComputerPatterns(
                            row,
                            nextRow,
                            nextNextRow,
                            column,
                            nextColumn,
                            nextNextColumn
                    );

                    if (advancedTurn) {
                        return;
                    }
                }
            }
        }

        this->computerAngleTurn();
    }

    /**
     * Более продвинутые паттерны компьютера
     *
     * @param nextRow
     * @param nextNextRow
     * @param nextColumn
     * @param nextNextColumn
     * @return
     */
    bool advancedComputerPatterns(int row, int nextRow, int nextNextRow, int column, int nextColumn, int nextNextColumn)
    {
        int thisSelectedValue = selectedGameCells[row][column];

        if (thisSelectedValue == ROUND_TURN) {
            int countRound = 0;

            // Прежде всего надо смотреть на нули, которые в шаге от победы

            // Горизонталь
            if (selectedGameCells[row][nextNextRow] == ROUND_TURN) {
                this->selectCell(row, nextRow);

                return true;
            }

            // Вертикаль
            if (selectedGameCells[nextNextRow][column] == ROUND_TURN) {
                this->selectCell(nextRow, column);

                return true;
            }

//            // Диагональ (право)
//            if (selectedGameCells[nextNextColumn][nextNextRow] == ROUND_TURN) {
//                this->selectCell(row, nextRow);
//
//                return true;
//            }
//
//            // Вертикаль
//            if (selectedGameCells[nextNextRow][column] == ROUND_TURN) {
//                this->selectedGameCells(nextRow, column);
//
//                return true;
//            }

            // Первая проверка будет по вертикале
            for(int i = 0, j = row; i < COUNT_SELECTED_FOR_WIN; i++, j--) {
                int iRow = row + i;
                int jRow = row - j;

                if (row + COUNT_SELECTED_FOR_WIN - 1 >= FIELD_COUNT_ROWS) {
                    break;
                }

                if (iRow >= FIELD_COUNT_ROWS) {
                    iRow--;
                }

                if (jRow < 0) {
                    jRow++;
                }

                if (selectedGameCells[iRow][column] == -1 && countRound > 1) {
                    this->selectCell(iRow, column);

                    return true;
                } else if (selectedGameCells[jRow][column] == -1 && countRound > 1) {
                    this->selectCell(jRow, column);

                    return true;
                }

                if (selectedGameCells[iRow][column] == ROUND_TURN || selectedGameCells[jRow][column] == ROUND_TURN) {
                    countRound++;

                    continue;
                }
            }

            countRound = 0;

            // Вторая по горизонтали
            for(int i = 0; i < COUNT_SELECTED_FOR_WIN; i++) {
                int iColumn = column + i;

                if (column + COUNT_SELECTED_FOR_WIN - 1 >= FIELD_COUNT_COLUMNS) {
                    break;
                }

                if (iColumn >= FIELD_COUNT_COLUMNS) {
                    iColumn--;
                }

                if (selectedGameCells[row][iColumn] == -1 && countRound > 1) {
                    this->selectCell(row, iColumn);

                    return true;
                }

                if (selectedGameCells[row][iColumn] == ROUND_TURN) {
                    countRound++;

                    continue;
                }
            }
        }

        return false;
    }

    /**
     * Метод базовых паттернов поведения компьютера
     *
     * @param row
     * @param nextRow
     * @param nextNextRow
     * @param column
     * @param nextColumn
     * @param nextNextColumn
     *
     * @return
     */
    bool basicComputerPatterns(int row, int nextRow, int nextNextRow, int column, int nextColumn, int nextNextColumn)
    {
        if (selectedGameCells[row][column] == ROUND_TURN) {
            return false;
        }

        // Между крестиками по горизонтали
        if (selectedGameCells[row][nextColumn] <= -1 && selectedGameCells[row][nextNextColumn] == CROSS_TURN) {
            cout << 1 << endl;

            this->selectCell(row, nextColumn);

            return true;
        }

        // Между крестиками по диагонали на правую сторону
        if (selectedGameCells[nextRow][nextColumn] <= -1 && selectedGameCells[nextNextRow][nextNextColumn] == CROSS_TURN) {
            cout << 2 << endl;

            this->selectCell(nextRow, nextColumn);

            return true;
        }

        // Между крестиками по диагонали на левую сторону
        if (column - 2 >= 0 && selectedGameCells[nextRow][column - 1] <= -1 &&  selectedGameCells[nextNextRow][column - 2] == CROSS_TURN) {
            cout << 3 << endl;

            this->selectCell(nextRow, column - 1);

            return true;
        }

        // Между крестиками по вертикали
        if (selectedGameCells[nextRow][column] <= -1 && selectedGameCells[nextNextRow][column] == CROSS_TURN) {
            cout << 4 << endl;

            this->selectCell(nextRow, column);

            return true;
        }

        return false;
    }

    /**
     * Заполнение углов компьютером
     */
    void computerAngleTurn()
    {
        int rows    = FIELD_COUNT_ROWS - 1;
        int columns = FIELD_COUNT_COLUMNS - 1;

        if (selectedGameCells[0][0] == -1) {
            this->selectCell(0, 0);

            return;
        } else if (selectedGameCells[0][columns] == -1) {
            this->selectCell(0, columns);

            return;
        } else if (selectedGameCells[rows][0] == -1) {
            this->selectCell(rows, 0);

            return;
        } else if (selectedGameCells[rows][columns] == -1) {
            this->selectCell(rows, columns);

            return;
        }
    }


    bool isInGameField(int X, int Y)
    {
        return (
                (X > FIELD_START_POSITION && X < FIELD_END_POSITION)
                && (Y > FIELD_START_POSITION && Y < FIELD_END_POSITION)
        );
    }

public:
    TicTacToe(RenderWindow &win) : window(win){
        this->init();

        Clock clock;
        float timer;

        while (window.isOpen()) {
            Event event;
            clock.restart();
            timer = timer / 400;

            while (window.pollEvent(event)) {
                this->setListeners(event);

                window.clear();

                if (this->gameOn) {
                    this->gameMenuDraw();
                    this->gameFieldDraw();
                } else {
                    this->menuDraw();
                }

                if (this->showAlert) {
                    this->winnerDraw();
                }

                window.display();
            }
        }
    }
};


int main() {
    RenderWindow window(VideoMode(WIDTH_GAME_WINDOW, HEIGHT_GAME_WINDOW), "Cross and Rounds");

    TicTacToe game(window);

    return 0;
}