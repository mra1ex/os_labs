#include "swap.h"

void Swap(char *left, char *right)
{
	char buffer = *left;
	*left = *right;
	*right = buffer;
}
