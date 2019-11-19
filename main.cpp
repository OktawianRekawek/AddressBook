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
    int id;
};

struct User {
    int id;
    string login, password;
};

void saveContact(Contact contact) {
    fstream file;

    file.open("listaKontaktow.txt", ios::app);
    if (file.good()==false) {
        cout << "Zapisanie kontaktu do pliku sie nie powiodlo!" << endl;
    } else {
        file << contact.id << "|";
        file << contact.name << "|";
        file << contact.surname << "|";
        file << contact.phone << "|";
        file << contact.email << "|";
        file << contact.address << "|" << endl;
        file.close();
    }
}

Contact addContact(int id) {
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

vector<Contact> readFromFile() {
    fstream file;
    Contact contact;
    vector<Contact> contacts;
    file.open("listaKontaktow.txt", ios::in);

    if (file.good()==false) {
        return contacts;
    }

    string line;
    int i = 0;

    while(getline(file, line, '|')) {
        switch(i%6) {
        case 0:
            contact.id = atoi(line.c_str());
            break;
        case 1:
            contact.name = line;
            break;
        case 2:
            contact.surname = line;
            break;
        case 3:
            contact.phone = line;
            break;
        case 4:
            contact.email = line;
            break;
        case 5:
            contact.address = line;
            contacts.push_back(contact);
            break;
        }
        i++;
    }
    file.close();
    return contacts;
}

void rewriteAddressBook(vector<Contact> contacts) {
    fstream file;
    file.open("listaKontaktow.txt", ios::out|ios::trunc);

    if (file.good()==false) {
        return;
    }
    file.close();
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
        saveContact(*itr);
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
    cout << "Wpisz id kontaktu do usuniecia: ";
    cin >> id;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++) {
        if (itr->id == id) {
            if (deleteAccept()) {
                itr = contacts.erase(itr) - 1;
                rewriteAddressBook(contacts);
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
            rewriteAddressBook(contacts);
            cout << "Zakonczono edycje!" << endl;
            return contacts;
        }

    }
    cout << "Brak kontaktu o takim id!" << endl;
    return contacts;
}

int loginMenu(int id) {
    vector<Contact> contacts;
    char choice;
    contacts = readFromFile();
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
                contacts.push_back(addContact(0));
            else
                contacts.push_back(addContact(contacts.back().id));
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
            system("pause");
            break;
        case '9':
            return 0;
        default:
            break;
        }

    }
}

int logoutMenu() {
    char choice;

    while(1) {
        system("cls");
        cout << "1. Logowanie" << endl;
        cout << "2. Rejestracja" << endl;
        cout << "9. Zakoncz program" << endl;
        cin >> choice;

        switch (choice) {
        case '1':
            return 1;
        case '2':
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

    int loginUserId = 0;

    while (1) {
        if (loginUserId)
            loginUserId = loginMenu(loginUserId);
        else
            loginUserId = logoutMenu();
    }
    return 0;
}
