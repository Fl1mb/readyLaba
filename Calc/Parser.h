#pragma once
#include "BigNumber.h"

BigNumber evaluate(const char* expr);
BigNumber parse_number(const char** expr);
BigNumber parse_factor(const char** expr);
BigNumber parse_term(const char** expr);
BigNumber parse_term(const char** expr);
BigNumber parse_expression(const char** expr);



BigNumber evaluate(const char* expr) {
	return parse_expression(&expr);
}

BigNumber parse_number(const char** expr) {
	char buffer[2500];
	int i = 0;
	while (isdigit(**expr)) {
		buffer[i] = **expr;
		(*expr)++;
		i++;
	}

	buffer[i] = '\0';
	return createBigNumber(buffer);
}

BigNumber parse_factor(const char** expr) {
	BigNumber num = parse_number(expr);

	while (**expr == '!') {
		(*expr)++;
		BigNumber temp = factorialBigNumber(&num);
		freeBigNumber(&num);
		num = temp;
	}

	while (**expr == '^') {
		(*expr)++;
		BigNumber degree = parse_factor(expr);
		BigNumber temp = degreeBigNumber(&num, &degree); // Исправлено: передаем num и degree
		freeBigNumber(&degree); // Освобождаем память для degree
		freeBigNumber(&num);
		num = temp;
	}
	return num;
}

BigNumber parse_term(const char** expr) {
	BigNumber num = parse_factor(expr);

	while (**expr == '*') {
		char op = **expr;
		(*expr)++; // пропускаем оператор
		BigNumber next_number = parse_factor(expr);
		
		BigNumber temp = multiplyBigNumber(&num, &next_number);
		freeBigNumber(&next_number);
		freeBigNumber(&num);
		num = temp;
	}
	return num;
}

BigNumber parse_expression(const char** expr) {
	BigNumber num = parse_term(expr);

	while (**expr == '+' || **expr == '-') {
		char op = **expr;
		(*expr)++; // Пропускаем оператор

		BigNumber next_number = parse_term(expr);
		if (op == '+') {
			BigNumber temp = addBigNumber(&num, &next_number);
			freeBigNumber(&next_number);
			freeBigNumber(&num);
			num = temp;
		}
		else if (op == '-') {
			BigNumber temp = subtractionBigNumber(&num, &next_number);
			freeBigNumber(&next_number);
			freeBigNumber(&num);
			num = temp;
		}
	}
	return num;
}


char* delete_spacing(const char* expr) {
	int size = strlen(expr);
	char* result = (char*)malloc(size + 1);
	if (!result) return NULL;

	int i = 0;
	for (int j = 0; j < size; j++) {
		if (!isspace(expr[j])) result[i++] = expr[j];
	}
	result[i] = '\0';
	return result;
}