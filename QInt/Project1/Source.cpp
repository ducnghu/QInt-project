#include "QInt.h"
#define _CRT_SECURE_NO_WARNINGS

//trả về từ đầu tiên sau vị trí first trong dãy str (từ là 1 chuỗi kí tự liên tục không có kí tự trông)
string getWord(string str, int& first)
{
	string ret = "";

	// tìm kí tự trắng tiếp theo sau kí tự trắng tại first
	int t = str.find(' ', first);
	
	// nếu ko tồn tại kí tự trắng sau first => trả về vị trí kí tự kết thúc chuỗi
	if (t > str.length()) t = str.length();

	// ret = chuỗi kí tự từ first -> t - 1
	ret = str.substr(first, t - first);

	// first = vị trí đầu từ tiếp theo | vị trí kí tự kết thúc chuỗi
	first = t + 1;

	return ret;
}

void process()
{
	while (!cin.eof())
	{
		QInt s1, s2; // giá trị đầu vào 1 và 2(nếu có)
		string tmp = ""; // biến tạm: tùy trường hợp xem như p2 | chuỗi giá trị | toán tử
		string p1 = ""; // hệ số của chuỗi giá trị đầu vào
		string s = ""; // phép toán đọc từ cin
		int solandich = 0; // số lần dịch trái hoặc phải(chỉ sử dụng nếu có phép dịch)

		// đọc phép toán
		getline(cin, s);

		// vị trí đang đọc trong chuỗi
		int firstchar = 0;

		// word đầu tiên chắc chắn là  hệ số đầu vào
		p1 = getWord(s, firstchar);

		tmp = getWord(s, firstchar);
		if (tmp == "2" || tmp == "10" || tmp == "16") // doi he || so hang dau trong toan tu 2 ngoi
		{
			// luu trang thai cu
			string str = tmp;
			int remember = firstchar;

			tmp = getWord(s, firstchar);
			if (!(tmp == "+" || tmp == "-" || tmp == "*" || tmp == "/" || tmp == "<<" || tmp == ">>" || tmp == "^" || tmp == "|" || tmp == "&" || tmp == "<<" || tmp == ">>")) // tmp la toan hang => doi he
			{
				s1.scanfQInt(tmp, stoi(p1));
				s1.printfQInt(stoi(str));
				cout << endl;
				continue;
			}
			// tmp la toan tu => str la so hang dau tien
			// set lai vi tri cu
			tmp = str;
			firstchar = remember;
		}
		if (tmp == "~" || tmp == "ror" || tmp == "rol") // toan tu 1 ngoi
		{
			string str = tmp;
			tmp = getWord(s, firstchar);
			s1.scanfQInt(tmp, stoi(p1));
			if (str == "~")
			{
				(~s1).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (str == "ror")
			{
				(s1.ror()).printfQInt(stoi(p1));
				cout << endl;
			}
			else
			{
				(s1.rol()).printfQInt(stoi(p1));
				cout << endl;
			}
			continue;
		}
		else // toan tu 2 ngoi => tmp la so
		{
			// toan hang 1
			s1.scanfQInt(tmp, stoi(p1));

			// lay toan tu
			tmp = getWord(s, firstchar); // toan tu

			// lay toan hang 2
			if (tmp == ">>" || tmp == "<<") solandich = stoi(getWord(s, firstchar));
			else s2.scanfQInt(getWord(s, firstchar), stoi(p1));

			// tinh toan
			if (tmp == "+")
			{
				(s1 + s2).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == "-")
			{
				(s1 - s2).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == "*")
			{
				(s1 * s2).printfQInt(stoi(p1));
				cout << endl;
			}

			else if (tmp == "/")
			{
				(s1 / s2).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == "&")
			{
				(s1 & s2).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == "|")
			{
				(s1 | s2).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == "^")
			{
				(s1 ^ s2).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == "<<")
			{
				(s1 << solandich).printfQInt(stoi(p1));
				cout << endl;
			}
			else if (tmp == ">>")
			{
				(s1 >> solandich).printfQInt(stoi(p1));
				cout << endl;
			}
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 3) return 0;
	freopen(argv[1], "rt", stdin);
	freopen(argv[2], "wt", stdout);
	process();
	return 1;
}