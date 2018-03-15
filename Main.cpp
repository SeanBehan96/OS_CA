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

std::vector<float> FIFO(std::vector<Job> jobs, int count);
std::vector<float> SJF(std::vector<Job> jobs, int count);
std::vector<Job> STCF(std::vector<Job> jobs, int count);
std::vector<float> RR(std::vector<Job> jobs, int count, int timeSlice, int total);

int main()
{
	std::ifstream input_file("/home/D00163396/ca1/Jobs.txt");

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

	std::vector<Job> stcf = jobs;

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
			for (int i = (timeSlice + 1);i <= total; i++)
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

	std::vector<float> fifoAv;
	std::vector<float> sjfAv;
	std::vector<float> RRAv;
	std::vector<float> RRAv2;
	for (int i = 0; i < jobs.size(); i++)
	{
		Job j = jobs[i];
		cout << j.name << " " << j.ArrivalTime << " " << j.duration << endl;
	}
	cout << left << setw(4) << "T" << setw(10) << "FIFO" << setw(15) << "RR" << setw(15) << "RR2" << setw(15) << "SJF" << timeSlice << " " << timeSlice2 << " " << total << endl;
	while (count != total + jobs.size())
	{
		cout << left << setw(4) << count;
		fifoAv = FIFO(jobs, count);
		RRAv=RR(jobs, count, timeSlice, total);
		RRAv2=RR(jobs, count, timeSlice2, total);
		sjfAv = SJF(jobs, count);
		//stcf=STCF(stcf, count);
		count++;
	}
	cout << endl << "Simulation complete" << endl << endl;
	cout << "Job AVG_turnabout AVG_response" << endl;
	for (int i = 0; i < fifoAv.size(); i++)
	{

		cout << "FIFO  " << fifoAv[i] << " ";
		if (i = fifoAv.size() - 1)
		{
			cout << fifoAv[i] << endl;
		}

	}
	for (int i = 0; i < sjfAv.size(); i++)
	{

		cout << "SJF  " << sjfAv[i] << " ";
		if (i = sjfAv.size() - 1)
		{
			cout << sjfAv[i] << endl;
		}

	}
	for (int i = 0; i < RRAv.size(); i++)
	{

		cout << "RR1  " << RRAv[i] << " ";
		if (i = RRAv.size() - 1)
		{
			cout << RRAv[i] << endl;
		}

	}
	for (int i = 0; i < RRAv2.size(); i++)
	{

		cout << "RR2  " << RRAv2[i] << " ";
		if (i = RRAv2.size() - 1)
		{
			cout << RRAv2[i] << endl;
		}

	}

	system("pause");
}

std::vector<float> FIFO(std::vector<Job> jobs, int count)
{
	std::vector<int> arrival;
	for (int i = 0;i < jobs.size();i++)
	{
		arrival.push_back(jobs[i].ArrivalTime);
	}
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
	float turn = 0;
	for (Job e : jobs)
	{
		turn = turn + e.duration;
	}
	turn = turn / jobs.size();

	float response = 0;

	for (int i = 0; i < jobs.size(); i++)
	{
		response = response + (jobs[i].ArrivalTime - arrival[i]);
	}
	response = response / jobs.size();

	std::vector<float> average;
	average.push_back(turn);
	average.push_back(response);
	return average;
}


std::vector<float> SJF(std::vector<Job> jobs, int count)
{
	sort(jobs.begin(), jobs.end(), [](const Job& lhs, const Job& rhs)
	{
		return lhs.duration < rhs.duration;
	}
	);

	std::vector<int> arrival;
	for (int i = 0;i < jobs.size();i++)
	{
		arrival.push_back(jobs[i].ArrivalTime);
	}
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

	float turn = 0;
	for (Job e : jobs)
	{
		turn = turn + e.duration;
	}
	turn = turn / jobs.size();

	float response = 0;

	for (int i = 0; i < jobs.size(); i++)
	{
		response = response + (jobs[i].ArrivalTime - arrival[i]);
	}
	response = response / jobs.size();

	std::vector<float> average;
	average.push_back(turn);
	average.push_back(response);
	return average;
}

/* STCF(shortest time to completion works work when two jobs collide the one with the shortest time from that point until its completion takes priority*/
/*The longer job's then pushed back on the schedule*/
std::vector<Job> STCF(std::vector<Job> jobs, int count)
{
	for (int i = 0;i < jobs.size();i++)
	{
		if (count == jobs[i].ArrivalTime)
		{
			for (int j = 0; j < i; j++)
				if (jobs[i].duration < jobs[j].duration&&jobs[i].ArrivalTime<(jobs[j].duration + jobs[j].ArrivalTime - 1) && jobs[i].ArrivalTime>jobs[j].ArrivalTime)
				{
					jobs[j].duration = jobs[j].duration - (jobs[j].ArrivalTime - count);
					jobs[j].ArrivalTime = jobs[i].ArrivalTime + jobs[i].duration - 1;
				}
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
	return jobs;
}


std::vector<float> RR(std::vector<Job> jobs, int count, int timeSlice, int total)
{

	std::vector<int> arrival;
	for (int i = 0;i < jobs.size();i++)
	{
		arrival.push_back(jobs[i].ArrivalTime);
	}

	std::vector<Job> RR;
	int full = 0;
	int place = 0;
	int allEmpty = 0;
	while (allEmpty < jobs.size() - 1)
	{

		Job j;
		j.name = jobs[place].name;
		if (RR.size() == 0)
		{
			j.ArrivalTime = 0;
		}
		else
		{
			j.ArrivalTime = RR[RR.size() - 1].duration + 1;
		}

		if (jobs[place].duration == 0)
		{
			if (place = jobs.size() - 1)
			{
				place = 0;
			}
			else
			{
				place++;
			}

			allEmpty++;
		}

		else if (jobs[place].duration >= timeSlice)
		{
			j.duration = j.ArrivalTime + timeSlice - 1;
			jobs[place].duration = jobs[place].duration - timeSlice;
			RR.push_back(j);
			allEmpty = 0;
			if (place < jobs.size() - 1)
			{
				place++;
			}
			else
				place = 0;
		}
		else if (jobs[place].duration < timeSlice)
		{
			j.duration = j.ArrivalTime + jobs[place].duration - 1;
			jobs[place].duration = 0;
			RR.push_back(j);
			allEmpty = 0;
			if (place < jobs.size() - 1)
			{
				place++;
			}
			else
				place = 0;
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

	float response = 0;
	std::vector<float>starts;
	for (int i = 0; i < jobs.size(); i++)
	{
		response = response + RR[i].ArrivalTime;
	}
	response = response / jobs.size();

	std::vector<string> list;
	for (Job p : jobs)
	{
		list.push_back(p.name);
	}


	float turn = 0;
	for (int i = RR.size() - 1; i >= 0; i--)
	{
		int n = 0;
		for (int l = 0; l < list.size();l++)
		{
			if (RR[i].name == list[l])
			{
				turn = turn + RR[i].duration;
				list.erase(list.begin() + l);
			}
		}
	}
	turn=turn/ jobs.size();

	std::vector<float> average;
	average.push_back(turn);
	average.push_back(response);
	return average;
}

void startup()
{

}
