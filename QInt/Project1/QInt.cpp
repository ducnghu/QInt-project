#include "QInt.h"

// khởi tạo QInt mang giá trị 0 
QInt QInt::zero()
{
	static QInt staticZero;
	static bool zeroInit = false;
	if(!zeroInit)
	{
		fill(staticZero.arr, staticZero.arr + QINT_SIZE, 0);
		zeroInit = true;
	}
	return staticZero;
}

// khởi tạo QInt mang giá trị 1
QInt QInt::one()
{
	static QInt staticOne;
	static bool oneInit = false;
	if (!oneInit)
	{
		fill(staticOne.arr, staticOne.arr + QINT_SIZE, 0);
		oneInit = true;
	}
	return staticOne;
}

// kiểm tra số âm bằng MSB = bit cao nhất của phần tử arr[0]
bool QInt::isNegative()
{
	return ((this->arr[0] >> sizeof(int) * 8 - 1) & 1);
}

// trả về giá trị bit thứ pos (bắt đầu từ 0)
bool QInt::getbit(int pos)
{
	int arrpos = QINT_SIZE - 1 - pos / 32;
	int bitpos = pos % 32;
	return ((arr[arrpos] >> bitpos) & 1);
}

// đặt bit thứ pos thành giá trị num(0,1)
void QInt::setbit(int pos, bool num)
{
	int arrpos = QINT_SIZE - 1 - pos / 32;
	int bitpos = pos % 32;
	if (num) // 1
		arr[arrpos] = (arr[arrpos] | (1 << bitpos));
	else
		arr[arrpos] = (arr[arrpos] & (~(1 << bitpos)));

}

// set giá trị biến QInt thành str với str là số theo hệ base(2, 10, 16)
void QInt::scanfQInt(string str, int base)
{
	int pos = 0;
	switch (base)
	{
	case 2:
		for (int i = str.length() - 1; i >= 0; i--)
		{
			this->setbit(pos, str[i] - '0');
			pos++;
		}
		break;
	case 10:
		*this = decToBin(str);
		break;
	case 16:
		*this = hexToBin(str);
		break;
	}
}

// In giá trị biến theo hệ số base
void QInt::printfQInt(int base)
{
	int pos = QINT_SIZE * 32 - 1;
	switch (base)
	{
	case 2:
		cout << toBinString();
		break;
	case 10:
		cout << binToDec(*this);
		break;
	case 16:
		cout << binToHex(*this);
		break;
	}
}

// Copy giá trị từ biến example
QInt QInt::operator=(QInt example)
{
	for (int i = 0; i < QINT_SIZE; i++)
	{
		this->arr[i] = example.arr[i];
	}
	return *this;
}

// Cộng 2 biến QInt
QInt QInt::operator+(QInt example)
{
	QInt ret;
	string bin = "";
	int add = 0; // biến nhớ

	// Cộng từ bit cuối đến đầu, dãy bin lưu từ đầu đến cuối => dãy bin ngược
	for (int i = 0; i < QINT_SIZE * 32; i++)
	{
		int temp = this->getbit(i) + example.getbit(i) + add;
		switch (temp)
		{
		case 0: // 00, bit = 0, nhớ = 0
			add = 0;
			bin += '0';
			break;
		case 1: // 01, bit = 1, nhớ = 0
			add = 0;
			bin += '1';
			break;
		case 2: // 10, bit = 0, nhớ = 1
			add = 1;
			bin += '0';
			break;
		case 3: // 11, bit = 1, nhớ = 1
			add = 1;
			bin += '1';
			break;
		}
	}

	//NOTE: nhớ cuối bằng 0 hay 1 không ảnh hưởng kết quả vì bị tràn khỏi vùng nhớ

	// Đảo dãy bit vừa tính
	reverse(bin.begin(), bin.end());
	while (bin[0] == '0' && bin.length() > 1) bin.erase(0, 1);

	// lưu kết quả vào biến trả về (kết quả là dãy bin mang giá trị theo hệ nhị phân)
	ret.scanfQInt(bin, 2);
	return ret;
}

//Trừ 2 biến QInt
QInt QInt::operator-(QInt example)
{
	// A - B = A + (-B) = A + (bù 2 của B)
	return *this + example.bu2();
}

// Nhân 2 biến QInt theo thuật toán Booth
QInt QInt::operator*(QInt example)
{
	// A Q Q1
	bool Q1 = 0;
	QInt A;
	QInt Q = example;
	for (int i = 0; i < QINT_SIZE * 32; i++)
	{
		bool Q0 = Q.getbit(0);
		if (Q0 && !Q1) //10
		{
			// A = A - M
			A = A - *this;
		}
		if (!Q0 && Q1) //01
		{
			// A = A + M
			A = A + *this;
		}

		//right shift
		Q1 = Q0;
		Q = Q >> 1;
		Q.setbit(QINT_SIZE * 32 - 1, A.getbit(0));
		A = A >> 1;
	}
	// kết quả phép nhân là AQ tuy nhiên các bit của A nằm ở phần bị tràn số nên chỉ còn Q
	return Q;
}

