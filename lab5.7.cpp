//Предусмотреть различные варианты заполнения массива :
//– из файла
//– с клавиатуры
//– случайным образом.
//Задачи 1) и 2) решаются с использованием указателей и адресной арифметики.
//В задаче 2):
//– минимум последний, максимум первый.
//– необходимо отдельной функцией найти границу(ы) фрагмента и отдельной функцией описать
//обработку фрагмента.

//В одномерном массиве, состоящем из n целых элементов, вычислить:
//1) номер максимального элемента массива;
//2) произведение элементов массива, расположенных между первым и вторым нулевыми элементами.
//3) преобразовать массив таким образом, чтобы в первой его половине располагались элементы, стоявшие 
//в нечетных позициях, а во второй половине – элементы, стоявшие в четных позициях.
#include <iostream>
#include <fstream>
#include <functional>
#include <Windows.h>

int menu(const char* message, std::function<bool(short)> predicate);
using TLambda = std::function<int()>;
template <typename T, typename Predicat>
void ReadAndCheck(T& x, Predicat condition, const char* message);
void ending(int size);
int exit();
int* memory_allocation(int size);
void fill(int* arr, int size, std::istream& stream);
void fill(int* begin, int* end, int a, int b);
void free_memory(int*& arr);
void print(int* arr, int size);
int checkFile(std::ifstream& file);
int task1(int* begin, int* end);
void findNulls(int* begin, int* end, int*& first, int*& second);
int task2(int* begin, int* end);
void circleShiftToRight(int* arr, int size);
void task3(int* arr, int size);

int main()
{
	SetConsoleOutputCP(1251);

	std::ifstream file("test3.txt");

	int a{}, b{};
	TLambda preamble[3] = {
		[&a, &b]()->int {int size; srand(GetTickCount()); ReadAndCheck(size, [](int n)->bool {return n > 0; }, "Введите количество элементов: \n");  std::cout << "Введите диапазон от a до b (a < b):\n"; ReadAndCheck(a, [](int x) {return true; }, "a: "); ReadAndCheck(b, [](int x) {return true; }, "b: "); return size; },
		[]()->int {int size; ReadAndCheck(size, [](int n)->bool {return n > 0; }, "Введите количество элементов: \n"); ending(size); return size; },
		[&file]()->int {int size; file >> size;  return size; }
	};

	short option;
	do
	{
		option = menu("Задачи:\n 1. Вычислить номер максимального элемента массива;\n 2. Вычислить произведение элементов массива, расположенных между первым и вторым нулевыми элементами;\n 3. Преобразовать массив таким образом, чтобы в первой его половине располагались элементы,\n стоявшие в нечетных позициях, а во второй половине – элементы, стоявшие в четных позициях;\n 4. Завершить работу", [](int x) {return x >= 1 && x <= 4; });

		if (option != 4)
		{
			short choice = menu("\nВыберите способ ввода массива:\n 1. Генератор случайных значений\n 2. Ручной ввод с клавиатуры\n 3. Из файла", [](int x) {return x >= 1 && x <= 3; });
			
			int size = preamble[choice - 1]();
			int* arr = memory_allocation(size);
			switch (choice)
			{
			case 1:
				fill(arr, arr + size, a, b);
				print(arr, size);
				break;
			case 2:
				fill(arr, size, std::cin);
				break;
			default:
				fill(arr, size, file);
				break;
			}

			switch (option)
			{
			case 1:
			{
				std::cout << "Номер максимального элемента массива: " << task1(arr, arr + size) << '\n';
			}
				break;

			case 2:
			{
				int res = task2(arr, arr + size);
				if (res == 0)
					std::cout << "В массиве нет достаточного количества нулей" << '\n';
				else
					std::cout << "Произведение элементов массива между первыми двумя нулями = " << res << '\n';
			}
				break;

			default:
			{
				task3(arr, size);
				print(arr, size);
			}
				break;
			}
		free_memory(arr);
		option = exit();
		}
	} while (option != 4);
}

int menu(const char* message, std::function<bool(short)> predicate)
{
	std::cout << message;
	short option{};
	ReadAndCheck(option, predicate, "\n->");
	return option;
}

