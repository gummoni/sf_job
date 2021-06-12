#pragma once

class Job;

/// <summary>
/// ジョブスケジューラ
/// </summary>
class JobScheduler {
private:
	Job* Top = NULL;
	Job* Last = NULL;

public:
	Job* Invoke(Job* job);
	void Execute();
};
