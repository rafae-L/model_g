#include <fstream>
#include <stdio.h>
#include <cmath>

using namespace std;

int const m = 25, k = 25, n = 5;
float x_0 = 5;
float y_0 = 5;
float t = 1;
float V_x = 1, V_y = 1;

float new_m[m+1][k+1], pre_m[m+1][k+1]; //massiv[x][y]

float f(int i, int j) //f(x,y)
{
    float y = y_0 * i / k;
    float x = x_0 * j / m;
    x = exp(-((x - x_0/2)*(x - x_0/2) + (y - y_0/2)*(y - y_0/2))/2);
    return x;
}

void go_to_pre()
{
    for(int i = 0; i < m + 1; i++)
        for(int j = 0; j < k + 1; j++)
            pre_m[i][j] = new_m[i][j];
}

int main()
{
    for(int i = 0; i < m + 1; i++) //заполняем для t = 0
        for(int j = 0; j < k + 1; j++)
            pre_m[i][j] = f(i, j);


    float a = V_x*t/n/x_0*m;
    float b = V_y*t/n/y_0*k;
    for(int l = 1; l < n + 1; l++)
    {
        for(int j = 0; j < k; j++)
            for(int i = 0; i < m; i++)
                new_m[i][j] = a*(pre_m[i+1][j] - pre_m[i][j]) + pre_m[i][j]; //сдвиг по x

        go_to_pre();

        for(int i = 0; i < m; i++)
            for(int j = 0; j < k; j++)
                new_m[i][j] = b*(pre_m[i][j+1] - pre_m[i][j]) + pre_m[i][j]; //сдвиг по y

        go_to_pre();

        ofstream out;
        char filename[25];
        sprintf(filename, "Data_t/out_%d.txt", l);
        out.open(filename);

        for(int i = 0; i < m + 1; i++)
        {
            float x_1 = x_0 * i / m;
            for(int j = 0; j < k + 1; j++)
                out << x_1 << " " << y_0 * j / k << " " << pre_m[i][j] << endl; //вывод: x, y, z
        }

        out.close();
    }

    return 0;
}
