#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Person {
    string firstname;
    string lastname;
    optional<string> patronymic;
};

optional<string> getOptStr(string& s) {
    if (s == "") {
        return nullopt;
    }
    return s;
}

ostream& operator<<(ostream& out, const Person& p) {
    out << ' ' << p.firstname << ' ' << p.lastname << ' ';
    if (p.patronymic.has_value()) {
        out << p.patronymic.value();
    }
    return out;
}

bool operator<(const Person& p1, const Person& p2) {
    return tie(p1.firstname, p1.lastname, p1.patronymic) < tie(p2.firstname, p2.lastname, p2.patronymic);
}

bool operator==(const Person& p1, const Person& p2) {
    return tie(p1.firstname, p1.lastname, p1.patronymic) == tie(p2.firstname, p2.lastname, p2.patronymic);
}

struct PhoneNumber {
    int countryCode;
    int cityCode;
    string number;
    optional<int> additionalNumber;
};

optional<int> getOptInt(string& s) {
    if (s == "") {
        return nullopt;
    }
    return stoi(s);
}

ostream& operator<<(ostream& out, const PhoneNumber& p) {
    out << '+' << p.countryCode << '(' << p.cityCode << ')' << p.number << ' ';
    if (p.additionalNumber.has_value()) {
        out << p.additionalNumber.value();
    }
    return out;
}

bool operator<(const PhoneNumber& p1, const PhoneNumber& p2) {
    return tie(p1.countryCode, p1.cityCode, p1.number, p1.additionalNumber) < tie(p2.countryCode, p2.cityCode, p2.number, p2.additionalNumber);
}

bool operator==(const PhoneNumber& p1, const PhoneNumber& p2) {
    return tie(p1.countryCode, p1.cityCode, p1.number, p1.additionalNumber) == tie(p2.countryCode, p2.cityCode, p2.number, p2.additionalNumber);
}

class PhoneBook {
private:
    vector<pair<Person, PhoneNumber>> personData;
public:
    PhoneBook(ifstream& file) {
        do {
            string line;
            getline(file, line);
            stringstream str(line);
            vector<string> tmpData;

            for (string s; getline(str, s, ' ');) {
                tmpData.push_back(s);
            }

            pair<Person, PhoneNumber> note;

            for (size_t i = 0; i < tmpData.size(); ++i) {
                switch (i) {
                case 0:
                    note.first.firstname = tmpData[i];
                    break;
                case 1:
                    note.first.lastname = tmpData[i];
                    break;
                case 2:
                    note.first.patronymic = getOptStr(tmpData[i]);
                    break;
                case 3:
                    note.second.countryCode = stoi(tmpData[i]);
                    break;
                case 4:
                    note.second.cityCode = stoi(tmpData[i]);
                    break;
                case 5:
                    note.second.number = tmpData[i];
                    break;
                case 6:
                    note.second.additionalNumber = getOptInt(tmpData[i]);
                    break;
                }
            }
            personData.push_back(note);

        } while (!file.eof());
    }

    friend ostream& operator<<(ostream& out, const PhoneBook& pb) {
        for (const auto& [first, second] : pb.personData) {
            out << first << ' ' << second << endl;
        }
        return out;
    }

    void SortByName() {
        sort(personData.begin(), personData.end(), [](const pair<Person, PhoneNumber>& firstP, const pair<Person, PhoneNumber>& secondP) { return firstP.first < secondP.first; });
    }

    void SortByPhone() {
        sort(personData.begin(), personData.end(), [](const pair<Person, PhoneNumber>& firstP, const pair<Person, PhoneNumber>& secondP) { return firstP.second < secondP.second; });
    }

    pair<string, vector<PhoneNumber>> GetPhoneNumber(const string& firstname) {
        vector<PhoneNumber> phoneNumbers;
        int count = 0;
        for_each(personData.begin(), personData.end(), [&](const auto& entry) {
            if (entry.first.firstname == firstname) {
                phoneNumbers.push_back(entry.second);
                ++count;
            }
            });

        switch (count) {
        case 0:
            return { "not found", phoneNumbers };
        case 1:
            return { "", phoneNumbers };
        default:
            return { "found more than 1", phoneNumbers };
        }
    }

    void ChangePhoneNumber(const Person& p, const PhoneNumber& pn) {
        auto entry = find_if(personData.begin(), personData.end(), [&](const auto& entry) { return entry.first == p; });

        if (entry != personData.end()) {
            entry->second = pn;
        }
    }
};

int main()
{
    ifstream file("PhoneBook.txt");
    if (!file) {
        cerr << "Error: Failed to open file" << endl;
        exit(-1);
    }
    PhoneBook book(file);
    cout << book;

    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
    auto printPhoneNumber = [&book](const string& surname) {
        cout << " " << surname << " ";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty()) {
            for (size_t i = 0; i < get<1>(answer).size(); ++i) {
                cout << get<1>(answer)[i];
            }
        }
        else {
            cout << get<0>(answer);
        }
        cout << endl;
    };

    printPhoneNumber("Ivanov");
    printPhoneNumber("Petrov");

    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;

    return 0;
}