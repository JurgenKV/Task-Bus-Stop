#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <algorithm>
#include <stdlib.h>

using namespace std;

class Company {
    string _companyName;
    string _departureTime;
    string _arrivalTime;
public:
    Company() {}

    Company(string companyName, string departureTime, string arrivalTime) {
        _companyName = companyName;
        _departureTime = departureTime;
        _arrivalTime = arrivalTime;
    }

    void setCompanyName(string companyName) {
        _companyName = companyName;
    }

    void setDepartureTime(string departureTime) {
        _departureTime = departureTime;
    }

    void setArrivalTime(string arrivalTime) {
        _arrivalTime = arrivalTime;
    }

    string getCompanyName() {
        return _companyName;
    }

    string getDepartureTime() {
        return _departureTime;
    }

    string getArrivalTime() {
        return _arrivalTime;
    }

    int getDepHour() {
        string hour = this->_departureTime.substr(0, 2);
        return atoi(hour.c_str());
    }

    int getDepMin() {
        string min = this->_departureTime.substr(3, 2);
        return atoi(min.c_str());
    }

    int getArrHour() {
        string hour = this->_arrivalTime.substr(0, 2);
        return atoi(hour.c_str());
    }

    int getArrMin() {
        string min = this->_arrivalTime.substr(3, 2);
        return atoi(min.c_str());
    }

    int getDepInMin() {
        return (getDepHour() * 60) + getDepMin();
    }

    int getArrInMin() {
        return (getArrHour() * 60) + getArrMin();
    }

    string getFull() {
        return this->_companyName + ' ' + this->_departureTime + ' ' + this->_arrivalTime;
    }

    // Если он начинается позже и достигает раньше.
    bool isEff1(list<Company>::iterator iter, list<Company>::iterator iter2) {
        return ((iter->getArrivalTime() < iter2->getArrivalTime()) &&
                (iter->getDepartureTime() > iter2->getDepartureTime()));
    }

    // Если он начнется позже и достигнет в то же время
    bool isEff2(list<Company>::iterator iter, list<Company>::iterator iter2) {
        return (((iter->getArrivalTime() == iter2->getArrivalTime())) &&
                (iter->getDepartureTime() > iter2->getDepartureTime()));
    }

    // Если он начинается в то же время и достигает более раннего срока
    bool isEff3(list<Company>::iterator iter, list<Company>::iterator iter2) {
        return ((iter->getDepartureTime() == iter2->getDepartureTime()) &&
                //        o Если он начинается в то же время и достигает более раннего срока, или
                (iter->getArrivalTime() < iter2->getArrivalTime()));
    }// Если обе компании предлагают услуги с одинаковым временем - всегда выбирайте Posh.
    bool isEqbutName(list<Company>::iterator iter, list<Company>::iterator iter2) {
        return (((iter->getCompanyName() != "Grotty") &&
                 (iter->getDepartureTime() == iter2->getDepartureTime())) &&
                (iter->getArrivalTime() == iter2->getArrivalTime()));
    }

    bool isEqual(list<Company>::iterator iter, list<Company>::iterator iter2) {
        return (((iter->getCompanyName() == iter2->getCompanyName()) &&
                 (iter->getDepartureTime() == iter2->getDepartureTime())) &&
                (iter->getArrivalTime() == iter2->getArrivalTime()));
    }

    friend bool cmpDepTime(Company a, Company b);

};

bool cmpDepTime(Company a, Company b) {
    return a.getDepInMin() < b.getDepInMin();
}


class TimeTable : public Company {
public:

    list<Company> infoFromFile(string filePath) {

        list<Company> listOfComp;
        string line;
        ifstream file(filePath);
        string companyEditLine;

        string name;
        string fTime;
        string lTime;

        if (file.is_open()) {
            while (getline(file, line)) {
                companyEditLine = line;

                for (int i = 0; i < companyEditLine.size(); i++) {
                    if (companyEditLine[i] == ' ') {
                        name = companyEditLine.substr(0, i);
                        companyEditLine.erase(0, i + 1);
                        break;
                    }
                }
                for (int i = 0; i < companyEditLine.size(); i++) {
                    if (companyEditLine[i] == ' ') {
                        fTime = companyEditLine.substr(0, i);
                        companyEditLine.erase(0, i + 1);
                        lTime = companyEditLine;
                        break;
                    }
                }
                Company company = *new Company(name, fTime, lTime);
                listOfComp.push_back(company);
            }
        }
        file.close();
        return listOfComp;
    }

    void createOutputFile(list<Company> listToFile) {
        list<Company>::iterator iter;
        string nameFirst = "Posh";
        string nameSec = "Grotty";
        list<Company> listOfFirsCompany = listToFile;
        list<Company> listOfSecCompany = listToFile;

        for (iter = listOfFirsCompany.begin(); iter != listOfFirsCompany.end(); ++iter) {
            if (iter->getCompanyName() == nameSec) {
                listOfFirsCompany.erase(iter);
            }
        }

        for (iter = listOfSecCompany.begin(); iter != listOfSecCompany.end(); ++iter) {
            if (iter->getCompanyName() == nameFirst) {
                listOfSecCompany.erase(iter);
            }
        }

        ofstream outFile;
        outFile.open("D:\\outputTable.txt", ios_base::out | ios_base::trunc);

        for (iter = listOfFirsCompany.begin(); iter != listOfFirsCompany.end(); ++iter) {
            outFile << iter->getFull() << endl;
        }

        outFile << endl;

        for (iter = listOfSecCompany.begin(); iter != listOfSecCompany.end(); ++iter) {
            outFile << iter->getFull() << endl;
        }

        listToFile.clear();
        outFile.close();
    }

    void sortByTime(list<Company> &listToSort) {
        listToSort.sort(cmpDepTime);
    }

    list<Company> notHour(list<Company> listToChange) {
        list<Company> resulList;
        int time = 0;
        auto iter = listToChange.begin();

        for (iter = listToChange.begin(); iter != listToChange.end(); ++iter) {
            time = iter->getArrInMin() - iter->getDepInMin();
            if (time <= 60 && time >= 0) {
                resulList.push_back(*iter);
            }
        }

        listToChange.clear();
        return resulList;
    }

    list<Company> effectiveList(list<Company> listToEffective) {

        list<Company>::iterator iter = listToEffective.begin();
        list<Company>::iterator iter2 = listToEffective.begin();

        while (iter != listToEffective.end()) {
            //  cout << iter->getFull()<< endl;
            while (iter2 != listToEffective.end()) {
                if (iter == iter2) {
                    ++iter2;
                    continue;
                }

                if (!isEff2(iter, iter2) && !isEqbutName(iter, iter2) && !isEff3(iter, iter2) && !isEff1(iter, iter2) &&
                    !isEqual(iter, iter2)) {
                    ++iter2;

                } else {
                    listToEffective.erase(iter2);
                    iter2 = listToEffective.begin();
                }
            }
            iter2 = listToEffective.begin();
            ++iter;
        }

        return listToEffective;
    }

};

int main() {
    list<Company> table;
    string filePath = "D:\\inputData.txt";
    TimeTable timeTable;

    //Читаем информацию из файла
    table = timeTable.infoFromFile(filePath);

    //Отбираем маршруты по времени
    table = timeTable.notHour(table);

    //Отбираем эффективные маршруты
    table = timeTable.effectiveList(table);

    //Сортируем список маршрутов
    timeTable.sortByTime(table);

    //Записываем готовый список маршрутов в файл, разделив его на два списка компаний
    timeTable.createOutputFile(table);

    return 0;
}