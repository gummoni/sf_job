#include "Job.h"

jmp_buf SuspendJmp;
JobScheduler* CurrentScheduler;

/// <summary>
/// �����ڏ�
/// </summary>
/// <param name="job"></param>
/// <returns></returns>
Job* JobScheduler::Invoke(Job* job) {
	if (NULL == Top) {
		Top = job;
		Last = job;
	}
	else {
		Last->next = job;
		Last = job;
	}
	return job;
}

/// <summary>
/// ���s����
/// </summary>
void JobScheduler::Execute() {
	//���݂̃X�P�W���[���ݒ�
	CurrentScheduler = this;

	//�X�P�W���[�����s
	Job* job = this->Top;
	if (0 != setjmp(SuspendJmp)) {
		job = this->Top->next;
		this->Top->next = NULL;
		this->Top = job;
	}
	if (NULL == job)
		return;

	//�W���u���s
	if (JobState::START == job->state) {
		job->state = JobState::BUSY;
		job->Ready();
		for (;;) {
			try {
				job->Start();
				job->state = JobState::SUCCESS;
				job->Success();
				break;
			}
			catch (int err_code) {
				job->Failsafe();

				//���[�U�ʒm
				if (false) {
					job->Ready();
				}
				else {
					//���f��I��
					job->error_code = err_code;
					job->state = JobState::FAILED;
					break;
				}
			}
		}
		longjmp(SuspendJmp, (int)job);
	}
	else {
		longjmp(job->buf, (int)job);
	}
}

