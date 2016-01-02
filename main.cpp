#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <cmath>
using namespace std;

enum DataType {OPERATION, MONOMIAL, FUNCTION};

class Element
{
private:
	DataType type;
	vector<Element> function;
	char operation;
	int coefficient, exponent;
public:
	Element()
	{
	}
	Element(char input)
	{
		type=OPERATION;
		operation=input;
		//		cout << input << " is an OPERATION\n";
	}
	Element(int newcoefficient, int newexponent)
	{
		type=MONOMIAL;
		coefficient=newcoefficient;
		exponent=newexponent;
		//		cout << coefficient << "x^" << exponent << " is a MONOMIAL\n";
	}
	Element(string input)
	{
		type=FUNCTION;
		//		cout << "New function begins:\n";
		if(input[0]=='+')
		{
			input.erase(input.begin());
			//			cout << "Leading '+' deleted\n";
		}
		else if(input[0]=='-')
		{
			function.push_back(Element(-1,0));
			function.push_back(Element('*'));
			input.erase(input.begin());
			//			cout << "Leading '-' converted to -1, *\n";
		}
		while(!input.empty())
		{
			if(input[0]=='(')
			{
				if(!function.empty()&&function.back().gettype()!=OPERATION)
				{
					//					cout << "Juxtaposition found, inserting '*'\n";
					function.push_back(Element('*'));
				}
				int closeparen=0, parencount=1;
				while(parencount>0)
				{
					closeparen++;
					if(input[closeparen]=='(')
					{
						parencount++;
					}
					else if(input[closeparen]==')')
					{
						parencount--;
					}
				}
				//				cout << "Parentheses found\n";
				string tempstring=input.substr(1,(closeparen-1));
				function.push_back(Element(tempstring));
				input.erase(0,(closeparen+1));
			}
			else if(input[0]=='x')
			{
				if(!function.empty()&&function.back().gettype()!=OPERATION)
				{
					//					cout << "Juxtaposition found, inserting '*'\n";
					function.push_back(Element('*'));
				}
				function.push_back(Element(1,1));
				input.erase(input.begin());
			}
			else if(isdigit(input[0]))
			{
				if(!function.empty()&&function.back().gettype()!=OPERATION)
				{
					//					cout << "Juxtaposition found, inserting '*'\n";
					function.push_back(Element('*'));
				}
				int numberends=0, hasx=0;
				while(isdigit(input[numberends]))
				{
					numberends++;
				}
				if(input[numberends]=='x')
				{
					hasx=1;
				}
				function.push_back(Element(atoi(input.substr(0,numberends).c_str()), hasx));
				if(hasx==1)
				{
					numberends++;
				}
				input.erase(0,numberends);
			}
			else if(input[0]=='-')
			{
				function.push_back(Element('+'));
				function.push_back(Element(-1,0));
				function.push_back(Element('*'));
				input.erase(input.begin());
			}
			else
			{
				function.push_back(Element(input[0]));
				input.erase(input.begin());
			}
		}
		//		cout << "Function ends\n";
	}
	const Element operator=(const Element &input)
	{
		type=input.type;
		if(type==FUNCTION)
		{
			function=input.function;
		}
		else if(type==OPERATION)
		{
			operation=input.operation;
		}
		else
		{
			coefficient=input.coefficient;
			exponent=input.exponent;
		}
		return *this;
	}
	DataType gettype()
	{
		return type;
	}
	vector<Element> getfunction()
	{
		if(type==FUNCTION)
		{
			return function;
		}
		else
		{
			cout << "Error: not a FUNCTION\n";
		}
	}
	char getoperation()
	{
		if(type==OPERATION)
		{
			return operation;
		}
		else
		{
			cout << "Error: not an OPERATION\n";
		}
	}
	int getcoefficient()
	{
		if(type==MONOMIAL)
		{
			return coefficient;
		}
		else
		{
			cout << "Error: not a MONOMIAL\n";
		}
	}
	int getexponent()
	{
		if(type==MONOMIAL)
		{
			return exponent;
		}
		else
		{
			cout << "Error: not a MONOMIAL\n";
		}
	}
	void operate(Element predecessor, Element successor)
	{
		if(type!=OPERATION)
		{
			cout << "Error: not an OPERATION, operate failed\n";
		}
		else
		{
			if(operation=='^')
			{
				if(predecessor.gettype()==OPERATION||successor.gettype()==OPERATION)
				{
					cout << "Error: operators cannot work on other operators\n";
				}
				else if(successor.gettype()!=MONOMIAL)
				{
					cout << "Error: '^' can only raise by monomials\n";
				}
				else if(successor.getexponent()>0)
				{
					cout << "Error: '^' can only raise by constants\n";
				}
				else if(predecessor.gettype()==MONOMIAL)
				{
					type=MONOMIAL;
//					double predcoeff=predecessor.getcoefficient();
//					coefficient=pow(predcoeff, successor.getcoefficient());
					coefficient=predecessor.getcoefficient();
					exponent=predecessor.getexponent()*successor.getcoefficient();
				}
				else
				{
					type=FUNCTION;
					function.clear();
					function.push_back(predecessor);
					for(int i=1; i<successor.getcoefficient(); i++)
					{
						function.push_back(Element('*'));
						function.push_back(predecessor);
					}
					simplify();
					//					predecessor.print();
					//					cout << "^";
					//					successor.print();
					//					cout << "=";
					//					print();
					//					cout << endl;
				}
			}
			else if(operation=='*')
			{
				if(predecessor.gettype()==OPERATION||successor.gettype()==OPERATION)
				{
					cout << "Error: operators cannot work on other operators\n";
				}
				else if(predecessor.gettype()==MONOMIAL&&successor.gettype()==MONOMIAL)
				{
					type=MONOMIAL;
					coefficient=predecessor.getcoefficient()*successor.getcoefficient();
					exponent=predecessor.getexponent()+successor.getexponent();
					//					predecessor.print();
					//					cout << "*";
					//					successor.print();
					//					cout << "=";
					//					print();
					//					cout << endl;
				}
				else if(predecessor.gettype()==MONOMIAL&&successor.gettype()==FUNCTION)
				{
					type=FUNCTION;
					function.clear();
					//					cout << "Multiplying ";
					//					predecessor.print();
					//					cout << " into ";
					//					successor.print();
					//					cout << endl;
					for(int i=0; i<successor.getfunction().size(); i++)
					{
						function.push_back(successor.getfunction()[i]);
						if(successor.getfunction()[i].gettype()!=OPERATION)
						{
							function.push_back(Element('*'));
							function.push_back(predecessor);
						}
					}
					simplify();
				}
				else if(predecessor.gettype()==FUNCTION&&successor.gettype()==MONOMIAL)
				{
					type=FUNCTION;
					function.clear();
					//					cout << "Multiplying ";
					//					successor.print();
					//					cout << " into ";
					//					predecessor.print();
					//					cout << endl;
					//					cout << "Function is ";
					//					print();
					//					cout << endl;
					for(int i=0; i<predecessor.getfunction().size(); i++)
					{
						function.push_back(predecessor.getfunction()[i]);
						//						cout << "Pushing back ";
						//						predecessor.getfunction()[i].print();
						//						cout << "\nFunction is now ";
						//						print();
						//						cout << endl;
						if(predecessor.getfunction()[i].gettype()!=OPERATION)
						{
							//							cout << "Pushing back *\n";
							function.push_back(Element('*'));
							//							cout << "Function is now ";
							//							print();
							//							cout << endl;
							//							cout << "Pushing back ";
							//							successor.print();
							function.push_back(successor);
							//							cout << "\nFunction is now ";
							//							print();
							//							cout << endl;
						}
					}
					simplify();
				}
				else
				{
					type=FUNCTION;
					function.clear();
					//					cout << "Multiplying ";
					//					predecessor.print();
					//					cout << " and ";
					//					successor.print();
					//					cout << endl;
					for(int i=0; i<predecessor.getfunction().size(); i++)
					{
						if(predecessor.getfunction()[i].gettype()!=OPERATION)
						{
							for(int j=0; j<successor.getfunction().size(); j++)
							{
								if(successor.getfunction()[j].gettype()!=OPERATION)
								{
									//Prof. Murphy, this is where the problem I mentioned on Thursday occurred.
									//As with all of my algorithm-ruining errors, it turned out to have been a 1-character typo.
									//									cout << "Multiplying ";
									//									predecessor.getfunction()[i].print();
									//									cout << " and ";
									//									successor.getfunction()[j].print();
									//									cout << endl;
									function.push_back(predecessor.getfunction()[i]);
									function.push_back(Element('*'));
									function.push_back(successor.getfunction()[j]);
									function.push_back(Element('+'));
								}
							}
						}
					}
					function.pop_back();
					simplify();
				}
			}
			else if(operation=='+')
			{
				if(predecessor.gettype()==OPERATION||successor.gettype()==OPERATION)
				{
					cout << "Error: operators cannot work on other operators\n";
				}
				else if(predecessor.gettype()==MONOMIAL&&successor.gettype()==MONOMIAL)
				{
					if(predecessor.getexponent()!=successor.getexponent())
					{
						cout << "Error: cannot add monomials with different exponents\n";
					}
					else
					{
						type=MONOMIAL;
						coefficient=predecessor.getcoefficient()+successor.getcoefficient();
						exponent=predecessor.getexponent();
						if(coefficient==0)
						{
							//							cout << "Coefficient is 0, setting exponent to 0 for simplification purposes\n";
							exponent=0;
						}
						//						predecessor.print();
						//						cout << "+";
						//						successor.print();
						//						cout << "=";
						//						print();
						//						cout << endl;
					}
				}
				else if(predecessor.gettype()==MONOMIAL&&successor.gettype()==FUNCTION)
				{
					type=FUNCTION;
					function.clear();
					//					cout << "Adding ";
					//					predecessor.print();
					//					cout << " into ";
					//					successor.print();
					//					cout << endl;
					function=successor.getfunction();
					function.push_back(Element('+'));
					function.push_back(predecessor);
					simplify();
				}
				else if(predecessor.gettype()==FUNCTION&&successor.gettype()==MONOMIAL)
				{
					type=FUNCTION;
					function.clear();
					//					cout << "Adding ";
					//					successor.print();
					//					cout << " into ";
					//					predecessor.print();
					//					cout << endl;
					function=predecessor.getfunction();
					function.push_back(Element('+'));
					function.push_back(successor);
					simplify();
				}
				else
				{
					type=FUNCTION;
					function.clear();
					//					cout << "Adding ";
					//					predecessor.print();
					//					cout << " and ";
					//					successor.print();
					//					cout << endl;
					function=predecessor.getfunction();
					function.push_back(Element('+'));
					for(int i=0; i<successor.getfunction().size(); i++)
					{
						function.push_back(successor.getfunction()[i]);
					}
					simplify();
				}
			}
		}
	}
	void simplify()
	{
		if(type!=FUNCTION)
		{
			//			cout << "Error: not a FUNCTION, cannot simplify\n";
		}
		else
		{
			//			cout << "Simplifying subfunctions...\n";
			for(int n=0; n<function.size(); n++)
			{
				if(function[n].gettype()==FUNCTION)
				{
					function[n].simplify();
				}
			}
			//			print();
			//			cout << "Exponentiating...\n";
			for(int n=0; n<function.size(); n++)
			{
				if(function[n].gettype()==OPERATION&&function[n].getoperation()=='^')
				{
					function[n].operate(function[n-1], function[n+1]);
					//					cout << "Erasing old ";
					//					function[n-1].print();
					//					cout << " and ";
					//					function[n+1].print();
					//					cout << endl;
					function.erase(function.begin()+n+1);
					function.erase(function.begin()+n-1);
					n--;
				}
			}
			//			print();
			//			cout << "Multiplying...\n";
			for(int n=0; n<function.size(); n++)
			{
				if(function[n].gettype()==OPERATION&&function[n].getoperation()=='*')
				{
					function[n].operate(function[n-1], function[n+1]);
					//					cout << "Erasing old ";
					//					function[n-1].print();
					//					cout << " and ";
					//					function[n+1].print();
					//					cout << endl;
					function.erase(function.begin()+n+1);
					function.erase(function.begin()+n-1);
					n--;
				}
			}
			//			print();
			//			cout << "Adding...\n";
			for(int n=0; n<function.size(); n++)
			{
				if(function[n].gettype()==OPERATION&&function[n].getoperation()=='+')
				{
					if(function[n-1].gettype()==MONOMIAL&&function[n+1].gettype()==MONOMIAL&&function[n-1].getexponent()!=function[n+1].getexponent())
					{
					}
					else
					{
						function[n].operate(function[n-1], function[n+1]);
						//						cout << "Erasing old ";
						//						function[n-1].print();
						//						cout << " and ";
						//						function[n+1].print();
						//						cout << endl;
						function.erase(function.begin()+n+1);
						function.erase(function.begin()+n-1);
						n--;
					}
				}
			}
			if(function.size()>1&&function[function.size()-1].gettype()==MONOMIAL&&function[function.size()-1].getcoefficient()==0&&function[function.size()-2].getoperation()=='+')
			{
				//				cout << "Removing trailing zero...\n";
				function.pop_back();
				function.pop_back();
			}
			if(function.size()==1&&function[0].gettype()==MONOMIAL)
			{
				type=MONOMIAL;
				coefficient=function[0].getcoefficient();
				exponent=function[0].getexponent();
			}
			else if(function.size()==1&&function[0].gettype()==FUNCTION)
			{
				function=function[0].getfunction();
			}
		}
	}
	void print()
	{
		if(type==OPERATION)
		{
			cout << operation;
		}
		else if(type==MONOMIAL)
		{
			if(exponent==0)
			{
				cout << coefficient;
			}
			else
			{
				if(coefficient!=1)
				{
					cout << coefficient;
				}
				if(coefficient!=0&&exponent>0)
				{
					cout << "x";
				}
				if(coefficient!=0&&exponent>1)
				{
					cout << "^" << exponent;
				}
			}
		}
		else if(type==FUNCTION)
		{
			if(function[0].gettype()==FUNCTION)
			{
				cout << "( ";
				function[0].print();
				cout << " )";
			}
			else
			{
				function[0].print();
			}
			for(int i=1; i<function.size(); i++)
			{
				cout << " ";
				if(function[i].gettype()==FUNCTION)
				{
					cout << "( ";
					function[i].print();
					cout << " )";
				}
				else
				{
					function[i].print();
				}
			}
		}
		else
		{
			cout << "Error: not a OPERATION, MONOMIAL or FUNCTION.  Something is seriously messed up.\n";
		}
	}
	void order()
	{
		if(type!=FUNCTION)
		{
			cout << "Error: cannot order, not a FUNCTION\n";
		}
		else
		{
			simplify();
			for(int count=0; count<function.size(); count++)
			{
				for(int i=0; i<function.size(); i++)
				{
					if(function[i].gettype()==FUNCTION)
					{
						function[i].order();
					}
					else if(function[i].gettype()==OPERATION&&function[i].getoperation()=='+')
					{
						if(function[i+1].gettype()==FUNCTION&&function[i-1].gettype()==MONOMIAL)
						{
							Element tempElement=function[i-1];
							function[i-1]=function[i+1];
							function[i+1]=tempElement;
						}
						else if(function[i-1].gettype()==MONOMIAL&&function[i+1].gettype()==MONOMIAL&&function[i+1].getexponent()>function[i-1].getexponent())
						{
							Element tempElement=function[i-1];
							function[i-1]=function[i+1];
							function[i+1]=tempElement;
						}
					}
				}
			}
		}
	}
	void minussub()
	{
		if(type!=FUNCTION)
		{
			cout << "Error: cannot substitute minuses, not a FUNCTION\n";
		}
		else
		{
			for (int i=0; i<function.size(); i++)
			{
				if(function[i].gettype()==FUNCTION)
				{
					function[i].minussub();
				}
				else if(function[i].gettype()==OPERATION&&function[i].getoperation()=='+'&&function[i+1].gettype()==MONOMIAL&&function[i+1].getcoefficient()<0)
				{
					function[i]=Element('-');
					function[i+1]=Element(-1*function[i+1].getcoefficient(), function[i+1].getexponent());
				}
			}
		}
	}
	void differentiate()
	{
		if(type==OPERATION)
		{
		}
		else if(type==MONOMIAL)
		{
			coefficient=coefficient*exponent;
			if(exponent>0)
			{
				exponent--;
			}
		}
		else if(type==FUNCTION)
		{
			for(int i=0; i<function.size(); i++)
			{
				function[i].differentiate();
			}
			order();
			simplify();
		}
	}
};

