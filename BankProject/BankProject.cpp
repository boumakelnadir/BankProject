#include<fstream>
#include<iomanip>
#include<iostream>
#include<string>
#include<vector>

#include "MyUnputLib.h";

using namespace std;
using namespace MyUnputLib;

const string ClientsFileName = "Client.txt";

enum enMainMenueOptions
{
	enClientList   = 1,   enAddNewClient = 2,
	enDeleteClient = 3,   enUpdateClient = 4,
	enFindClient   = 5,   enTransations  = 6, 
	enExit		   = 7
};

enum enTransationsMenueOptions
{
	enDeposit		= 1,  enWithdraw  = 2,
	enTotalBalances = 3 , enMainMenue = 4,
};

struct stClient {
	string AccountNumber, Name;
	int PinCode = 0, PhoneNumber = 0;
	float Balance = 0.0;
	bool MarkForDelete = 0;
};

void ShowTransationsMenueScreen();

void ShowMainMenueScreen();

void GoBackToMainScreen()
{
	cout << "\n\nPress Any Key To go back to main Menue ....";
	system("pause>0");
	ShowMainMenueScreen();
}

void GoBackToTransationsMenueScreen()
{
	cout << "\n\nPress Any Key To go back to Transations Menue ....";
	system("pause>0");
	ShowTransationsMenueScreen();
}

void ClearScreen()
{
	system("cls");
}

vector <string> SplitString(string Line, string Separator)
{
	vector <string> vString;
	short pos = 0;
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

stClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	stClient Client;

	vector <string> vClient = SplitString(Line, Separator);

	Client.AccountNumber = vClient[0];
	Client.PinCode = stoi(vClient[1]);
	Client.Name = vClient[2];
	Client.PhoneNumber = stoi(vClient[3]);
	Client.Balance = stof(vClient[4]);

	return Client;
}

string ConvertRecordToLine(stClient Client , string Separator = "#//#")
{
	string Line;
	
	Line += Client.AccountNumber + Separator;
	Line += to_string(Client.PinCode) + Separator;
	Line += Client.Name + Separator;
	Line += to_string(Client.PhoneNumber) + Separator;
	Line += to_string(Client.Balance);

	return Line;
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
			Client = ConvertLineToRecord(Line);
			vClient.push_back(Client);
		}
		ClientFile.close();
	}
	return vClient;
}

void PrintClientRecordLine(stClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(12) << Client.PinCode;
	cout << "| " << left << setw(35) << Client.Name;
	cout << "| " << left << setw(17) << Client.PhoneNumber;
	cout << "| " << left << setw(12) << Client.Balance ;
}

void ShowAllClientsListScreen()
{
	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);

	cout << "\t\t\t\tClient List (" << vClient.size() << ") Client (s)." << endl;
	cout << "--------------------------------------------------------------------------------------------------\n";
	cout << "| "  << setw(12)<< left << "Account Number ";
	cout << "| "  << setw(12)<< left << "Pin Code ";
	cout << "| "  << setw(35)<< left << "Client Name ";
	cout << "| "  << setw(17)<< left << "Phone Number ";
	cout << "| "  << setw(12)<< left << "Balance |";
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

bool ClientExistsByAccountNumber(string AccountNumber, string ClientFileName)
{
	fstream File;
	File.open(ClientFileName , ios::in);
	
	if (File.is_open())
	{
		string Line;
		stClient Client;

		while (getline(File, Line))
		{
			Client = ConvertLineToRecord(Line);

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

void SaveDataLineToFile(string FileName ,string Line)
{
	fstream File;
	File.open(FileName, ios::out | ios::app);

	if (File.is_open())
	{
		File << Line << endl;
	}
	File.close();
}

void CardScreen(string ScreenName)
{
	cout << "\n--------------------------------------------\n";
	cout << "\t " << ScreenName <<"\n";
	cout << "--------------------------------------------\n";
}

void AddNewClient()
{
	stClient Client;
	
	Client = ReadNewClient();

	SaveDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
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
	CardScreen("Add New Clients Screen");

	AddNewClients();
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

void PrintClientCard(stClient Client)
{
	cout << "\nThe Following are The Client Details :\n";
	cout << "\n--------------------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Code Pin       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone Number   : " << Client.PhoneNumber << endl;
	cout << "Account Balance: " << Client.Balance << endl;
	cout << "\n--------------------------------------------\n";
}

void MarkClientByAccountNumber(string AccountNumber, vector <stClient>& vClient ) 
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
				Line = ConvertRecordToLine(C);
				File << Line << endl;
			}
		}
	}
	File.close();
		
}

