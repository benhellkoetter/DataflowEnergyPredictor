#include <iostream>

int main()
{
    int n = 15;

    if (n <= 0)
    {
        std::cout << "Fibonacci(" << n << ") = 0\n";
        return 0;
    }
    if (n == 1)
    {
        std::cout << "Fibonacci(" << n << ") = 1\n";
        return 0;
    }

    int a = 0;
    int b = 1;
    for (int i = 2; i <= n; ++i)
    {
        int c = a + b;
        a = b;
        b = c;
    }

    std::cout << "Fibonacci(" << n << ") = " << b << '\n';
    return 0;
}
