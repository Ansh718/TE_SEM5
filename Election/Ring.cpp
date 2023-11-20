#include <bits/stdc++.h>
using namespace std;
class Ring
{
private:
    int total;
    vector<bool> state;

public:
    Ring(int);
    void up(int);
    void down(int);
    void election(int);
};

Ring::Ring(int total)
{
    this->total = total;
    state.resize(total, true);
}

void Ring::up(int pos)
{
    state[pos - 1] = true;
}

void Ring::down(int pos)
{
    state[pos - 1] = false;
}

void Ring::election(int pos)
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
    int ptr = pos % total;
    int last = pos - 1;
    while (ptr != pos - 1)
    {
        if (state[ptr])
        {
            cout << "\nNode " << last + 1 << " sends election message to Node " << ptr + 1 << endl;
            last = ptr;
        }
        ptr = (ptr + 1) % total;
    }
    cout << "\nNode " << last + 1 << " sends election message to Node " << ptr + 1 << endl;
    int maxp = -1;
    for (int i = 0; i < total; i++)
    {
        if (state[i])
        {
            maxp = i;
        }
    }
    cout << "\nElection is won by :-> Node " << maxp + 1 << endl;
    cout << "\nNode " << maxp + 1 << " informs everyone that it is the new coordinator." << endl;
}

int main()
{
    Ring r(5);
    r.down(4);
    r.down(2);
    r.election(1);
    return 0;
}