// CHia 2 biến QInt
QInt QInt::operator/(QInt example)
{
	// Q/M du A
	QInt Q = *this;
	QInt M = example;
	QInt A = QInt::zero();
	
	// đánh dấu các số âm
	bool isQNeg = Q.isNegative();
	bool isMNeg = M.isNegative();


	// chuyển số chia và số bị chia về số dương
	if (isQNeg) Q = Q.bu2();
	if (isMNeg) M = M.bu2();

	// Thuật toán chia 2 số dương
	for (int i = 0; i < QINT_SIZE * 32; i++)
	{
		// shift left (A,Q)
		A = A << 1;
		A.setbit(0, Q.getbit(QINT_SIZE * 32 - 1));
		Q = Q << 1; // Q0 = 0

		A = A - M;

		// nếu A < 0 => A = A + M, Q0 = 0(ko thay đổi Q)
		// nếu A >= 0 => Q0 = 1
		if (A.isNegative()) A = A + M;
		else Q.setbit(0, 1);
	}

	// số dư cùng dấu số chia
	if (isMNeg) A = A.bu2();

	// số bị chia và số chia trái dấu => thương âm
	if (isQNeg + isMNeg == 1) Q = Q.bu2();
	return Q;
}

// Phép NOT
QInt QInt::operator~()
{
	QInt ret;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		ret.arr[i] = ~arr[i];
	}
	return ret;
}

//Phép OR
QInt QInt::operator|(QInt example)
{
	QInt ret;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		ret.arr[i] = this->arr[i] | example.arr[i];
	}
	return ret;
}

// Phép AND
QInt QInt::operator&(QInt example)
{
	QInt ret;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		ret.arr[i] = this->arr[i] & example.arr[i];
	}
	return ret;
}

// Phép XOR
QInt QInt::operator^(QInt example)
{
	QInt ret;
	for (int i = 0; i < QINT_SIZE; i++)
	{
		ret.arr[i] = this->arr[i] ^ example.arr[i];
	}
	return ret;
}

// left shift
QInt QInt::operator<<(int num)
{
	// left shift quá số bit của QInt => tất cả bit = 0
	if (num >= QINT_SIZE * 32) return QInt::zero();

	QInt ret = *this;

	// left shift từng bước
	while (num > 0)
	{
		for (int i = 0; i < QINT_SIZE - 1; i++)
		{
			ret.arr[i] = ret.arr[i] << 1;
			if ((ret.arr[i + 1] >> 31) & 1) ret.arr[i] = 1 | ret.arr[i];
		}
		ret.arr[QINT_SIZE - 1] = ret.arr[QINT_SIZE - 1] << 1;

		num--;
	}
	return ret;
}

// right shift
QInt QInt::operator>>(int num)
{
	// right shift quá số bit của QInt => tất cả bit = bit dấu
	// nếu là số âm => 11111...1111 = -1
	// nếu là số dương => 0000...000 = 0
	if (num >= QINT_SIZE * 32)
		if (this->isNegative()) return QInt::one().bu2();
		else  return QInt::zero();

	QInt ret = *this;

	// đánh dấu số âm
	bool staticNegative = ret.isNegative();

	// right shift từng bước
	while (num > 0)
	{
		for (int i = QINT_SIZE - 1; i > 0; i--)
		{
			ret.arr[i] = ret.arr[i] >> 1;
			if (ret.arr[i - 1] & 1) ret.arr[i] = ret.arr[i] | (1 << 31);
		}
		ret.arr[0] = ret.arr[0] >> 1;
		if (staticNegative) ret.arr[0] = ret.arr[0] | (1 << 31);

		num--;
	}
	return ret;
}

// Xoay trái
QInt QInt::rol()
{
	QInt ret = *this;

	// lưu giá trị bit MSB
	bool remember = (ret.arr[0] >> 31) & 1;

	// left shift
	for (int i = 0; i < QINT_SIZE - 1; i++)
	{
		ret.arr[i] = ret.arr[i] << 1;
		if ((ret.arr[i + 1] >> 31) & 1) ret.arr[i] = 1 | ret.arr[i];
	}
	ret.arr[QINT_SIZE - 1] = ret.arr[QINT_SIZE - 1] << 1;
	
	// gán MSB cũ vào LSB
	if (remember) ret.arr[QINT_SIZE - 1] = ret.arr[QINT_SIZE - 1] | 1;

	return ret;
}

// Xoay phải
QInt QInt::ror()
{
	QInt ret = *this;

	// lưu giá trị bit LSB
	bool remember = ret.arr[QINT_SIZE - 1] & 1;

	//right shift
	for (int i = QINT_SIZE - 1; i > 0; i--)
	{
		ret.arr[i] = ret.arr[i] >> 1;
		if (ret.arr[i - 1] & 1) ret.arr[i] = ret.arr[i] | (1 << 31);
	}
	ret.arr[0] = ret.arr[0] >> 1;

	// gán LSB cũ vào MSB
	if (remember) ret.arr[0] = ret.arr[0] | (1 << 31);

	return ret;
}

