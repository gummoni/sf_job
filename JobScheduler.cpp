#include "Job.h"

jmp_buf SuspendJmp;
JobScheduler* CurrentScheduler;

/// <summary>
/// 処理移譲
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
/// 実行処理
/// </summary>
void JobScheduler::Execute() {
	//現在のスケジューラ設定
	CurrentScheduler = this;

	//スケジューラ実行
	Job* job = this->Top;
	if (0 != setjmp(SuspendJmp)) {
		job = this->Top->next;
		this->Top->next = NULL;
		this->Top = job;
	}
	if (NULL == job)
		return;

	//ジョブ実行
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

				//ユーザ通知
				if (false) {
					job->Ready();
				}
				else {
					//中断を選択
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

