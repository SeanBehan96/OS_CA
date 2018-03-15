using namespace std;
#include <iostream>
#include <fstream>
#include<string>
#include <algorithm>
#include<iterator>
#include<sstream>
#include<vector>
#include<istream>
#include<iomanip>


class Job
{
public:


	string name;
	int ArrivalTime, duration;

	friend std::istream& operator >> (std::istream& is, Job& Job)
	{
		std::string line;
		while (std::getline(is, line))
		{
		std:istringstream iss(line);

			if (!(iss >> Job.name >> Job.ArrivalTime >> Job.duration))
				break;

			return is;
		}
	}

};

void FIFO(std::vector<Job> jobs, int count);
void SJF(std::vector<Job> jobs, int count);
void STCF(std::vector<Job> jobs, int count, std::vector<string> started);
void RR(std::vector<Job> jobs, int count, int timeSlice, int total);

int main()
{
	std::ifstream input_file("C:\\Users\\Sean\\Desktop\\jobs.txt");

	std::vector<Job> jobs;
	Job job;
	string name;
	int ArrivalTime, duration;
	while (input_file >> name >> ArrivalTime >> duration)
	{
		job.name = name;
		job.ArrivalTime = ArrivalTime;
		job.duration = duration;
		jobs.push_back(job);
	}

	std::vector<string> started;

	int count = 0;
	int total = 0;


	sort(jobs.begin(), jobs.end(), [](const Job& lhs, const Job& rhs)
	{
		return lhs.ArrivalTime < rhs.ArrivalTime;
	}
	);

	for (Job e : jobs)
	{
		{
			total = total + e.duration;
		}
	}
	total = total-1;
	int timeSlice = 0;
	int timeSlice2 = 0;

	while (timeSlice == 0)
	{
		for (int i = 1;i <= total; i++)
		{
			if (total % i == 0 && i != 1)
			{
				timeSlice = i;
				break;
			}
		}

		while (timeSlice2 == 0)
		{
			for (int i = 1;i <= total; i++)
			{
				if (total % i == 0 && i != 1 && i != timeSlice)
				{
					timeSlice2 = i;
					break;
				}
			}
		}
	}

	vector<string> RRname;
	vector<int> RRduration;
	for (Job e : jobs)
	{
		RRname.push_back(e.name);
		RRduration.push_back(e.duration);
	}
	int RRcount = 1;
	int RRjobs = jobs.size() - 1;
	int RRposition = 0;


	for (int i = 0; i < jobs.size(); i++)
	{
		Job j = jobs[i];
		cout << j.name << " " << j.ArrivalTime << " " << j.duration << endl;
	}

	total = total + jobs[0].ArrivalTime;
	cout << left << setw(4) << "T" << setw(10) << "FIFO" << setw(15) << "RR" << setw(15) << "RR2" << setw(15) << "SJF" << timeSlice << " " << timeSlice2 << " " << total << endl;
	while (count != total + jobs.size())
	{
		cout << left << setw(4) << count;
		FIFO(jobs, count);
		RR(jobs, count, timeSlice, total);
		RR(jobs, count, timeSlice2, total);
		SJF(jobs, count);
		//STCF(jobs, count,started);
		count++;
	}
	cout << endl << "Simulation complete" << endl << endl;
	system("pause");
}

void FIFO(std::vector<Job> jobs, int count)
{
	for (int i = 0; i < jobs.size(); i++)
	{
		if (i != 0)
		{
			if (jobs[i].ArrivalTime <= jobs[i - 1].duration)
			{
				jobs[i].ArrivalTime = jobs[i - 1].duration + 1;
			}
			jobs[i].duration = jobs[i].duration + jobs[i].ArrivalTime;
		}
		else
			jobs[i].duration = jobs[i].duration + jobs[i].ArrivalTime;
	}
	int none = 0;
	for (Job e : jobs)
	{
		if (count == e.ArrivalTime)
		{
			cout << e.name << left << setw(15) << " has started ";
			none++;
		}
		else if (count == e.duration)
		{
			cout << e.name << left << setw(15) << " has Finished ";
			none++;
		}
		else if (count<e.duration&&count>e.ArrivalTime)
		{
			cout << left << setw(15) << e.name;
			none++;
		}
		if (none == 0 && e.name == jobs[jobs.size() - 1].name)
		{
			cout << left << setw(15) << "NIL";
		}
	}
}


