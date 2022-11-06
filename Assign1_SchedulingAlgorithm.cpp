#include <bits/stdc++.h>
using namespace std;

class Process
{
    int id;
    int at;      // arrival time
    int bt, btc; // burst time and burst time copy
    int pr;
    int wt;
    int tat;

public:
    Process()
    {
        cout << "New Process entry:\n";
         cout << "Enter Process id: ";
        cin >> id;
        cout << "Enter arrival time: ";
        cin >> at;
        cout << "Enter burst time: ";
        cin >> bt;

        cout << "Enter priority of process: ";
        cin >> pr;
    }
    friend class SchedulingAlgo;
};

class SchedulingAlgo
{
    int n;
    Process *proc;

    void FCFS_WT(int comp_time[]);
    void SJF_WT(int wt[]);
    void PR_WT(int comp_time[]);
    void RR_WT(int wt[], int comp_time[]);

public:
    // Constructor
    SchedulingAlgo(int n)
    {
        this->n = n;
        proc = new Process[n];
    }

    void FCFS_avg();
    void SJF_avg();
    void PR_avg();
    void RR_avg();
};

void SchedulingAlgo ::FCFS_WT(int comp_time[])
{
    sort(proc, proc + n, [](Process &a, Process &b)
         { return a.at < b.at; });
    // Finding the completion time
    int current_time = 0;
    current_time = comp_time[0] = proc[0].at + proc[0].bt;
    for (int i = 1; i < n; i++)
    {
        if (proc[i].at > current_time)
        {
            current_time = proc[i].at;
        }
        comp_time[i] = current_time + proc[i].bt;
        current_time = comp_time[i];
    }
}

void SchedulingAlgo ::FCFS_avg()
{
    int comp_time[n] = {0};
    int wt[n];
    int tat[n];
    int sum_wt = 0;
    int sum_tat = 0;

    FCFS_WT(comp_time);
    // calculating TAT
    for (int i = 0; i < n; i++)
    {
        tat[i] = comp_time[i] - proc[i].at;
    }
    // calculating WT
    for (int i = 0; i < n; i++)
    {
        wt[i] = tat[i] - proc[i].bt;
    }
    // AVG
    for (int i = 0; i < n; i++)
    {
        sum_wt += wt[i];
        sum_tat += tat[i];
    }
    // cout << "completion TIME \n";
    // for (int i = 0; i < n; i++)
    // {
    //     cout << comp_time[i] << "-->" << i << endl;
    // }
    // cout << "WAITIG TIME \n";
    // for (int i = 0; i < n; i++)
    // {
    //     cout << wt[i] << "-->" << i << endl;
    // }
    // cout << "tat TIME \n";
    // for (int i = 0; i < n; i++)
    // {
    //     cout << tat[i] << "-->" << i << endl;
    // }
    cout << "Average wating time of FCFS scheduling algorithm is: " << sum_wt / (float)n << endl;
    cout << "Average turnaround time of FCFS scheduling algorithm is: " << sum_tat / (float)n << endl;
}

void SchedulingAlgo ::SJF_WT(int wt[])
{
    int nopCompleted = 0;
    int rem_bt[n];

    // initialising remaining burst time array
    for (int i = 0; i < n; i++)
    {
        rem_bt[i] = proc[i].bt;
    }

    int timeInst = 0;
    int minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;

    while (nopCompleted != n)
    {
        // Find process with minimum
        // remaining time among the
        // processes that arrives till the
        // current time`
        for (int j = 0; j < n; j++)
        {
            if ((proc[j].at <= timeInst) &&
                (rem_bt[j] < minm) && rem_bt[j] > 0)
            {
                minm = rem_bt[j];
                shortest = j;
                check = true;
            }
        }

        if (check == false)
        {
            timeInst++;
            continue;
        }

        // Reduce remaining time by one
        rem_bt[shortest]--;

        // Update minimum
        minm = rem_bt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        // If a process gets completely
        // executed
        if (rem_bt[shortest] == 0)
        {

            // Increment complete
            nopCompleted++;
            check = false;

            // Find finish time of current
            // process
            finish_time = timeInst + 1;

            // Calculate waiting time
            wt[shortest] = finish_time -
                           proc[shortest].bt -
                           proc[shortest].at;

            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }
        // increment time instance
        timeInst++;
    }
}

void SchedulingAlgo ::SJF_avg()
{
    int wt[n];
    int tat[n];
    int sum_wt = 0;
    int sum_tat = 0;

    SJF_WT(wt);
    // calculating TAT
    for (int i = 0; i < n; i++)
    {
        tat[i] = wt[i] + proc[i].bt;
    }

    // AVG
    for (int i = 0; i < n; i++)
    {
        sum_wt += wt[i];
        sum_tat += tat[i];
    }
    cout << "Average wating time of SJF scheduling algorithm is: " << sum_wt / (float)n << endl;
    cout << "Average turnaround time of SJF scheduling algorithm is: " << sum_tat / (float)n << endl;
}