template<typename T, typename Predicat>
void ReadAndCheck(T& x, Predicat condition, const char* message)
{
	std::cout << message;
	while (!(std::cin >> x && condition(x))) {
		std::cout << "Ошибка! Введите корректное значение:\n";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cout << message;
	}
}

void ending(int size)
{
	std::cout << "Введите " << size << " элемент";
	if (size % 100 < 21 && size % 100 > 10)
		std::cout << "ов: ";
	else {
		switch (size % 10) {
		case 1:
			break;
		case 2:
		case 3:
		case 4:
			std::cout << "а: ";
			break;
		default:
			std::cout << "ов: ";
			break;
		}
	}
}

int exit()
{
	std::cout << "\nЗавершить работу? (Y/любая клавиша): ";
	char isExit;
	short optionEx{};
	std::cin >> isExit;
	if (isExit == 'Y' || isExit == 'y')
		optionEx = 4;
	std::cout << '\n';
	return optionEx;
}

int* memory_allocation(int size)
{
	return new int[size];
}

void fill(int* arr, int size, std::istream& stream)
{
	for (int i{}; i < size; ++i)
	{
		stream >> arr[i];
	}
}

void fill(int* begin, int* end, int a, int b)
{
	srand(GetTickCount());
	for (int* ptr = { begin }; ptr != end; ++ptr)
	{
		*ptr = rand() % (b - a + 1) + a;
	}
}

void free_memory(int*& arr)
{
	delete[]arr;
	arr = nullptr;
}

void print(int* arr, int size)
{
	std::cout << '\n';
	for (int i = 0; i != size; i++)
	{
		std::cout << arr[i] <<' ';
	}
	std::cout << '\n';
}

int checkFile(std::ifstream& file)
{
	int result = 1;
	if (!file)
		result = -1;
	else
		if (file.peek() == EOF)
			result = 0; 

	return result;
}

// номер максимального элемента массива;
int task1(int* begin, int* end)
{
	int* num{begin};
	for (int* ptr{ begin }; ptr < end; ++ptr)
	{
		if (*ptr > *num)
		{
			num = ptr;
		}; 
	}
	return num - begin + 1;
}

void findNulls(int* begin, int* end, int*& first, int*& second)
{
	int* ptr{ begin };
	while (*first != 0 && ptr < end)
	{
		++ptr;
		if (*ptr == 0) first = ptr;
	}
	if (first)
	{
		second = first + 1;
		while ( *second!=0 && ptr < end  )
		{
			++ptr;
			if (*ptr == 0) second = ptr;
		}
	}
}

//произведение элементов массива, расположенных между первым и вторым нулевыми элементами
int task2(int* begin, int* end)
{
	int product{1};
	int* first =  begin ;
	int* second = begin;
	findNulls(begin, end, first, second);

	if ( !first || !second || second - first == 1) product = 0;
	else
	{
		for (int* ptr{ first + 1}; ptr < second; ++ptr)
		{
			product *= *ptr;
		}
	}
	return product;
}

//переделать массив так чтоб всё что было на нечёт местах стало в перю половину и наоборот
//void task3(int* arr, int size)
//{
//	int temp{};
//	if (size % 2)
//	{
//		for (int i = 1; i < size / 2; i += 2)
//		{
//			temp = arr[i];
//			arr[i] = arr[size - i];
//			arr[size - i] = temp;
//		}
//	}
//	else
//	{
//		for (int i = 1; i < size / 2; i += 2)
//		{
//			temp = arr[i];
//			arr[i] = arr[size - i-1];
//			arr[size - i-1] = temp;
//		}
//	}
//}


void circleShiftToRight(int* arr, int size)
{
	int tmp = arr[size - 1];
	for (int i = size - 1; i > 0; --i)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = tmp;
}

void task3(int* arr, int size)
{
	int cnt{0};
	for (int i = 1; i != size; ++i)
	{
		if (!(i % 2))
		{
			++cnt;
			circleShiftToRight(&arr[cnt], i + 1 - cnt);
		}
	}
}