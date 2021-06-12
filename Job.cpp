#include "Job.h"
#include <string.h>

/// <summary>
/// ��񂨋x��
/// </summary>
void Job::Yield() {
	static Job* __ret;
	static int __size;

	CurrentScheduler->Invoke(this);

	if (NULL == (__ret = (Job*)setjmp(buf))) {
		__size = SuspendJmp[0] - buf[4];
		memcpy(memory, (void*)buf[4], __size);
		longjmp(SuspendJmp, (int)this);
	}
	else {
		__size = SuspendJmp[0] - __ret->buf[4];
		memcpy((void*)__ret->buf[4], __ret->memory, __size);
	}
}

/// <summary>
/// �����҂�
/// </summary>
/// <param name="parent"></param>
int Job::Wait(Job* parent) {
	while (state <= JobState::BUSY)
		parent->Yield();
	return error_code;
}

/// <summary>
/// �L�����Z��
/// </summary>
void Job::Cancel() {
	this->state = JobState::CANCELED;
}

