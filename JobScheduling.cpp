#include <bits/stdc++.h>
using namespace std;
class Job
{
private:
    int iD;
    int arrival;
    int burst;
    int completionTime;
    int turnAroundTime;
    int waiting;
    int priority;

public:
    Job();
    void accept();
    void acceptForPriority();
    friend class Scheduler;
};

Job::Job()
{
    iD = arrival = burst = completionTime = turnAroundTime = waiting = priority = 0;
}

void Job::accept()
{
    cout << "Enter process ID :->" << endl;
    cin >> iD;
    cout << "Enter arrival time :->" << endl;
    cin >> arrival;
    cout << "Enter burst time :->" << endl;
    cin >> burst;
}

void Job::acceptForPriority()
{
    cout << "Enter process ID :->" << endl;
    cin >> iD;
    cout << "Enter arrival time :->" << endl;
    cin >> arrival;
    cout << "Enter burst time :->" << endl;
    cin >> burst;
    cout << "Enter priority :->" << endl;
    cin >> priority;
}

class Scheduler
{
private:
    vector<Job> jobs;
    int n;
    int *remainingTime;

public:
    Scheduler();
    void get();
    void getForPriority();
    static bool compareArrivalTime(Job x, Job y);
    void display();
    void displayForPriority();
    void firstComeFirstServe();
    void shortestJobFirst();
    void roundRobin(int);
    void priority();
};

Scheduler::Scheduler()
{
    cout << "Enter number of processes :->" << endl;
    cin >> n;
}

void Scheduler::get()
{
    for (int i = 0; i < n; i++)
    {
        Job obj;
        cout << "\n"
             << endl;
        obj.accept();
        jobs.push_back(obj);
    }
}

void Scheduler::getForPriority()
{
    for (int i = 0; i < n; i++)
    {
        Job obj;
        cout << "\n"
             << endl;
        obj.acceptForPriority();
        jobs.push_back(obj);
    }
}

bool Scheduler::compareArrivalTime(Job x, Job y)
{
    return x.arrival < y.arrival;
}

void Scheduler::display()
{
    cout << "\n-----------------------------------------------------------------------------------------------";
    cout << "\nProcess ID | Arrival Time | Burst Time | Completion Time | Turn-Around Time | Wait Time ";
    cout << "\n-----------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < n; i++)
    {
        cout << "    " << jobs[i].iD << "\t\t" << jobs[i].arrival << "\t\t" << jobs[i].burst << "\t\t" << jobs[i].completionTime << "\t\t" << jobs[i].turnAroundTime << "\t\t" << jobs[i].waiting << "\n"
             << endl;
    }
    float avg_WaitingTime = 0;
    float avg_TurnAroundTime = 0;
    for (int i = 0; i < n; i++)
    {
        avg_WaitingTime += jobs[i].waiting;
        avg_TurnAroundTime += jobs[i].turnAroundTime;
    }
    avg_TurnAroundTime /= n;
    avg_WaitingTime /= n;
    cout << "Average waiting time :->" << avg_WaitingTime << endl;
    cout << "Average turn around time :->" << avg_TurnAroundTime << endl;
    cout << "------------------------------------------------------------------------------------------------\n";
}

void Scheduler::displayForPriority()
{
    cout << "\n-------------------------------------------------------------------------------------------------------";
    cout << "\nProcess ID | Arrival Time | Burst Time | Completion Time | Turn-Around Time | Wait Time | Priority";
    cout << "\n-------------------------------------------------------------------------------------------------------\n";
    float avg_WaitingTime = 0;
    float avg_TurnAroundTime = 0;
    for (int i = 0; i < n; i++)
    {
        avg_WaitingTime += jobs[i].waiting;
        avg_TurnAroundTime += jobs[i].turnAroundTime;
    }
    avg_TurnAroundTime /= n;
    avg_WaitingTime /= n;
    cout << "Average waiting time :->" << avg_WaitingTime << endl;
    cout << "Average turn around time :->" << avg_TurnAroundTime << endl;
    cout << "------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < n; i++)
    {
        cout << "    " << jobs[i].iD << "\t\t" << jobs[i].arrival << "\t\t" << jobs[i].burst << "\t\t" << jobs[i].completionTime << "\t\t" << jobs[i].turnAroundTime << "\t\t" << jobs[i].waiting << "\t\t" << jobs[i].priority << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------------\n";
}

void Scheduler::firstComeFirstServe()
{
    sort(jobs.begin(), jobs.end(), compareArrivalTime);
    jobs[0].completionTime = jobs[0].burst;
    jobs[0].turnAroundTime = jobs[0].burst;
    jobs[0].waiting = 0;
    int temp = jobs[0].burst;
    for (int i = 1; i < n; i++)
    {
        temp += jobs[i].burst;
        jobs[i].completionTime = temp;
    }
    for (int i = 0; i < n; i++)
    {
        jobs[i].turnAroundTime = jobs[i].completionTime - jobs[i].arrival;
        jobs[i].waiting = jobs[i].turnAroundTime - jobs[i].burst;
    }
}

void Scheduler::shortestJobFirst()
{
    remainingTime = new int[n];
    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = jobs[i].burst;
    }
    int currentTime = 0;
    int completedTasks = 0;
    int shortestTask = 0;
    int minBurst = INT_MAX;
    bool active = false;
    while (completedTasks != n)
    {
        for (int j = 0; j < n; j++)
        {
            if (jobs[j].arrival <= currentTime && jobs[j].burst < minBurst && remainingTime[j] > 0)
            {
                shortestTask = j;
                minBurst = jobs[j].burst;
                active = true;
            }
        }
        if (active == false)
        {
            currentTime++;
            continue;
        }
        minBurst = --remainingTime[shortestTask];
        if (minBurst == 0)
        {
            minBurst = INT_MAX;
        }
        if (remainingTime[shortestTask] == 0)
        {
            completedTasks++;
            active = false;
            jobs[shortestTask].completionTime = currentTime + 1;
            jobs[shortestTask].turnAroundTime = jobs[shortestTask].completionTime - jobs[shortestTask].arrival;
            jobs[shortestTask].waiting = jobs[shortestTask].turnAroundTime - jobs[shortestTask].burst;
            if (jobs[shortestTask].waiting < 0)
            {
                jobs[shortestTask].waiting = 0;
            }
        }
        currentTime++;
    }
}

