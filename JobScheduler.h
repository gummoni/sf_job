#pragma once

class Job;

/// <summary>
/// �W���u�X�P�W���[��
/// </summary>
class JobScheduler {
private:
	Job* Top = NULL;
	Job* Last = NULL;

public:
	Job* Invoke(Job* job);
	void Execute();
};
