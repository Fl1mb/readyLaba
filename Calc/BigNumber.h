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

    // �������� �� ������ ������
    if (num == NULL || strlen(num) == 0) {
        fprintf(stderr, "Invalid input: NULL or empty string\n");
        BigNum.length = 0;
        BigNum.number = NULL;
        BigNum.sign = '+'; // ���� �� ���������
        return BigNum;
    }

    // ���������� ����
    if (num[0] == '-') {
        BigNum.sign = '-';
        num++; // ������� ���� �� ������ ��� ���������� ���������
    }
    else if (num[0] == '+') {
        BigNum.sign = '+'; // ������� ����, ���� �� �������������
        num++; // ������� ���� �� ������ ��� ���������� ���������
    }
    else {
        BigNum.sign = '+'; // ���� �� ���������
    }

    // �������� ������ ��� ����� ��� �����
    BigNum.length = strlen(num);
    BigNum.number = (char*)malloc(BigNum.length + 1); // +1 ��� �������� �������
    if (BigNum.number) {
        // ���������� strcpy_s ��� ����������� ����������� ������
        if (strcpy_s(BigNum.number, BigNum.length + 1, num) != 0) {
            fprintf(stderr, "Error copying string\n");
            free(BigNum.number); // ����������� ������ � ������ ������
            BigNum.length = 0;
            BigNum.number = NULL;
            BigNum.sign = '+'; // ���� �� ���������
        }
    }
    else {
        // ��������� ������: �� ������� �������� ������
        BigNum.length = 0; // ������������� ����� � 0
        BigNum.number = NULL; // ������������� ��������� � NULL
        BigNum.sign = '+'; // ���� �� ���������
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


    // ����������� ����� ����������
    char resultSign = '+';
    if (sign1 == '-' && sign2 == '-') {
        resultSign = '-';
    }
    else if (sign1 == '-' || sign2 == '-') {
        // ���� ����� ������, �� �� ������ ��������� ���������
        if (compareBigNumbers(a, b) >= 0) {
            return subtractionBigNumber(a, b);
        }
        else {
            return subtractionBigNumber(b, a);
        }
    }

    // �������� ������ ��� ����������
    char* pResult = (char*)malloc(len1 > len2 ? len1 + 2 : len2 + 2); // +2 ��� ���������� �������� � ������������ ����
    if (pResult == NULL) {
        // ��������� ������ ��������� ������
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int carry = 0;

    number1 = number1 + len1 - 1; // ��������� �� ��������� ����� ������� �����
    number2 = number2 + len2 - 1; // ��������� �� ��������� ����� ������� �����

    char* pCurrent = pResult; // ��������� ��� ������ ����������

    while (len1 > 0 || len2 > 0 || carry > 0) {
        int digit1 = (len1 > 0) ? (*number1-- - '0') : 0; // ������� ����� �� ������� �����
        int digit2 = (len2 > 0) ? (*number2-- - '0') : 0; // ������� ����� �� ������� �����

        int sum = digit1 + digit2 + carry; // ���������� ����� � �������
        carry = sum / 10; // ��������� ����� �������
        *pCurrent++ = (sum % 10) + '0'; // ��������� ��������� ����� � ����������

        if (len1 > 0) len1--;
        if (len2 > 0) len2--;
    }

    *pCurrent = '\0'; // ��������� ������

    // ��������� ����������
    int resultLength = pCurrent - pResult; // ����� ����������
    for (int i = 0; i < resultLength / 2; i++) {
        char temp = pResult[i];
        pResult[i] = pResult[resultLength - 1 - i];
        pResult[resultLength - 1 - i] = temp;
    }

    // ������� ������� ����
    int startIndex = 0;
    while (startIndex < resultLength && pResult[startIndex] == '0') {
        startIndex++;
    }

    // ���� ��� ����, �� ��������� ����� "0"
    if (startIndex == resultLength) {
        free(pResult);
        return createBigNumber("0"); // ���������� 0
    }

    // ������� ����� BigNumber � ���������� ���������
    BigNumber result;
    result.length = resultLength - startIndex; // ����� ���������� ��� ������� �����
    result.number = _strdup(pResult + startIndex); // �������� ��������� ��� ������� �����
    result.sign = resultSign; // ������������� ���� ����������

    free(pResult); // ����������� ������
    return result;
}

BigNumber multiplyBigNumber(const BigNumber* a, const BigNumber* b) {
    // ������� ��������� ����� ��� ������
    BigNumber aCopy = createBigNumber(a->number);
    BigNumber bCopy = createBigNumber(b->number);

    char* res;
    int mul, i, j, l1 = aCopy.length, l2 = bCopy.length;

    // �������� ������ ��� ����������
    res = (char*)malloc((l1 + l2 + 1) * sizeof(char));
    if (!res) {
        fprintf(stderr, "Memory allocation failed\n");
        freeBigNumber(&aCopy);
        freeBigNumber(&bCopy);
        BigNumber emptyResult = { NULL, 0, '+' };
        return emptyResult; // ���������� ������ BigNumber � ������ ������
    }

    memset(res, 0, l1 + l2 + 1); // �������������� ������, ������� ������� ������

    // ����������� ������� � �����
    for (i = 0; i < l1; i++) aCopy.number[i] -= '0';
    for (j = 0; j < l2; j++) bCopy.number[j] -= '0';

    // ���������
    for (i = l1 - 1; i >= 0; i--) {
        int carry = 0; // �������
        for (j = l2 - 1; j >= 0; j--) {
            mul = aCopy.number[i] * bCopy.number[j] + res[i + j + 1] + carry;
            carry = mul / 10;
            res[i + j + 1] = mul % 10;
        }
        res[i + j + 1] += carry; // ��������� �������
    }

    // ����������� ����� ������� � �������
    for (i = 0; i < l1 + l2; i++) {
        res[i] += '0';
    }

    // ������� ������� ����
    int startIndex = 0;
    while (startIndex < l1 + l2 && res[startIndex] == '0') {
        startIndex++;
    }

    // ���� ��� ����, �� ��������� ����� "0"
    if (startIndex == l1 + l2) {
        free(res);
        freeBigNumber(&aCopy);
        freeBigNumber(&bCopy);
        return createBigNumber("0"); // ���������� 0
    }

    // ������� ����� BigNumber � ���������� ���������
    BigNumber result;
    result.length = (l1 + l2) - startIndex; // ����� ����������
    result.number = _strdup(res + startIndex); // �������� ��������� ��� ������� �����
    result.sign = (a->sign != b->sign) ? '-' : '+'; // ������������� ���� ����������

    free(res); // ����������� ������
    freeBigNumber(&aCopy); // ����������� ������ ��� �����
    freeBigNumber(&bCopy); // ����������� ������ ��� �����
    return result;
}

inline BigNumber subtractionBigNumber(const BigNumber* a, const BigNumber* b)
{

    // ��������� ����� ���� ������ �� ����� max(len(a), len(b))
    char* result = (char*)calloc(a->length + 1, sizeof(char)); // +1 ��� ������������ ����
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int carry = 0;
    int i, j;

    // ��������� � �����
    for (i = a->length - 1, j = b->length - 1; i >= 0; i--, j--) {
        int digitA = a->number[i] - '0'; // ������� ����� �� ������� �����
        int digitB = (j >= 0) ? (b->number[j] - '0') : 0; // ������� ����� �� ������� �����

        // ���� ������� ����� A ������ ������� ����� B + �������, ����������
        if (digitA - carry < digitB) {
            digitA += 10;
            carry = 1;
        }
        else {
            carry = 0;
        }

        // ��������� ������� ����� ����������
        result[i] = (digitA - digitB - carry) + '0';
    }

    // ������ ����� ���������� � ������ ������� ����
    int startIndex = 0;
    while (startIndex < a->length && result[startIndex] == '0') {
        startIndex++;
    }

    // ���� ��� ����, �� ��������� ����� "0"
    BigNumber difference;
    difference.length = a->length - startIndex;
    if (difference.length <= 0) {
        difference.length = 1;
        difference.number = _strdup("0");
    }
    else {
        difference.number = _strdup(result + startIndex); // �������� ���������
    }

    free(result); // ����������� ������
    return difference;
}

BigNumber factorialBigNumber(const BigNumber* a) {
    // ��������� �������� ����������
    BigNumber result = createBigNumber("1");
    BigNumber fact = createBigNumber("2");

    // ��������, ��� a �� �������������
    if (a->sign == '-') {
        fprintf(stderr, "Factorial is not defined for negative numbers.\n");
        freeBigNumber(&fact);
        freeBigNumber(&result);
        exit(1);
    }

    // �������� �� ������, ���� a ����� "0"
    if (compareBigNumbers(a, &result) == 0) {
        freeBigNumber(&fact);
        return result; // ��������� 0 ����� 1
    }

    // ���������� compareBigNumbers ��� ��������
     do{
        // �������� ������� ��������� �� ������� �������� fact
        BigNumber temp = multiplyBigNumber(&result, &fact);
        freeBigNumber(&result); // ����������� ������ ���������
        result = temp; // ��������� ���������

        // ����������� fact �� 1
        BigNumber increment = createBigNumber("1");
        BigNumber newFact = addBigNumber(&fact, &increment);
        freeBigNumber(&fact); // ����������� ������ �������� fact
        fact = newFact; // ��������� fact

        freeBigNumber(&increment); // ����������� ��������� ����������
    }while (compareBigNumbers(&fact, a) <= 0);

    // ����������� ������ ��� fact
    freeBigNumber(&fact);
    return result; // ���������� ���������
}

inline BigNumber degreeBigNumber(const BigNumber* a, const BigNumber* b)
{
    if (b->number[0] == '0') {
        return createBigNumber("1");
    }

    // �������� �� ������, ���� b �������������
    if (b->sign == '-') {
        fprintf(stderr, "Negative exponent is not supported for BigNumber.\n");
        exit(1);
    }

    // ��������� �������� ����������
    BigNumber result = createBigNumber("1");
    BigNumber base = createBigNumber(a->number); // �������� ���������

    // ���������� ���� ��� ���������� � �������
    for (int i = 0; i < atoi(b->number); i++) {
        BigNumber temp = multiplyBigNumber(&result, &base); // result *= base
        freeBigNumber(&result); // ����������� ������ ���������
        result = temp; // ��������� ���������
    }

    freeBigNumber(&base); // ����������� ������ ��� ���������
    return result; // ���������� ���������
}



int compareBigNumbers(const BigNumber* a, const BigNumber* b)
{
    // ��������� ����
    if (a->length != b->length) {
        return a->length - b->length;
    }
    // ��������� �� ��������
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