void Scheduler::roundRobin(int timeQuantum)
{
    queue<int> readyqueue;
    int currentTime = 0;
    int completedTasks = 0;
    int current;
    remainingTime = new int[n];
    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = jobs[i].burst;
    }
    for (int i = 0; i < n; i++)
    {
        if (jobs[i].arrival <= currentTime)
        {
            readyqueue.push(i);
        }
    }
    while (completedTasks != n)
    {
        current = readyqueue.front();
        readyqueue.pop();
        int counter = 0;
        while (counter != timeQuantum && counter != remainingTime[current])
        {
            currentTime++;
            counter++;
            for (int i = 0; i < n; i++)
            {
                if (jobs[i].arrival == currentTime)
                {
                    readyqueue.push(i);
                }
            }
        }
        remainingTime[current] -= counter;
        if (remainingTime[current] == 0)
        {
            jobs[current].completionTime = currentTime;
            jobs[current].turnAroundTime = jobs[current].completionTime - jobs[current].arrival;
            jobs[current].waiting = jobs[current].turnAroundTime - jobs[current].burst;
            completedTasks++;
        }
        else
        {
            readyqueue.push(current);
        }
    }
}

void Scheduler::priority()
{
    remainingTime = new int[n];
    for (int i = 0; i < n; i++)
    {
        remainingTime[i] = jobs[i].burst;
    }
    int currentTime = 0;
    int completedTasks = 0;
    int completionTime = 0;
    int highPriority = 0;
    int remainingBurst = 0;
    int maxPriority = INT_MAX;
    bool taskActive = false;
    while (completedTasks != n)
    {
        for (int i = 0; i < n; i++)
        {
            if (jobs[i].arrival <= currentTime && remainingTime[i] > 0 && jobs[i].priority < maxPriority)
            {
                highPriority = i;
                maxPriority = jobs[i].priority;
                taskActive = true;
            }
        }
        if (!taskActive)
        {
            currentTime++;
            continue;
        }
        currentTime += jobs[highPriority].burst;
        remainingTime[highPriority] = 0;
        maxPriority = INT_MAX;
        taskActive = false;
        completedTasks++;
        jobs[highPriority].completionTime = currentTime;
        jobs[highPriority].turnAroundTime = jobs[highPriority].completionTime - jobs[highPriority].arrival;
        jobs[highPriority].waiting = jobs[highPriority].turnAroundTime - jobs[highPriority].burst;
    }
}

int main()
{
    Scheduler obj;
    int chc;
    int timeQuantum = 0;
    int flag = 1;
    while (flag == 1)
    {
        cout << "\n--Menu--\n";
        cout << "1.FCFS" << endl;
        cout << "2.SJF" << endl;
        cout << "3.Round Robin" << endl;
        cout << "4.Priority" << endl;
        cin >> chc;
        switch (chc)
        {
        case 1:
            obj.get();
            obj.firstComeFirstServe();
            obj.display();
            break;
        case 2:
            obj.get();
            obj.shortestJobFirst();
            obj.display();
            break;

        case 3:
            cout << "\n Enter time quantum:->\n";
            cin >> timeQuantum;
            obj.get();
            obj.roundRobin(timeQuantum);
            obj.display();
            break;

        case 4:
            obj.getForPriority();
            obj.priority();
            obj.displayForPriority();
            break;

        default:
            cout << "Enter valid choice" << endl;
            break;
        }
        cout << "\nDo you want to continue?(1/0)\n";
        cin >> flag;
    }
    return 0;
}