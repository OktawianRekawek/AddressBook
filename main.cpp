#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>

using namespace std;

struct Contact {
    string name,
           surname,
           phone,
           email,
           address;
    int id, userId;
};

struct User {
    int id;
    string login, password;
};

void saveContact(Contact contact) {
    fstream file;

    file.open("Adresaci.txt", ios::app);
    if (file.good()==false) {
        cout << "Zapisanie kontaktu do pliku sie nie powiodlo!" << endl;
    } else {
        file << contact.userId << "|";
        file << contact.id << "|";
        file << contact.name << "|";
        file << contact.surname << "|";
        file << contact.phone << "|";
        file << contact.email << "|";
        file << contact.address << "|" << endl;
        file.close();
    }
}

Contact addContact(int id, int loginUserId) {
    Contact newContact;

    cout << "Podaj imie: ";
    cin >> newContact.name;
    cout << "Podaj nazwisko: ";
    cin >> newContact.surname;
    cout << "Podaj nr telefonu: ";
    cin >> newContact.phone;
    cout << "Podaj email: ";
    cin >> newContact.email;
    cout << "Podaj adres: ";
    cin.sync();
    getline(cin, newContact.address);
    newContact.id = id + 1;
    newContact.userId = loginUserId;
    saveContact(newContact);
    cout << endl << "Dodano kontakt" << endl << endl;
    return newContact;
}

void printContact(Contact contact) {

    cout << "ID kontaktu: " << contact.id << endl;
    cout << contact.name << " " << contact.surname << endl;
    cout << "Nr telefonu: " << contact.phone << endl;
    cout << "E-mail: " << contact.email << endl;
    cout << "Adres: " << contact.address << endl << endl;
}

void findContactByName(vector<Contact> contacts) {
    string searches;
    bool found = false;
    cout << "Wpisz szukane imie: ";
    cin >> searches;
    cout << endl << "Wynik wyszukiwania:" << endl << endl;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++) {
        if (itr->name == searches) {
            printContact(*itr);
            found = true;
        }
    }
    if (found == false)
        cout << "Nie znaleziono kontaku o takim imieniu." << endl;
    cout << endl;
}

void findContactBySurname(vector<Contact> contacts) {
    string searches;
    bool found = false;
    cout << "Wpisz szukane nazwisko: ";
    cin >> searches;
    cout << endl << "Wynik wyszukiwania:" << endl << endl;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++) {
        if (itr->surname == searches) {
            printContact(*itr);
            found = true;
        }
    }
    if (found == false)
        cout << "Nie znaleziono kontaku o takim nazwisku." << endl;
    cout << endl;
}

void printAllContacts(vector<Contact> contacts) {
    if (contacts.empty()) {
        cout << endl << "Lista kontaktow jest pusta" << endl;
    } else {
        for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++) {
            printContact(*itr);
            cout << endl;
        }
    }
}

vector<Contact> readFromFile(int loginUserId) {
    fstream file;
    Contact contact;
    vector<Contact> contacts;
    file.open("Adresaci.txt", ios::in);

    if (file.good()==false) {
        return contacts;
    }

    string line;
    int i = 0;

    while(getline(file, line, '|')) {
        if (i == 0) {
            contact.userId = atoi(line.c_str());
        } else if (contact.userId == loginUserId) {
            switch(i) {
            case 1:
                contact.id = atoi(line.c_str());
                break;
            case 2:
                contact.name = line;
                break;
            case 3:
                contact.surname = line;
                break;
            case 4:
                contact.phone = line;
                break;
            case 5:
                contact.email = line;
                break;
            case 6:
                contact.address = line;
                contacts.push_back(contact);
                break;
            }
        }
        i++;
        if (i == 7)
            i = 0;
    }
    file.close();
    return contacts;
}

void rewriteLine(string line){
    fstream file;
    file.open("Adresaci.txt", ios::app);
    file << line << endl;
    file.close();
}

