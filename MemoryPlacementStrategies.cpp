#include <bits/stdc++.h>
using namespace std;
class Placement
{
private:
    int x;
    int y;
    int *processSize;
    int *blockSize;
    int *allocationArray;
    int *originalSize;

public:
    Placement();
    void restore();
    void input();
    void firstFit();
    void nextFit();
    void bestFit();
    void worstFit();
    void display();
};

Placement::Placement()
{
    originalSize = NULL;
};

void Placement::restore()
{
    for (int i = 0; i < x; i++)
    {
        allocationArray[i] = -1;
    }
    for (int i = 0; i < y; i++)
    {
        blockSize[i] = originalSize[i];
    }
}

void Placement::input()
{
    cout << "\nEnter the number of process :-> " << endl;
    cin >> x;
    processSize = new int[x];
    allocationArray = new int[x];
    cout << "\nEnter the number of blocks :-> " << endl;
    cin >> y;
    blockSize = new int[y];
    originalSize = new int[y];
    for (int i = 0; i < x; i++)
    {
        int temp;
        cout << "\nEnter the size of the process :-> " << endl;
        cin >> temp;
        processSize[i] = temp;
    }
    for (int i = 0; i < y; i++)
    {
        int temp;
        cout << "\nEnter the size of the block :-> " << endl;
        cin >> temp;
        blockSize[i] = temp;
        originalSize[i] = temp;
    }
}

void Placement::firstFit()
{
    restore();
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if (processSize[i] <= blockSize[j])
            {
                allocationArray[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }
}

void Placement::nextFit()
{
    restore();
    int j = 0;
    int temp = y - 1;
    for (int i = 0; i < x; i++)
    {
        while (j < y)
        {
            if (processSize[i] <= blockSize[j])
            {
                allocationArray[i] = j;
                blockSize[j] -= processSize[i];
                temp = (j - 1) % y;
                break;
            }
            if (temp == j)
            {
                temp = (j - 1) % y;
                break;
            }
            j = (j + 1) % y;
        }
    }
}

void Placement::bestFit()
{
    restore();
    for (int i = 0; i < x; i++)
    {
        int ind = -1;
        for (int j = 0; j < y; j++)
        {
            if (processSize[i] <= blockSize[j])
            {
                if (ind == -1)
                {
                    ind = j;
                }
                else if (blockSize[ind] > blockSize[j])
                {
                    ind = j;
                }
            }
        }
        if (ind != -1)
        {
            allocationArray[i] = ind;
            blockSize[ind] -= processSize[i];
        }
    }
}

void Placement::worstFit()
{
    restore();
    for (int i = 0; i < x; i++)
    {
        int ind = -1;
        for (int j = 0; j < y; j++)
        {
            if (processSize[i] <= blockSize[j])
            {
                if (ind == -1)
                {
                    ind = j;
                }
                else if (blockSize[ind] < blockSize[j])
                {
                    ind = j;
                }
            }
        }
        if (ind != -1)
        {
            allocationArray[i] = ind;
            blockSize[ind] -= processSize[i];
        }
    }
}

void Placement::display()
{
    cout << "\nProcess\t\t\tProcess_Size\t\t\t\tBlock_Number" << endl;
    for (int i = 0; i < x; i++)
    {
        cout << i + 1 << "\t\t\t\t" << processSize[i] << "\t\t\t\t";
        if (allocationArray[i] != -1)
        {
            cout << allocationArray[i] + 1;
        }
        else
        {
            cout << "Not Allocated";
        }
        cout << "\n";
    }
}

int main()
{
    int chc = 0;
    int flag = 1;
    Placement obj;
    while (flag == 1)
    {
        cout << "Menu" << endl;
        cout << "1. To Enter the information of Block and Processes" << endl;
        cout << "2. To perform Memory Management with FirstFit" << endl;
        cout << "3. To perform Memory Management with NextFit" << endl;
        cout << "4. To perform Memory Management with BestFit" << endl;
        cout << "5. To perform Memory Management with WorstFit" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice:-> " << endl;
        cin >> chc;
        switch (chc)
        {
        case 1:
            obj.input();
            break;
        case 2:
            obj.firstFit();
            obj.display();
            break;
        case 3:
            obj.nextFit();
            obj.display();
            break;
        case 4:
            obj.bestFit();
            obj.display();
            break;
        case 5:
            obj.worstFit();
            obj.display();
            break;
        case 6:
            return 0;
            break;
        default:
            cout << "Enter Valid Choice" << endl;
            break;
        }
        cout << "Do you want to continue?(1/0):" << endl;
        cin >> flag;
    }
    return 0;
}
