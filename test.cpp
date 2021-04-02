#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

class CFabricData;

class CPoly2
{
public:
	vector<vector<int>> arr;

	int N = 0;
	string cfname;

private:
	void delete_all()
	{
		for (int i = 0; i < N; i++)
		{
			arr[i].resize(0);
		}
		arr.resize(0);
		cfname.resize(0);
		N = 0;
	}
	static int digit(string str, int &error)
	{
		int d = 0, sgn = 1;
		unsigned int i = 0;
		if (str[0] == '-')
		{
			sgn = -1;
			i++;
		}
		for (; i < str.size(); i++)
		{
			d *= 10;
			if (!isdigit(str[i]))
			{
				error = 1;
				return d;
			}
			d += (int)str[i] - 48;
		}
		return d * sgn;
	}

	static int ssqrt(int N)
	{
		int n = N, k = 0, m;
		while (n - k > 1)
		{
			m = (n + k) / 2;
			if (m * m >= N)
				n = m;
			else
				k = m;
		}
		return n;
	}

public:
	CPoly2()
	{
		N = 0;
	}
	CPoly2(string &cfname_, vector<vector<int>> &arr_, int N_)
	{
		arr = arr_;
		N = N_;
		cfname = cfname_;
	}
	virtual ~CPoly2()
	{
		delete_all();
	}
	CPoly2(const CPoly2 &other)
	{
		cout << "конструктор копирования " << this << " " << &other << endl;

		cfname = other.cfname;
		N = other.N;
		arr.resize(N);
		for (int i = 0; i < N; i++)
		{
			arr[i] = other.arr[i];
		}
	}
	CPoly2(CPoly2 &&other)
	{
		cout << "конструктор перемещения " << this << " " << &other << endl;

		cfname = move(other.cfname);
		N = other.N;
		other.N = 0;
		arr = move(other.arr);
	}
	CPoly2 &operator=(const CPoly2 &other)
	{
		cout << "присваивание копированием " << this << " " << &other << endl;

		if (this == &other)
		{
			cout << "a = a" << endl;
			return *this;
		}
		cfname = other.cfname;
		N = other.N;
		arr = other.arr;
		return *this;
	}
	CPoly2 &operator=(CPoly2 &&other)
	{
		cout << "присваивание перемещением " << this << " " << &other << endl;

		delete_all();
		N = other.N;
		other.N = 0;
		arr = move(other.arr);
		cfname = move(other.cfname);
		return *this;
	}

	virtual int output() = 0;
	static CPoly2 *CreateData(string, CFabricData **);
};

class CData0 : public CPoly2
{
public:
	CData0() : CPoly2() {}
	CData0(string &fname, vector<vector<int>> &arr, int N) : CPoly2(fname, arr, N)
	{
		cout << "дочерний конструктор \n";
	}
	// ~CData0()
	// {
	// }

	CData0(const CPoly2 &other) : CPoly2(other)
	{
		cout << "дочерний конструктор копирования \n";
	}
	// CData0(CPoly2 &&other) : CPoly2(other)
	// {
	// }

	int output()
	{
		if (cfname.empty())
			return 1;
		ofstream fout(cfname, ios_base::app);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				fout << arr[i][j] << " ";
			}
		}
		fout << endl;
		fout.close();
		return 0;
	};
};

class CData1 : public CPoly2
{
public:
	CData1() : CPoly2() {}
	CData1(string &fname, vector<vector<int>> &arr, int N) : CPoly2(fname, arr, N)
	{
	}
	// ~CData1()
	// {
	// }
	CData1(const CPoly2 &other) : CPoly2(other)
	{
	}
	int output()
	{
		if (cfname.empty())
			return 1;
		ofstream fout(cfname, ios_base::app);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				fout << arr[j][i] << " ";
			}
		}
		fout << endl;
		fout.close();

		return 0;
	};
};

class CFabricData
{
public:
	virtual CPoly2 *create(string &fname, vector<vector<int>> &arr, int N) = 0;
	virtual ~CFabricData() {}
};

class CFabricData0 : public CFabricData
{
	virtual CData0 *create(string &fname, vector<vector<int>> &arr, int N)
	{
		return new CData0(fname, arr, N);
	}
};