string ReadClientAccountNumber(string msg)
{
	string  AccountNumbr;
	cout << msg ;
	cin >> AccountNumbr;
	return AccountNumbr;
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
	CardScreen("Delete Client Screen");

	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber("\nPlease Enter Account Number ? ");
	DeleteClientByAccountNumber(AccountNumber, vClient);
	
}

void ShowExitScreen()
{
	ClearScreen();

	cout << "--------------------------------------------\n";
	cout << "\t\tExit Screen.\n";
	cout << "--------------------------------------------\n";


	cout << "\n\nPress Any Key To go back to main Menue ....";
	
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

void SaveClientsDataToFile(vector <stClient> vClient, string ClientFileName)
{
	string Line;

	fstream File;
	File.open(ClientFileName, ios::out);

	if (File.is_open())
	{
		for (stClient& C : vClient)
		{
			Line = ConvertRecordToLine(C);
			File << Line << endl;
		}
	}
	File.close();
}

void UpdateClientByAccountNumber(string AccountNumber, vector<stClient> vClient)
{
	stClient Client;
	char Answer = 'n';
	
	if (FindClientByAccountNumber(AccountNumber,vClient,Client))
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
	CardScreen("Update Client Info Screen");

	vector <stClient> vClient = LoadClientFromfile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber("\nPlease Enter Account Number ? ");
	UpdateClientByAccountNumber(AccountNumber, vClient);

}

void ShowFindClientScreen()
{
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

void DepositClientByAccountNumber(string AccountNumber , vector<stClient>& vClient,stClient Client)
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
				cout << "Balance now are : (" << C.Balance<<")" << endl;
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

	while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "\n Client Has Account Number (" << AccountNumber << ") Not Found ! ";
		AccountNumber = ReadClientAccountNumber("Please Enter Another Account Number ? ");
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
	short Withdraw;

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

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);

		WithdrawClientByAccountNumber(AccountNumber, vClient, Client);
	}
	else
	{
		cout << "\n Client Has Account Number (" << AccountNumber << ") Not Found ! ";
		AccountNumber = ReadClientAccountNumber("Please Enter Another Account Number ? ");
		DepositScreen(AccountNumber);
	}

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
	int TotalBalances = 0;
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
	case enMainMenue:
	{
		ShowMainMenueScreen();
		break;
	}
	
	}
}

void ShowTransationsMenueScreen()
{
	ClearScreen();

	CardScreen("Transations Menue Screen");
	
	cout << "\t[1] Deposit." << endl;
	cout << "\t[2] Withdraw." << endl;
	cout << "\t[3] Total Balances." << endl;
	cout << "\t[4] Main Menue." << endl;
	cout << "--------------------------------------------\n";

	short ReadTranMenueOptions = MyReadPositiveNumber("Choose What do you Want to do ? [1 to 4] : ");
	
	PerformanceTransationsOptions((enTransationsMenueOptions)ReadTranMenueOptions);
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
		case enExit:
		{
			ClearScreen();
			ShowExitScreen();
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
	cout << "\t[7] Exit." << endl;
	cout << "--------------------------------------------\n";

	short ReadMainMenueOptions = MyReadPositiveNumber("Choose What do you Want to do ? [1 to 7] : ");

	PerformanceMainOptions((enMainMenueOptions) ReadMainMenueOptions);
}

int main()
{
	ShowMainMenueScreen();
	
	system("pause>0");
	return 0;
}