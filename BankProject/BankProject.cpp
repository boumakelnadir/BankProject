#include<fstream>
#include<iomanip>
#include<iostream>
#include<string>
#include<vector>

//  › —÷ Â–Â «·„ﬂ »«  √‰ MyReadPositiveNumber „ÊÃÊœ… Â‰«
#include "MyUnputLib.h"
#include "MyDateLib.h"

using namespace std;
using namespace MyInputLib;
using namespace MyDateLib;

const string ClientsFileName = "Client.txt";
const string UserFileName = "Users.txt";

// **********************************************
// ********* «·œÊ«· «·„”«⁄œ… «·„ı⁄·‰ ⁄‰Â« „ı”»ﬁ« *********
// **********************************************

// ≈⁄·«‰ „ı”»ﬁ ··œÊ«· «· Ì  Õ «Ã ≈·Ï „⁄—› Â«
void ShowTransationsMenueScreen();
void LogInScreen();
void ShowManageUserMenueScreen();
bool UserExistsByUsername(string UserName, string UserFileName);
void ShowMainMenueScreen();
vector <string> SplitStringClient(string Line, string Separator); //  „ «·≈»ﬁ«¡ ⁄·ÌÂ« Â‰« Ê·ﬂ‰ ”Ì „ ‰ﬁ· «· ⁄—Ì›

// **********************************************
// ******************* «·ÂÌ«ﬂ· (Structs) *******************
// **********************************************

struct stUser {
	string UserName,
		Password;
	int Permission = 1; // «·«› —«÷Ì 1 (”Ì „ «” Œœ«„Â ··„” Œœ„Ì‰ «·Ãœœ)
	bool MarkForDelete = false;
};

stUser CurrentUser; // „·«ÕŸ…: Ì›÷·  Ã‰» «·„ €Ì—«  «·⁄«„… „À· CurrentUser

enum enMainMenueOptions
{
	enClientList = 1, enAddNewClient = 2,
	enDeleteClient = 3, enUpdateClient = 4,
	enFindClient = 5, enTransations = 6,
	enManageUsers = 7, enLogout = 8
};

enum enTransationsMenueOptions
{
	enDeposit = 1, enWithdraw = 2,
	enTotalBalances = 3, enMainMenueTran = 4,
};

enum enManageUserMenueOptions {
	enUsertList = 1, enAddNewUser = 2,
	enDeleteUser = 3, enUpdateUser = 4,
	enFindUser = 5, enMainMenueManage = 6,
};

enum enMainMenuePermissions
{
	pAll = -1,
	pClientList   = 1,   pAddNewClient = 2,
	pDeleteClient = 4,   pUpdateClient = 8,
	pFindClient   = 16,  pTransations  = 32,
	pManageUsers  = 64
};

struct stClient {
	string AccountNumber,
		Name;

	int PinCode = 0,
		PhoneNumber = 0;

	float Balance = 0.0;

	bool MarkForDelete = 0;
};

// **********************************************
// ************* œÊ«·  ﬁ”Ì„ «·‰’Ê’ Ê«· ÕÊÌ· *************
// **********************************************

// **  „ ‰ﬁ· «· ⁄—Ì› Â‰« ·Ì”»ﬁ ConvertLineToRecordUser **
vector <string> SplitStringClient(string Line, string Separator)
{
	vector <string> vString;
	size_t pos = 0;
	string token;

	while ((pos = Line.find(Separator)) != string::npos)
	{
		token = Line.substr(0, pos);

		if (!token.empty())
		{
			vString.push_back(token);
		}
		Line.erase(0, pos + Separator.length());
	}
	if (!Line.empty())
	{
		vString.push_back(Line);
	}
	return vString;
}

stUser ConvertLineToRecordUser(string Line, string Separator = "#//#")
{
	stUser User;
	vector <string> vUserData;

	vUserData = SplitStringClient(Line, Separator);

	User.UserName = vUserData[0];
	User.Password = vUserData[1];
	// «· √ﬂœ „‰ √‰ «· ÕÊÌ· ≈·Ï ⁄œœ ’ÕÌÕ ¬„‰
	if (vUserData.size() > 2)
		User.Permission = stoi(vUserData[2]);

	return User;
}

