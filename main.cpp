#include <iostream>
#include <windows.h>
#include <chrono> // Библиотека, чтобы считать время.

using namespace std;
using namespace chrono;

std::chrono::milliseconds;

void outArray(int* a, int nRow, int nColumn)
{
 for (int i = 0; i < nRow; ++i)
 {
   for (int j = 0; j < nColumn; ++j)
   {
     cout << *(a + nColumn * i + j) << " ";
   }
   cout << endl;
 }
}

int main()
{
 setlocale(LC_ALL, "rus");

 const long long number = 10E+7; // Просто большое число. 1000-7 я гуль

 chrono::steady_clock timer1; // Рассчет времени для пустого цикла.
 auto start = timer1.now();
 for (long long i = 0; i < number; ++i);
 auto end = timer1.now();
 cout << "Время выполнения пустого цикла в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

 chrono::steady_clock timer2; // Рассчет времени для непосредственной адрессации.
 start = timer2.now();
 for (long long i = 0; i < number; ++i) {
   __asm {
     // Команда выполняется много раз потому что иначе основное время выполнялся бы пустой цикл
     // и мы не заметили бы разницы.
     // Чтобы выполнение команд занимало ощутимое время - надо написать их много раз (в нашем случае 20).
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
     add eax, 10
   }
 }
 end = timer2.now();
 cout << "Время выполнения c использованием непосрественной адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;


 chrono::steady_clock timer3; // Рассчет времени для регистровой адресации.
 start = timer3.now();
 for (long long i = 0; i < number; ++i) {
   __asm {
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
     add eax, ebx
   }
 }
 end = timer3.now();
 cout << "Время выполнения c использованием регистровой адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

 chrono::steady_clock timer4; // Расчет времени для косвенно-регистровой адресации.
 start = timer4.now();
 for (long long i = 0; i < number; ++i) {
   __asm {
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
     add[ebx], eax
   }
 }
 end = timer4.now();
 cout << "Время выполнения c использованием косвенно-регистровой адресации в мс : " << duration_cast<milliseconds>(end - start).count() << endl;

 int arr[4][5] = { 1, 1, 1, 1, 1,
                   2, 2, 2, 2, 2,
                   3, 3, 3, 3, 3,
                   4, 4, 4, 4, 4 };
 int k = 9;
 cout << "Изначальный массив: " << endl;
 outArray(arr[0], 4, 5);

 __asm {
   cld // Сбрасываем флаги.
   mov ebx, 2 // Записываем в ebx количество четных строк в массиве. (Так как у нас 64 бита, у нас используется приставка e)
   lea edi, [arr + 20] // Записываем в edi первый элемент первой четной строки (массив + 20 байт (5 элементов))

   mov eax, k // Записываем в eax число, на которое хотим менять.
   loop1:
     mov ecx, 5 // Записываем в счетчик количство чисел в строчке.
     rep stosd // Кладем значения из eax в edi и перескакиваем на следующий элемент (и так exc раз).
     add edi, 20 // Пропускаем нечетную строку.

     dec ebx // Понижаем ebx на единицу
     cmp ebx, 0 // и сравниваем его с нулем (надо ли дальше менять элементы?).
     jne loop1 // Цикл, пока ebx не ноль.
 }
 cout << "Массив после изменения четных строк : " << endl;
 outArray(arr[0], 4, 5);

 while (true)
 {
   Sleep(10);
 }
 return 0;
}
