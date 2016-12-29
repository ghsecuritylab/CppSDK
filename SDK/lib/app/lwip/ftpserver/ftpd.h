/*
 * Copyright (c) 2002 Florian Schulze.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ftpd.h - This file is part of the FTP daemon for lwIP
 *
 */

#ifndef __FTPD_H__
#define __FTPD_H__

#include "vfs.h"
#include "interface/LwIp/src/include/lwip/ip4_addr.h"
#include "lib/buffers/ring_buff.h"


enum ftpd_state_e {
	FTPD_USER,
	FTPD_PASS,
	FTPD_IDLE,
	FTPD_NLST,
	FTPD_LIST,
	FTPD_RETR,
	FTPD_RNFR,
	FTPD_STOR,
	FTPD_QUIT
};

typedef struct ftpd_datastate_s {
	int connected;
	vfs_dir_t *vfs_dir;
	vfs_dirent_t *vfs_dirent;
	vfs_file_t *vfs_file;
	GI::Buff::RingBuff fifo;
	struct tcp_pcb *msgpcb;
	void *msgfs;
}ftpd_datastate;

typedef struct  {
	enum ftpd_state_e state;
	GI::Buff::RingBuff fifo;
	vfs_t *vfs;
	struct ip4_addr dataip;
	unsigned short dataport;
	struct tcp_pcb *datapcb;
	ftpd_datastate *datafs;
	int passive;
	char *renamefrom;
}ftpd_msgstate;

void ftpd_init(unsigned short port);

#endif	/* __FTPD_H__ */
