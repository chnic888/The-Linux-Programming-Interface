#include "svmsg_demo_server.h"

int svmsgDemoServer(int argc, char *argv[]) {
	int msqid;
	key_t key;
	const int MQ_PERMS = S_IRUSR | S_IWUSR | S_IWGRP;

	key = ftok(KEY_FILE, 1);
	if (key == -1) {
		fprintf(stderr, "ftok failed(), %d\n", errno);
		exit(EXIT_FAILURE);
	}

	while ((msqid = msgget(key, IPC_CREAT | IPC_EXCL | MQ_PERMS)) == -1) {
		if (errno == EEXIST) {
			msqid = msgget(key, 0);
			if (msqid == -1) {
				fprintf(stderr, "failed to call msgget(), %d\n", errno);
				exit(EXIT_FAILURE);
			}

			if (msgctl(msqid, IPC_RMID, NULL) == -1) {
				fprintf(stderr, "failed to call msgctl() to delete the old queue, %d\n", errno);
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "failed to call msgget(), %d\n", errno);
		}

	}

	return (EXIT_SUCCESS);
}