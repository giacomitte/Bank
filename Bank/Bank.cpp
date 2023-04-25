// Bank.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

const string ClientsFileName = "clients.txt";
const string UsersFileName = "users.txt";

enum enMainMenuePermissions {pAll = -1, pClientsList = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClient = 8, pFindClient = 16, pTransactions = 32, pManageUsers = 64};

enum enTransactionsMenueOptions { tDeposit = 1, tWithdraw = 2, tTotalBalances = 3, tMainMenue = 4 };

enum enManageUsersMenueOptions { mListUsers = 1, mAddNewUser = 2, mDeleteUser = 3, mUpdateUser = 4, mFindUser = 5, mMainMenue = 6 };

enum enMainMenueOptions {enShowClientsList = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, enTransactions = 6, enManageUsers = 7, enLogout = 8};

struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
    bool MarkForDelete = false;
};

struct stUser {
    string UserName;
    string Password;
    short Permissions;
    bool MarkForDelete = false;
};

stUser CurrentUser;

void TransactionsMenue();
void ManageUsersMenue();
void GoBackToMainMenue();
void MainMenue();
bool CheckAccessPermissions(enMainMenuePermissions);
void Login();



vector<string>Split(string Sentence, string Separator = "#//#") {
    vector<string>vString;
    short Pos = 0;
    string Word = "";

    while ((Pos = Sentence.find(Separator)) != std::string::npos) {

        Word = Sentence.substr(0, Pos);

        if (Word != "")
            vString.push_back(Word);

        Sentence.erase(0, Pos + Separator.length());
    }

    if (Sentence != "")
        vString.push_back(Sentence);

    return vString;
}

string ConvertRecordToLine(stClient Client, string Separator = "#//#") {

    string DataLine = "";

    DataLine = Client.AccountNumber + Separator;
    DataLine += Client.PinCode + Separator;
    DataLine += Client.Name + Separator;
    DataLine += Client.Phone + Separator;
    DataLine += to_string(Client.AccountBalance);

    return DataLine;
}

string ConvertUserRecordToLine(stUser User, string Separator = "#//#") {

    string DataLine = "";
    DataLine = User.UserName + Separator;
    DataLine += User.Password + Separator;
    DataLine += to_string(User.Permissions );

    return DataLine;
}

stClient ConvertLineToRecord(string Sentence, string Separator = "#//#") {
    stClient Client;
    vector<string>vClientData;

    vClientData = Split(Sentence);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

stUser ConvertLineToUserRecord(string Line, string Separator = "#//#") {
    stUser User;

    vector<string>vUsers = Split(Line);

    User.UserName = vUsers[0];
    User.Password = vUsers[1];
    User.Permissions = stoi(vUsers[2]);

    return User;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        stClient Client;
        string Line;

        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line, "#//#");
            if (Client.AccountNumber == AccountNumber) {

                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }

    return false;
}