stClient ConvertLineToRecordClient(string Line, string Separator = "#//#")
{
	stClient Client;

	vector <string> vClient = SplitStringClient(Line, Separator);

	Client.AccountNumber = vClient[0];
	Client.PinCode = stoi(vClient[1]);
	Client.Name = vClient[2];
	Client.PhoneNumber = stoi(vClient[3]);
	Client.Balance = stof(vClient[4]);

	return Client;
}

// **********************************************
// **************** œÊ«· ﬁ—«¡… «·»Ì«‰«  ***************
// **********************************************

string ReadUsername(string msg)
{
	string user;
	cout << msg;
	// «” Œœ«„ getline(cin >> ws, user) ’ÕÌÕ ·ﬂ‰ ÌÃ» «·«‰ »«Â · »⁄« Â
	getline(cin >> ws, user);
	return user;
}

string ReadUser(string msg)
{
	string s;
	cout << "\n" << msg;
	getline(cin >> ws, s);

	return s;
}

string ReadClientAccountNumber(string msg)
{
	string AccountNumbr;
	cout << msg;
	cin >> ws >> AccountNumbr; // «” Œœ«„ cin >> ws >> var ·ﬁ—«¡… ﬂ·„… Ê«Õœ…/—ﬁ„ »⁄œ  Ã«Â· «·›—«€« 
	return AccountNumbr;
}

// **********************************************
// **************** œÊ«· «· Õ„Ì· Ê«·»ÕÀ ****************
// **********************************************

vector <stUser> LoadUsersDataFromfile(string UsersFileName)
{
	vector <stUser> vUser;

	fstream UserFile;
	UserFile.open(UsersFileName, ios::in);

	if (UserFile.is_open())
	{
		string Line;

		while (getline(UserFile, Line))
		{
			// ** Â‰«  „ «” Œœ«„ CurrentUser „ƒﬁ « · Œ“Ì‰ «·”Ã· À„ ≈÷«› Â ≈·Ï «·‹ vector. Ì›÷·  Ã‰» –·ﬂ. **
			// ** «·Õ· «·√›÷· ÂÊ «” Œœ«„ „ €Ì— „Õ·Ì **
			stUser User = ConvertLineToRecordUser(Line);
			vUser.push_back(User);
		}
		UserFile.close();
	}
	return vUser;
}

vector <stClient> LoadClientFromfile(string ClientFileName)
{
	vector <stClient> vClient;

	fstream ClientFile;
	ClientFile.open(ClientFileName, ios::in);

	if (ClientFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(ClientFile, Line))
		{
			Client = ConvertLineToRecordClient(Line);
			vClient.push_back(Client);
		}
		ClientFile.close();
	}
	return vClient;
}

// **  „  ⁄œÌ· «·œ«·… · √Œ– „—Ã⁄ stUser& User · ⁄Ìœ »Ì«‰«  «·„” Œœ„ «·–Ì  „ «·⁄ÀÊ— ⁄·ÌÂ **
bool FindUserByUsernameAndPassword(string UserName, string Password, stUser& User)
{
	vector <stUser> vUser = LoadUsersDataFromfile(UserFileName);

	for (stUser& U : vUser)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return 1;
		}
	}
	return 0;
}

// **  „  ⁄œÌ· «·œ«·… · √Œ– „—Ã⁄ stUser& User · ⁄Ìœ »Ì«‰«  «·„” Œœ„ «·–Ì  „ «·⁄ÀÊ— ⁄·ÌÂ **
bool FindUserByUsername(string Username, stUser& User)
{
	vector <stUser> vUser = LoadUsersDataFromfile(UserFileName);

	for (stUser& C : vUser)
	{
		if (C.UserName == Username)
		{
			User = C; // Ì „  Œ“Ì‰ «·„” Œœ„ «·–Ì  „ «·⁄ÀÊ— ⁄·ÌÂ ›Ì «·„ €Ì— «·„—Ã⁄Ì
			return 1;
		}

	}
	return 0;
}

bool ClientExistsByAccountNumber(string AccountNumber, string ClientFileName)
{
	fstream File;
	File.open(ClientFileName, ios::in);

	if (File.is_open())
	{
		string Line;
		stClient Client;

		while (getline(File, Line))
		{
			Client = ConvertLineToRecordClient(Line);

			if (Client.AccountNumber == AccountNumber)
			{
				File.close();
				return 1;
			}
		}
	}
	File.close();
	return 0;
}

bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClient, stClient& Client)
{
	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return 1;

		}
	}
	return 0;
}

