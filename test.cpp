#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

class CFabricData;

class CPoly2
{
protected:
	int **arr = nullptr;

	int N = 0;

private:
	void delete_arr()
	{
		for (int i = 0; i < N; i++)
		{
			delete[] arr[i];
			arr[i] = nullptr;
		}
		delete[] arr;
		arr = nullptr;
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
	CPoly2(int **arr_, int N_)
	{
		arr = arr_;
		N = N_;
	}
	virtual ~CPoly2()
	{
		delete_arr();
	}
	/*
	CPoly2(const CPoly2 &other)
	{
		N = other.N;
		arr = new int *[N];
		for (int i = 0; i < N; i++)
		{
			arr[i] = new int[N];
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = other.arr[i][j];
			}
		}
	}
	CPoly2(CPoly2 &&other)
	{
		N = other.N;
		other.N = 0;
		arr = other.arr;
		other.arr = nullptr;
	}
	CPoly2 &operator=(const CPoly2 &other)
	{
		if (this == &other)
		{
			cout << "a = a" << endl;
			return *this;
		}
		delete_arr();
		N = other.N;
		arr = new int *[N];
		for (int i = 0; i < N; i++)
		{
			arr[i] = new int[N];
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = other.arr[i][j];
			}
		}
		return *this;
	}
	CPoly2 &operator=(CPoly2 &&other)
	{
		delete_arr();
		N = other.N;
		arr = other.arr;
		return *this;
	}
	CPoly2 operator+(const CPoly2 &other)
	{
		int sum_N = max(N, other.N);
		int **sum_arr = new int *[sum_N];
		arr = new int *[N];
		for (int i = 0; i < N; i++)
		{
			arr[i] = new int[N];
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = 0;
				if (i <= other.N and j <= other.N)
					arr[i][j] += other.arr[i][j];
				if (i <= other.N and j <= N)
					arr[i][j] += arr[i][j];
			}
		}
		return CPoly2(sum_arr, sum_N);
	}
	CPoly2 operator-(const CPoly2 &other)
	{
		int sum_N = max(N, other.N);
		int **sum_arr = new int *[sum_N];
		arr = new int *[N];
		for (int i = 0; i < N; i++)
		{
			arr[i] = new int[N];
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = 0;
				if (i <= other.N and j <= other.N)
					arr[i][j] -= other.arr[i][j];
				if (i <= other.N and j <= N)
					arr[i][j] += arr[i][j];
			}
		}
		return CPoly2(sum_arr, sum_N);
	}
	CPoly2 operator--(void)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N - 1; j++)
			{
				arr[i][j] = arr[i][j + 1];
			}
			arr[i][N - 1] = 0;
		}
		return *this;
	}
	CPoly2 operator++(void)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N - 1; j++)
			{
				arr[j][i] = arr[j + 1][i];
			}
			arr[N - 1][i] = 0;
		}
		return *this;
	}
	*/
	virtual int output() = 0;
	static CPoly2 *CreateData(char *, CFabricData **);
};

class CData0 : public CPoly2
{
	const char *fname = nullptr;

public:
	CData0(const char *str, int **arr, int N) : CPoly2(arr, N)
	{
		fname = str;
	}
	~CData0()
	{
		delete[] fname;
	}

	int output()
	{
		ofstream fout(fname, ios_base::app);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N - i; j++)
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
	const char *fname = nullptr;

public:
	CData1(const char *str, int **arr, int N) : CPoly2(arr, N)
	{
		fname = str;
	}
	~CData1()
	{
		delete[] fname;
	}

	int output()
	{
		ofstream fout(fname, ios_base::app);
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
	virtual CPoly2 *create(const char *, int **, int) = 0;
	virtual ~CFabricData() {}
};

class CFabricData0 : public CFabricData
{
	virtual CData0 *create(const char *fname, int **arr, int N)
	{
		return new CData0(fname, arr, N);
	}
};

class CFabricData1 : public CFabricData
{
	virtual CData1 *create(const char *fname, int **arr, int N)
	{
		return new CData1(fname, arr, N);
	}
};

CPoly2 *CPoly2::CreateData(char *str, CFabricData **f)
{
	CPoly2 *tmp = nullptr;
	int N = 2, I = 1, len_tmp_arr = 0, size_tmp_arr = 4;
	int **arr = nullptr;
	int *tmp_arr = new int[size_tmp_arr];
	int *tmp_tmp_arr = nullptr;
	char *pch = strtok(str, " \r"), *fname = nullptr;
	if (pch == 0)
	{
		delete[] tmp_arr;
		return nullptr;
	}
	if (pch[0] == '0')
		I = 0;
	pch = strtok(nullptr, " \r");
	if (pch == 0)
	{
		delete[] tmp_arr;
		return nullptr;
	}
	fname = new char[strlen(pch) + 1];
	strcpy(fname, pch);

	while ((pch = strtok(nullptr, " \r")))
	{
		tmp_arr[len_tmp_arr] = digit(pch);
		len_tmp_arr++;
		if (len_tmp_arr == size_tmp_arr)
		{
			size_tmp_arr *= 2;
			tmp_tmp_arr = new int[size_tmp_arr];
			for (int i = 0; i < len_tmp_arr; i++)
			{
				tmp_tmp_arr[i] = tmp_arr[i];
			}
			delete[] tmp_arr;
			tmp_arr = tmp_tmp_arr;
		}
	}
	if (len_tmp_arr == 0)
	{
		delete[] fname;
		delete[] tmp_arr;
		return nullptr;
	}
	for (int i = len_tmp_arr; i < size_tmp_arr; i++)
	{
		tmp_arr[i] = 0;
	}
	N = ssqrt(len_tmp_arr);
	arr = new int *[N];
	for (int i = 0; i < N; i++)
	{
		arr[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			if (i * N + j < size_tmp_arr)
				arr[i][j] = tmp_arr[i * N + j];
			else
				arr[i][j] = 0;
		}
	}
	delete[] tmp_arr;
	if (I == 0)
		tmp = f[0]->create(fname, arr, N);
	else
		tmp = f[1]->create(fname, arr, N);
	return tmp;
}

int main()
{

	int len_parr = 0, size_parr = 4;
	char str[32];

	ifstream fin("input.txt");

	CFabricData *f[2] = {new CFabricData0(), new CFabricData1()};
	CPoly2 **parr = new CPoly2 *[size_parr], **tmp_parr = nullptr;

	while (fin.getline(str, 31))
	{

		parr[len_parr] = CPoly2::CreateData(str, f);
		if (parr[len_parr] != 0)
			len_parr++;
		if (len_parr == size_parr)
		{
			size_parr *= 2;
			tmp_parr = new CPoly2 *[size_parr];
			for (int i = 0; i < len_parr; i++)
			{
				tmp_parr[i] = parr[i];
			}
			delete[] parr;
			parr = tmp_parr;
		}
	}
	fin.close();

	for (int i = 0; i < len_parr; i++)
	{
		parr[i]->output();
	}

	for (int i = 0; i < len_parr; i++)
	{
		delete parr[i];
		parr[i] = nullptr;
	}
	delete[] parr;
	parr = nullptr;

	for (int i = 0; i < 2; i++)
	{
		delete f[i];
		f[i] = nullptr;
	}

	return 0;
}