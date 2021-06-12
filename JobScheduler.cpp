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
	if (0 != setjmp(SuspendJmp)) {
		Job* job = Top->next;
		Top->next = NULL;
		Top = job;
	}
	if (NULL == Top)
		return;

	//�W���u���s
	if (JobState::START == Top->state) {
		Top->state = JobState::BUSY;
		Top->Ready();
		for (;;) {
			try {
				Top->Start();
				Top->state = JobState::SUCCESS;
				Top->Success();
				break;
			}
			catch (int err_code) {
				Top->Failsafe();

				//���[�U�ʒm
				if (false) {
					Top->Ready();
				}
				else {
					//���f��I��
					Top->error_code = err_code;
					Top->state = JobState::FAILED;
					break;
				}
			}
		}
		longjmp(SuspendJmp, (int)Top);
	}
	else {
		longjmp(Top->buf, (int)Top);
	}
}

