#pragma once
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* number;
    int length;
    char sign;
}BigNumber;

void freeBigNumber(BigNumber* num);
BigNumber addBigNumber(const BigNumber* a, const BigNumber* b);
BigNumber multiplyBigNumber(const BigNumber* a, const BigNumber* b);
BigNumber subtractionBigNumber(const BigNumber* a, const BigNumber* b);
BigNumber factorialBigNumber(const BigNumber* a);
BigNumber degreeBigNumber(const BigNumber* a, const BigNumber* b);
int compareBigNumbers(const BigNumber* a, const BigNumber* b);
bool isBigger(const BigNumber* a, const BigNumber* b);


void printNumber(const BigNumber* number);

typedef unsigned long long ull;



BigNumber createBigNumber(const char* num) {
    BigNumber BigNum;

    // Проверка на пустую строку
    if (num == NULL || strlen(num) == 0) {
        fprintf(stderr, "Invalid input: NULL or empty string\n");
        BigNum.length = 0;
        BigNum.number = NULL;
        BigNum.sign = '+'; // Знак по умолчанию
        return BigNum;
    }

    // Определяем знак
    if (num[0] == '-') {
        BigNum.sign = '-';
        num++; // Убираем знак из строки для дальнейшей обработки
    }
    else if (num[0] == '+') {
        BigNum.sign = '+'; // Убираем знак, если он положительный
        num++; // Убираем знак из строки для дальнейшей обработки
    }
    else {
        BigNum.sign = '+'; // Знак по умолчанию
    }

    // Выделяем память для числа без знака
    BigNum.length = strlen(num);
    BigNum.number = (char*)malloc(BigNum.length + 1); // +1 для нулевого символа
    if (BigNum.number) {
        // Используем strcpy_s для безопасного копирования строки
        if (strcpy_s(BigNum.number, BigNum.length + 1, num) != 0) {
            fprintf(stderr, "Error copying string\n");
            free(BigNum.number); // Освобождаем память в случае ошибки
            BigNum.length = 0;
            BigNum.number = NULL;
            BigNum.sign = '+'; // Знак по умолчанию
        }
    }
    else {
        // Обработка ошибки: не удалось выделить память
        BigNum.length = 0; // Устанавливаем длину в 0
        BigNum.number = NULL; // Устанавливаем указатель в NULL
        BigNum.sign = '+'; // Знак по умолчанию
    }

    return BigNum;
}




void printNumber(const BigNumber* number) {
    if (number->sign == '-')putchar('-');
    printf(number->number);
}


void freeBigNumber(BigNumber* num)
{
    free(num->number);
    num->length = 0;
    num->number = NULL;
    num->sign = 0;
}