bool CheckAccessPermission(enMainMenuePermissions Permission)
{
	if (CurrentUser.Permission == enMainMenuePermissions::pAll)
		return 1;

	if ((Permission & CurrentUser.Permission) == Permission)
		return 1;
	else
		return 0;

}

// **********************************************
// **************** œÊ«· «· ÕÊÌ· Ê«· Œ“Ì‰ ****************
// **********************************************

string ConvertRecordToLineClient(stClient Client, string Separator = "#//#")
{
	string Line;

	Line += Client.AccountNumber + Separator;
	Line += to_string(Client.PinCode) + Separator;
	Line += Client.Name + Separator;
	Line += to_string(Client.PhoneNumber) + Separator;
	Line += to_string(Client.Balance);

	return Line;
}

string ConvertRecordToLineUser(stUser User, string Separator = "#//#")
{
	string Line;

	Line += User.UserName + Separator;
	Line += User.Password + Separator;
	Line += to_string(User.Permission);

	return Line;
}

void SaveDataLineToFile(string FileName, string Line)
{
	fstream File;
	File.open(FileName, ios::out | ios::app);

	if (File.is_open())
	{
		cout << "\n";
		File << Line << endl;
	}
	File.close();
}

void SaveClientsDataToFile(vector <stClient> vClient, string ClientFileName)
{
	string Line;

	fstream File;
	File.open(ClientFileName, ios::out);

	if (File.is_open())
	{
		for (stClient& C : vClient)
		{
			Line = ConvertRecordToLineClient(C);
			File << Line << endl;
		}
	}
	File.close();
}

void SaveUserDataToFile(vector <stUser> vUser, string UserFileName)
{
	string Line;

	fstream File;
	File.open(UserFileName, ios::out);

	if (File.is_open())
	{
		for (stUser& C : vUser)
		{
			Line = ConvertRecordToLineUser(C);
			File << Line << endl;
		}
	}
	File.close();
}

// **********************************************
// **************** œÊ«· «·≈œŒ«· Ê«·ﬁ—«¡… ****************
// **********************************************

int ReadPermissionToSet()
{
	int Permission = 0;
	char Answer = 'n';

	cout << "\nDo you want to give full access? (y/n) ? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pAll;
		return -1;
	}

	cout << "\nDo you want to give access to : \n";

	cout << "\nshow client list? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pClientList;
	}
	
	cout << "\nAdd New client ? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pAddNewClient;
	}
	
	cout << "\nDelete client ? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pDeleteClient;
	}

	cout << "\nUpdate client ? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pUpdateClient;
	}

	cout << "\nFind client ? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pFindClient;
	}

	cout << "\nTransation client ? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pTransations;
	}

	cout << "\nManager User ? (y/n)? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permission += enMainMenuePermissions::pManageUsers;
	}

	return Permission;

}

stClient ReadNewClient()
{
	stClient Client;

	cout << "\nEnter Account Number : ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient With (" << Client.AccountNumber << ") Already Exists, Enter Another Account Number : " << endl;
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "\nEnter Pin Code : ";
	cin >> Client.PinCode;

	cout << "Enter Name : ";
	getline(cin >> ws, Client.Name);

	cout << "Enter Phone Number : ";
	cin >> Client.PhoneNumber;

	cout << "Enter Balance : ";
	cin >> Client.Balance;

	return Client;
}

stUser ReadNewUser()
{
	stUser User; // «” Œœ«„ „ €Ì— „Õ·Ì

	cout << "\nEnter Username : ";
	// **  „  ’ÕÌÕ ÿ—Ìﬁ… «·ﬁ—«¡… **
	getline(cin >> ws, User.UserName);

	while (UserExistsByUsername(User.UserName, UserFileName))
	{
		cout << "\nUser With (" << User.UserName << ") Already Exists, Enter Another Username : " << endl;
		getline(cin >> ws, User.UserName);
	}

	cout << "\nEnter Password : ";
	// **  „  ’ÕÌÕ ÿ—Ìﬁ… «·ﬁ—«¡… **
	getline(cin >> ws, User.Password);

	//add Permissions
	User.Permission = ReadPermissionToSet();

	return User;
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "\nEnter Pin Code : ";
	cin >> Client.PinCode;

	cout << "Enter Name : ";
	getline(cin >> ws, Client.Name);

	cout << "Enter Phone Number : ";
	cin >> Client.PhoneNumber;

	cout << "Enter Balance : ";
	cin >> Client.Balance;

	return Client;
}

