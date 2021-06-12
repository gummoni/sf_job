#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Job.h"
#include "JobScheduler.h"
#include <memory>

/// <summary>
/// ジョブ１
/// </summary>
class J1 : public Job {
public:
	int count = 0;

	virtual void Ready() {
		printf("start : j1\n");
	}

	virtual void Start() {
		for (;;) {
			if (count++ <= 10) {
				printf("yield: %d\n", count);
				Yield();
			}
			else {
				break;
			}
		}
	}

	virtual void Success() {
	}

	virtual void Failsafe() {
	}

	virtual void Retry() {
	}
};

class J2 : public Job {
private:
	J1* child = NULL;
	int count = 0;

public:

	virtual void Ready() {
		printf("start : j2\n");
	}

	virtual void Start() {
		for (;;) {
			if (count++ <= 10) {
				printf("yield: %d\n", count);
				Yield();
			}
			else {
				break;
			}
		}

		//exe
		child = new J1();
		child->count = -10;
		CurrentScheduler->Invoke((Job*)child)->Wait(this);
		delete child;
	}

	virtual void Success() {
	}

	virtual void Failsafe() {
	}

	virtual void Retry() {
	}
};

/// <summary>
/// メイン処理
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int main(void) {
	JobScheduler* sc = new JobScheduler();
	J1* j1 = new J1();
	J2* j2 = new J2();
	j1->count = 2;

	sc->Invoke((Job*)j1);
	sc->Invoke((Job*)j2);

	//実行
	sc->Execute();

	return 0;
}
