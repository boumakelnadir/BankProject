#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime> // ≈÷«›… ·÷„«‰ ⁄„· time() Ê localtime()

#pragma warning(disable : 4996)

using namespace std;

namespace MyDateLib
{
	short NumberOfDaysInMonth(short Month, short Year);

	enum enCompareDates { Before = -1, After = 1, Equal = 0 };

	struct stDate
	{
		short Year;
		short Month;
		short Day;
	};

	struct stPeriod
	{
		MyDateLib::stDate Start;
		MyDateLib::stDate End;
	};

	stDate GetSystemDate()
	{
		stDate Date;

		time_t t = time(0);
		tm* now = localtime(&t);

		Date.Day = now->tm_mday;
		Date.Month = now->tm_mon + 1;
		Date.Year = now->tm_year + 1900;

		return Date;
	}

	short EnterUser(string msg)
	{
		short Number;
		cout << msg;
		cin >> Number;
		return Number;
	}

	string EnterUserString(string msg)
	{
		string StringContent;
		cout << msg;
		getline(cin >> ws, StringContent);

		return StringContent;
	}

	stDate ReadFulldate()
	{
		stDate Date;

		do {
			Date.Day = EnterUser("\nEnter The Day : ");

		} while (Date.Day > 31 || Date.Day < 1);

		do
		{
			Date.Month = EnterUser("Enter The Month : ");

		} while (Date.Month > 12 || Date.Month < 1);

		Date.Year = EnterUser("Enter The Year : ");

		// «· Õﬁﬁ „‰ ’·«ÕÌ… «·ÌÊ„ ›Ì ‘Â— ›»—«Ì—  ÕœÌœ«
		if (Date.Month == 2 && Date.Day > NumberOfDaysInMonth(Date.Month, Date.Year))
		{
			cout << "\nError: Day max is "
				<< NumberOfDaysInMonth(Date.Month, Date.Year)
				<< " for month 2 in year : " << Date.Year << endl;

			do {
				Date.Day = EnterUser("\nEnter The Day again : ");
			} while (Date.Day > NumberOfDaysInMonth(Date.Month, Date.Year) || Date.Day < 1);
		}
		return Date;
	}

	stPeriod ReadFullPeriodDate()
	{
		stPeriod Period;

		cout << "Enter Start Date :\n";
		Period.Start = ReadFulldate();

		cout << "\nEnter End Date :\n";
		Period.End = ReadFulldate();

		return Period;
	}

	bool IsLeapYear(short Year)
	{
		return ((Year % 4 == 0 && Year % 100 != 0) || Year % 400 == 0);
	}

	short NumberOfDaysInMonth(short Month, short Year)
	{
		int DaysInMonth[] = { 31, 28, 31, 30, 31, 31, 30, 31, 30, 31, 30, 31 };

		return (Month == 2) ? (IsLeapYear(Year) ? 29 : 28) : DaysInMonth[Month - 1];
	}

	bool IsLastDayInMonth(stDate Date)
	{
		return (Date.Day == NumberOfDaysInMonth(Date.Month, Date.Year));
	}

	bool IsLastMonthInYear(short Month)
	{
		return (Month == 12);
	}

	bool IsDate1LessThanDate2(stDate Date1, stDate Date2)
	{
		if (Date1.Year != Date2.Year) {
			return Date1.Year < Date2.Year;
		}
		if (Date1.Month != Date2.Month) {
			return Date1.Month < Date2.Month;
		}
		return Date1.Day < Date2.Day;
	}

	stDate IncreaseDateByOneDay(stDate Date)
	{
		if (IsLastDayInMonth(Date))
		{
			if (IsLastMonthInYear(Date.Month))
			{
				Date.Day = 1;
				Date.Month = 1;
				Date.Year++;
			}
			else
			{
				Date.Day = 1;
				Date.Month++;
			}
		}
		else
		{
			Date.Day++;
		}
		return Date;
	}