stUser ChangeUserRecord(string Username)
{
	stUser User;

	User.UserName = Username;

	cout << "\nEnter Password : ";
	cin >> ws >> User.Password; // «” Œœ«„ cin >> ws >> var ·ﬁ—«¡… ﬂ·„… Ê«Õœ…/—ﬁ„ »⁄œ  Ã«Â· «·›—«€« 

	User.Permission = ReadPermissionToSet();

	return User;
}

// **********************************************
// ****************** œÊ«· «·‘«‘«  ******************
// **********************************************

void GoBackToMainScreen()
{
	cout << "\n\nPress Any Key To go back to main Menue ....";
	system("pause>0");
	ShowMainMenueScreen();
}

void GoBackToManageUserMenueScreen()
{
	cout << "\n\nPress Any Key To go back to Manage User Menue ....";
	system("pause>0");
	ShowManageUserMenueScreen();

}

void GoBackToTransationsMenueScreen()
{
	cout << "\n\nPress Any Key To go back to Transations Menue ....";
	system("pause>0");
	ShowTransationsMenueScreen();
}

void CardScreen(string ScreenName)
{
	cout << "\n--------------------------------------------\n";
	cout << "\t " << ScreenName << "\n";
	cout << "--------------------------------------------\n";
}

void PrintClientRecordLine(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(12) << Client.PinCode;
	cout << "| " << left << setw(35) << Client.Name;
	cout << "| " << left << setw(17) << Client.PhoneNumber;
	cout << "| " << left << setw(12) << Client.Balance;
}

void ClearScreen()
{
	system("cls");
}

void ShowAccessDeniedMessage()
{
	cout << "\n--------------------------------------------" << endl;
	cout << "Access Denied , \nyou dont have permission to do this ,\nplease contact Admin " << endl;
	cout << "\n--------------------------------------------" << endl;
}

void ShowAllClientsListScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pClientList))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);

	cout << "\t\t\t\tClient List (" << vClient.size() << ") Client (s)." << endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(12) << left << "Account Number ";
	cout << "| " << setw(12) << left << "Pin Code ";
	cout << "| " << setw(35) << left << "Client Name ";
	cout << "| " << setw(17) << left << "Phone Number ";
	cout << "| " << setw(12) << left << "Balance |";
	cout << "\n--------------------------------------------------------------------------------------------------\n";

	if (vClient.size() == 0)
		cout << "\nNo Clients Available In The System" << endl;

	else
		for (stClient& C : vClient)
		{
			PrintClientRecordLine(C);
			cout << endl;
		}
}

void PrintClientCard(stClient Client)
{
	cout << "\nThe Following are The Client Details :\n";
	cout << "\n--------------------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Code Pin : " << Client.PinCode << endl;
	cout << "Name : " << Client.Name << endl;
	cout << "Phone Number : " << Client.PhoneNumber << endl;
	cout << "Account Balance: " << Client.Balance << endl;
	cout << "\n--------------------------------------------\n";
}

void PrintCardUser(stUser User)
{
	cout << "\nThe Following are The User Details :\n";
	cout << "\n--------------------------------------------\n";
	cout << "Username : " << User.UserName << endl;
	cout << "Password : " << User.Password << endl;
	cout << "Permission : " << User.Permission << endl;
	cout << "\n--------------------------------------------\n";
}

void AddNewClient()
{
	stClient Client;

	Client = ReadNewClient();

	SaveDataLineToFile(ClientsFileName, ConvertRecordToLineClient(Client));
}