class CFabricData1 : public CFabricData
{
	virtual CData1 *create(string &fname, vector<vector<int>> &arr, int N)
	{
		return new CData1(fname, arr, N);
	}
};

CPoly2 *CPoly2::CreateData(string str, CFabricData **f)
{
	CPoly2 *tmp = nullptr;
	int N = 2, I = 1;
	vector<vector<int>> arr;
	vector<int> tmp_arr;
	tmp_arr.reserve(4);
	stringstream s(str);
	string word;
	string fname;
	s >> word;
	if (word.empty())
	{
		cout << "Ошибка: пустая строка" << endl;
		return nullptr;
	}
	if (word == "0")
		I = 0;
	else if (word == "1")
		I = 1;
	else
	{
		cout << "Ошибка: неверно указан тип вывода данных" << endl;
		return nullptr;
	}
	s >> word;
	if (word.empty())
	{
		cout << "Ошибка: в строке из входного файла нет имени выходного файла" << endl;
		return nullptr;
	}
	fname = word;
	int error = 0;
	while (!s.eof())
	{
		s >> word;
		tmp_arr.push_back(digit(word, error));
		if (error)
		{
			cout << "Ошибка: в строке из входного файла после имени файло введено не число" << endl;
			return nullptr;
		}
		if (tmp_arr.size() == tmp_arr.capacity())
		{
			tmp_arr.reserve(tmp_arr.size() * 2);
		}
	}
	if (tmp_arr.size() == 0)
	{
		cout << "Ошибка: в строке из входного файла нет массива чисел" << endl;
		return nullptr;
	}
	N = ssqrt(tmp_arr.size());
	if (tmp_arr.size() != N * N)
	{
		cout << "Ошибка: в строке из входного файла количество чисел не является квадратом целого числа" << endl;
		return nullptr;
	}
	arr.resize(N);
	for (int i = 0; i < N; i++)
	{
		arr[i].resize(N);
		for (int j = 0; j < N; j++)
		{

			arr[i][j] = tmp_arr[i * N + j];
		}
	}
	if (I == 0)
		tmp = f[0]->create(fname, arr, N);
	else
		tmp = f[1]->create(fname, arr, N);
	return tmp;
}

CData0 operator+(const CPoly2 &first, const CPoly2 &second)
{
	int N_sum = max(first.N, second.N);
	vector<vector<int>> arr_sum;
	string fname_sum = first.cfname;
	arr_sum.resize(N_sum);
	for (int i = 0; i < N_sum; i++)
	{
		arr_sum[i].resize(N_sum);
		for (int j = 0; j < N_sum; j++)
		{
			arr_sum[i][j] = 0;
			if (i < first.N and j < first.N)
				arr_sum[i][j] += first.arr[i][j];
			if (i < second.N and j < second.N)
				arr_sum[i][j] += second.arr[i][j];
		}
	}
	CData0 sum(fname_sum, arr_sum, N_sum);
	// cout << "end sum\n";
	return sum;
}

CData0 operator-(const CPoly2 &first, const CPoly2 &second)
{
	int N_sum = max(first.N, second.N);
	vector<vector<int>> arr_sum;
	string fname_sum = first.cfname;
	arr_sum.resize(N_sum);
	for (int i = 0; i < N_sum; i++)
	{
		arr_sum[i].resize(N_sum);
		for (int j = 0; j < N_sum; j++)
		{
			arr_sum[i][j] = 0;
			if (i < first.N and j < first.N)
				arr_sum[i][j] -= first.arr[i][j];
			if (i < second.N and j < second.N)
				arr_sum[i][j] -= second.arr[i][j];
		}
	}
	CData0 sum(fname_sum, arr_sum, N_sum);
	// cout << "end sum\n";
	return sum;
}

