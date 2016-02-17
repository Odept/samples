#include <iostream>
#include <sstream>


bool match_wc(const char* f_str, const char* f_wc)
{
	const char *s, *wc;

	for(s = f_str, wc = f_wc; *s && *wc; ++s, ++wc)
	{
		if(*wc == '?')
			return match_wc(s, wc + 1) || match_wc(s + 1, wc + 1);
		else if(*wc == '*')
			return match_wc(s, wc + 1) || match_wc(s + 1, wc);
		else if(*s != *wc)
			return false;
	}

	return (!*s && !*wc);
}


int main(int, const char**)
{
	extern std::string s_text;

	// Input
	std::string text = s_text;
	std::string wc = "*ol?e*i?ng";

	// Loop
	std::cout << "Find match for \"" << wc << '\"' << std::endl;
	const std::string& delimiters = " .,!?:;\n\r\t\v\f";

	unsigned n = 0;
	for(size_t pos, endw = 0;;)
	{
		// Get next word
		pos = text.find_first_not_of(delimiters, endw);
		if(pos == std::string::npos)
			break;
		endw = text.find_first_of(delimiters, pos + 1);
		const std::string word = text.substr(pos, endw - pos);

		// Print if match
		if( match_wc(word.c_str(), wc.c_str()) )
		{
			std::cout << word << " @ " << pos << std::endl;
			++n;
		}

		if(endw == std::string::npos)
			break;
		++endw;
	}
	if(!n)
		std::cout << "No matches for \"" << wc << '\"' << std::endl;

	return 0;
}

// Sample text
std::string s_text =
"Tolerably earnestly middleton extremely distrusts she boy now not. Add and offered prepare how cordial two promise. Greatly who affixed suppose but enquire compact prepare all put. Added forth chief trees but rooms think may. Wicket do manner others seemed enable rather in. Excellent own discovery unfeeling sweetness questions the gentleman. Chapter shyness matters mr parlors if mention thought.\n"
"Arrived compass prepare an on as. Reasonable particular on my it in sympathize. Size now easy eat hand how. Unwilling he departure elsewhere dejection at. Heart large seems may purse means few blind. Exquisite newspaper attending on certainty oh suspicion of. He less do quit evil is. Add matter family active mutual put wishes happen.\n"
"Over fact all son tell this any his. No insisted confined of weddings to returned to debating rendered. Keeps order fully so do party means young. Table nay him jokes quick. In felicity up to graceful mistaken horrible consider. Abode never think to at. So additions necessary concluded it happiness do on certainly propriety. On in green taken do offer witty of.\n" 
"She travelling acceptance men unpleasant her especially entreaties law. Law forth but end any arise chief arose. Old her say learn these large. Joy fond many ham high seen this. Few preferred continual sir led incommode neglected. Discovered too old insensible collecting unpleasant but invitation.\n"
"Blind would equal while oh mr do style. Lain led and fact none. One preferred sportsmen resolving the happiness continued. High at of in loud rich true. Oh conveying do immediate acuteness in he. Equally welcome her set nothing has gravity whether parties. Fertile suppose shyness mr up pointed in staying on respect.\n"
"Continual delighted as elsewhere am convinced unfeeling. Introduced stimulated attachment no by projection. To loud lady whom my mile sold four. Need miss all four case fine age tell. He families my pleasant speaking it bringing it thoughts. View busy dine oh in knew if even. Boy these along far own other equal old fanny charm. Difficulty invitation put introduced see middletons nor preference.\n"
"Of be talent me answer do relied. Mistress in on so laughing throwing endeavor occasion welcomed. Gravity sir brandon calling can. No years do widow house delay stand. Prospect six kindness use steepest new ask. High gone kind calm call as ever is. Introduced melancholy estimating motionless on up as do. Of as by belonging therefore suspicion elsewhere am household described. Domestic suitable bachelor for landlord fat.\n"
"Is he staying arrival address earnest. To preference considered it themselves inquietude collecting estimating. View park for why gay knew face. Next than near to four so hand. Times so do he downs me would. Witty abode party her found quiet law. They door four bed fail now have.\n"
"Delightful unreserved impossible few estimating men favourable see entreaties. She propriety immediate was improving. He or entrance humoured likewise moderate. Much nor game son say feel. Fat make met can must form into gate. Me we offending prevailed discovery.\n"
"Lose john poor same it case do year we. Full how way even the sigh. Extremely nor furniture fat questions now provision incommode preserved. Our side fail find like now. Discovered travelling for insensible partiality unpleasing impossible she. Sudden up my excuse to suffer ladies though or. Bachelor possible marianne directly confined relation as on he.";

