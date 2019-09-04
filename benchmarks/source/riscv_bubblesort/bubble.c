
int main()
{
    int a[8];
    a[0] = 563;
    a[1] = 892;
    a[2] = 8910;
    a[3] = 76711;
    a[4] = 61;
    a[5] = 1523;
    a[6] = 901;
    a[7] = 162;

    int tmp = 0;

    for(int i = 0; i < 7; ++i)
    {
        for(int j = 0; j+i < 7; ++j)
        {
            if(a[j] > a[j+1])
            {
                tmp = a[j];
                a[j] = a[j+1];
                a[j+1] = tmp;
            }
        }
    }

    return 0;
}
