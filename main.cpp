#include <fstream>
#include <stdio.h>
#include <cmath>

using namespace std;

int const m = 25, k = 25, n = 300, p = 10, z = 10;
float x_0 = 5; // ��������� �� m
float y_0 = 5; // ��������� �� k
float t = 1; // ��������� �� n
float f_0 = 100;
float V_x = 1; // �� -p*V_x �� p*V_x
float V_y = 1; // �� -z*V_y �� z*V_y

float a1 = 0.05; // � ������ a1 = V_x*t/n/x_0*m
float b1 = 0.05; // � ������ b1 = V_y*t/n/y_0*k

float new_m[m+1][k+1][2*p+1][2*z+1], pre_m[m+1][k+1][2*p+1][2*z+1]; //massiv[x][y][v_x][v_y]

float e_V = 0;// ���� ����� ���������

void fill_e_V()
{
    for(int s = - z; s <= z; s++)
        for(int u = -p; u <= p; u++)
            if(u*u + s*s <= p*z)
                e_V += exp(-((u*V_x)*(u*V_x) + (s*V_y)*(s*V_y)) / 2);
}


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
    float f = 10000*exp(-((x - x_0/2)*(x - x_0/2) + (y - y_0/2)*(y - y_0/2))/2);
    for(int u = -p; u <= p; u++)
        for(int s = -z; s <= z; s++)
            if(s == 4 && u == 9)
            pre_m[i][j][u+p][s+z] = f;
        else pre_m[i][j][u+p][s+z] = 0;
}

void go_to_pre()
{
    for(int i = 0; i <= m; i++)
        for(int j = 0; j <= k; j++)
            for(int u = -p; u <= p; u++)
                for(int s = -z; s <= z; s++)
                    pre_m[i][j][u+p][s+z] = new_m[i][j][u+p][s+z];
}

void go_to_data_in_number(int l)
{
    ofstream out;
    char filename[25];
    sprintf(filename, "Data_t/out_%d.txt", l);
    out.open(filename);

    for(int i = 0; i <= m; i++)
    {
        float x = x_0 * i / m;
        for(int j = 0; j <= k; j++)
        {
            float f = 0;
            for(int u = -p; u <= p; u++)
                for(int s = -z; s <= z; s++)
                    f += pre_m[i][j][u+p][s+z];
            out << x << " " << y_0 * j / k << " " << f << endl; //�����: x, y, z
        }
    }

    out.close();
}

float energy()
{
    float e = 0;

    for(int i = 0; i <= m; i++)
        for(int j = 0; j <= k; j++)
            for(int s = -z; s <= z; s++)
                for(int u = -p; u <= p; u++)
                    if(s*s + u*u <= p*z)
                        e += pre_m[i][j][u+p][s+z] * ((u*a1/t*n*x_0/m)*(u*a1/t*n*x_0/m) + (s*b1/t*n*y_0/k)*(s*b1/t*n*y_0/k));

    return e;
}

