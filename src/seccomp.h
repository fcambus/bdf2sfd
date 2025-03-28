/*
 * bdf2sfd 1.1.9
 * Copyright (c) 2019-2025, Frederic Cambus
 * https://github.com/fcambus/bdf2sfd
 *
 * Created:      2019-11-21
 * Last Updated: 2020-09-17
 *
 * bdf2sfd is released under the BSD 2-Clause license.
 * See LICENSE file for details.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SECCOMP_H
#define SECCOMP_H

#include <stddef.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>

#if defined(__i386__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_I386
#elif defined(__x86_64__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_X86_64
#elif defined(__arm__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_ARM
#elif defined(__aarch64__)
#define SECCOMP_AUDIT_ARCH AUDIT_ARCH_AARCH64
#else
#error "Seccomp is only supported on i386, x86_64, arm, and aarch64 architectures."
#endif

#define BDF2SFD_SYSCALL_ALLOW(syscall) \
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_##syscall, 0, 1), \
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

static struct sock_filter filter[] = {
	/* Validate architecture */
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, arch)),
	BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, SECCOMP_AUDIT_ARCH, 1, 0),
	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),

	/* Load syscall */
	BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),

	BDF2SFD_SYSCALL_ALLOW(brk),
	BDF2SFD_SYSCALL_ALLOW(clock_gettime),	/* i386 glibc */
	BDF2SFD_SYSCALL_ALLOW(close),
	BDF2SFD_SYSCALL_ALLOW(exit_group),
	BDF2SFD_SYSCALL_ALLOW(fstat),
#if defined(__NR_fstat64)
	BDF2SFD_SYSCALL_ALLOW(fstat64),		/* i386 glibc */
#endif
	BDF2SFD_SYSCALL_ALLOW(gettimeofday),	/* i386 glibc */
	BDF2SFD_SYSCALL_ALLOW(ioctl),
#if defined(__NR_open)
	BDF2SFD_SYSCALL_ALLOW(open),
#endif
	BDF2SFD_SYSCALL_ALLOW(openat),
	BDF2SFD_SYSCALL_ALLOW(read),
	BDF2SFD_SYSCALL_ALLOW(write),
	BDF2SFD_SYSCALL_ALLOW(writev),

	BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL)
};

struct sock_fprog bdf2sfd = {
	.len = sizeof(filter)/sizeof(filter[0]),
	.filter = filter
};

#endif /* SECCOMP_H */