void AddNewClients()
{
	char AddMore = 'y';

	do {
		cout << "Adding New Client :\n";

		AddNewClient();

		cout << "\nClient Added Successfully ,do you want to add more clients ? [y/n] : ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	CardScreen("Add New Clients Screen");

	AddNewClients();
}

void MarkClientByAccountNumber(string AccountNumber, vector <stClient>& vClient)
{
	for (stClient& C : vClient)

		if (C.AccountNumber == AccountNumber)

			C.MarkForDelete = 1;

}

void DeletedAndSaveDataClientToFile(vector <stClient> vClient, string ClientFileName)
{

	fstream File;
	File.open(ClientFileName, ios::out);

	if (File.is_open())
	{
		string Line;

		for (stClient& C : vClient)
		{
			if (C.MarkForDelete == 0)
			{
				Line = ConvertRecordToLineClient(C);
				File << Line << endl;
			}
		}
	}
	File.close();

}

void DeleteClientByAccountNumber(string AccountNumber, vector<stClient> vClient) {
	char Answer = 'n';
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{

		PrintClientCard(Client);

		cout << "Are You Sure Want Delete This Client ? [y/n] ? ";
		cin >> Answer;

		if (toupper(Answer) == 'Y')
		{

			MarkClientByAccountNumber(AccountNumber, vClient);
			DeletedAndSaveDataClientToFile(vClient, ClientsFileName);

			cout << "\n\nClient Deleted Successfully.\n\n";
		}
	}
	else
	{
		cout << "\nThe Client Has Account Number (" << AccountNumber << ") Not Found\n\n";
	}

}

void ShowDeleteClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	CardScreen("Delete Client Screen");

	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber("\nPlease Enter Account Number ? ");
	DeleteClientByAccountNumber(AccountNumber, vClient);

}

void UpdateClientByAccountNumber(string AccountNumber, vector<stClient> vClient)
{
	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		cout << "Are You Sure Want Update This Client ? [y/n] ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (stClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(vClient, ClientsFileName);

			cout << "\n\nClient Updated Successfully.\n\n";
		}

	}
	else
	{
		cout << "\nThe Client Has Account Number (" << AccountNumber << ") Not Found\n\n";
	}
}

void ShowUpdateClientInfoScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	CardScreen("Update Client Info Screen");

	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber("\nPlease Enter Account Number ? ");
	UpdateClientByAccountNumber(AccountNumber, vClient);

}

void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	CardScreen("Find Client Screen");

	stClient Client;
	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber("\nPlease Enter Account Number ? ");

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nThe Client Has Account Number (" << AccountNumber << ") Not Found\n\n";
	}

}

void DepositClientByAccountNumber(string AccountNumber, vector<stClient>& vClient, stClient Client)
{
	char Answer = 'n';
	double Amount;

	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			cout << "\nPlease Enter Deposit Amount ? ";
			cin >> Amount;

			cout << "\nAre You sure you want perform this transation? [y/n] ? ";
			cin >> Answer;

			if (tolower(Answer) == 'y')
			{
				C.Balance += Amount;

				cout << "\nTransations Are Done, Status is Successfully.\n";
				cout << "Balance now are : (" << C.Balance << ")" << endl;
				break;
			}
		}
	}
	SaveClientsDataToFile(vClient, ClientsFileName);
}

void DepositScreen(string AccountNumber)
{
	stClient Client;
	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);

	// Õ·ﬁ… «·»ÕÀ Ê«· √ﬂœ „‰ ÊÃÊœ «·⁄„Ì·
	if (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
		{
			cout << "\n Client Has Account Number (" << AccountNumber << ") Not Found ! ";
			AccountNumber = ReadClientAccountNumber("Please Enter Another Account Number ? ");
		}
	}

	PrintClientCard(Client);
	DepositClientByAccountNumber(AccountNumber, vClient, Client);
}

void ShowDepositScreen()
{
	CardScreen("Deposit Screen");
	string AccountNumber = ReadClientAccountNumber("Please Enter Account Number ? ");
	DepositScreen(AccountNumber);
}

void WithdrawClientByAccountNumber(string AccountNumber, vector<stClient>& vClient, stClient Client)
{
	char Answer = 'n';
	float Withdraw; // ÌÃ» √‰ ÌﬂÊ‰ float ·Ì ‰«”» „⁄ Balance

	for (stClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			cout << "\nPlease Enter Withdraw Amount ? ";
			cin >> Withdraw;

			while (C.Balance < Withdraw)
			{
				cout << "\nAmount Exceeds the Balance, You Can Withdraw up to " << C.Balance << endl;
				cout << "\nPlease Enter Amount Again ? ";
				cin >> Withdraw;
			}

			cout << "\nAre You sure you want perform this transation? [y/n] ? ";
			cin >> Answer;

			if (tolower(Answer) == 'y')
			{
				C.Balance -= Withdraw;

				cout << "\nTransations Are Done, Status is Successfully.\n";
				cout << "New Balance is : (" << C.Balance << ")" << endl;
				break;
			}
		}
	}
	SaveClientsDataToFile(vClient, ClientsFileName);
}

