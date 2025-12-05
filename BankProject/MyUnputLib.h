#pragma once

#include <iostream>
#include <string>
#include <limits> // ãØáæÈ áÜ numeric_limits
#include <cstdlib> // ãØáæÈ áÜ rand() æ srand()
#include <ctime> // ãØáæÈ áÜ time() (ÅĞÇ ÃÑÏÊ ÊåíÆÉ srand åäÇ)

using namespace std;

namespace MyInputLib
{
	// ** ÏÇáÉ ŞÑÇÁÉ ÑŞã ãæÌÈ (ãÚ ÇáÊÍŞŞ ãä ÇáÅÏÎÇá) **
	int MyReadPositiveNumber(string message)
	{
		int number;

		do
		{
			cout << message; // ÚÑÖ ÑÓÇáÉ ÇáØáÈ
			cin >> number;

			while (cin.fail())
			{
				cin.clear();
				// ÊäÙíİ ÇáãÎÒä ÇáãÄŞÊ
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				cout << "Invalid Number, Please Enter a Valid one : ";
				cin >> number;
			}

		} while (number < 0); // ÇáÊßÑÇÑ ØÇáãÇ ÇáÑŞã ÃŞá ãä ÕİÑ

		return number;
	}

	// ** ÏÇáÉ ŞÑÇÁÉ Ãí ÑŞã (ãÚ ÇáÊÍŞŞ ãä ÇáÅÏÎÇá) **
	int MyReadNumber(string message)
	{
		int number;

		cout << message;
		cin >> number;

		while (cin.fail())
		{
			cin.clear();
			// ÊäÙíİ ÇáãÎÒä ÇáãÄŞÊ
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			cout << "Invalid Number, Please Enter a Valid one : ";
			cin >> number;
		}

		return number;
	}

	// ** ÏÇáÉ ÊæáíÏ ÑŞã ÚÔæÇÆí Öãä ãÏì ãÍÏÏ **
	int MyRandomNumber(int from, int to)
	{
		// íÌÈ ÇáÊÃßÏ ãä ÇÓÊÏÚÇÁ srand(time(0)) ãÑÉ æÇÍÏÉ İí ÏÇáÉ main
		// áÊåíÆÉ ãæáÏ ÇáÃÑŞÇã ÇáÚÔæÇÆíÉ.
		return rand() % (to - from + 1) + from;
	}
}