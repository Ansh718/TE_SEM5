#include <bits/stdc++.h>
using namespace std;
class Bully
{
private:
    int total;
    vector<bool> state;

public:
    Bully(int);
    void up(int);
    void down(int);
    void election(int);
};

Bully::Bully(int total)
{
    this->total = total;
    state.resize(total, true);
}

void Bully::up(int pos)
{
    state[pos - 1] = true;
}

void Bully::down(int pos)
{
    state[pos - 1] = false;
}

void Bully::election(int pos)
{
    if (pos > total)
    {
        cout << "\nEnter valid node Id.\n";
    }
    if (!state[pos - 1])
    {
        cout << "\nThe node " << pos << " cannot start the election as it is dead.\n";
    }
    cout << "\nElection started by node " << pos << "." << endl;
    for (int i = pos; i < total; i++)
    {
        cout << "\nNode " << pos << " sends a message to " << i + 1 << "." << endl;
    }
    for (int i = pos; i < total; i++)
    {
        if (state[i])
        {
            cout << "\nNode " << i + 1 << " responds OK to Node " << pos << endl;
        }
    }
    for (int i = pos; i < total; i++)
    {
        if (state[i])
        {
            election(i + 1);
            return;
        }
    }
    int maxp = -1;
    for (int i = 0; i < total; i++)
    {
        if (state[i])
        {
            maxp = i;
        }
    }
    if (maxp != -1)
    {
        cout << "\nElection is won by: Node " << maxp + 1 << "." << endl;
        cout << "\nNode " << maxp + 1 << " informs everyone that it is the new coordinator." << endl;
    }
}

int main()
{
    Bully b(8);
    b.down(5);
    b.election(3);
    return 0;
}