void SJF(std::vector<Job> jobs, int count)
{
	sort(jobs.begin(), jobs.end(), [](const Job& lhs, const Job& rhs)
	{
		return lhs.duration < rhs.duration;
	}
	);

	for (int i = 0; i < jobs.size(); i++)
	{
		if (i == 0)
		{
			jobs[i].ArrivalTime = 0;
			jobs[i].duration = jobs[i].duration - 1;
		}
		else
		{
			jobs[i].ArrivalTime = jobs[i - 1].duration + 1;
			jobs[i].duration = jobs[i].ArrivalTime + jobs[i].duration;
		}
	}
	int none = 0;
	for (Job e : jobs)
	{
		if (count == e.ArrivalTime)
		{
			cout << e.name << left << setw(15) << " has started " << endl;
			none++;
		}
		else if (count == e.duration)
		{
			cout << e.name << left << setw(15) << " has Finished " << endl;
			none++;
		}
		else if (count<e.duration&&count>e.ArrivalTime)
		{
			cout << left << setw(15) << e.name << endl;
			none++;
		}
		if (none == 0 && e.name == jobs[jobs.size() - 1].name)
		{
			cout << left << setw(15) << "NIL" << endl;
		}
	}
}

void STCF(std::vector<Job> jobs, int count, std::vector<string> started)
{
	sort(jobs.begin(), jobs.end(), [](const Job& lhs, const Job& rhs)
	{
		return lhs.ArrivalTime < rhs.ArrivalTime;
	});
	int orig = jobs.size() - 1;
std:vector<int> endTimes;
	for (Job e : jobs)
	{
		int end = e.ArrivalTime + e.duration;
		endTimes.push_back(end);
	}
	for (int i = 0; i <= orig; i++)
	{
		if (i != 0 && jobs[i].ArrivalTime < endTimes[i - 1])
		{
			if (jobs[i].duration > endTimes[i - 1] - jobs[i].ArrivalTime)
			{
				jobs[i].ArrivalTime = endTimes[i - 1] + 1;
				endTimes[i] = jobs[i].ArrivalTime + jobs[i].duration;
			}
			if (jobs[i].duration < endTimes[i - 1] - jobs[i].ArrivalTime)
			{
				endTimes[i - 1] = jobs[i].ArrivalTime - 1;
				Job j;
				j.duration = jobs[i - 1].duration - (endTimes[i - 1] - jobs[i - 1].ArrivalTime);
				j.name = jobs[i - 1].name;
				j.ArrivalTime = endTimes[i] + 1;
				jobs.push_back(j);
			}
		}

	}
	for (Job e : jobs)
	{
		cout << e.name << " " << e.ArrivalTime << " " << e.duration << " " << " " << endl;
	}

}


void RR(std::vector<Job> jobs, int count, int timeSlice, int total)
{
	std::vector<Job> RR;
	int full = 0;
	int place = 0;
	int allEmpty = 0;
	while (allEmpty < jobs.size() - 1)
	{
		if (jobs[place].duration == 0)
		{
			if (place = jobs.size() - 1)
			{
				place = 0;
			}
			else
				place++;

			allEmpty++;
		}
		else {
			Job j;
			j.name = jobs[place].name;
			if (RR.size() == 0)
			{
				j.ArrivalTime = 0;
			}
			else
				j.ArrivalTime = RR[RR.size() - 1].duration + 1;

			if (jobs[place].duration >= timeSlice)
			{
				j.duration = j.ArrivalTime + timeSlice;
				jobs[place].duration = jobs[place].duration - timeSlice;
				if (place < jobs.size() - 2)
				{
					place++;
				}
				else
					place = 0;
			}
			else if (jobs[place].duration < timeSlice)
			{
				j.duration = j.ArrivalTime + jobs[place].duration;
				jobs[place].duration = 0;
			}
			RR.push_back(j);
			allEmpty = 0;
		}

	}

	int none = 0;
	for (int r = 0; r < RR.size(); r++)
	{
		if (count == RR[r].ArrivalTime)
		{
			cout << RR[r].name << left << setw(15) << " has started ";
			none++;
		}
		else if (count == RR[r].duration)
		{
			cout << RR[r].name << left << setw(15) << " has Finished ";
			none++;
		}
		else if (count<RR[r].duration&&count>RR[r].ArrivalTime)
		{
			cout << left << setw(15) << RR[r].name;
			none++;
		}
		if (none == 0 && r == RR.size() - 1)
		{
			cout << left << setw(15) << "NIL";
		}
	}

}

void startup()
{

}
