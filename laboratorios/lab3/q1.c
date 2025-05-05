#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int not_repeat(int *const arr, int n_size)
{
  int i, j, k;

  for (i = 0; i < n_size; i++)
  {
    for (j = i + 1; j < n_size;)
    {
      if (arr[i] == arr[j])
      {
        for (k = j; k < n_size - 1; k++)
        {
          arr[k] = arr[k + 1];
        }
        n_size--;
      }
      else
      {
        j++;
      }
    }
  }

  return n_size;
}

int main()
{
  int n;
  printf("Digite a quantidade de elementos: ");
  scanf("%d", &n);
  printf("\n");

  int *arr = malloc(n * sizeof(int));

  srand(time(NULL));

  printf("n = %d\n", n);
  printf("[");
  for (int i = 0; i < n; i++)
  {
    arr[i] = rand() % 200;
    printf("%d", arr[i]);
    if (i != n - 1)
      printf(",");
  }
  printf("]\n");

  n = not_repeat(arr, n);

  printf("\n");

  printf("n = %d\n", n);
  printf("[");
  for (int i = 0; i < n; i++)
  {
    printf("%d", arr[i]);
    if (i != n - 1)
      printf(",");
  }
  printf("]\n");

  free(arr);
  return 0;
}