void rewriteAddressBook(vector<Contact> contacts, int loginUserId) {
    fstream file;

    rename("Adresaci.txt", "Adresaci_stary.txt");
    file.open("Adresaci_stary.txt", ios::in);

    string line;
    int i = 0;
    while(getline(file, line)) {
        if (atoi(line.c_str()) == loginUserId) {
            if (!contacts.empty()) {
                saveContact(contacts[i]);
                i++;
            }
        } else
            rewriteLine(line);
    }
    file.close();
    remove("Adresaci_stary.txt");
}

bool deleteAccept() {
    char choice;
    cout << "Wcisnij 't' aby potwierdzic usuniecie: ";
    cin >> choice;
    if (choice == 't')
        return true;
    else
        return false;

}

vector<Contact> deleteContact (vector<Contact> contacts) {
    int id;
    int loginUserId = contacts[0].userId;
    cout << "Wpisz id kontaktu do usuniecia: ";
    cin >> id;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++) {
        if (itr->id == id) {
            if (deleteAccept()) {
                itr = contacts.erase(itr) - 1;
                rewriteAddressBook(contacts, loginUserId);
                cout << "Usunieto kontakt!" << endl;
            }
            return contacts;
        }
    }
    cout << "Brak kontaktu o takim id!" << endl;
    return contacts;
}

char editContactMenu() {
    char choice;
    while(1) {
        system("cls");
        cout << "1. Imie" << endl;
        cout << "2. Nazwisko" << endl;
        cout << "3. Nr telefonu" << endl;
        cout << "4. E-mail" << endl;
        cout << "5. Adres" << endl;
        cout << "6. Powrot do menu glownego" << endl;
        cout << "Twoj wybor: ";
        cin >> choice;
        if (choice >= '1' && choice <='6')
            return choice;
    }
}

Contact editContactField(Contact contact, char whichField) {
    string newData;
    switch (whichField) {
    case '1':
        cout << "Edytuj imie: ";
        cin >> newData;
        contact.name = newData;
        break;
    case '2':
        cout << "Edytuj nazwisko: ";
        cin >> newData;
        contact.surname = newData;
        break;
    case '3':
        cout << "Edytuj nr telefonu: ";
        cin >> newData;
        contact.phone = newData;
        break;
    case '4':
        cout << "Edytuj e-mail: ";
        cin >> newData;
        contact.email = newData;
        break;
    case '5':
        cout << "Edytuj adres: ";
        cin.sync();
        getline(cin, contact.address);
        break;
    }
    return contact;
}

vector<Contact> editContact(vector<Contact> contacts) {
    int id;
    char whichField;
    cout << "Wpisz id kontaktu do edycji: ";
    cin >> id;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++) {
        if (itr->id == id) {

            whichField = editContactMenu();
            if(whichField == '6')
                return contacts;
            else
                *itr = editContactField(*itr, whichField);
            rewriteAddressBook(contacts, contacts[0].userId);
            cout << "Zakonczono edycje!" << endl;
            return contacts;
        }

    }
    cout << "Brak kontaktu o takim id!" << endl;
    return contacts;
}

void saveUser(User user) {
    fstream file;

    file.open("Uzytkownicy.txt", ios::app);
    if (file.good()==false) {
        cout << "Zapisanie uzytkownika do pliku sie nie powiodlo!" << endl;
    } else {
        file << user.id << "|";
        file << user.login << "|";
        file << user.password << "|" << endl;
        file.close();
    }
}

vector<User> * readUsersFromFile() {
    fstream file;
    User user;
    vector<User> * users = new vector<User>;
    file.open("Uzytkownicy.txt", ios::in);

    if (file.good()==false) {
        return users;
    }

    string line;
    int i = 0;

    while(getline(file, line, '|')) {
        switch(i%3) {
        case 0:
            user.id = atoi(line.c_str());
            break;
        case 1:
            user.login = line;
            break;
        case 2:
            user.password = line;
            users->push_back(user);
            break;
        }
        i++;
    }
    file.close();
    return users;
}

