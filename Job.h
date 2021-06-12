#pragma once
#include <setjmp.h>
#include "JobScheduler.h"

extern jmp_buf SuspendJmp;
extern JobScheduler* CurrentScheduler;

#define MEMORY_SIZE	1024

/// <summary>
/// ジョブ状態
/// </summary>
enum class JobState {
	START = 0,								//開始
	BUSY,									//動作中
	CANCELED,								//ユーザによる中断
	SUCCESS,								//正常終了
	FAILED,									//システムによる中断
};

/// <summary>
/// ジョブ
/// </summary>
class Job {
	unsigned char memory[MEMORY_SIZE] = { 0 };			//退避バッファ
public:
	jmp_buf buf = { 0 };					//バッファ
	int error_code = 0;						//停止理由
	JobState state = JobState::START;		//動作状態
	Job* next = NULL;						//次キュー

	virtual void Ready() = 0;				//準備処理
	virtual void Start() = 0;				//実行処理
	virtual void Success() = 0;				//処理成功後
	virtual void Failsafe() = 0;			//エラー発生後
	virtual void Retry() = 0;				//リトライ処理
	void Yield();							//一回お休み
	void Cancel();							//キャンセル
	int Wait(Job* parent);					//完了待ち
};
