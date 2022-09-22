/*Задание 1. Телефонный справочник

Что нужно сделать
Напишите программу «Телефонный справочник».
На вход программе поступают запросы трёх типов:
69-70-30 Ivanov — добавить телефон и фамилию абонента в справочник
69-70-30 — узнать фамилию абонента по номеру телефона
Ivanov — узнать телефон абонента по фамилии 

Необходимо выводить в консоль ответ на запросы 2 и 3. Обратите внимание,
что абоненты по разным номерам могут иметь одинаковые фамилии. В таком
случае на запрос 3 необходимо выдать все номера через пробелы. 
Операции запросов должны работать за O(logn) или O(logn × количество номеров с этой фамилией).*/

#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <regex>

namespace constants {
    const std::string P_TEL = "^\\d{2}-\\d{2}-\\d{2}$";
    const std::string P_NAME = "^[A-Z]{1}[a-z]{0,}$";
    const std::string P_TEL_NAME = "^\\d{2}-\\d{2}-\\d{2}\\s+[A-Z]{1}[a-z]{0,}$";
    const std::string PARS_STRING = "([^\\s]{8})""([\\s]+)""([^\\s]*)";
    const std::string PATH = "telDir.txt";
}

//Функция загрузки словаря
void loadTel(std::map <std::string, std::string>& tel_dir) {
    std::ifstream ifs(constants::PATH);
    if (!ifs.is_open()) {
        std::cout << "I can't open the file\n";
        return;
    }
    else {
        int size;
        std::string name;
        std::string tel;
        ifs >> size;
        for (int i = 0; i < size; i++) {
            ifs >> tel;
            ifs >> name;
            tel_dir[tel] = name;
        }
    }
    ifs.close();
}

//Функция сохранения словаря
void saveTel(std::map <std::string, std::string>& tel_dir) {
    std::ofstream ofs(constants::PATH);
    if (!ofs.is_open()) {
        std::cout << "I can't open the file\n";
        return;
    }
    else {
        int size = tel_dir.size();
        ofs << size; ofs << "\n";
        for (auto p : tel_dir) {
            ofs << p.first << " " << p.second << "\n";
        }
    }
    ofs.close();
}

//Функция валидации данных и выбор действия (попробовал регулярные выражения, это УДОБНО!)
int validInput(const std::string& str) {
    std::regex rgx_tel(constants::P_TEL);
    std::regex rgx_name(constants::P_NAME);
    std::regex rgx_tel_name(constants::P_TEL_NAME);

    if (std::regex_match(str, rgx_tel))
        return 0;
    if (std::regex_match(str, rgx_name))
        return 1;
    if (std::regex_match(str, rgx_tel_name))
        return 2;
    return -1;
}

//Функция выбора по номеру телефона
std::string searchNumTel(std::map <std::string, std::string>& tel_dir, std::string str) { 

    std::map <std::string, std::string>::iterator it = tel_dir.find(str);
    if (it != tel_dir.end())         
        return it->first + " - " + it->second;    
    else
        return "The phone number does not exist!";    
}

//Функция выбора по фамилии абонента
std::map <std::string, std::string> searchName(std::map <std::string, std::string>& tel_dir, std::string str) {
    std::map <std::string, std::string> tmp;
    for (std::map <std::string, std::string>::iterator it = tel_dir.begin(); it != tel_dir.end(); it++) {
        if (it->second == str) {
            std::pair <std::string, std::string> cur(it->first, it->second);
            tmp.insert(cur);
        }            
    }
    return tmp;
}

//Функция добавления абонента в справочник
void addTel(std::map <std::string, std::string>& tel_dir, std::string str) {
    std::regex p_tel_name(constants::PARS_STRING);
    std::cmatch result;
    std::regex_match(str.c_str(), result, p_tel_name);
    if (tel_dir.find(result[1]) == tel_dir.end()) {
        std::pair <std::string, std::string> el_tel_dir(result[1], result[3]);        
        tel_dir.insert(el_tel_dir);
    }
    else
        std::cout << "The number already exists!\n";    
}

//Выбор действия
void chosAction(int comand, std::map <std::string, std::string>& tel_dir, std::string str) {
    switch (comand)
    {
    case 0:
        std::cout << searchNumTel(tel_dir, str);
        break;
    case 1:
        if (searchName(tel_dir, str).size() != 0) {
            for (auto p : searchName(tel_dir, str))
                std::cout << p.second << " - " << p.first << std::endl;
        }
        else
            std::cout << "There is no subscriber with the last name " << str;
        break;
    case 2:
        addTel(tel_dir, str);
        saveTel(tel_dir);
        break;
    default:
        break;
    }
}

void startTelDir() {
    std::map <std::string, std::string> tel_dir;
    loadTel(tel_dir);

    std::string str;
    std::cout << "Enter the information to enter or search through the dictionary ";
    std::getline(std::cin, str);
    int comand = validInput(str);
    while (comand == -1) {
        std::cout << "Incorrect input!\nTry again\n";
        std::getline(std::cin, str);
        comand = validInput(str);
    }
    chosAction(comand, tel_dir, str);
}

int main()
{
    startTelDir();    
}