void WithdrawtScreen(string AccountNumber)
{
	stClient Client;
	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);

	// Õ·ﬁ… «·»ÕÀ Ê«· √ﬂœ „‰ ÊÃÊœ «·⁄„Ì·
	if (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
		{
			cout << "\n Client Has Account Number (" << AccountNumber << ") Not Found ! ";
			AccountNumber = ReadClientAccountNumber("Please Enter Another Account Number ? ");
		}
	}

	PrintClientCard(Client);
	WithdrawClientByAccountNumber(AccountNumber, vClient, Client);
}

void ShowWithdrawScreen()
{
	CardScreen("Withdraw Screen");
	string AccountNumber = ReadClientAccountNumber("Please Enter Account Number ? ");
	WithdrawtScreen(AccountNumber);
}

void PrintBalancesClientRecordLine(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(35) << Client.Name;
	cout << "| " << left << setw(12) << Client.Balance;
}

void ShowTotalBalancesScreen()
{
	// **  „  ’ÕÌÕ ‰Ê⁄ TotalBalances ≈·Ï float **
	float TotalBalances = 0.0;
	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);

	cout << "\t\t\t\tClient List (" << vClient.size() << ") Client (s)." << endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(12) << left << "Account Number ";
	cout << "| " << setw(35) << left << "Client Name ";
	cout << "| " << setw(12) << left << "Balance";
	cout << "\n--------------------------------------------------------------------------------------------------\n";

	if (vClient.size() == 0)
		cout << "\nNo Clients Available In The System" << endl;

	else
		for (stClient& C : vClient)
		{
			TotalBalances += C.Balance;
			PrintBalancesClientRecordLine(C);
			cout << endl;
		}

	cout << "\n--------------------------------------------------------------------------------------------------\n";
	cout << "\t\t\t\t\tTotal Balances = " << TotalBalances << endl;

}

void PerformanceTransationsOptions(enTransationsMenueOptions TranMenueOpt)
{
	switch (TranMenueOpt)
	{
	case enDeposit:
	{
		ClearScreen();
		ShowDepositScreen();
		GoBackToTransationsMenueScreen();
		break;
	}
	case enWithdraw:
	{
		ClearScreen();
		ShowWithdrawScreen();
		GoBackToTransationsMenueScreen();
		break;
	}
	case enTotalBalances:
	{
		ClearScreen();
		ShowTotalBalancesScreen();
		GoBackToTransationsMenueScreen();
		break;
	}
	case enMainMenueTran:
	{
		ShowMainMenueScreen();
		break;
	}

	}
}

void ShowTransationsMenueScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pTransations))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	CardScreen("Transations Menue Screen");

	cout << "\t[1] Deposit." << endl;
	cout << "\t[2] Withdraw." << endl;
	cout << "\t[3] Total Balances." << endl;
	cout << "\t[4] Main Menue." << endl;
	cout << "--------------------------------------------\n";

	short ReadTranMenueOptions = MyReadPositiveNumber("Choose What do you Want to do ? [1 to 4] : ");

	PerformanceTransationsOptions((enTransationsMenueOptions)ReadTranMenueOptions);
}

void PrintUserRecordLine(stUser& User)
{
	cout << "| " << left << setw(17) << User.UserName;
	cout << "| " << left << setw(17) << User.Password;
	cout << "| " << left << setw(35) << User.Permission;

}

void ShowAllUsersListScreen()
{
	vector <stUser> vUser = LoadUsersDataFromfile(UserFileName);

	cout << "\t\t\t\tUsers List (" << vUser.size() << ") User(s)." << endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(17) << left << "User Name ";
	cout << "| " << setw(17) << left << "Password ";
	cout << "| " << setw(35) << left << "Permissions ";

	cout << "\n--------------------------------------------------------------------------------------------------\n";

	if (vUser.size() == 0)
		cout << "\nNo User Available In The System" << endl;

	else
		for (stUser& U : vUser)
		{
			PrintUserRecordLine(U);
			cout << endl;
		}
}

bool UserExistsByUsername(string UserName, string UserFileName)
{
	fstream File;
	File.open(UserFileName, ios::in);

	if (File.is_open())
	{
		string Line;
		stUser User; // „ €Ì— „Õ·Ì ··»ÕÀ

		while (getline(File, Line))
		{
			User = ConvertLineToRecordUser(Line);

			if (User.UserName == UserName)
			{
				File.close();
				return 1;
			}
		}
	}
	File.close();
	return 0;
}

