#pragma once


#include <iostream>
using namespace std;

namespace MyUnputLib

{


	int MyReadPositiveNumber(string message)
	{
		int number;

		do
		{
			cout << message << endl;
			cin >> number;

			while (cin.fail())
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				cout << "Invalid Number , Enter a Valid one : " << endl;
				cin >> number;
			}

		} while (number < 0);

		return number;
	}

	int MyReadNumber(string message)
	{
		int number;

			cout << message << endl;
			cin >> number;

			while (cin.fail())
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				cout << "Invalid Number , Enter a Valid one : " << endl;
				cin >> number;
			}

		return number;
	}

	int MyRandomNumber(int from, int to)
	{
		int Random;
		Random = rand() % (to - from + 1) + from;
		return Random;

	}


}