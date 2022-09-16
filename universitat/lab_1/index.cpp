#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum CAR_COLUMNS {
    brand = 0,
    engine_power,
    tank_volume,
    color
};

string trim(string str = "") {
    if (!str.length()) {
        return "";
    }

    bool trimLeft = true;
    bool trimRight = true;

    for (int left = 0, right = str.length() - 1; right >= 0; left++, right--) {
        if (!trimLeft && !trimRight) {
            break;
        }

        if (trimLeft && str[left] == ' ') {
            str = str.erase(left, 1);
        } else {
            trimLeft = false;
        }

        if (trimRight && str[right] == ' ') {
            str = str.erase(right, 1);
        } else {
            trimRight = false;
        }
    }

    return str;
}

/**
 * Структура для работы с файлом
 */
struct FileTools {
    fstream file;

    int rowCount         = 0;
    int FILE_ROW_COLUMNS = 4;

    string** fileData;
    string path;

    const int END_EDIT        = 0;
    const int CONTINUE_EDIT   = 1;
    const int CHANGE_EDIT_ROW = 2;

    FileTools(){};

    FileTools(string path)
    {
        this->open(path);
    }

    /**
     * Метод открытия файла
     *
     * @param path
     */
    void open(string path = "") {
        if (path.length() == 0) {
            cerr << "file path must be no empty!";

            exit(0);
        }

        this->path = path;

        this->file.open(path, ios::out | ios::in | ios::app);

        if (!this->file.is_open()) {
            cerr << "incorrect path to file";

            exit(0);
        }
    }

    /**
     * Метод чтения файла
     *
     * @return
     */
    void read(string path = "") {
        path = path.length() ? path : this->path;

        this->setRowCount();
        this->open(path);

        this->fileData = new string*[this->rowCount];

        string fileLine = "";
        int i           = 0;

        while (getline(this->file, fileLine)) {
            this->fileData[i] = new string[4];
            string *columns   = this->splitFileRow(fileLine, ";");

            this->fileData[i][CAR_COLUMNS::brand]        = columns[CAR_COLUMNS::brand];
            this->fileData[i][CAR_COLUMNS::engine_power] = columns[CAR_COLUMNS::engine_power];
            this->fileData[i][CAR_COLUMNS::tank_volume]  = columns[CAR_COLUMNS::tank_volume];
            this->fileData[i][CAR_COLUMNS::color]        = columns[CAR_COLUMNS::color];

            i++;
        }

        this->file.close();
    }

    /**
     * Метод показа данных файла
     */
    void showData()
    {
        cout << endl;

        for(int row = 0; row < this->rowCount; row++) {
            cout << "[" << row << "] ";

            for(int column = 0; column < this->FILE_ROW_COLUMNS; column++) {
                cout << this->fileData[row][column] << " ";
            }

            cout << endl;
        }
    }

    /**
     * Метод добавления строки
     */
    void add() {
        cout << "Добавьте новый автомобиль!\nМарка автомобиля: ";
        string carBrand;

        this->setValue(carBrand);

        cout << "Мощность двигателя: ";

        int enginePower;

        this->setValue(enginePower);

        cout << "Объём бака: ";

        int tankVolume;

        this->setValue(tankVolume);

        cout << "Цвет автомобиля: ";

        string color;

        this->setValue(color);

        this->open(this->path);
        this->file << carBrand << ";" << enginePower << ";" << tankVolume << ";" << color << '\n';
        this->file.close();

        this->setRowCount();
    }

    /**
     * Метод редактирования строки
     *
     * @param rowIndex
     */
    void editRow(int rowIndex = -1) {
        int action = this->CONTINUE_EDIT;

        while(action) {
            if (rowIndex == -1 || action == this->CHANGE_EDIT_ROW) {
                cout << "Выберите, какую строку вы хотите изменить: ";

                this->showData();

                cout << ": ";

                cin >> rowIndex;
            }

            if (rowIndex > this->rowCount) {
                rowIndex = this->rowCount;
            } else if (rowIndex < 0) {
                cerr << "incorrect index";

                exit(1);
            }

            for(int row = 0; row < this->rowCount; row++) {
                if (row == rowIndex) {
                    cout << "Какую колонку вы хотите изменить?" << endl;

                    while(action) {
                        for(int column = 1; column <= this->FILE_ROW_COLUMNS; column++) {
                            cout << "[" << column << "] " << this->fileData[row][column-1] << " ";
                        }

                        cout << "\n: ";

                        int columnIndex = 0;

                        cin >> columnIndex;

                        action = this->editColumn(rowIndex, columnIndex-1);

                        if (action == this->CHANGE_EDIT_ROW) {
                            break;
                        }
                    }

                    break;
                }
            }
        }
    }