void SchedulingAlgo ::PR_WT(int comp_time[])
{
    sort(proc, proc + n, [](Process &a, Process &b)
         {  if(a.at == b.at){
            return a.pr < b.pr;
         }
            return a.at < b.at; });
    int completed = 1;
    int index = 0;
    int p = 0;
    bool visited[n] = {0};
    int current_time = 0;
    vector<Process> arrived;
    current_time = comp_time[proc[0].id - 1] = proc[0].at + proc[0].bt;

    // visited[proc[0].id - 1] = 1;
    while (completed < n)
    {
        for (int i = 1; i < n; i++)
        {
            if (visited[proc[i].id - 1] == 0 && proc[i].at <= current_time)
            {
                visited[proc[i].id - 1] = 1;
                arrived.push_back(proc[i]);
            }
        }
        if (arrived.size() > 1)
        {
            sort(arrived.begin(), arrived.end(), [](Process &a, Process &b)
                 { return a.pr < b.pr; });
        }
        if (arrived.size() > 0)
        {
            Process temp = arrived[0];
            auto it = arrived.begin();
            arrived.erase(it);
            if (temp.at > current_time)
            {
                current_time = temp.at;
            }
            current_time += temp.bt;
            visited[temp.id - 1] = 1;
            comp_time[temp.id - 1] = current_time;
            completed++;
        }
        else
        {
            Process nextprocess;
            bool flag = false;
            for (int i = 1; i < n; i++)
            {
                if (visited[proc[i].id - 1] == 0)
                {
                    nextprocess = proc[i];
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                arrived.push_back(nextprocess);
                visited[nextprocess.id - 1] = 1;
                current_time = nextprocess.at;
                // comp_time[]
            }
        }
    }
}

void SchedulingAlgo ::PR_avg()
{
    int wt[n];
    int tat[n];
    int sum_wt = 0;
    int sum_tat = 0;
    int comp_time[n] = {0};
    PR_WT(comp_time);

    // calculating TAT
    for (int i = 0; i < n; i++)
    {
        tat[i] = comp_time[proc[i].id - 1] - proc[i].at;
    }

    // Calculation WT
    for (int i = 0; i < n; i++)
    {
        wt[i] = tat[i] - proc[i].bt;
    }
    // AVG
    for (int i = 0; i < n; i++)
    {
        sum_wt += wt[i];
        sum_tat += tat[i];
    }
    cout << "Average wating time of Priority scheduling algorithm is: " << sum_wt / (float)n << endl;
    cout << "Average turnaround time of Priority scheduling algorithm is: " << sum_tat / (float)n << endl;
}

void SchedulingAlgo ::RR_WT(int wt[], int comp_time[])
{
    int r_bt[n];
    // initialising remaining burst time array
    for (int i = 0; i < n; i++)
    {
        r_bt[i] = proc[i].bt;
    }

    sort(proc, proc + n, [](Process &a, Process &b)
         { return a.at < b.at; });

    queue<Process> q;
    int quantam;
    cout << "Enter the quantam: ";
    cin >> quantam;

    q.push(proc[0]);
    int time = proc[0].at;
    int complete = 0;
    int p = 0;
    while (complete != n && !q.empty())

    {
        Process temp = q.front();
        if (r_bt[temp.id - 1] <= quantam && r_bt[temp.id - 1] != 0)
        {

            time += r_bt[temp.id - 1];

            r_bt[temp.id - 1] = 0;

            q.pop();
            complete++;
            comp_time[temp.id - 1] = time;
        }
        else /* (r_bt[temp.pno - 1] > quantam) */
        {
            time += quantam;

            r_bt[temp.id - 1] -= quantam;
        }
        int i;
        for (i = p + 1; i < n; i++)
        {
            if (proc[i].at <= time)
            {
                p++;
                q.push(proc[i]);
            }
            else
                break;
        }
        if (r_bt[temp.id - 1] != 0)
        {
            q.push(temp);
            q.pop();
        }
        if (q.empty() && complete != n)
        {
            p++;
            q.push(proc[p]);
            time = proc[p].at;
        }
    }
}

void SchedulingAlgo ::RR_avg()
{
    int wt[n];
    int tat[n];
    int sum_wt = 0;
    int sum_tat = 0;
    int comp_time[n];
    RR_WT(wt, comp_time);
    // calculating TAT
    for (int i = 0; i < n; i++)
    {

        tat[i] = comp_time[proc[i].id - 1] - proc[i].at;
    }
    // calculating WT
    for (int i = 0; i < n; i++)
    {
        wt[i] = tat[i] - proc[i].bt;
    }

    // AVG
    for (int i = 0; i < n; i++)
    {
        sum_wt += wt[i];
        sum_tat += tat[i];
    }
    // cout << "completion TIME IS\n";
    // for (int i = 0; i < n; i++)
    // {
    //     cout << "completion time of " << i + 1 << " : " << comp_time[i] << endl;
    // }
    // cout << "waiting TIME IS\n";
    // for (int i = 0; i < n; i++)
    // {
    //     cout << "waiting time of " << i << " : " << wt[i] << endl;
    // }
    // cout << "TURNAROUND TIME IS\n";
    // for (int i = 0; i < n; i++)
    // {
    //     cout << "turnaround time of " << i << " : " << tat[i] << endl;
    // }
    cout << "Average wating time of Roundrobin scheduling algorithm is: " << sum_wt / (float)n << endl;
    cout << "Average turnaround time of Roundrobin scheduling algorithm is: " << sum_tat / (float)n << endl;
}

int main()
{
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    SchedulingAlgo obj(n);
    obj.FCFS_avg();
    obj.SJF_avg();
    obj.PR_avg();
    obj.RR_avg();
    return 0;
}

// Test Case :-
// 1 0 3 5
// 2 2 6 2
// 3 4 4 4
// 4 6 5 3
// 5 8 2 1