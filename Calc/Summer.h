#pragma once
#include "Parser.h"

BigNumber getSum(const char* expr);
bool checkForSum(const char* expr);

bool checkForSum(const char* expr) {
	return strchr(expr, '_') != NULL; // Проверяем наличие символа '_'
}


BigNumber getSum(const char* expr) {
    if (!checkForSum(expr)) {
        fprintf(stderr, "Invalid format: no '_' found in expression.\n");
        return createBigNumber("0"); // Возвращаем 0 в случае ошибки
    }

    // Находим позицию символа '_'
    const char* underscore = strchr(expr, '_');
    if (underscore == NULL) {
        fprintf(stderr, "Invalid format: no '_' found in expression.\n");
        return createBigNumber("0");
    }

    // Извлекаем числа a и b
    char a_str[250], b_str[250];

    // Копируем подстроки до и после '_'
    size_t a_len = underscore - expr; // длина a
    strncpy_s(a_str, sizeof(a_str), expr, a_len);
    a_str[a_len] = '\0'; // Завершаем строку

    strcpy_s(b_str, sizeof(b_str), underscore + 1); // Копируем часть после '_'

    // Преобразуем строки в BigNumber
    BigNumber a = createBigNumber(a_str);
    BigNumber b = createBigNumber(b_str);

    // Проверяем, что a <= b
    if (compareBigNumbers(&a, &b) > 0) {
        fprintf(stderr, "Invalid range: a should be less than or equal to b.\n");
        freeBigNumber(&a);
        freeBigNumber(&b);
        return createBigNumber("0");
    }

    // Суммируем числа от a до b
    BigNumber sum = createBigNumber("0");

    // Используем временную переменную для инкрементации
    BigNumber i = a;

    while (compareBigNumbers(&i, &b) <= 0) {
        sum = addBigNumber(&sum, &i);

        // Увеличиваем i на 1
        BigNumber increment = createBigNumber("1");
        BigNumber new_i = addBigNumber(&i, &increment);

        freeBigNumber(&i); // Освобождаем старое значение i
        i = new_i; // Обновляем i

        freeBigNumber(&increment); // Освобождаем память для increment
    }

    freeBigNumber(&b);
    freeBigNumber(&i); // Освобождаем память для i
    return sum;
}