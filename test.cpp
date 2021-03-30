#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

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
	static int digit(char *str)
	{
		int d = 0;
		char *uk = str;
		while (*uk != '\0')
		{
			d *= 10;
			d += (int)*uk - 48;
			uk++;
		}
		return d;
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
	static CPoly2 *CreateData(char *, CFabricData **);
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

CPoly2 *CPoly2::CreateData(char str[], CFabricData **f)
{
	CPoly2 *tmp = nullptr;
	int N = 2, I = 1, len_tmp_arr = 0, size_tmp_arr = 4;
	vector<vector<int>> arr;
	vector<int> tmp_arr(size_tmp_arr);
	char *pch = strtok(str, " \r");
	string fname;
	if (pch == 0)
	{
		cout << "Ошибка: пустая строка" << endl;
		return nullptr;
	}
	if (pch[0] == '0')
		I = 0;
	pch = strtok(nullptr, " \r");
	if (pch == 0)
	{
		cout << "Ошибка: в строке из входного файла нет имени выходного файла" << endl;
		return nullptr;
	}
	fname = pch;

	while ((pch = strtok(nullptr, " \r")))
	{
		tmp_arr[len_tmp_arr] = digit(pch);
		len_tmp_arr++;
		if (len_tmp_arr == size_tmp_arr)
		{
			size_tmp_arr *= 2;
			tmp_arr.resize(size_tmp_arr);
		}
	}
	if (len_tmp_arr == 0)
	{
		cout << "Ошибка: в строке из входного файла нет массива чисел" << endl;
		return nullptr;
	}
	for (int i = len_tmp_arr; i < size_tmp_arr; i++)
	{
		tmp_arr[i] = 0;
	}
	N = ssqrt(len_tmp_arr);
	if (len_tmp_arr != N * N)
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
			if (i * N + j < size_tmp_arr)
				arr[i][j] = tmp_arr[i * N + j];
			else
				arr[i][j] = 0;
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
	char str[128];

	ifstream fin("input.txt");

	vector<CPoly2 *> parr;
	vector<char[128]> str_r;
	str_r.reserve(4);
	parr.reserve(4);

	while (fin.getline(str, 127))
	{

		parr.push_back(CPoly2::CreateData(str, f));
		if (!parr.back())
		{
			parr.pop_back();
		}
		if (parr.size() == parr.capacity())
		{
			parr.reserve(parr.size() * 2);
		}
	}
	while (fin.getline(str, 127))
	{

		parr.push_back(CPoly2::CreateData(str, f));
		if (!parr.back())
		{
			parr.pop_back();
		}
		if (parr.size() == parr.capacity())
		{
			parr.reserve(parr.size() * 2);
		}
	}
	fin.close();
	{

		for (unsigned int i = 0; i < parr.size(); i++)
		{
			parr[i]->output();
		}
		CPoly2 *tmp = new CData0(*parr[0]);
		for (unsigned int i = 0; i < parr.size() - 1; i++)
		{
			*parr[i] = *parr[i + 1];
		}
		*parr[parr.size() - 1] = *tmp;
		CPoly2 *sum = new CData0();
		sum->output();
		for (unsigned int i = 0; i < parr.size(); i++)
		{
			*sum = *parr[i] + *sum;
			sum->output();
		}
		sum->output();
		delete tmp;
		delete sum;

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