void AddNewUser()
{
	stUser NewUser;
	
	NewUser = ReadNewUser(); // «” Œœ«„ „ €Ì— „Õ·Ì

	SaveDataLineToFile(UserFileName, ConvertRecordToLineUser(NewUser));
}

void AddNewUsers()
{
	char AddMore = 'y';

	do {
		cout << "Adding New User :\n";

		AddNewUser();

		cout << "\nUser Added Successfully ,\ndo you want to add more Users ? [y/n] : ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewUserScreen()
{
	CardScreen("Add New User Screen");

	AddNewUsers();
}

void MarkUserByUserName(string UserName, vector <stUser>& vUser)
{
	for (stUser& C : vUser)

		if (C.UserName == UserName)

			C.MarkForDelete = 1;

}

void DeletedAndSaveDataUserToFile(vector <stUser> vUser, string UserFileName)
{
	fstream File;
	File.open(UserFileName, ios::out);

	if (File.is_open())
	{
		string Line;

		for (stUser& U : vUser)
		{
			if (U.MarkForDelete == 0)
			{
				Line = ConvertRecordToLineUser(U);
				File << Line << endl;
			}
		}
	}
	File.close();

}

void DeleteUserByUserName(string Username, vector <stUser> vUser)
{
	char Answer = 'n';
	stUser User; // „ €Ì— „Õ·Ì

	if (FindUserByUsername(Username, User)) // «·»ÕÀ Ê Œ“Ì‰ «·„” Œœ„ ›Ì User
	{
		if (Username != "Admin")
		{
			PrintCardUser(User);

			cout << "Are You Sure Want Delete This Client ? [y/n] ? ";
			cin >> Answer;

			if (toupper(Answer) == 'Y')
			{

				MarkUserByUserName(Username, vUser);

				DeletedAndSaveDataUserToFile(vUser, UserFileName);

				cout << "\n\nUser Deleted Successfully.\n\n";
			}

		}
		else {
			cout << "\nYou Can't Delete this user \n";
		}

	}
	else
	{
		cout << "\nThe User Has Username (" << Username << ") Not Found\n\n";
	}
}

void ShowDeleteUserScreen()
{
	CardScreen("Delete User Screen");

	vector <stUser> vUser = LoadUsersDataFromfile(UserFileName);
	string UserName = ReadUsername("\nPlease Enter UserName ? ");
	DeleteUserByUserName(UserName, vUser);

}

void UpdateUserByUsername(string Username, vector<stUser> vUser)
{
	char Answer = 'n';
	stUser User; // „ €Ì— „Õ·Ì

	if (FindUserByUsername(Username, User)) // «·»ÕÀ Ê Œ“Ì‰ «·„” Œœ„ ›Ì User
	{
		PrintCardUser(User);

		cout << "Are You Sure Want Update This User ? [y/n] ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			for (stUser& C : vUser)
			{
				if (C.UserName == Username)
				{
					C = ChangeUserRecord(Username);
					break;
				}
			}
			SaveUserDataToFile(vUser, UserFileName);

			cout << "\n\nUser Updated Successfully.\n\n";
		}

	}
	else
	{
		cout << "\nThe User Has Username (" << Username << ") Not Found\n\n";
	}
}

void ShowUpdateUsertInfoScreen()
{
	CardScreen("Update User Info Screen");

	vector <stUser> vUser = LoadUsersDataFromfile(UserFileName);
	string Username = ReadUsername("\nPlease Enter Username ? ");
	UpdateUserByUsername(Username, vUser);

}

void ShowFindUserScreen()
{
	CardScreen("Find User Screen");

	string Username = ReadUsername("\nPlease Enter Username ? ");
	stUser User; // „ €Ì— „Õ·Ì

	if (FindUserByUsername(Username, User)) // «·»ÕÀ Ê Œ“Ì‰ «·„” Œœ„ ›Ì User
	{
		PrintCardUser(User);
	}
	else
	{
		cout << "\nThe User (" << Username << ") Not Found\n\n";
	}
}

void PerformanceManageUserMenueOptions(enManageUserMenueOptions ManageUserOption)
{
	switch (ManageUserOption)
	{
		case enUsertList:
		{
			ClearScreen();
			ShowAllUsersListScreen();
			GoBackToManageUserMenueScreen();
			break;
		}
	
		case enAddNewUser:
		{
			ClearScreen();
			ShowAddNewUserScreen();
			GoBackToManageUserMenueScreen();
			break;
		}
	
		case enDeleteUser:
		{
			ClearScreen();
			ShowDeleteUserScreen();
			GoBackToManageUserMenueScreen();
			break;
		}
	
		case enUpdateUser:
		{
			ClearScreen();
			ShowUpdateUsertInfoScreen();
			GoBackToManageUserMenueScreen();
			break;
		}
	
		case enFindUser:
		{
			ClearScreen();
			ShowFindUserScreen();
			GoBackToManageUserMenueScreen();
			break;
		}
	
		case enMainMenueManage:
	{
		ClearScreen();
		ShowMainMenueScreen();
		break;
	}
	}
}

void ShowManageUserMenueScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToMainScreen();
		return;
	}

	ClearScreen();
	CardScreen("Manage User Menue Screen");

	cout << "\t[1] List User." << endl;
	cout << "\t[2] Add New User." << endl;
	cout << "\t[3] Delete User." << endl;
	cout << "\t[4] Update User." << endl;
	cout << "\t[5] Find User." << endl;
	cout << "\t[6] Main Menue." << endl;
	cout << "--------------------------------------------\n";

	short ReadTManageUserMenueOptions = MyReadPositiveNumber("Choose What do you Want to do ? [1 to 6] : ");
	PerformanceManageUserMenueOptions((enManageUserMenueOptions)ReadTManageUserMenueOptions);
}