    /**
     * Метод редактирования колонки
     *
     * @param row
     * @param columnIndex
     *
     * @return
     */
    int editColumn(int &row, int columnIndex)
    {
        if (columnIndex < 0 || columnIndex >= this->FILE_ROW_COLUMNS) {
            cout << "Введите корректный номер колонки ";

            return this->CHANGE_EDIT_ROW;
        }

        cout << "Было: " << this->fileData[row][columnIndex] << endl;

        string newColumnValue;

        cin.get();

        getline(cin, newColumnValue);

        cout << "Подтвердите действие:\n 0. Отменить редактирование\n 1. Подтверждение редактирования\n 2. Сменить строчку для редактирования\n 3. Выйти" << endl;

        int editConfirm = 0;

        cin >> editConfirm;

        switch (editConfirm) {
            case 0:
                cout << "Продолжение редактирования" << endl;

                return this->CONTINUE_EDIT;
            case 1:
                this->editFile(newColumnValue, row, columnIndex);

                return this->CONTINUE_EDIT;
            case 2:
                return this->CHANGE_EDIT_ROW;
            case 3:
                cout << "Досвидания!" << endl;

                return this->END_EDIT;
            default:
                cout << "Введите корректную цифру!" << endl;

                return this->CONTINUE_EDIT;
        }
    }

    /**
     * Непосредственное редактирование файла
     *
     * @param value
     * @param row
     * @param column
     */
    void editFile(string value, int row, int column)
    {
        remove(this->path.c_str());
        ofstream newFile(this->path);

        string fileRow;

        for(int i = 0; i < this->rowCount; i++) {
            for(int j = 0; j < this->FILE_ROW_COLUMNS; j++) {
                if (row == i && column == j) {
                    fileRow += value;

                    fileData[i][j] = value;
                } else {
                    fileRow += fileData[i][j];
                }

                if (j + 1 == this->FILE_ROW_COLUMNS) {
                    fileRow += + "\n";

                    break;
                } else {
                    fileRow += + ';';
                }
            }

            newFile << fileRow;

            fileRow.clear();
        }

        newFile.close();

        this->open(this->path);
    }

    /**
     * Метод для установки количества колонок в строке файла
     *
     * @param count
     */
    void setRowFileColumns(int count = 0) {
        if (count <= 0) {
            count = 4;
        }

        this->FILE_ROW_COLUMNS = count;
    }

    /**
     * Метод разделения строки файла по делиметру
     * Пример: abc,dsf => ["abc","dsf"]
     *
     * @param str
     * @param delimetr
     * @return
     */
    string *splitFileRow(string str = "", string delimetr = "")
    {
        static string splitedRow[4];

        int start = 0;
        int end   = 0;

        for (int i = 0; i < 4; i++) {
            end           = str.find(delimetr, end) + 1;
            splitedRow[i] = trim(str.substr(start, end - start - 1));
            start         = end;
        }

        return splitedRow;
    }

    /**
     * Метод нахождения самой мощной машины
     */
    void findTheMostPowerfulCar()
    {
        int maxEnginePower    = 0;
        int maxEnginePowerRow = 0;

        for(int row = 0; row < this->rowCount; row++) {
            int carEnginePower = stoi(this->fileData[row][CAR_COLUMNS::engine_power]);

            if (maxEnginePower < carEnginePower) {
                maxEnginePower    = carEnginePower;
                maxEnginePowerRow = row;
            }
        }

        cout << "Самая мощная машина из имеющихся: " << this->fileData[maxEnginePowerRow][CAR_COLUMNS::brand] << " лс: " << maxEnginePower << endl;
    }

    /**
     * Метод установки количества строк в файле
     */
    void setRowCount()
    {
        string fileLine = "";
        int i           = 0;

        this->file.close();
        this->open(this->path);

        while (getline(this->file, fileLine)) {
            i++;
        }

        this->file.close();

        this->rowCount = i;
    }

    /**
     * Метод получения количества строк из файла
     *
     * @return
     */
    int getRowCount()
    {
        return this->rowCount;
    }

    /**
     * Метод записи значения из консоли
     *
     * @param value
     */
    void setValue(string &value)
    {
        string buffer = "";

        getline(cin, buffer);

        value = buffer;
    }

    /**
     * Метод записи значения из консоли
     *
     * @param value
     */
    void setValue(int &value)
    {
        string buffer = "";

        getline(cin, buffer);

        value = stoi(buffer);
    }
};

int main() {
    FileTools file;

    file.open("./bd.txt");
    file.read();
    file.findTheMostPowerfulCar();

    return 0;
}