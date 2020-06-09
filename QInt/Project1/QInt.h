#pragma once
#include <iostream>
#include <string>
#include <bitset>

#define QINT_SIZE 4
#define HEX_STRING "0123456789ABCDEF"

using namespace std;
class QInt
{
private:
	unsigned int arr[4]; // 128 bits
public:
	
	static QInt zero();
	static QInt one();

	bool isNegative();
	bool getbit(int pos);
	void setbit(int pos, bool num);

	void scanfQInt(string str, int base);
	void printfQInt(int base);

	QInt operator=(QInt example);
	QInt operator+(QInt example);
	QInt operator-(QInt example);
	QInt operator*(QInt example);
	QInt operator/(QInt example);

	QInt operator~();
	QInt operator|(QInt example);
	QInt operator&(QInt example);
	QInt operator^(QInt example);

	QInt operator<<(int num);
	QInt operator>>(int num);
	QInt rol();
	QInt ror();

	QInt bu2();

	string toBinString();

	QInt();
	~QInt();
};
string div2(string str);
string mul2(string str, bool addition);
QInt decToBin(string dec);
QInt hexToBin(string hex);
string binToDec(QInt example);
string binToHex(QInt example);