/**
 * \file <alsa/hwdep.h>
 * \brief Application interface library for the ALSA driver
 * \author Jaroslav Kysela <perex@suse.cz>
 * \author Abramo Bagnara <abramo@alsa-project.org>
 * \author Takashi Iwai <tiwai@suse.de>
 * \date 1998-2001
 *
 * Application interface library for the ALSA driver
 *
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 2.1 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef __ALSA_HWDEP_H
#define __ALSA_HWDEP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \defgroup HwDep Hardware Dependant Interface
 *  The Hardware Dependant Interface.
 *  \{
 */

/** dlsym version for interface entry callback */
#define SND_HWDEP_DLSYM_VERSION		_dlsym_hwdep_001

/** HwDep information container */
typedef struct _snd_hwdep_info snd_hwdep_info_t;

/** HwDep interface */
typedef enum _snd_hwdep_iface {
	SND_HWDEP_IFACE_OPL2 = 0,	/**< OPL2 raw driver */
	SND_HWDEP_IFACE_OPL3,		/**< OPL3 raw driver */
	SND_HWDEP_IFACE_OPL4,		/**< OPL4 raw driver */
	SND_HWDEP_IFACE_SB16CSP,	/**< SB16CSP driver */
	SND_HWDEP_IFACE_EMU10K1,	/**< EMU10K1 driver */
	SND_HWDEP_IFACE_YSS225,		/**< YSS225 driver */
	SND_HWDEP_IFACE_ICS2115,	/**< ICS2115 driver */
	SND_HWDEP_IFACE_LAST = SND_HWDEP_IFACE_ICS2115,  /**< last know hwdep interface */
} snd_hwdep_iface_t;

/** open for reading */
#define SND_HWDEP_OPEN_READ		(O_RDONLY)
/** open for writing */
#define SND_HWDEP_OPEN_WRITE		(O_WRONLY)
/** open for reading and writing */
#define SND_HWDEP_OPEN_DUPLEX		(O_RDWR)
/** flag: open in nonblock mode */
#define SND_HWDEP_OPEN_NONBLOCK		(O_NONBLOCK)

/** HwDep handle type */
typedef enum _snd_hwdep_type {
	/** Kernel level HwDep */
	SND_HWDEP_TYPE_HW,
	/** Shared memory client HwDep (not yet implemented) */
	SND_HWDEP_TYPE_SHM,
	/** INET client HwDep (not yet implemented) */
	SND_HWDEP_TYPE_INET,
} snd_hwdep_type_t;

/** HwDep handle */
typedef struct _snd_hwdep snd_hwdep_t;

int snd_hwdep_open(snd_hwdep_t **hwdep, const char *name, int mode);
int snd_hwdep_close(snd_hwdep_t *hwdep);
int snd_hwdep_poll_descriptors(snd_hwdep_t *hwdep, struct pollfd *pfds, unsigned int space);
int snd_hwdep_poll_descriptors_revents(snd_hwdep_t *hwdep, struct pollfd *pfds, unsigned int nfds, unsigned short *revents);
int snd_hwdep_nonblock(snd_hwdep_t *hwdep, int nonblock);
int snd_hwdep_info(snd_hwdep_t *hwdep, snd_hwdep_info_t * info);
int snd_hwdep_ioctl(snd_hwdep_t *hwdep, unsigned int request, void * arg);
ssize_t snd_hwdep_write(snd_hwdep_t *hwdep, const void *buffer, size_t size);
ssize_t snd_hwdep_read(snd_hwdep_t *hwdep, void *buffer, size_t size);

size_t snd_hwdep_info_sizeof(void);
/** allocate #snd_hwdep_info_t container on stack */
#define snd_hwdep_info_alloca(ptr) do { assert(ptr); *ptr = (snd_hwdep_info_t *) alloca(snd_hwdep_info_sizeof()); memset(*ptr, 0, snd_hwdep_info_sizeof()); } while (0)
int snd_hwdep_info_malloc(snd_hwdep_info_t **ptr);
void snd_hwdep_info_free(snd_hwdep_info_t *obj);
void snd_hwdep_info_copy(snd_hwdep_info_t *dst, const snd_hwdep_info_t *src);

unsigned int snd_hwdep_info_get_device(const snd_hwdep_info_t *obj);
int snd_hwdep_info_get_card(const snd_hwdep_info_t *obj);
const char *snd_hwdep_info_get_id(const snd_hwdep_info_t *obj);
const char *snd_hwdep_info_get_name(const snd_hwdep_info_t *obj);
snd_hwdep_iface_t snd_hwdep_info_get_iface(const snd_hwdep_info_t *obj);
void snd_hwdep_info_set_device(snd_hwdep_info_t *obj, unsigned int val);

/** \} */

#ifdef __cplusplus
}
#endif

#endif /* __ALSA_HWDEP_H */