BigNumber addBigNumber(const BigNumber* a, const BigNumber* b) {
    const char* number1 = a->number;
    const char* number2 = b->number;
    int len1 = a->length, len2 = b->length;
    char sign1 = a->sign, sign2 = b->sign;


    // Определение знака результата
    char resultSign = '+';
    if (sign1 == '-' && sign2 == '-') {
        resultSign = '-';
    }
    else if (sign1 == '-' || sign2 == '-') {
        // Если знаки разные, то мы должны выполнить вычитание
        if (compareBigNumbers(a, b) >= 0) {
            return subtractionBigNumber(a, b);
        }
        else {
            return subtractionBigNumber(b, a);
        }
    }

    // Выделяем память для результата
    char* pResult = (char*)malloc(len1 > len2 ? len1 + 2 : len2 + 2); // +2 для возможного переноса и завершающего нуля
    if (pResult == NULL) {
        // Обработка ошибки выделения памяти
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int carry = 0;

    number1 = number1 + len1 - 1; // Указатель на последнюю цифру первого числа
    number2 = number2 + len2 - 1; // Указатель на последнюю цифру второго числа

    char* pCurrent = pResult; // Указатель для записи результата

    while (len1 > 0 || len2 > 0 || carry > 0) {
        int digit1 = (len1 > 0) ? (*number1-- - '0') : 0; // текущая цифра из первого числа
        int digit2 = (len2 > 0) ? (*number2-- - '0') : 0; // текущая цифра из второго числа

        int sum = digit1 + digit2 + carry; // складываем цифры и перенос
        carry = sum / 10; // вычисляем новый перенос
        *pCurrent++ = (sum % 10) + '0'; // добавляем последнюю цифру к результату

        if (len1 > 0) len1--;
        if (len2 > 0) len2--;
    }

    *pCurrent = '\0'; // Завершаем строку

    // Переворот результата
    int resultLength = pCurrent - pResult; // Длина результата
    for (int i = 0; i < resultLength / 2; i++) {
        char temp = pResult[i];
        pResult[i] = pResult[resultLength - 1 - i];
        pResult[resultLength - 1 - i] = temp;
    }

    // Убираем ведущие нули
    int startIndex = 0;
    while (startIndex < resultLength && pResult[startIndex] == '0') {
        startIndex++;
    }

    // Если все нули, то результат равен "0"
    if (startIndex == resultLength) {
        free(pResult);
        return createBigNumber("0"); // Возвращаем 0
    }

    // Создаем новый BigNumber с правильным значением
    BigNumber result;
    result.length = resultLength - startIndex; // Длина результата без ведущих нулей
    result.number = _strdup(pResult + startIndex); // Копируем результат без ведущих нулей
    result.sign = resultSign; // Устанавливаем знак результата

    free(pResult); // Освобождаем память
    return result;
}

BigNumber multiplyBigNumber(const BigNumber* a, const BigNumber* b) {
    // Создаем временные копии для работы
    BigNumber aCopy = createBigNumber(a->number);
    BigNumber bCopy = createBigNumber(b->number);

    char* res;
    int mul, i, j, l1 = aCopy.length, l2 = bCopy.length;

    // Выделяем память для результата
    res = (char*)malloc((l1 + l2 + 1) * sizeof(char));
    if (!res) {
        fprintf(stderr, "Memory allocation failed\n");
        freeBigNumber(&aCopy);
        freeBigNumber(&bCopy);
        BigNumber emptyResult = { NULL, 0, '+' };
        return emptyResult; // Возвращаем пустой BigNumber в случае ошибки
    }

    memset(res, 0, l1 + l2 + 1); // Инициализируем память, включая нулевой символ

    // Преобразуем символы в числа
    for (i = 0; i < l1; i++) aCopy.number[i] -= '0';
    for (j = 0; j < l2; j++) bCopy.number[j] -= '0';

    // Умножение
    for (i = l1 - 1; i >= 0; i--) {
        int carry = 0; // Перенос
        for (j = l2 - 1; j >= 0; j--) {
            mul = aCopy.number[i] * bCopy.number[j] + res[i + j + 1] + carry;
            carry = mul / 10;
            res[i + j + 1] = mul % 10;
        }
        res[i + j + 1] += carry; // Добавляем перенос
    }

    // Преобразуем числа обратно в символы
    for (i = 0; i < l1 + l2; i++) {
        res[i] += '0';
    }

    // Убираем ведущие нули
    int startIndex = 0;
    while (startIndex < l1 + l2 && res[startIndex] == '0') {
        startIndex++;
    }

    // Если все нули, то результат равен "0"
    if (startIndex == l1 + l2) {
        free(res);
        freeBigNumber(&aCopy);
        freeBigNumber(&bCopy);
        return createBigNumber("0"); // Возвращаем 0
    }

    // Создаем новый BigNumber с правильным значением
    BigNumber result;
    result.length = (l1 + l2) - startIndex; // Длина результата
    result.number = _strdup(res + startIndex); // Копируем результат без ведущих нулей
    result.sign = (a->sign != b->sign) ? '-' : '+'; // Устанавливаем знак результата

    free(res); // Освобождаем память
    freeBigNumber(&aCopy); // Освобождаем память для копий
    freeBigNumber(&bCopy); // Освобождаем память для копий
    return result;
}

inline BigNumber subtractionBigNumber(const BigNumber* a, const BigNumber* b)
{

    // Результат может быть длиной не более max(len(a), len(b))
    char* result = (char*)calloc(a->length + 1, sizeof(char)); // +1 для завершающего нуля
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int carry = 0;
    int i, j;

    // Вычитание с конца
    for (i = a->length - 1, j = b->length - 1; i >= 0; i--, j--) {
        int digitA = a->number[i] - '0'; // текущая цифра из первого числа
        int digitB = (j >= 0) ? (b->number[j] - '0') : 0; // текущая цифра из второго числа

        // Если текущая цифра A меньше текущей цифры B + перенос, заимствуем
        if (digitA - carry < digitB) {
            digitA += 10;
            carry = 1;
        }
        else {
            carry = 0;
        }

        // Вычисляем текущую цифру результата
        result[i] = (digitA - digitB - carry) + '0';
    }

    // Найдем длину результата и удалим ведущие нули
    int startIndex = 0;
    while (startIndex < a->length && result[startIndex] == '0') {
        startIndex++;
    }

    // Если все нули, то результат равен "0"
    BigNumber difference;
    difference.length = a->length - startIndex;
    if (difference.length <= 0) {
        difference.length = 1;
        difference.number = _strdup("0");
    }
    else {
        difference.number = _strdup(result + startIndex); // копируем результат
    }

    free(result); // освобождаем память
    return difference;
}

BigNumber factorialBigNumber(const BigNumber* a) {
    // Начальное значение факториала
    BigNumber result = createBigNumber("1");
    BigNumber fact = createBigNumber("2");

    // Проверка, что a не отрицательное
    if (a->sign == '-') {
        fprintf(stderr, "Factorial is not defined for negative numbers.\n");
        freeBigNumber(&fact);
        freeBigNumber(&result);
        exit(1);
    }

    // Проверка на случай, если a равно "0"
    if (compareBigNumbers(a, &result) == 0) {
        freeBigNumber(&fact);
        return result; // Факториал 0 равен 1
    }

    // Используем compareBigNumbers для проверки
     do{
        // Умножаем текущий результат на текущее значение fact
        BigNumber temp = multiplyBigNumber(&result, &fact);
        freeBigNumber(&result); // Освобождаем старый результат
        result = temp; // Обновляем результат

        // Увеличиваем fact на 1
        BigNumber increment = createBigNumber("1");
        BigNumber newFact = addBigNumber(&fact, &increment);
        freeBigNumber(&fact); // Освобождаем старое значение fact
        fact = newFact; // Обновляем fact

        freeBigNumber(&increment); // Освобождаем временную переменную
    }while (compareBigNumbers(&fact, a) <= 0);

    // Освобождаем память для fact
    freeBigNumber(&fact);
    return result; // Возвращаем результат
}

inline BigNumber degreeBigNumber(const BigNumber* a, const BigNumber* b)
{
    if (b->number[0] == '0') {
        return createBigNumber("1");
    }

    // Проверка на случай, если b отрицательное
    if (b->sign == '-') {
        fprintf(stderr, "Negative exponent is not supported for BigNumber.\n");
        exit(1);
    }

    // Начальное значение результата
    BigNumber result = createBigNumber("1");
    BigNumber base = createBigNumber(a->number); // Копируем основание

    // Используем цикл для возведения в степень
    for (int i = 0; i < atoi(b->number); i++) {
        BigNumber temp = multiplyBigNumber(&result, &base); // result *= base
        freeBigNumber(&result); // Освобождаем старый результат
        result = temp; // Обновляем результат
    }

    freeBigNumber(&base); // Освобождаем память для основания
    return result; // Возвращаем результат
}



int compareBigNumbers(const BigNumber* a, const BigNumber* b)
{
    // Сравнение длин
    if (a->length != b->length) {
        return a->length - b->length;
    }
    // Сравнение по символам
    return strcmp(a->number, b->number);
}

bool isBigger(const BigNumber* a, const BigNumber* b)
{
    if (a->length > b->length)return true;
    else if (a->length == b->length) {
        for (int i = 0; i < a->length; i++) {
            if (a->number[i] > b->number[i])continue;
            return false;
        }
        return true;
    }
    else {
        return false;
    }
}