void addUser(vector<User> * users) {
    User newUser;
    cout << "Podaj nazwe uzytkownika: ";
    cin >> newUser.login;
    if (!(users->empty())) {
        vector<User>::iterator itr = users->begin();
        while ( itr != users->end()) {
            if (itr->login == newUser.login) {
                cout <<"Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika: ";
                cin >> newUser.login;
                itr = users->begin();
            } else
                itr++;
        }
        newUser.id = users->back().id+1;
    } else
        newUser.id = 1;
    cout << "Podaj haslo: ";
    cin >> newUser.password;
    users->push_back(newUser);
    saveUser(newUser);
    cout << "Konto zalozone" << endl;
    Sleep(1000);
}

User * login(vector<User> *users) {
    string login, password;
    cout << "Podaj nazwe uzytkownika: ";
    cin >> login;
    vector<User>::iterator itr = users->begin();
    while ( itr != users->end()) {
        if (itr->login == login) {
            for (int i = 0; i < 3; i++) {
                cout <<"Podaj haslo. Pozostalo prob " << 3-i <<": ";
                cin >> password;
                if (password == itr->password) {
                    cout << "Zalogowales sie." << endl;
                    Sleep(1000);
                    return &(*itr);
                }
            }
            cout << "Wpisales 3 razy bledne haslo. Poczekaj 3 sekundy przed kolejna proba";
            Sleep(3000);
            return 0;
        } else
            itr++;
    }
    cout << "Nie ma takiego uzytkownika" << endl;
    Sleep(1000);
    return 0;
}

void changePassword(User *user) {
    cout << "Podaj nowe haslo: ";
    cin >> user->password;
    cout << "Haslo zostalo zmienione" << endl;
    Sleep(1500);
}

void rewriteUsersBook(vector<User> * users) {
    fstream file;
    User user;
    file.open("Uzytkownicy.txt", ios::out|ios::trunc);

    if (file.good()==false) {
        return;
    }
    file.close();
    for (vector<User>::iterator itr = users->begin(); itr != users->end(); itr++) {
        user = *itr;
        saveUser(user);
    }
}

User * loginMenu(vector<User> * users, User *user) {
    vector<Contact> contacts;
    char choice;
    contacts = readFromFile(user->id);
    while(1) {
        system("cls");
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "7. Zmien haslo" << endl;
        cout << "9. Wyloguj sie" << endl;
        cout << "Twoj wybor: ";
        cin >> choice;

        switch (choice) {
        case '1':
            if (contacts.empty())
                contacts.push_back(addContact(0, user->id));
            else
                contacts.push_back(addContact(contacts.back().id, user->id));
            system("pause");
            break;
        case '2':
            findContactByName(contacts);
            system("pause");
            break;
        case '3':
            findContactBySurname(contacts);
            system("pause");
            break;
        case '4':
            printAllContacts(contacts);
            system("pause");
            break;
        case '5':
            contacts = deleteContact(contacts);
            system("pause");
            break;
        case '6':
            contacts = editContact(contacts);
            system("pause");
            break;
        case '7':
            changePassword(user);
            rewriteUsersBook(users);
            system("pause");
            break;
        case '9':
            return NULL;
        default:
            break;
        }

    }
}

User * logoutMenu(vector<User> * users) {
    char choice;

    while(1) {
        system("cls");
        cout << "1. Logowanie" << endl;
        cout << "2. Rejestracja" << endl;
        cout << "9. Zakoncz program" << endl;
        cin >> choice;

        switch (choice) {
        case '1':
            return login(users);
        case '2':
            addUser(users);
            break;
        case '9':
            exit(0);
            break;
        default:
            break;
        }
    }
}

int main() {

    vector<User> * users = readUsersFromFile();
    User *loginUser = NULL;
    while (1) {
        if (loginUser)
            loginUser = loginMenu(users, loginUser);
        else
            loginUser = logoutMenu(users);
    }
    return 0;
}