bool UserExistsByUserName(string UserName, string FileName) {

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        stUser User;
        string Line; 
        while (getline(MyFile, Line)) {
            User = ConvertLineToUserRecord(Line);
            if (User.UserName == UserName) {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

stClient ReadNewClient() {
    stClient Client;

    cout << "\nEnter Account Number: ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
        cout << "\nAccount Number already exists, Enter Another One: ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "Enter PinCode: ";
    getline(cin, Client.PinCode);
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    cout << "Enter Phone: ";
    getline(cin, Client.Phone);
    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

short ReadPermissionsToSet() {

    short Permissions = 0;

    char Answer = 'N';
    cout << "\nDo You Want Give Full Access [Y/N]? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {
        return -1;
    }

    cout << "\nDo You Want Give Access To: ";

    cout << "\nList Clients [Y/N]? ";
    cin >> Answer;
    if(toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pClientsList;

    cout << "\nAdd New Client [Y/N]? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pAddNewClient;

    cout << "\nDelete Client [Y/N]? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pDeleteClient;

    cout << "\nUpdate Client [Y/N]? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pUpdateClient;

    cout << "\nFind Client [Y/N]? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pFindClient;

    cout << "\nTransactions [Y/N]? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pTransactions;

    cout << "\nManage Users [Y/N]? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions += enMainMenuePermissions::pManageUsers;

    return Permissions;
}

stUser ReadNewUser() {

    stUser User;

    cout << "\nEnter User Name: ";
    getline(cin >> ws, User.UserName);
    while (UserExistsByUserName(User.UserName, UsersFileName)) {
        cout << "\nUser Name Already Exists, Enter Another User Name: ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Enter Password: ";
    getline(cin, User.Password);
    
    User.Permissions = ReadPermissionsToSet();

    return User;
}

vector<stClient>LoadDataFromFileToVector(string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    vector<stClient>vClients;
    if (MyFile.is_open()) {
        string Line;
        stClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

vector<stUser>LoadUserDataFromFileToVector(string FileName) {
   
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    vector<stUser>vUsers;
    if (MyFile.is_open()) {
        string Line;
        stUser User;
        while (getline(MyFile, Line)) {
            User = ConvertLineToUserRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

vector<stClient>SaveClientsDataToFile(string FileName, vector<stClient>&vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {

        string Line;

        for (stClient& c : vClients) {
            if (c.MarkForDelete != true) {
                Line = ConvertRecordToLine(c);
                MyFile << Line << endl;
            }
            
        }
        MyFile.close();
    }
    return vClients;
}

vector<stUser>SaveUsersDataToFile(string FileName, vector<stUser>& vUsers) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {
        string Line;
        for (stUser& user : vUsers) {
            if (user.MarkForDelete == false) {
                Line = ConvertUserRecordToLine(user);
                MyFile << Line << endl;
            }
        }


        MyFile.close();
    }
    return vUsers;
}

void PrintClient(stClient Client) {
    cout << "\nClient Info:\n";
    cout << "___________________________________________";
    cout << "\nAcccount Number : " << Client.AccountNumber;
    cout << "\nPinCode         : " << Client.PinCode;
    cout << "\nName            : " << Client.Name;
    cout << "\nAcccount Phone  : " << Client.Phone;
    cout << "\nAcccount Balance: " << Client.AccountBalance;
    cout << "\n___________________________________________\n";
}

void PrintUser(stUser User) {
    cout << "\nUser Info:\n";
    cout << "_____________________________";
    cout << "\nUser Name: " << User.UserName;
    cout << "\nPassword: " << User.Password;
    cout << "\nPermissions: " << User.Permissions;
    cout << "\n_____________________________\n";
}

void ShowAccessDeniedMessage() {
    system("cls");
    cout << "\n____________________________________________\n";
    cout << "\t Access Denied Screen\n";
    cout << "  You Do Not Have Permaissions To Do This,\n  Please Contact Your Admin";
    cout << "\n____________________________________________\n";
    GoBackToMainMenue();
}

void AddDataLineToFile(string FileName, string Line) {

    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {
        MyFile << Line << endl;

        MyFile.close();
    }
}

void AddNewClient() {
    stClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewUser() {
    stUser User;
    User = ReadNewUser();
    AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddClients() {

    char AddMore = 'N';
    do {
        system("cls");

        cout << "\nAdding New Client\n";
        AddNewClient();

        cout << "\nClient Added Successfully, Do You Want Add More Clients [Y/N]? ";
        cin >> AddMore;

    } while (AddMore == 'Y' || AddMore == 'y');
}

void AddUsers() {

    stUser User;
    char AddMore = 'N';

    do {

        system("cls");

        cout << "\nAdding New User:\n";
        AddNewUser();

        cout << "\nUser Added Successfully, Do You Want Add More [Y/N]? ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientScreen() {
    
    if (!CheckAccessPermissions(enMainMenuePermissions::pAddNewClient)) {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n_______________________________________\n";
    cout << "\t\t\t\t Add Clients Screen";
    cout << "\n_______________________________________\n";

    AddClients();
}

void ShowAddNewUserScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t\t\t\t Add Users Screen";
    cout << "\n_______________________________________\n";

    AddUsers();
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient>vClients, stClient& Client) {

    for (stClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            Client = c;
            return true;
        }
    }
    return false;
}

bool FindUserByUserName(string UserName, vector<stUser>vUsers, stUser& User) {

    for (stUser& user : vUsers) {
        if (user.UserName == UserName) {
            User = user;
            return true;
        }
    }
    return false;
}

bool FindUserByUserNameAndPassword(string UserName, string Passowrd, stUser& User) {

    vector<stUser>vUsers = LoadUserDataFromFileToVector(UsersFileName);
    for (stUser& user : vUsers) {
        if (user.UserName == UserName && user.Password == Passowrd) {
            User = user;
            return true;
        }
    }
    return false;
}

void PrintClientRecord(stClient Client) {

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(12) << left << Client.PinCode;
    cout << "| " << setw(30) << left << Client.Name;
    cout << "| " << setw(15) << left << Client.Phone;
    cout << "| " << setw(15) << left << Client.AccountBalance;
}

void PrintUserRecord(stUser User) {
    cout << "| " << setw(20) << left << User.UserName;
    cout << "| " << setw(40) << left << User.Password;
    cout << "| " << setw(20) << left << User.Permissions;
}

void ShowClientsList() {

    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\n\t\t\t\t  Client List (" << vClients.size() << ") Client(s).";
    cout << "\n___________________________________________________________________________________________________\n";
    cout << "| " << setw(15) << left << "Account Number";
    cout << "| " << setw(12) << left << "PinCode";
    cout << "| " << setw(30) << left << "Client Name";
    cout << "| " << setw(15) << left << "Phone";
    cout << "| " << setw(15) << left << "Account Balance\n";
    cout << "\n___________________________________________________________________________________________________\n";

    if (vClients.size() == 0)
        cout << "\nNo Clients Available in The System\n";
    else {

        for (stClient& C : vClients) {
            PrintClientRecord(C);
            cout << endl;
        }
    }

    cout << "\n___________________________________________________________________________________________________\n";

}

void ShowUsersList() {
    vector<stUser>vUsers = LoadUserDataFromFileToVector(UsersFileName);

    cout << "\n\t\t\t\t Users List (" << vUsers.size() << ") User(s)\n";
    cout << "________________________________________________________________________________________________________\n";
    cout << "| " << setw(20) << left << "User Name";
    cout << "| " << setw(40) << left << "Password";
    cout << "| " << setw(20) << left << "Permissions";
    cout << "\n________________________________________________________________________________________________________\n";

    if (vUsers.size() == 0)
        cout << "\n\t\t\t\tNo Users Available in The System\n";
    else {
        for (stUser& user : vUsers) {
            PrintUserRecord(user);
            cout << endl;
        }
    }

    cout << "\n________________________________________________________________________________________________________\n";
}

void ShowClientsListScreen() {

    if (!CheckAccessPermissions(enMainMenuePermissions::pClientsList)) {
        ShowAccessDeniedMessage();
        return;
    }

    ShowClientsList();
}

void ShowUsersListScreen() {
    ShowUsersList();
}

bool MarkedForDeleted(string AccountNumber, vector<stClient>&vClients, stClient& Client) {

    for (stClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            c.MarkForDelete = true;
            Client = c;
            return true;
        }
    }
    return false;
}

bool MarkedUserForDeleted(string UserName, vector<stUser>&vUsers, stUser&User) {
    for (stUser& user : vUsers) {
        if (user.UserName == UserName) {
            user.MarkForDelete = true;
            User = user;
            return true;
        }
    }
    return false;
}

string ReadAccountNumber() {
    string AccountNumber;
    getline(cin >> ws, AccountNumber);
    return AccountNumber;
}

string ReadUserName() {
    string UserName;
    getline(cin >> ws, UserName);
    return UserName;
}

string ReadPassword() {
    string Password;
    getline(cin >> ws, Password);
    return Password;;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>&vClients) {
    stClient Client;
    char Answer = 'N';
    vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\nEnter Account Number: ";
    AccountNumber = ReadAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        
        PrintClient(Client);

        cout << "\nAre You Sure You Want To Delete This Client [Y/N]? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y') {
            MarkedForDeleted(AccountNumber, vClients, Client);
            SaveClientsDataToFile(ClientsFileName, vClients);

            //Update Vector
            vClients = LoadDataFromFileToVector(ClientsFileName);

            cout << "\nClient Deleted Successfully\n";
            return true;
        }

    }else{

        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!";
        return false;
    }
}

bool DeleteUserByUserName(string UserName, vector<stUser>& vUsers) {

    stUser User;

    vUsers = LoadUserDataFromFileToVector(UsersFileName);
    
    cout << "\nEnter User Name: ";
    UserName = ReadUserName();

    if (UserName == "Admin") {
        cout << "You Can not Delete This User\n";
        return false;
    }
    
    if (FindUserByUserName(UserName, vUsers, User)) {

        PrintUser(User);

        char Answer = 'N';
        cout << "\nAre You Sure You Want Delete This User [Y/N]? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y') {

            MarkedUserForDeleted(UserName, vUsers, User);

            SaveUsersDataToFile(UsersFileName, vUsers);

            vUsers = LoadUserDataFromFileToVector(UsersFileName);

            cout << "\nUser Deleted Successfully\n";
            return true;
        }
        
    }
    else {
        cout << "\nUser With User Name (" << UserName << ") Not Found\n";
        return false;
    }
}

void ShowDeleteClientScreen() {

    if (!CheckAccessPermissions(enMainMenuePermissions::pDeleteClient)) {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n_______________________________________\n";
    cout << "\t   Delete Client Screen";
    cout << "\n_______________________________________\n";
    string AccountNumber;
    vector<stClient> vClients;

    DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowDeleteUserScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Delete User Screen";
    cout << "\n_______________________________________\n";
    
    string UserName;
    vector<stUser> vUsers;

    DeleteUserByUserName(UserName, vUsers);
}

stClient ChangeClientRecord(string AccountNumber) {
    stClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\nEnter PinCode: ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    cout << "Enter Phone: ";
    getline(cin, Client.Phone);
    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;

    return Client;
}

stUser ChangeUserRecord(string UserName) {
    stUser User;

    cout << "\nEnter User Name: ";
    getline(cin >> ws, User.UserName);

    cout << "Enter Password: ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>&vClients) {
    stClient Client;
    char Answer = 'N';

    vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\nEnter Account Number: ";
    AccountNumber = ReadAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClient(Client);

        cout << "\nAre You Sure You Want Update This Client [Y/N]? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y') {

            for (stClient& c : vClients) {
                if (c.AccountNumber == AccountNumber) {
                    c = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveClientsDataToFile(ClientsFileName, vClients);

            vClients = LoadDataFromFileToVector(ClientsFileName);

            cout << "\nClient Updated Successfully\n";
            return true;
        }
    }
    else {
        cout << "\nClient With Account Number (" << AccountNumber << ") Not Found!";
        return false;
    }
}

bool UpdateUserByUserName(string UserName, vector<stUser>& vUsers) {
    stUser User;
    char Answer = 'N';

    vUsers = LoadUserDataFromFileToVector(UsersFileName);

    cout << "\nEnter UserName: ";
    UserName = ReadUserName();

    if (FindUserByUserName(UserName, vUsers, User)) {

        PrintUser(User);

        cout << "\nAre You Sure You Want Update This User [Y/N]? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y') {

            for (stUser& user : vUsers) {
                if (User.UserName == UserName) {
                    user = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);

            vUsers = LoadUserDataFromFileToVector(UsersFileName);

            cout << "\nUser Updated Successfully\n";
            return true;
        }

    }
    else {
        cout << "\nUser With User Name (" << UserName << ") Not Found!\n";
        return false;
    }
}

void ShowUpdateClientScreen() {

    if (!CheckAccessPermissions(enMainMenuePermissions::pUpdateClient)) {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n_______________________________________\n";
    cout << "\t   Update Client Screen";
    cout << "\n_______________________________________\n";

    string AccountNumber;
    vector<stClient>vClients;

    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateUserScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Update User Screen";
    cout << "\n_______________________________________\n";

    string UserName;
    vector<stUser>vUsers;

    UpdateUserByUserName(UserName, vUsers);
}

void ShowFindClientScreen() {

    if (!CheckAccessPermissions(enMainMenuePermissions::pFindClient)) {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n_______________________________________\n";
    cout << "\t   Find Client Screen";
    cout << "\n_______________________________________\n";

    string AccountNumber;
    stClient Client;
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\nEnter Account Number: ";
    AccountNumber = ReadAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClient(Client);
    }
    else {
        cout << "\nClient With Account Number (" << AccountNumber << ") Not Found!\n";
    }

}

void ShowFindUserScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Find User Screen";
    cout << "\n_______________________________________\n";
    
    stUser User;
    vector<stUser>vUsers = LoadUserDataFromFileToVector(UsersFileName);

    string UserName;
    cout << "\nEnter UserName: ";

    UserName = ReadUserName();

    if (FindUserByUserName(UserName, vUsers, User)) {
        PrintUser(User);
    }
    else {
        cout << "\nUser With UserName (" << UserName << ") Not Found!\n";
    }

}

void ShowLogoutScreen() {
    Login();

}

bool Deposit(string AccountNumber, double Amount, vector<stClient>& vClients) {

    char Answer = 'N';
    cout << "\nAre You Sure You Want Preform This Transaction: ";
    cin >> Answer;

    if (toupper(Answer) == 'Y') {

        for (stClient& c : vClients) {
            if (c.AccountNumber == AccountNumber) {
                c.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);

                cout << "\nDone Successfully, New Balance is: " << c.AccountBalance << endl;
                return true;
            }
        }
        return false;
    }
}

void ShowDepositScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Deposit Screen";
    cout << "\n_______________________________________\n";

    stClient Client;
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\nEnter AccountNumber: ";
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "\nClient With Account Number Not Found, Enter AccountNumber: ";
        AccountNumber = ReadAccountNumber();
    }

    PrintClient(Client);

    double Amount = 0;
    cout << "\nEnter Deposit Amount: ";
    cin >> Amount;
    
    Deposit(AccountNumber, Amount, vClients);
}

void ShowWithdrawScreen() {
    cout << "\n_______________________________________\n";
    cout << "\t   Withdraw Screen";
    cout << "\n_______________________________________\n";

    stClient Client;
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\nEnter Account Number: ";
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "\nClient With Account Number Not Found, Enter Account Number: ";
        AccountNumber = ReadAccountNumber();
    }

    PrintClient(Client);

    double Amount;
    cout << "Enter Amount: ";
    cin >> Amount;

    while (Amount > Client.AccountBalance) {
        cout << "\nAmount Exceeds The Balance, You Can Withdraw Up to: " << Client.AccountBalance << ", Enter Amount : ";
        cin >> Amount;
    }

    Deposit(AccountNumber, Amount * -1, vClients);
}

void PrintClientRecordBalanceLine(stClient Client) {

    cout << "| " << setw(20) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(20) << left << Client.AccountBalance;
}

void ShowTotalBalancesScreen() {
    vector<stClient>vClients = LoadDataFromFileToVector(ClientsFileName);

    cout << "\n\t\t\t\tTotal Balances List (" << vClients.size() << ") Client(s)\n";
    cout << "______________________________________________________________________________________________________\n";
    cout << "| " << setw(20) << left << "Account Number";
    cout << "| " << setw(40) << left << "Client Name";
    cout << "| " << setw(20) << left << "Account Balance\n";
    cout << "\n______________________________________________________________________________________________________\n";

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\n\t\tNo Clients Available in The System!\n";
    else {
        for (stClient& c : vClients) {
            TotalBalances += c.AccountBalance;

            PrintClientRecordBalanceLine(c);
            cout << endl;
        }
    }

    cout << "\n______________________________________________________________________________________________________\n";

    cout << "\n\t\t\t\t\tTotal Balances: " << TotalBalances << endl;

}

void GoBackToTransactionsMenue() {
    cout << "\nPress Any Key To Go Back To Transactions Menue...";
    system("pause>0");
    TransactionsMenue();
}

void GoBackToManageUsersMenue() {
    cout << "\nPress Any Key To Go Back To Manage Users Menue...";
    system("pause>0");
    ManageUsersMenue();
}

void GoBackToMainMenue() {
    cout << "\nPress Any Key To Go Back To Main Menu...";
    system("pause>0");
    MainMenue();
}

short ReadTransactionsMenueOption() {
    short Choice = 0;

    cout << "\nChoose an Option From [1 to 4]: ";
    cin >> Choice;

    while (Choice > 4 || Choice < 1) {
        cout << "\nChoose an Option From [1 to 4]: ";
        cin >> Choice;
    }

    return Choice;
}

short ReadManageUsersMenueOption() {
    short Choice = 0;

    cout << "\nChoose An Option From [1 to 6]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 6) {
        cout << "\nChoose An Option From [1 to 6]: ";
        cin >> Choice;
    }
    return Choice;
}

void PreformTransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOption) {
    switch (TransactionsMenueOption) {
    case enTransactionsMenueOptions::tDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsMenueOptions::tWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsMenueOptions::tTotalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    case enTransactionsMenueOptions::tMainMenue:
        GoBackToMainMenue();
        break;
    }
}

void PreformManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption) {
    switch (ManageUsersMenueOption) {
    case enManageUsersMenueOptions::mListUsers:
        system("cls");
        ShowUsersListScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::mAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::mDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::mUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::mFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::mMainMenue:
        MainMenue();
        break;
    }
}

void TransactionsMenue() {

    if (!CheckAccessPermissions(enMainMenuePermissions::pTransactions)) {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");

    cout << "\n_______________________________________\n";
    cout << "\tTransactions Menue Screen";
    cout << "\n_______________________________________\n";
    cout << "  [1] Deposit\n";
    cout << "  [2] Withdraw\n";
    cout << "  [3] Total Balances\n";
    cout << "  [4] Main Menue\n";
    cout << "\n_______________________________________\n";

    PreformTransactionsMenueOption((enTransactionsMenueOptions) ReadTransactionsMenueOption());
}

void ManageUsersMenue() {

    if (!CheckAccessPermissions(enMainMenuePermissions::pManageUsers)) {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "\n_______________________________________\n";
    cout << "\tManage Users Menue Screen";
    cout << "\n_______________________________________\n";
    cout << "  [1] List Users\n";
    cout << "  [2] Add New User\n";
    cout << "  [3] Delete User\n";
    cout << "  [4] Update User\n";
    cout << "  [5] Find User\n";
    cout << "  [6] Main Menue\n";
    cout << "\n_______________________________________\n";

    PreformManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());
}

short ReadMainMenueOption() {
    short Choice;
    cout << "Choose an Option From [1 to 8]: ";
    cin >> Choice;

    while (Choice > 8 || Choice < 1) {
        cout << "Choose an Option From [1 to 8]: ";
        cin >> Choice;
    }
    return Choice;
}

void PreformMainMenueOption(enMainMenueOptions MainMenueOption) {

    switch (MainMenueOption) {
    case enMainMenueOptions::enShowClientsList:
        system("cls");
        ShowClientsListScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enAddNewClient:
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enTransactions:
        system("cls");
        TransactionsMenue();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enManageUsers:
        system("cls");
        ManageUsersMenue();
        GoBackToMainMenue();
        break;
    case enMainMenueOptions::enLogout:
        system("cls");
        ShowLogoutScreen();
        break;
    }
}

void MainMenue() {

    system("cls");

    cout << "\n_______________________________________\n";
    cout << "\t   Main Menue Screen";
    cout << "\n_______________________________________\n";
    cout << "  [1] Show Clients List\n";
    cout << "  [2] Add New Client\n";
    cout << "  [3] Delete Client\n";
    cout << "  [4] Update Client\n";
    cout << "  [5] Find Client\n";
    cout << "  [6] Transactions\n";
    cout << "  [7] Manage Users\n";
    cout << "  [8] Logout\n";
    cout << "\n_______________________________________\n";

    PreformMainMenueOption((enMainMenueOptions)ReadMainMenueOption());

}

bool CheckAccessPermissions(enMainMenuePermissions Permissions) {

    if ((CurrentUser.Permissions == enMainMenuePermissions::pAll))
        return true;
    if ((Permissions & CurrentUser.Permissions) == Permissions)
        return true;
    else
        return false;
}

bool LoadUserInfo(string UserName, string Password) {

    if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
        return true;
    else
        return false;
}

void Login() {
    
    string UserName, Password;
    bool LoginFailed = false;

    do {

        system("cls");
        cout << "\n_______________________________________\n";
        cout << "\t Login Screen";
        cout << "\n_______________________________________\n";

        if (LoginFailed) 
            cout << "\nInvalid UserName/Password";
            
        cout << "\nEnter User Name: ";
        UserName = ReadUserName();

        cout << "\nEnter Password: ";
        Password = ReadPassword();
        
        LoginFailed = !LoadUserInfo(UserName, Password);

    } while (LoginFailed);

    MainMenue();
}

int main()
{

    Login();
    
    system("pause>0");
    return 0;
}


