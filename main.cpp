#include <fstream>
#include <stdio.h>
#include <cmath>

using namespace std;

int const m = 25, k = 25, n = 5, p = 5, z = 5;
float x_0 = 5; // разделено на m
float y_0 = 5; // разделено на k
float t = 1; // разделено на n
float f_0 = 100;
float V_x = 1; // от -p*V_x до p*V_x
float V_y = 1; // от -z*V_y до z*V_y

float new_m[m+1][k+1][2*p+1][2*z+1], pre_m[m+1][k+1][2*p+1][2*z+1]; //massiv[x][y][v_x][v_y]

int sign(int a)
{
    if(a > 0) return 1;
    else if(a == 0) return 0;
    else return -1;
}

void fill_start(int i, int j) //f(x,y)
{
    float y = y_0 * i / k;
    float x = x_0 * j / m;
    for(int u = -p; u <= p; u++)
        for(int s = -z; s <= z; s++)
            pre_m[i][j][u][s] = exp(-((x - x_0/2)*(x - x_0/2) + (y - y_0/2)*(y - y_0/2))/2);
}

void go_to_pre()
{
    for(int i = 0; i < m; i++)
        for(int j = 0; j < k; j++)
            for(int u = -p; u <= p; u++)
                for(int s = -z; s <= z; s++)
                    pre_m[i][j][u][s] = new_m[i][j][u][s];
}

void go_to_data_in_number(int l)
{
    ofstream out;
    char filename[25];
    sprintf(filename, "Data_t/out_%d.txt", l);
    out.open(filename);

    for(int i = 0; i < m + 1; i++)
    {
        float x = x_0 * i / m;
        for(int j = 0; j < k + 1; j++)
        {
            int f = 0;
            for(int u = -p; u <= p; u++)
                for(int s = -z; s <= z; s++)
                    f += pre_m[i][j][u][s];
            out << x << " " << y_0 * j / k << " " << f << endl; //вывод: x, y, z
        }
    }

    out.close();
}

int main()
{
    for(int i = 0; i < m + 1; i++) //заполняем для t = 0
        for(int j = 0; j < k + 1; j++)
            fill_start(i, j);

    go_to_data_in_number(0);

    float a = V_x*t/n/x_0*m;
    float b = V_y*t/n/y_0*k;
    for(int l = 1; l < n + 1; l++)
    {
        for(int j = 0; j < k + 1; j++) //сдвиг по x
            {
                for(int i = 1; i < m; i++) //x принимающий значения (0; x_0)
                    for(int u = -p; u <= p; u++)
                    {
                        a = p*V_x*t/n/x_0*m;
                        for(int s = -z; s <= z; s++)
                            new_m[i][j][u][s] = a*(pre_m[i-sign(u)][j][u][s] - pre_m[i][j][u][s]) + pre_m[i][j][u][s];
                    }
                for(int u = -p; u < 0; u++) //крайнее положение x = 0
                {
                    a = p*V_x*t/n/x_0*m;
                    for(int s = -z; s <= z; s++)
                        new_m[0][j][u][s] = a*(pre_m[1][j][u][s] - pre_m[0][j][u][s]) + pre_m[0][j][u][s];

                }
                for(int u = 1; u <= p; u++) //крайнее положение x = x_0
                {
                    a = p*V_x*t/n/x_0*m;
                    for(int s = -z; s <= z; s++)
                        new_m[m][j][u][s] = a*(pre_m[m-1][j][u][s] - pre_m[m][j][u][s]) + pre_m[m][j][u][s];

                }
            }
        go_to_pre();


        for(int i = 0; i < m + 1; i++) //сдвиг по y
            {
                for(int j = 1; j < k; j++) //y принимающий значения (0; y_0)
                    for(int s = -z; s <= z; s++)
                    {
                        b = p*V_y*t/n/y_0*k;
                        for(int u = -p; u <= p; u++)
                            new_m[i][j][u][s] = b*(pre_m[i][j-sign(u)][u][s] - pre_m[i][j][u][s]) + pre_m[i][j][u][s];
                    }
                for(int s = -z; s < 0; s++) //крайнее положение y = 0
                {
                    b = p*V_y*t/n/y_0*k;
                    for(int u = -p; u <= p; u++)
                        new_m[i][0][u][s] = b*(pre_m[i][1][u][s] - pre_m[i][0][u][s]) + pre_m[i][0][u][s];

                }
                for(int s = 1; s <= z; s++) //крайнее положение y = y_0
                {
                    b = p*V_y*t/n/y_0*k;
                    for(int u = -p; u <= p; u++)
                        new_m[i][k][u][s] = b*(pre_m[i][k-1][u][s] - pre_m[i][k][u][s]) + pre_m[i][k][u][s];

                }
            }
        go_to_pre();

        go_to_data_in_number(l);
    }

    return 0;
}
