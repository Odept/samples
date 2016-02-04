/**
 * Class which implements basic date operations
 */
#include <iostream>


class Date
{
	static const unsigned BaseYear = 1601;
	static const unsigned BaseDaysSinceCentury = 366;
	// 1 Jan 1601 was Monday
	static const unsigned BaseDayOfTheWeek = 1;

public:
	Date(unsigned year, unsigned month, unsigned day):
		m_year(year), m_month(month), m_day(day)
	{
		if(m_year < BaseYear)
			throw std::domain_error("Invalid year");
		if(!m_month || m_month > 12)
			throw std::domain_error("Invalid month");
		if(!m_day || m_day > monthDays(m_year, m_month))
			throw std::domain_error("Invalid day");
	}

	unsigned  year() const { return m_year ; }
	unsigned month() const { return m_month; }
	unsigned   day() const { return m_day  ; }

	bool isLeap() const { return isLeap(m_year); }

	unsigned dayOfTheWeek() const { return dayOfTheWeek(daysSince()); }

	// String routines
	const std::string& strMonth() const
	{
		static const std::string months[] =
		{
			       "Jan", "Feb",
			"Mar", "Apr", "May",
			"Jun", "Jul", "Aug",
			"Sep", "Oct", "Nov",
			"Dec"
		};
		return months[m_month - 1];
	}
	const std::string& strDayOfTheWeek() const
	{
		static const std::string dow[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
		return dow[dayOfTheWeek()];
	}

	// Operators
	bool operator<(const Date& d) const
	{
		if(m_year < d.m_year)
			return true;
		if(m_year > d.m_year)
			return false;
		if(m_month < d.m_month)
			return true;
		if(m_month > d.m_month)
			return false;
		return (m_day < d.m_day);
	}

	/*static Date operator+(const Date& a, const Date& b)
	{
		unsigned days_a = a.daysSince();
		unsigned days_b = b.daysSince();
		return Date(days_a + days_b);
	}
	static Date operator-(const Date& a, const Date& b)
	{
		unsigned days_a = a.daysSince();
		unsigned days_b = b.daysSince();
		if(days_a < days_b)
			throw std::range_error("Invalid dates combination");
		return Date(days_a - days_b);
	}
	//*/

	// Helpers
private:
	static constexpr unsigned century(unsigned year) { return year - (year % 100); }
	static unsigned daysSinceCentury(unsigned year)
	{
		unsigned c = century(year);
		unsigned full_years = year % 100;
		if(!full_years)
			return 0;

		unsigned leaps = (full_years - 1) / 4 + (isLeap(c) ? 1 : 0);
		return (366 * leaps + 365 * (full_years - leaps));
	}
	unsigned daysSince() const
	{
		unsigned days = m_day - 1;
		for(unsigned i = m_month - 1; i; --i)
			days += monthDays(m_year, i);

		days += daysSinceCentury(m_year);
		for(unsigned y = century(m_year) - 1; y > century(BaseYear); y -= 100)
			days += daysSinceCentury(y) + 365;

		return (days - BaseDaysSinceCentury);
	}

	static bool isLeap(unsigned year)
	{	
		if(year % 400 == 0)
			return true;
		if(year == 0 || year == 4 || (year % 100 == 0))
			return false;
		return (year % 4 == 0);
	}

	static unsigned monthDays(unsigned year, unsigned month)
	{
		static const unsigned days[] =
		{
				31, 28,
			31, 30, 31,
			30, 31, 31,
			30, 31, 30,
			31
		};
		return days[month - 1] + ((month == 2 && isLeap(year)) ? 1 : 0);
	}
	
	static unsigned dayOfTheWeek(unsigned daysSince) { return (BaseDayOfTheWeek + daysSince) % 7; }

private:
	unsigned m_year;
	unsigned m_month;
	unsigned m_day;
};

int main(int, const char**)
{
	try
	{
		unsigned y, m, _d;
		std::cout << "Year: " ; std::cin >> y;
		std::cout << "Month: "; std::cin >> m;
		std::cout << "Day: "  ; std::cin >> _d;
		Date d(y, m, _d);

		//Date d(1602, 1, 1);

		std::cout << "Year " << d.year() << ' ' << d.strMonth() << ' ' << d.day() << " (" << d.strDayOfTheWeek() << ')' << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cout << "EXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}

