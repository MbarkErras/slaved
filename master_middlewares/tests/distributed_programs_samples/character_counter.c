#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	uint32_t size;
	int i;
	char	*content;
	uint64_t result;

	read(0, &size, sizeof(uint32_t));

	// printf("size: %d\n", size);
	content = malloc(sizeof(char) * size);
	read(0, content, size);

	// printf("input: %s\n", content);
	result = 0;
	i = -1;
	while (++i < size)
		result += content[i];
	size = sizeof(result);

	// printf("size: %d\n", size);
	write(1, &size, sizeof(uint32_t));

	// printf("output: %ld\n", result);
	write(1, &result, size);
	return (0);
}