	stDate IncreaseDateByXDays(short Days, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 1; i <= Days; i++)
		{
			Date = IncreaseDateByOneDay(Date);
		}
		return Date;
	}

	stDate IncreaseDateByOneWeek(stDate Date)
	{
		return IncreaseDateByXDays(7, Date); // «” Œœ«„ œ«·… IncreaseDateByXDays
	}

	stDate IncreaseDateByXWeeks(short Weeks, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 1; i <= Weeks; i++)
		{
			Date = IncreaseDateByOneWeek(Date);
		}
		return Date;
	}

	stDate IncreaseDateByOneMonth(stDate Date)
	{
		if (Date.Month == 12)
		{
			Date.Month = 1;
			Date.Year++;
		}
		else {
			Date.Month++;
		}
		short DaysInMonth = NumberOfDaysInMonth(Date.Month, Date.Year);

		if (Date.Day > DaysInMonth)
		{
			Date.Day = DaysInMonth;
		}
		return Date;
	}

	stDate IncreaseDateByXMonths(short Months, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 0; i < Months; i++)
		{
			Date = IncreaseDateByOneMonth(Date);
		}

		return Date;
	}

	stDate IncreaseDateByOneYear(stDate Date)
	{
		Date.Year++;
		//  ’ÕÌÕ: ≈–« ﬂ«‰ «·ÌÊ„ 29 ›»—«Ì— Ê ÕÊ· ≈·Ï ”‰… €Ì— ﬂ»Ì”…° Ì’»Õ 28 ›»—«Ì—
		if (Date.Month == 2 && Date.Day == 29 && !IsLeapYear(Date.Year))
		{
			Date.Day = 28;
		}
		return Date;
	}

	stDate IncreaseDateByXYears(short Years, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 0; i < Years; i++)
		{
			Date = IncreaseDateByOneYear(Date);
		}
		return Date;
	}

	stDate IncreaseDateByXYearsFaster(short Years, stDate Date)
	{
		Date.Year += Years;
		//  ’ÕÌÕ: ≈–« ﬂ«‰ «·ÌÊ„ 29 ›»—«Ì— Ê ÕÊ· ≈·Ï ”‰… €Ì— ﬂ»Ì”…° Ì’»Õ 28 ›»—«Ì—
		if (Date.Month == 2 && Date.Day == 29 && !IsLeapYear(Date.Year))
		{
			Date.Day = 28;
		}
		return Date;
	}

	stDate IncreaseDateByOneDecade(stDate Date)
	{
		return IncreaseDateByXYearsFaster(10, Date);
	}

	stDate IncreaseDateByXDecades(short Decades, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		// ÌÃ» √‰  ﬂÊ‰ «·Õ·ﬁ… ⁄·Ï ⁄œœ «·”‰Ê«  «·≈Ã„«·Ì Ê·Ì” ⁄·Ï ⁄œœ «·⁄ﬁœ „÷—Ê»« »‹ 10
		for (short i = 0; i < Decades * 10; i++)
		{
			Date = IncreaseDateByOneYear(Date);
		}

		return Date;
	}

	stDate IncreaseDateByXDecadesFaster(short Decades, stDate Date)
	{
		return IncreaseDateByXYearsFaster(Decades * 10, Date);
	}

	stDate IncreaseDateByOneCentury(stDate Date)
	{
		return IncreaseDateByXYearsFaster(100, Date);
	}

	stDate IncreaseDateByOneMillennium(stDate Date)
	{
		return IncreaseDateByXYearsFaster(1000, Date);
	}

	stDate DecreaseDateByOneDay(stDate Date)
	{
		if (Date.Day == 1)
		{
			if (Date.Month == 1)
			{
				Date.Year--;
				Date.Month = 12;
				Date.Day = 31;
			}
			else
			{
				Date.Month--;
				Date.Day = NumberOfDaysInMonth(Date.Month, Date.Year);
			}
		}
		else
		{
			Date.Day--;
		}

		return Date;
	}

	stDate DecreaseDateByXDays(short Days, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 1; i <= Days; i++)
		{
			Date = DecreaseDateByOneDay(Date);
		}
		return Date;
	}

	stDate DecreaseDateByOneWeek(stDate Date)
	{
		return DecreaseDateByXDays(7, Date);
	}

	stDate DecreaseDateByXWeeks(short Weeks, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 1; i <= Weeks; i++)
		{
			Date = DecreaseDateByOneWeek(Date);
		}
		return Date;
	}

	stDate DecreaseDateByOneMonth(stDate Date)
	{
		if (Date.Month == 1)
		{
			Date.Month = 12;
			Date.Year--;
		}
		else {
			Date.Month--;
		}
		short DaysInMonth = NumberOfDaysInMonth(Date.Month, Date.Year);

		if (Date.Day > DaysInMonth)
		{
			Date.Day = DaysInMonth;
		}
		return Date;
	}

	stDate DecreaseDateByXMonths(short Months, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 1; i <= Months; i++)
		{
			Date = DecreaseDateByOneMonth(Date);
		}

		return Date;
	}

	stDate DecreaseDateByOneYear(stDate Date)
	{
		Date.Year--;
		return Date;
	}

	stDate DecreaseDateByXYears(short Years, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 1; i <= Years; i++)
		{
			Date = DecreaseDateByOneYear(Date);
		}
		return Date;
	}

	stDate DecreaseDateByXYearsFaster(short Years, stDate Date)
	{
		Date.Year -= Years;
		return Date;
	}

	stDate DecreaseDateByOneDecade(stDate Date)
	{
		return DecreaseDateByXYearsFaster(10, Date);
	}

	stDate DecreaseDateByXDecades(short Decades, stDate Date)
	{
		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 0; i < Decades * 10; i++)
		{
			Date = DecreaseDateByOneYear(Date);
		}

		return Date;
	}

	stDate DecreaseDateByXDecadesFaster(short Decades, stDate Date)
	{
		return DecreaseDateByXYearsFaster(Decades * 10, Date);
	}

	stDate DecreaseDateByOneCentury(stDate Date)
	{
		return DecreaseDateByXYearsFaster(100, Date);
	}

	stDate DecreaseDateByOneMillennium(stDate Date)
	{
		return DecreaseDateByXYearsFaster(1000, Date);
	}

	short DayOfWeekOrder(stDate Date) {
		short a, y, m;
		a = (14 - Date.Month) / 12;
		y = Date.Year - a;
		m = Date.Month + (12 * a) - 2;
		// Gregorian://0:sun, 1:Mon, 2:Tue...etc
		return (Date.Day + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;
	}

	string DayShortName(short Day)
	{
		string DaysName[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

		return DaysName[Day];
	}

	string MonthShortName(short Month)
	{
		string MonthsName[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

		return MonthsName[Month - 1];
	}

	bool IsEndOfWeek(stDate Date)
	{
		return (DayOfWeekOrder(Date) == 6);
	}

	bool IsWeekEnd(stDate Date)
	{
		short IndexDay = DayOfWeekOrder(Date);

		return (IndexDay == 6 || IndexDay == 5);
	}

	bool IsBusinessDay(stDate Date)
	{
		return !IsWeekEnd(Date);
	}

	short UntilEndOfWeek(stDate Date)
	{
		return (6 - DayOfWeekOrder(Date));
	}

	short UntilEndOfMonth(stDate Date)
	{
		return (NumberOfDaysInMonth(Date.Month, Date.Year) - Date.Day);
	}

	short UntilEndOfYear(stDate Date)
	{
		short DaysInYear = 0;

		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = Date.Month; i <= 12; i++)
		{
			DaysInYear += NumberOfDaysInMonth(i, Date.Year);
		}
		return DaysInYear - Date.Day;
	}

	short CalculateTheActualVacationDays(stDate DateFrom, stDate DateTo)
	{
		short VacationPriodCount = 0;

		while (IsDate1LessThanDate2(DateFrom, DateTo))
		{
			if (IsBusinessDay(DateFrom))
				VacationPriodCount++;

			DateFrom = IncreaseDateByOneDay(DateFrom);
		}
		return VacationPriodCount;
	}

	stDate CalculateReturnDayFromVacation(stDate Date, short VacationNeed)
	{
		short WeekEndcount = 0;

		while (IsWeekEnd(Date))
		{
			Date = IncreaseDateByOneDay(Date);
		}

		//  „ «· €ÌÌ— „‰ size_t ≈·Ï short · Ã‰» C4267
		for (short i = 0; i < VacationNeed + WeekEndcount; i++)
		{
			if (IsWeekEnd(Date))
			{
				WeekEndcount++;
			}

			Date = IncreaseDateByOneDay(Date);
		}

		while (IsWeekEnd(Date))
		{
			Date = IncreaseDateByOneDay(Date);
		}

		return Date;
	}

	bool IsEqualDates(stDate Date1, stDate Date2)
	{
		return (Date1.Year == Date2.Year && Date1.Month == Date2.Month && Date1.Day == Date2.Day);
	}

	enCompareDates ComparedDates(stDate Date1, stDate Date2)
	{
		if (IsEqualDates(Date1, Date2))
			return enCompareDates::Equal;

		else if (IsDate1LessThanDate2(Date1, Date2))
			return enCompareDates::Before;

		else
			return enCompareDates::After;
	}

	bool IsOverLapPeriod(stPeriod Date1, stPeriod Date2)
	{
		// ·« ÌÊÃœ  œ«Œ· ≈–« ﬂ«‰  ‰Â«Ì… «·› —… 2 ﬁ»· »œ«Ì… «·› —… 1° √Ê »œ«Ì… «·› —… 2 »⁄œ ‰Â«Ì… «·› —… 1
		if (ComparedDates(Date2.End, Date1.Start) == enCompareDates::Before ||
			ComparedDates(Date2.Start, Date1.End) == enCompareDates::After)
			return false;
		else
			return true;
	}

	short GetDifferencInDays(stDate Date1, stDate Date2, bool IncludingEndDate = 0)
	{
		short LengthDay = 0;

		while (MyDateLib::IsDate1LessThanDate2(Date1, Date2))
		{
			Date1 = MyDateLib::IncreaseDateByOneDay(Date1);
			LengthDay++;
		}
		return IncludingEndDate ? ++LengthDay : LengthDay;
	}

	bool IsDateInPeriod(stDate Date, stPeriod Period)
	{
		return !(ComparedDates(Date, Period.Start) == enCompareDates::Before
			|| ComparedDates(Date, Period.End) == enCompareDates::After);
	}

	short OverlapCount(stPeriod Period1, stPeriod Period2)
	{
		short OverlapDays = 0;

		if (!IsOverLapPeriod(Period1, Period2))
		{
			return 0;
		}

		//  ÕœÌœ › —… «·»œ¡ «· Ì ”‰ﬁÊ„ »«·⁄œ „‰Â« · Ã‰» «· ﬂ—«— ›Ì «·Õ”«»
		stDate StartDate = (ComparedDates(Period1.Start, Period2.Start) == enCompareDates::After)
			? Period1.Start : Period2.Start;

		//  ÕœÌœ › —… «·«‰ Â«¡ «· Ì ”‰ﬁÊ„ »«·⁄œ ≈·ÌÂ«
		stDate EndDate = (ComparedDates(Period1.End, Period2.End) == enCompareDates::Before)
			? Period1.End : Period2.End;

		// ‰Õ”» «·›—ﬁ »Ì‰  «—ÌŒÌ «· œ«Œ· (‘«„· ÌÊ„ «·‰Â«Ì…)
		OverlapDays = GetDifferencInDays(StartDate, EndDate, true);

		return OverlapDays;
	}

	bool IsValidDate(stDate Date)
	{
		if (Date.Year < 1) return false;

		if (Date.Month < 1 || Date.Month > 12)
			return false;

		if (Date.Day < 1 || Date.Day > NumberOfDaysInMonth(Date.Month, Date.Year))
			return false;

		return true;
	}

	vector <string> SplitingString(string S, string Separator = "/")
	{
		vector <string> vString;
		//  „ «· €ÌÌ— „‰ short ≈·Ï size_t ·· Ê«›ﬁ „⁄ find()
		size_t pos;
		string token;

		while ((pos = S.find(Separator)) != string::npos)
		{
			token = S.substr(0, pos);
			if (!token.empty())
			{
				vString.push_back(token);
			}
			S.erase(0, pos + Separator.length());
		}
		if (!S.empty())
		{
			vString.push_back(S);
		}
		return vString;
	}

	string DateToString(stDate Date, string Separator = "/")
	{
		return to_string(Date.Day) +
			Separator + to_string(Date.Month) +
			Separator + to_string(Date.Year);
	}

	stDate StringToDate(string S, string Separator = "/")
	{
		vector <string> vString;
		stDate Date;

		vString = SplitingString(S, Separator);

		Date.Day = stoi(vString[0]);
		Date.Month = stoi(vString[1]);
		Date.Year = stoi(vString[2]);

		return Date;
	}

	string ReplaceWordInString(string S1, string StringToReplace, string sRepalceTo)
	{
		//  „ «· €ÌÌ— „‰ short ≈·Ï size_t
		size_t pos = S1.find(StringToReplace);

		//  ’ÕÌÕ: ÌÃ» √‰  ” „— «·Õ·ﬁ… Õ Ï ·«  Ãœ «·ﬂ·„… «·„—«œ «” »œ«·Â« (string::npos)
		// Ê«· √ﬂœ „‰ ≈“«·… «·⁄Êœ… «·„»ﬂ—… (return) œ«Œ· «·Õ·ﬁ…
		while (pos != std::string::npos)
		{
			S1 = S1.replace(pos, StringToReplace.length(), sRepalceTo);
			pos = S1.find(StringToReplace); //find next
		}
		return S1;
	}

	string FormattingDate(stDate Date, string Formatting = "dd/mm/yyyy")
	{
		string ReplaceString;
		ReplaceString = ReplaceWordInString(Formatting, "dd", to_string(Date.Day));
		ReplaceString = ReplaceWordInString(ReplaceString, "mm", to_string(Date.Month));
		ReplaceString = ReplaceWordInString(ReplaceString, "yyyy", to_string(Date.Year));

		return ReplaceString;
	}

}