void PerformanceMainOptions(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enClientList:
	{
		ClearScreen();
		ShowAllClientsListScreen();
		GoBackToMainScreen();
		break;
	}

	case enAddNewClient:
	{
		ClearScreen();
		ShowAddNewClientsScreen();
		GoBackToMainScreen();
		break;
	}

	case enDeleteClient:
	{
		ClearScreen();
		ShowDeleteClientScreen();
		GoBackToMainScreen();
		break;
	}

	case enUpdateClient:
	{
		ClearScreen();
		ShowUpdateClientInfoScreen();
		GoBackToMainScreen();
		break;
	}

	case enFindClient:
	{
		ClearScreen();
		ShowFindClientScreen();
		GoBackToMainScreen();
		break;
	}

	case enTransations:
	{
		ClearScreen();
		ShowTransationsMenueScreen();
		break;
	}

	case enManageUsers:
	{
		ClearScreen();
		ShowManageUserMenueScreen();
		break;
	}

	case enLogout:
	{
		ClearScreen();
		LogInScreen();
		break;
	}
	}
}

void ShowMainMenueScreen()
{
	ClearScreen();

	CardScreen("Main Menue Screen ");
	cout << "\t[1] Show Client List." << endl;
	cout << "\t[2] Add New Client." << endl;
	cout << "\t[3] Delete Client." << endl;
	cout << "\t[4] Update Client Info." << endl;
	cout << "\t[5] Find Client." << endl;
	cout << "\t[6] Transations." << endl;
	cout << "\t[7] Manage Users." << endl;
	cout << "\t[8] Logout." << endl;
	cout << "--------------------------------------------\n";

	short ReadMainMenueOptions = MyReadPositiveNumber("Choose What do you Want to do ? [1 to 8] : ");

	PerformanceMainOptions((enMainMenueOptions)ReadMainMenueOptions);
}

bool LoadUserInfo(string Username, string Password)
{
	// ** Ì „  Œ“Ì‰ „⁄·Ê„«  «·„” Œœ„ «·–Ì ﬁ«„ » ”ÃÌ· «·œŒÊ· »‰Ã«Õ ›Ì «·„ €Ì— «·⁄«„ CurrentUser **
	if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void LogInScreen()
{
	string UserName, Password;
	bool FailLogin = false;

	// **  „  ⁄œÌ· „‰ÿﬁ «·ÿ»«⁄… **
	do {
		ClearScreen();
		CardScreen("Log In Screen");

		if (FailLogin)
			cout << "\nInvalid Username / Password !\n";

		cout << "\nEnter Username ? ";
		getline(cin >> ws, UserName);

		cout << "\nEnter Password ? ";
		getline(cin >> ws, Password);

		// «· Õﬁﬁ Ê ÕœÌÀ Õ«·… ›‘·  ”ÃÌ· «·œŒÊ·
		FailLogin = !LoadUserInfo(UserName, Password);

	} while (FailLogin);

	ShowMainMenueScreen();
}

int main()
{
	LogInScreen();

	system("pause>0");
	return 0;
}