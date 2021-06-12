#pragma once
#include <setjmp.h>
#include "JobScheduler.h"

extern jmp_buf SuspendJmp;
extern JobScheduler* CurrentScheduler;

#define MEMORY_SIZE	1024

/// <summary>
/// �W���u���
/// </summary>
enum class JobState {
	START = 0,								//�J�n
	BUSY,									//���쒆
	CANCELED,								//���[�U�ɂ�钆�f
	SUCCESS,								//����I��
	FAILED,									//�V�X�e���ɂ�钆�f
};

/// <summary>
/// �W���u
/// </summary>
class Job {
	unsigned char memory[MEMORY_SIZE] = { 0 };			//�ޔ��o�b�t�@
public:
	jmp_buf buf = { 0 };					//�o�b�t�@
	int error_code = 0;						//��~���R
	JobState state = JobState::START;		//������
	Job* next = NULL;						//���L���[

	virtual void Ready() = 0;				//��������
	virtual void Start() = 0;				//���s����
	virtual void Success() = 0;				//����������
	virtual void Failsafe() = 0;			//�G���[������
	virtual void Retry() = 0;				//���g���C����
	void Yield();							//��񂨋x��
	void Cancel();							//�L�����Z��
	int Wait(Job* parent);					//�����҂�
};
