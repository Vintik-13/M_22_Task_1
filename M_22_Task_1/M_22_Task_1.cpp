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
#include <vector>

namespace constants {
    const std::string P_TEL = "^\\d{2}-\\d{2}-\\d{2}$";
    const std::string P_NAME = "^[A-Z]{1}[a-z]{0,}$";
    const std::string P_TEL_NAME = "^\\d{2}-\\d{2}-\\d{2}\\s+[A-Z]{1}[a-z]{0,}$";
    const std::string PARS_STRING = "([^\\s]{8})""([\\s]+)""([^\\s]*)";
    const std::string PATH = "telDir.txt";
}

using t_map = std::map <std::string, std::string>;
using n_map = std::map <std::string, std::vector<std::string>>;

//Функция загрузки словаря
void loadTel(t_map& tel_dir, n_map& name_dir) {
    std::ifstream ifs(constants::PATH);
    if (!ifs.is_open()) {
        std::cout << "I can't open the file\n";
        return;
    }
    else {
        int size;
        std::string name;
        std::string tel;
        n_map::iterator it_name;
        ifs >> size;
        for (int i = 0; i < size; i++) {
            ifs >> tel;
            ifs >> name;
            tel_dir[tel] = name;
            std::vector<std::string> tmp;
            it_name = name_dir.find(name);
            if (it_name == name_dir.end()) {
                
                tmp.push_back(tel);
                name_dir.emplace(name, tmp);
            }
            else {                
                it_name->second.push_back(tel);
            }
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
std::string searchNumTel(t_map& tel_dir, std::string str) {

    t_map::iterator it = tel_dir.find(str);
    if (it != tel_dir.end())         
        return it->first + " - " + it->second;    
    else
        return "The phone number does not exist!";    
}

//Функция выбора по фамилии абонента
std::vector<std::string> searchName(n_map& name_dir, std::string str, bool& search_name) {
    std::vector<std::string> tmp;
    n_map::iterator it = name_dir.find(str);
    if (it == name_dir.end())
        return tmp;
    else {
        for (int i = 0; i < it->second.size(); i++) {
            tmp.push_back(it->second[i]);
        }
        search_name = true;
        return tmp;
    }
    
}

//Функция добавления абонента в справочник
void addTel(t_map& tel_dir, n_map& name_dir, std::string str) {
    std::regex p_tel_name(constants::PARS_STRING);
    std::cmatch result;
    std::regex_match(str.c_str(), result, p_tel_name);
    if (tel_dir.find(result[1]) == tel_dir.end()) {
        std::pair <std::string, std::string> el_tel_dir(result[1], result[3]);        
        tel_dir.insert(el_tel_dir);
    }    
    else
        std::cout << "The number already exists!\n";
    n_map::iterator it_name = name_dir.find(result[3]);
    std::vector<std::string> tmp;
    if (it_name == name_dir.end()) {        
        tmp.push_back(result[1]);
        name_dir.emplace(result[3], tmp);
    }
    else {
        tmp.push_back(result[1]);
        it_name->second = tmp;
    }        
}

//Выбор действия
void chosAction(int comand, t_map& tel_dir, n_map name_dir, std::string str) {
    std::vector<std::string> tmp;
    bool search_name{ false };
    switch (comand)
    {
    case 0:
        std::cout << searchNumTel(tel_dir, str);
        break;
    case 1:        
        tmp = searchName(name_dir, str, search_name);
        if (search_name) {
            std::cout << str << std::endl;
            for (auto p : tmp)
                std::cout << p << std::endl;
        }
        else
            std::cout << "There is no subscriber with the last name " << str;
        break;
    case 2:
        addTel(tel_dir, name_dir, str);
        saveTel(tel_dir);
        break;    
    }
}

void startTelDir() {
    t_map tel_dir;
    n_map name_dir;
    loadTel(tel_dir, name_dir);

    std::string str;
    std::cout << "Enter the information to enter or search through the dictionary ";
    std::getline(std::cin, str);
    int comand = validInput(str);
    while (comand == -1) {
        std::cout << "Incorrect input!\nTry again\n";
        std::getline(std::cin, str);
        comand = validInput(str);
    }
    chosAction(comand, tel_dir, name_dir, str);
}

int main()
{
    startTelDir();    
}
