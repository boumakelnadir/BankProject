#pragma once

#include <iostream>
#include <string>
#include <limits> // „ÿ·Ê» ·‹ numeric_limits
#include <cstdlib> // „ÿ·Ê» ·‹ rand() Ê srand()
#include <ctime> 

using namespace std;

namespace MyInputLib
{
	
	int MyReadPositiveNumber(string message)
	{
		int number;

		do
		{
			cout << message; 
			cin >> number;

			while (cin.fail())
			{
				cin.clear();
				
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				cout << "Invalid Number, Please Enter a Valid one : ";
				cin >> number;
			}

		} while (number < 0);

		return number;
	}

	
	int MyReadNumber(string message)
	{
		int number;

		cout << message;
		cin >> number;

		while (cin.fail())
		{
			cin.clear();
			
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			cout << "Invalid Number, Please Enter a Valid one : ";
			cin >> number;
		}

		return number;
	}

	
	int MyRandomNumber(int from, int to)
	{
		
		return rand() % (to - from + 1) + from;
	}
}