// trả về -A (dạng bù 2)
QInt QInt::bu2()
{
	QInt ret = ~(*this);
	for (int i = QINT_SIZE - 1; i >= 0; i--)
		if (ret.arr[i] == 4294967295)
			ret.arr[i] = 0;
		else
		{
			ret.arr[i]++;
			return ret;
		}
}

// trả về dãy nhị phân
string QInt::toBinString()
{
	string ret = "";
	for (int i = QINT_SIZE * 32 - 1; i > -1; i--)
	{
		ret += getbit(i) + '0';
	}
	while (ret[0] == '0' && ret.length() > 1)
	{
		ret.erase(0, 1);
	}
	return ret;
}

// khởi tạo biến mặc định mang giá trị 0 (tất cả bit = 0)
QInt::QInt()
{
	fill(arr, arr + QINT_SIZE, 0);
}

QInt::~QInt()
{
}

// trả về str / 2 (hệ 10)
string div2(string str)
{
	string ret = "";
	int remember = 0;
	for (int i = 0; i < str.length(); i++)
	{
		ret += (str[i] - '0') / 2 + remember + '0';
		remember = ((str[i] - '0') % 2) ? 5 : 0;
	}
	if (ret[0] == '0' && ret.length() > 1) ret.erase(0, 1);
	return ret;
}

// trả về str*2 + addition (hệ 10)
string mul2(string str, bool addition)
{
	string ret = "";
	int add = 0;

	//tính từ cuối đến đầu, ret lưu từ đầu đến cuối => lưu ngược
	ret += ((str[str.length() - 1] - '0') * 2 + addition) % 10 + '0';
	add = ((str[str.length() - 1] - '0') * 2 + add) / 10;
	for (int i = str.length() - 2; i >= 0; i--)
	{
		ret += ((str[i] - '0') * 2 + add) % 10 + '0';
		add = ((str[i] - '0') * 2 + add) / 10;
	}
	if (add > 0) ret += add + '0';

	// đảo lại dãy kết quả
	reverse(ret.begin(), ret.end());

	return ret;
}

// chuyển dãy số string hệ 10 sang QInt
QInt decToBin(string dec)
{
	QInt ret;
	string bin = "";

	// kiểm tra số âm
	bool isNegative = false;
	if (dec[0] == '-')
	{
		isNegative = true;
		dec.erase(0, 1);
	}

	// chuyển dãy số string dương hệ 10 thành dãy số string hệ 2
	do
	{
		bin += (dec[dec.length() - 1] - '0') % 2 + '0';
		dec = div2(dec);
	} while (dec != "0");
	reverse(bin.begin(),bin.end());

	// lưu vào biến QInt trả về
	ret.scanfQInt(bin, 2);
	if (isNegative) ret = ret.bu2();

	return ret;
}

// chuyển dãy số string hệ 16 sang QInt
QInt hexToBin(string hex)
{
	QInt ret;
	string bin = "";

	//chuyển dãy số string hệ 16 thành dãy số string hệ 2
	for (int i = 0; i < hex.length(); i++)
	{
		// mỗi kí tự trong dãy số hê 16 tương đương 4 bit mang cùng giá trị trong dãy nhị phân
		int num = ((string)HEX_STRING).find(hex[i], 0);
		bin += bitset<4>(num).to_string();
	}

	// lưu vào biến QInt trả về
	ret.scanfQInt(bin, 2);

	return ret;
}

// trả về dãy string giá trị QInt theo hệ 10
string binToDec(QInt example)
{
	string ret = "0";

	// kiểm tra số âm
	bool staticNegative = example.isNegative();

	// chuyển example về số dương
	if (staticNegative)
	{
		example = example.bu2();
	}

	//Chuyển QInt thành dãy sô string hệ 10
	string bin = example.toBinString();
	if (bin == "0") return ret;
	for (int i = 0; i < bin.length(); i++)
	{
		ret = mul2(ret, bin[i] - '0');
	}
	if (staticNegative)
	{
		ret = '-' + ret;
	}

	return ret;
}

// trả về dãy string giá trị QInt theo hệ 16
string binToHex(QInt example)
{
	string ret = "";
	string bin = example.toBinString();

	// 4 bit liên tục của dãy bin = 1 kí tự dãy hex => số kí tự dãy bin chia hết cho 4
	while (bin.length() % 4 != 0) bin = '0' + bin;
	for (int i = 0; i < bin.length()/4; i++)
	{
		//chuyển từng 4 bit dãy bin thành 1 kí tự dãy hex
		int pos = bitset<4>(bin.substr(i * 4, 4)).to_ulong();
		ret += ((string)HEX_STRING)[pos];
	}

	return ret;
}
