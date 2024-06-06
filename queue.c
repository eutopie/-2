#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LAST_MESSAGE 255

int main() {
	int msgid;
	char pathname[] = "key.ipc";
	key_t key;
	int i, len;

	struct mybuf {
		long mtype;
		char mtext[81];
	} mybuf;

	if ((key = ftok(pathname, 0)) < 0){
		printf("Не удалось сгенерировать ключ ipc\n");
		exit(-1);
	}

	if((msgid = msgget(key, 0666| IPC_CREAT)) < 0){
		printf("Не удалось создать или найти очередь сообщения\n");
		exit(-1);
	}

	for(i = 1; i<=5;i++){
		mybuf.mtype = 1;
		strcpy(mybuf.mtext, "Change da world, my final message");
		len = strlen(mybuf.mtext) + 1;
		if (msgsnd(msgid, (struct mybuf*) &mybuf, len, 0) < 0){
			printf("Не удалось отправить сообщение в очередь\n");
			msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL);
			exit(-1);
		}
	}
	mybuf.mtype = LAST_MESSAGE;
	len = 0;

	if (msgsnd(msgid, (struct mybuf*) &mybuf, len, 0) < 0){
		printf("Не удалось отправить последнее сообщение\n");
		msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL);
		exit(-1);
	}
	return 0;
}
