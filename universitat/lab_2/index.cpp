#include <iostream>
#include <vector>

using namespace std;

/**
 * Класс машины
 */
class Car
{
    string brand;
    string color;

    int enginePower;
    int tankVolume;
public:
    Car(string brand = "lada vesta", string color = "white", int enginePower = 50, int tankVolume = 40)
    {
        setBrand(brand);
        setColor(color);
        setEnginePower(enginePower);
        setTankVolume(tankVolume);
    }

    string getBrand()
    {
        return this->brand;
    }

    string getColor()
    {
        return this->color;
    }

    int getEnginePower()
    {
        return this->enginePower;
    }

    int getTankVolume()
    {
        return this->tankVolume;
    }

    void setBrand(string brand = "")
    {
        this->brand = brand;
    }

    void setColor(string color = "")
    {
        this->color = color;
    }

    void setEnginePower(int enginePower = 0)
    {
        this->enginePower = enginePower;
    }

    void setTankVolume(int tankVolume)
    {
        this->tankVolume = tankVolume;
    }
};

vector<Car> cars;

/**
 * Метод показа всех машин
 */
void showAllCars() {
    cout << endl;
    for(int i = 0; i < cars.size(); i++) {
        cout << "[" << i << "] Марка: " << cars[i].getBrand();
        cout << "; Цвет: " << cars[i].getColor();
        cout << "; ЛС: " << cars[i].getEnginePower();
        cout << "; Объём бака: " << cars[i].getTankVolume() << endl;
    }
}

/**
 * Метод редактирования машина
 *
 * @param index
 */
void editCar() {
    cout << "Выберите строку для редактирования: ";

    showAllCars();

    int editCarIndex = 0;

    cout << ": ";
    cin >> editCarIndex;
    cin.get();

    cout << "Марка автомобиля: ";
    string brand;

    getline(cin, brand);
    cin.clear();

    cout << "Цвет автомобиля: ";
    string color;

    getline(cin, color);
    cin.clear();

    cout << "Мощность двигателя: ";
    int enginePower;

    cin >> enginePower;

    cout << "Объём двигателя: ";
    int tankVolume;

    cin >> tankVolume;

    for(int i = 0; i < cars.size(); i++) {
        if (i == editCarIndex) {
            cars[i].setBrand(brand);
            cars[i].setColor(color);
            cars[i].setEnginePower(enginePower);
            cars[i].setTankVolume(tankVolume);

            cout << "Успешная замена!" << endl;

            break;
        }
    }

    showAllCars();
}

/**
 * Функция нахождения самого мощного двигателя
 */
void findTheMostPowerfulEngine() {
    int    maxEnginePower = 0;
    string carBrand       = "";

    for(int i = 0; i < cars.size(); i++) {
      if (maxEnginePower < cars[i].getEnginePower()) {
          maxEnginePower = cars[i].getEnginePower();
          carBrand       = cars[i].getBrand();
      }
    }

    cout << "Максимальная мощность у автомобиля: " << carBrand << " его мощность: " << maxEnginePower << endl;
}

int main() {
    for(int i = 0; i < 5; i++) {
        Car newCar;

        cars.push_back(newCar);
    }

    editCar();

    findTheMostPowerfulEngine();
}