CData0 operator--(CPoly2 &th)
{
	for (int i = 0; i < th.N; i++)
	{
		for (int j = 0; j < th.N - 1; j++)
		{
			th.arr[i][j] = (j + 1) * th.arr[i][j + 1];
		}
		th.arr[i][th.N - 1] = 0;
	}
	return th;
}
CData0 operator--(CPoly2 &th, int)
{
	CData0 tmp(th);
	for (int i = 0; i < th.N; i++)
	{
		for (int j = 0; j < th.N - 1; j++)
		{
			th.arr[i][j] = (j + 1) * th.arr[i][j + 1];
		}
		th.arr[i][th.N - 1] = 0;
	}
	return tmp;
}
CData0 operator++(CPoly2 &th)
{
	for (int i = 0; i < th.N; i++)
	{
		for (int j = 0; j < th.N - 1; j++)
		{
			th.arr[j][i] = (j + 1) * th.arr[j + 1][i];
		}
		th.arr[th.N - 1][i] = 0;
	}
	return th;
}
CData0 operator++(CPoly2 &th, int)
{
	CData0 tmp(th);
	for (int i = 0; i < th.N; i++)
	{
		for (int j = 0; j < th.N - 1; j++)
		{
			th.arr[j][i] = (j + 1) * th.arr[j + 1][i];
		}
		th.arr[th.N - 1][i] = 0;
	}
	return tmp;
}

int main()
{
	ofstream fout("output.txt", ios_base::trunc);
	fout.close();
	CFabricData *f[2] = {new CFabricData0(), new CFabricData1()};
	string str;

	ifstream fin("input.txt");

	vector<string> str_r;
	str_r.reserve(4);
	while (getline(fin, str))
	{
		str_r.push_back(str);
		if (str_r.size() == str_r.capacity())
		{
			str_r.reserve(str_r.size() * 2);
		}
	}

	fin.close();
	{
		vector<CPoly2 *> parr;
		parr.reserve(str_r.size());
		for (unsigned int i = 0; i < str_r.size(); i++)
		{

			parr.push_back(CPoly2::CreateData(str_r[i], f));
			if (!parr.back())
			{
				parr.pop_back();
			}
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			parr[i]->output();
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			(*parr[i])++;
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			parr[i]->output();
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			delete parr[i];
			parr[i] = nullptr;
		}
	}
	{
		vector<CPoly2 *> parr;
		parr.reserve(str_r.size());
		for (unsigned int i = 0; i < str_r.size(); i++)
		{

			parr.push_back(CPoly2::CreateData(str_r[i], f));
			if (!parr.back())
			{
				parr.pop_back();
			}
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			(*parr[i])--;
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			parr[i]->output();
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			delete parr[i];
			parr[i] = nullptr;
		}
	}
	{
		vector<CPoly2 *> parr;

		parr.reserve(str_r.size());
		for (unsigned int i = 0; i < str_r.size(); i++)
		{

			parr.push_back(CPoly2::CreateData(str_r[i], f));
			if (!parr.back())
			{
				parr.pop_back();
			}
		}
		CPoly2 *sum = new CData0();

		for (vector<CPoly2 *>::iterator i_parr = parr.begin(); i_parr != parr.end(); i_parr++)
		{
			*sum = **i_parr + *sum;
		}
		sum->output();
		delete sum;
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			delete parr[i];
			parr[i] = nullptr;
		}
	}
	{
		vector<CPoly2 *> parr;
		parr.reserve(str_r.size());
		for (unsigned int i = 0; i < str_r.size(); i++)
		{

			parr.push_back(CPoly2::CreateData(str_r[i], f));
			if (!parr.back())
			{
				parr.pop_back();
			}
		}
		CPoly2 *tmp = new CData0(*parr[0]);
		for (unsigned int i = 0; i < parr.size() - 1; i++)
		{
			*parr[i] = *parr[i + 1];
		}
		*parr[parr.size() - 1] = *tmp;
		delete tmp;

		for (unsigned int i = 0; i < parr.size(); i++)
		{
			parr[i]->output();
		}
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			delete parr[i];
			parr[i] = nullptr;
		}
	}
	for (unsigned int i = 0; i < 2; i++)
	{
		delete f[i];
		f[i] = nullptr;
	}
	return 0;
}
// вводится 1 y x xy
// вот так записывается матрица 2 на 2
// 1 y
// x xy
// ++ дифферинцирование по х
// -- дифферинцирование по y
// stl