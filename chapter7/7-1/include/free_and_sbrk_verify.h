#ifndef THE_LINUX_PROGRAMMING_INTERFACE_FREE_AND_SBRK_VERIFY_H
#define THE_LINUX_PROGRAMMING_INTERFACE_FREE_AND_SBRK_VERIFY_H

#define GN_GT_O 02

#define GN_ANY_BASE 0100
#define GN_BASE_8 0200
#define GN_BASE_16 0400

int freeSbrkVerify(int, char *[]);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_FREE_AND_SBRK_VERIFY_H
