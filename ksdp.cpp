#include <iostream>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

/* Function to find maximum of two integers */
int max(int a, int b) { return (a > b) ? a : b; }

/* Returns the maximum value that can be put in a knapsack of capacity W */
int knapSack(int W, int weight[], int value[], int n)
{
  int i, w;
  int K[n + 1][W + 1];

  /* Build table K[][] in bottom up manner */
  for (i = 0; i <= n; i++)
  {
    for (w = 0; w <= W; w++)
    {
      if (i == 0 || w == 0)
        K[i][w] = 0;
      else if (weight[i - 1] <= w)
        K[i][w] = max(value[i - 1] + K[i - 1][w - weight[i - 1]], K[i - 1][w]);
      else
        K[i][w] = K[i - 1][w];
    }
  }

  return K[n][W];
}
int main()
{
  cout << "###########DYNAMIC PROGRAMMING KNAPSACK 0/1###########" << endl;
  int n;
  cout << "\nEnter the number of items : ";
  cin >> n;
  int value[n];
  int weight[n];
  int W;
  cout << "\nEnter the capacity of the knapsack : ";
  cin >> W;
  cout << "\nEnter the item’s weight and its value \n";

  /*for (int i = 0; i < n; i++)
  {
    cin >> weight[i] >> value[i];
  }*/

  srand(time(NULL));
  for (int col = 0; col < n; col++)
  {
    weight[col] = (rand() % W) + 1;
    value[col] = (rand() % (1000 - 20 + 1));
    cout << "Weight: " << weight[col] << " | Value: " << value[col] << endl;
  }n

  auto start = high_resolution_clock::now();
  cout << "\nMaximum value in a 0-1 knapsack : " << knapSack(W, weight, value, n) << endl;
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);

  cout << "Time taken(microseconds) : " << duration.count();
  return 0;
}