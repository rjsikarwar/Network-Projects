#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
char *strrev(char *str) {
	char *p1, *p2;

	if (!str || !*str)
		return str;

	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}

	return str;
}

char *itoa(int n, char *s, int b) {
	static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	int i=0, sign;
    
	if ((sign = n) < 0)
		n = -n;

	do {
		s[i++] = digits[n % b];
	} while ((n /= b) > 0);

	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';

	return strrev(s);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void copy(void * src, void * dest, int size, int pos)
{
	char* d = (char*) dest;
	char* s = (char*) src + pos;
	while(size-- > 0) 
		*d++ = *s++;	
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
char *strrev(char *str) {
	char *p1, *p2;

	if (!str || !*str)
		return str;

	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}

	return str;
}

char *itoa(int n, char *s, int b) {
	static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	int i=0, sign;
    
	if ((sign = n) < 0)
		n = -n;

	do {
		s[i++] = digits[n % b];
	} while ((n /= b) > 0);

	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';

	return strrev(s);
}
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
void copy(void * src, void * dest, int size, int pos)
{
	char* d = (char*) dest;
	char* s = (char*) src + pos;
	while(size-- > 0) 
		*d++ = *s++;	
}