string sanitize(string inputstring)
{
	bool sanitized=0;
	do
	{
		sanitized=1;
		int counter=0;
		while(counter<inputstring.length())
		{
			if(isspace(inputstring.at(counter)))
			{
				inputstring.erase(inputstring.begin()+counter);
				sanitized=0;
			}
			else if(inputstring.at(counter)=='*'&&inputstring.at(counter+1)=='*')
			{
				inputstring.at(counter)='^';
				inputstring.erase(inputstring.begin()+counter+1);
				sanitized=0;
			}
			else if((inputstring.at(counter)=='+'&&inputstring.at(counter+1)=='+')||(inputstring.at(counter)=='-'&&inputstring.at(counter+1)=='-'))
			{
				inputstring.at(counter)='+';
				inputstring.erase(inputstring.begin()+counter+1);
				sanitized=0;
			}
			else if((inputstring.at(counter)=='+'&&inputstring.at(counter+1)=='-')||(inputstring.at(counter)=='-'&&inputstring.at(counter+1)=='+'))
			{
				inputstring.at(counter)='-';
				inputstring.erase(inputstring.begin()+counter+1);
				sanitized=0;
			}
			else
			{
				counter++;
			}
		}		
	}while(sanitized==0);
	return inputstring;
}

class Word
{
private:
	int versenum;
	int linenum;
	string coreword, onebefore, twobefore, oneafter, twoafter;
public:
	Word(string inputstring)
	{
		inputstring.append(1, ' ');
		int startpos=0, endpos=1, corestartpos, coreendpos;
		while(inputstring.at(endpos)!=':')
		{
			endpos++;
		}
		versenum=atoi((inputstring.substr(startpos, (endpos-startpos))).c_str());
		startpos=endpos+1;
		endpos=startpos+1;
		while(inputstring.at(endpos)!=' ')
		{
			endpos++;
		}
		linenum=atoi((inputstring.substr(startpos, (endpos-startpos))).c_str());
		startpos=(inputstring.find('#', 0)+1);
		corestartpos=startpos;
		endpos=startpos+1;
		while(inputstring.at(endpos)!=' ')
		{
			endpos++;
		}
		coreendpos=endpos;
		coreword=inputstring.substr(startpos, (endpos-startpos));
		endpos=corestartpos-3;
		startpos=endpos;
		if(isdigit(inputstring.at(startpos))&&inputstring.at(startpos-1)==':')
		{
			onebefore="";
			twobefore="";
		}
		else
		{
			while(inputstring.at(startpos)!=' ')
			{
				startpos--;
			}
			onebefore=inputstring.substr((startpos+1), (endpos-startpos));
			endpos=startpos-1;
			startpos=endpos;
			if(isdigit(inputstring.at(startpos))&&inputstring.at(startpos-1)==':')
			{
				twobefore="";
			}
			else
			{
				while(inputstring.at(startpos)!=' ')
				{
					startpos--;
				}
				twobefore=inputstring.substr((startpos+1), (endpos-startpos));
			}
		}
		if(coreendpos==(inputstring.length()-1))
		{
			oneafter="";
			twoafter="";
		}
		else
		{
			startpos=coreendpos+1;
			endpos=startpos;
			while(inputstring.at(endpos)!=' ')
			{
				endpos++;
			}
			oneafter=inputstring.substr(startpos, (endpos-startpos));
			if(endpos==(inputstring.length()-1))
			{
				twoafter="";
			}
			else
			{
				startpos=endpos+1;
				endpos=startpos;
				while(inputstring.at(endpos)!=' ')
				{
					endpos++;
				}
				twoafter=inputstring.substr(startpos, (endpos-startpos));
			}
		}
	}
	Word(const Word &inputword)
	{
		versenum=inputword.versenum;
		linenum=inputword.linenum;
		coreword=inputword.coreword;
		onebefore=inputword.onebefore;
		twobefore=inputword.twobefore;
		oneafter=inputword.oneafter;
		twoafter=inputword.twoafter;
	}
	int getversenum() const
	{
		return versenum;
	}
	int getlinenum() const
	{
		return linenum;
	}
	string getcoreword() const
	{
		return coreword;
	}
	string getonebefore() const
	{
		return onebefore;
	}
	string gettwobefore() const
	{
		return twobefore;
	}
	string getoneafter() const
	{
		return oneafter;
	}
	string gettwoafter() const
	{
		return twoafter;
	}
	bool comesbefore(Word testword)
	{
		if((onebefore==testword.gettwobefore())&&(coreword==testword.getonebefore())&&(oneafter==testword.getcoreword())&&(twoafter==testword.getoneafter()))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
};

class Line
{
private:
	int versenum;
	int linenum;
	vector<Word> words;
public:
	Line(Word inputword)
	{
		versenum=inputword.getversenum();
		linenum=inputword.getlinenum();
		words.push_back(inputword);
	}
	Line(const Line &inputline)
	{
		versenum=inputline.versenum;
		linenum=inputline.linenum;
		words=inputline.words;
	}
	void addword(Word inputword)
	{
		if((versenum==inputword.getversenum())&&(linenum==inputword.getlinenum()))
		{
			words.push_back(inputword);
		}
		else
		{
			cout << "Verse and/or line number mismatch\n";
		}
	}
	int getversenum() const
	{
		return versenum;
	}
	int getlinenum() const
	{
		return linenum;
	}
	void printline() const
	{
		cout << versenum << ": " << linenum << "\t";
		string teststring;
		for(int i=0; i<words.size(); i++)
		{
			teststring.append(words[i].getcoreword());
		}
		teststring=sanitize(teststring);
		Element fullequation(teststring);
		//	fullequation.print();
		//	cout << endl;
		bool hasparens=1;
		while(hasparens)
		{
			hasparens=0;
			fullequation.simplify();
			if(fullequation.gettype()==FUNCTION)
			{
				for(int foo=0; foo<fullequation.getfunction().size(); foo++)
				{
					if(fullequation.getfunction()[foo].gettype()==FUNCTION)
					{
						hasparens=1;
					}
				}
				
			}
		}
		//	fullequation.print();
		if(fullequation.gettype()==FUNCTION)
		{
			//		cout << "Ordering...\n";
			fullequation.order();
			fullequation.simplify();
		}
		fullequation.differentiate();
		if(fullequation.gettype()==FUNCTION)
		{
			//		cout << "Substituting minuses...\n";
			fullequation.minussub();
		}
		fullequation.print();
		cout << endl;
/*		cout << versenum << ": " << linenum << "\t";
		for(int i=0; i<words.size(); i++)
		{
			cout << words[i].getcoreword() << " ";
		}
		cout << endl;*/
	}
	void sort()
	{
		if(!(words[0].getonebefore().empty()))
		{
			for(int i=1; i<words.size(); i++)
			{
				if(words[i].getonebefore()=="")
				{
					Word transferword=words[0];
					words[0]=words[i];
					words[i]=transferword;
				}
			}
		}
		int atcounter=0, targetcounter=1;
		bool lineended=0;
		while(lineended==0)
		{
			if((atcounter==words.size()-1))
			{
				lineended=1;
			}
			else if(words[atcounter].comesbefore(words[atcounter+1]))
			{
				atcounter++;
				targetcounter=atcounter+1;
			}
			else
			{
				while(!(words[atcounter].comesbefore(words[targetcounter])))
				{
					targetcounter++;
				}
				Word transferword=words[atcounter+1];
				words[atcounter+1]=words[targetcounter];
				words[targetcounter]=transferword;
				atcounter++;
				targetcounter=atcounter+1;
			}
		}
	}
};

int main()
{
	vector<Word> allinput;
	vector<Line> alllines;
	string tempstring;
//	bool end=0;
	while(getline(cin, tempstring))
	{
/*			while(end==0)
		 {
		 getline(cin, tempstring);
		 if(tempstring=="done")
		 {
		 end=1;
		 }
		 else
		 {*/
		Word tempword(tempstring);
		allinput.push_back(tempword);
//					cout << tempword.getversenum()  << ":" << tempword.getlinenum() << tempword.gettwobefore() << "," << tempword.getonebefore() << "," << tempword.getcoreword() << "," << tempword.getoneafter() << "," << tempword.gettwoafter() << endl;
//				}
	}
	Line templine(allinput[0]);
	alllines.push_back(templine);
	bool lineneeded;
	for(int i=1; i<allinput.size(); i++)
	{
		lineneeded=1;
		for(int j=0; j<alllines.size(); j++)
		{
			if((allinput[i].getversenum()==alllines[j].getversenum())&&(allinput[i].getlinenum()==alllines[j].getlinenum()))
			{
				lineneeded=0;
				alllines[j].addword(allinput[i]);
			}
		}
		if(lineneeded==1)
		{
			Line templine(allinput[i]);
			alllines.push_back(templine);
		}
	}
	bool fullysorted;
	do
	{
		fullysorted=1;
		for(int i=0; i<(alllines.size()-1); i++)
		{
			if((alllines[i].getversenum()>alllines[i+1].getversenum())||((alllines[i].getversenum()==alllines[i+1].getversenum())&&(alllines[i].getlinenum()>alllines[i+1].getlinenum())))
			{
				fullysorted=0;
				Line transferline=alllines[i];
				alllines[i]=alllines[i+1];
				alllines[i+1]=transferline;
			}
		}
	}while(fullysorted==0);
	for(int k=0; k<alllines.size(); k++)
	{
		alllines[k].sort();
	}
	for(int k=0; k<alllines.size(); k++)
	{
		alllines[k].printline();
	}
	cout << endl;
	return 0;
}
