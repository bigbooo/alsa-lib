/*
 *  RawMIDI Interface - main file
 *  Copyright (c) 1998 by Jaroslav Kysela <perex@jcu.cz>
 *
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "asoundlib.h"

#define SND_FILE_RAWMIDI	"/dev/snd/midiC%iD%i"
#define SND_RAWMIDI_VERSION_MAX	SND_PROTOCOL_VERSION( 1, 0, 0 )

typedef struct {
	int card;
	int device;
	int fd;
} snd_rawmidi_t;

int snd_rawmidi_open(void **handle, int card, int device, int mode)
{
	int fd, ver;
	char filename[32];
	snd_rawmidi_t *rmidi;

	*handle = NULL;
	
	if (card < 0 || card >= SND_CARDS)
		return -EINVAL;
	sprintf(filename, SND_FILE_RAWMIDI, card, device);
	if ((fd = open(filename, mode)) < 0) {
		snd_card_load(card);
		if ((fd = open(filename, mode)) < 0)
			return -errno;
	}
	if (ioctl(fd, SND_RAWMIDI_IOCTL_PVERSION, &ver) < 0) {
		close(fd);
		return -errno;
	}
	if (SND_PROTOCOL_UNCOMPATIBLE(ver, SND_RAWMIDI_VERSION_MAX)) {
		close(fd);
		return -SND_ERROR_UNCOMPATIBLE_VERSION;
	}
	rmidi = (snd_rawmidi_t *) calloc(1, sizeof(snd_rawmidi_t));
	if (rmidi == NULL) {
		close(fd);
		return -ENOMEM;
	}
	rmidi->card = card;
	rmidi->device = device;
	rmidi->fd = fd;
	*handle = rmidi;
	return 0;
}

int snd_rawmidi_close(void *handle)
{
	snd_rawmidi_t *rmidi;
	int res;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	res = close(rmidi->fd) < 0 ? -errno : 0;
	free(rmidi);
	return res;
}

int snd_rawmidi_file_descriptor(void *handle)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	return rmidi->fd;
}

int snd_rawmidi_block_mode(void *handle, int enable)
{
	snd_rawmidi_t *rmidi;
	long flags;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	if ((flags = fcntl(rmidi->fd, F_GETFL)) < 0)
		return -errno;
	if (enable)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	if (fcntl(rmidi->fd, F_SETFL, flags) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_info(void *handle, snd_rawmidi_info_t * info)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !info)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_INFO, info) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_output_switch_list(void *handle, snd_switch_list_t * list)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !list)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_OSWITCH_LIST, list) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_output_switch_read(void *handle, snd_switch_t * sw)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !sw || sw->name[0] == '\0')
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_OSWITCH_READ, sw) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_output_switch_write(void *handle, snd_switch_t * sw)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !sw || sw->name[0] == '\0')
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_OSWITCH_WRITE, sw) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_input_switch_list(void *handle, snd_switch_list_t * list)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_ISWITCH_LIST, list) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_input_switch_read(void *handle, snd_switch_t * sw)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !sw || sw->name[0] == '\0')
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_ISWITCH_READ, sw) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_input_switch_write(void *handle, snd_switch_t * sw)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !sw || sw->name[0] == '\0')
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_ISWITCH_WRITE, sw) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_output_params(void *handle, snd_rawmidi_output_params_t * params)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !params)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_OUTPUT_PARAMS, params) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_input_params(void *handle, snd_rawmidi_input_params_t * params)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !params)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_INPUT_PARAMS, params) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_output_status(void *handle, snd_rawmidi_output_status_t * status)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !status)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_OUTPUT_STATUS, status) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_input_status(void *handle, snd_rawmidi_input_status_t * status)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || !status)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_INPUT_STATUS, status) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_drain_output(void *handle)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_DRAIN_OUTPUT) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_flush_output(void *handle)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_FLUSH_OUTPUT) < 0)
		return -errno;
	return 0;
}

int snd_rawmidi_flush_input(void *handle)
{
	snd_rawmidi_t *rmidi;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi)
		return -EINVAL;
	if (ioctl(rmidi->fd, SND_RAWMIDI_IOCTL_FLUSH_INPUT) < 0)
		return -errno;
	return 0;
}

ssize_t snd_rawmidi_write(void *handle, const void *buffer, size_t size)
{
	snd_rawmidi_t *rmidi;
	ssize_t result;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || (!buffer && size > 0) || size < 0)
		return -EINVAL;
	result = write(rmidi->fd, buffer, size);
	if (result < 0)
		return -errno;
	return result;
}

ssize_t snd_rawmidi_read(void *handle, void *buffer, size_t size)
{
	snd_rawmidi_t *rmidi;
	ssize_t result;

	rmidi = (snd_rawmidi_t *) handle;
	if (!rmidi || (!buffer && size > 0) || size < 0)
		return -EINVAL;
	result = read(rmidi->fd, buffer, size);
	if (result < 0)
		return -errno;
	return result;
}
