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
	if (0 != setjmp(SuspendJmp)) {
		Job* job = Top->next;
		Top->next = NULL;
		Top = job;
	}
	if (NULL == Top)
		return;

	//ジョブ実行
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

				//ユーザ通知
				if (false) {
					Top->Ready();
				}
				else {
					//中断を選択
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