int main()
{
    fill_e_V();

    for(int i = 0; i <= m; i++) //��������� ��� t = 0
        for(int j = 0; j <= k; j++)
            fill_start(i, j);

    go_to_data_in_number(0);


    //----------------------------
    //ofstream out;
    //char filename[25];
    //sprintf(filename, "Energy.txt");
    //out.open(filename);

    //out << energy() << endl;
    //----------------------------



    float a, b;
    for(int l = 1; l <= n; l++)
    {
        for(int j = 0; j <= k; j++) //����� �� x
        {
            float f_x_0 = 0;
            float f_x_m = 0;

            for(int i = 1; i <= m - 1; i++) // x ����������� �������� (0; x_0)
                for(int s = -z; s <= z; s++)
                    for(int u = -p; u <= p; u++)
                        if(s*s + u*u <= p*z) // �������� �� ��, ��� �������� ������ 5*V
                        {
                            a = sign(u)*u*a1;
                            new_m[i][j][u+p][s+z] = a*(pre_m[i-sign(u)][j][u+p][s+z] - pre_m[i][j][u+p][s+z]) + pre_m[i][j][u+p][s+z];

                            if(u == 0) //������������� ��� ������
                                {
                                    new_m[0][j][u+p][s+z] = pre_m[0][j][u+p][s+z];
                                    new_m[m][j][u+p][s+z] = pre_m[m][j][u+p][s+z];
                                }
                            else if(u > 0)
                                {
                                    new_m[0][j][-u+p][s+z] = a*(pre_m[1][j][-u+p][s+z] - pre_m[0][j][-u+p][s+z]) + pre_m[0][j][-u+p][s+z]; // �������� ������� �� ���� ���� �������� � ������, �� �� �������� � ���
                                    new_m[m][j][u+p][s+z] = a*(pre_m[m-1][j][u+p][s+z] - pre_m[m][j][u+p][s+z]) + pre_m[m][j][u+p][s+z];

                                    f_x_0 += a * pre_m[0][j][-u+p][s+z]; // � ������ ��� ������� �� ������
                                    f_x_m += a * pre_m[m][j][u+p][s+z];
                                }
                        }
            for(int s = -z; s <= z; s++)
                for(int u = 1; u <= p; u++) //�������� ���������� �� ������
                    if(s*s + u*u <= p*z)
                        {
                            a = sign(u)*u*a1;
                            float r = exp(-((u*V_x)*(u*V_x) + (s*V_y)*(s*V_y)) / 2) / e_V;

                            new_m[0][j][u+p][s+z] = (1 - a)*pre_m[0][j][u+p][s+z] + r * f_x_0; //������� ����� + ���������� ��������
                            new_m[m][j][-u+p][s+z] = (1 - a)*pre_m[m][j][u+p][s+z] +  r * f_x_m;
                        }

        }
        go_to_pre();


        for(int i = 0; i <= m; i++) //����� �� y
        {
            float f_y_0 = 0;
            float f_y_k = 0;

            for(int j = 1; j <= k - 1; j++) //y ����������� �������� (0; y_0)
                for(int u = -p; u <= p; u++)
                    for(int s = -z; s <= z; s++)
                        if(s*s + u*u <= p*z)
                        {
                            b = sign(s)*s*b1;
                            new_m[i][j][u+p][s+z] = b*(pre_m[i][j-sign(s)][u+p][s+z] - pre_m[i][j][u+p][s+z]) + pre_m[i][j][u+p][s+z];

                            if(s == 0) // ��� ������� ���������
                                {
                                    new_m[i][0][u+p][s+z] = pre_m[i][0][u+p][s+z];
                                    new_m[i][k][u+p][s+z] = pre_m[i][k][u+p][s+z];
                                }
                            else if(s > 0)
                                {
                                    new_m[i][0][u+p][-s+z] = b*(pre_m[i][1][u+p][-s+z] - pre_m[i][0][u+p][-s+z]) + pre_m[i][0][u+p][-s+z];
                                    new_m[i][k][u+p][s+z] = b*(pre_m[i][k-1][u+p][s+z] - pre_m[i][k][u+p][s+z]) + pre_m[i][k][u+p][s+z];


                                    f_y_0 += b * pre_m[i][0][u+p][-s+z]; // � ������ ��� ������� �� ������
                                    f_y_k += b * pre_m[i][k][u+p][s+z];
                                }
                        }

            for(int u = -p; u <= p; u++)
                for(int s = 1; s <= z; s++) //�������� ���������� �� ������
                    if(s*s + u*u <= p*z)
                        {
                            b = sign(s)*s*b1;
                            float r = exp(-((u*V_x)*(u*V_x) + (s*V_y)*(s*V_y)) / 2) / e_V;

                            new_m[i][0][u+p][s+z] = (1 - b)*pre_m[i][0][u+p][s+z] + r * f_y_0; //������� ����� + ���������� ��������
                            new_m[i][k][u+p][-s+z] = (1 - b)*pre_m[i][k][u+p][-s+z] +  r * f_y_k;
                        }
        }
        go_to_pre();

        go_to_data_in_number(l);
        //out << energy() << endl;
    }

    return 0;
}
