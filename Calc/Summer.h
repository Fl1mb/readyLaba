#pragma once
#include "Parser.h"

BigNumber getSum(const char* expr);
bool checkForSum(const char* expr);

bool checkForSum(const char* expr) {
	return strchr(expr, '_') != NULL; // ��������� ������� ������� '_'
}


BigNumber getSum(const char* expr) {
    if (!checkForSum(expr)) {
        fprintf(stderr, "Invalid format: no '_' found in expression.\n");
        return createBigNumber("0"); // ���������� 0 � ������ ������
    }

    // ������� ������� ������� '_'
    const char* underscore = strchr(expr, '_');
    if (underscore == NULL) {
        fprintf(stderr, "Invalid format: no '_' found in expression.\n");
        return createBigNumber("0");
    }

    // ��������� ����� a � b
    char a_str[250], b_str[250];

    // �������� ��������� �� � ����� '_'
    size_t a_len = underscore - expr; // ����� a
    strncpy_s(a_str, sizeof(a_str), expr, a_len);
    a_str[a_len] = '\0'; // ��������� ������

    strcpy_s(b_str, sizeof(b_str), underscore + 1); // �������� ����� ����� '_'

    // ����������� ������ � BigNumber
    BigNumber a = createBigNumber(a_str);
    BigNumber b = createBigNumber(b_str);

    // ���������, ��� a <= b
    if (compareBigNumbers(&a, &b) > 0) {
        fprintf(stderr, "Invalid range: a should be less than or equal to b.\n");
        freeBigNumber(&a);
        freeBigNumber(&b);
        return createBigNumber("0");
    }

    // ��������� ����� �� a �� b
    BigNumber sum = createBigNumber("0");

    // ���������� ��������� ���������� ��� �������������
    BigNumber i = a;

    while (compareBigNumbers(&i, &b) <= 0) {
        sum = addBigNumber(&sum, &i);

        // ����������� i �� 1
        BigNumber increment = createBigNumber("1");
        BigNumber new_i = addBigNumber(&i, &increment);

        freeBigNumber(&i); // ����������� ������ �������� i
        i = new_i; // ��������� i

        freeBigNumber(&increment); // ����������� ������ ��� increment
    }

    freeBigNumber(&b);
    freeBigNumber(&i); // ����������� ������ ��� i
    return sum;
}