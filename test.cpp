#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

class CFabricData; //fdososfjds

class CPoly2
{
protected:
	int **arr = nullptr;

	int N;

public:
	CPoly2(int **arr_, int N_)
	{
		arr = arr_;
		N = N_;
	}
	~CPoly2()
	{
		for (int i = 0; i < N; i++)
		{
			delete arr[i];
		}
		delete arr;
	}
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
		delete fname;
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
		delete fname;
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
	int N = 2;
	int **arr = new int *[N];
	char *pch = strtok(str, " .,"), *fname = nullptr;
	fname = strtok(nullptr, " .,");

	for (int i = 0; i < N; i++)
	{
		arr[i] = new int[N];
		for (int j = 0; j < N; j++)
		{
			arr[i][j] = i * N + j;
		}
	}
	if (str[0] == '0')
		tmp = f[0]->create("output.txt", arr, N);
	else
		tmp = f[1]->create("output.txt", arr, N);
	return tmp;
}

int main()
{

	int I, len_arr = 0, size_arr = 4;
	char str[32];

	ifstream fin("input.txt");

	CFabricData *f[2] = {new CFabricData0(), new CFabricData1()};
	CPoly2 **arr = new CPoly2 *[size_arr], **tmp_arr = nullptr;

	while (fin.getline(str, 31))
	{

		arr[len_arr] = CPoly2::CreateData(str, f);
		len_arr++;
		if (len_arr == size_arr)
		{
			size_arr *= 2;
			tmp_arr = new CPoly2 *[size_arr];
			for (int i = 0; i < len_arr; i++)
			{
				tmp_arr[i] = arr[i];
			}
			delete arr;
			arr = tmp_arr;
		}
	};
	fin.close();

	for (int i = 0; i < len_arr; i++)
	{
		arr[i]->output();
	}

	for (int i = 0; i < len_arr; i++)
	{
		delete arr[i];
		arr[i] = nullptr;
	}
	delete arr;
	arr = nullptr;

	for (int i = 0; i < 2; i++)
	{
		delete f[i];
		f[i] = nullptr;
	}

	return 0;
}