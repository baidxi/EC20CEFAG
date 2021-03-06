#ifndef __FOTA_INFO_H
#define __FOTA_INFO_H

#include "crc32.h"
#include "user.h"

#define FOTAINFO_PART       "rawdata"
#define FOTAINFO_CONFIG		"/etc/fota.conf"
#define FOTAINFO_BLOCK_A    6
#define FOTAINFO_BLOCK_B    7
#define FOTAINFO_PAGE       0
#define FOTA_MAGIC          "FOTA#"

typedef enum 
{
	STAT_SUCCEED = 0,
    STAT_UPDATE = 1,
    STAT_BACKUP = 2,
	STAT_FAILED = 3,
	STAT_MAX,
}ESTAT;

typedef enum
{
	FOTA_NULL = 0,
    FOTA_AB = 1,
    FOTA_FULL = 2,
    FOTA_DIFF = 3,
	TYPE_MAX,
}FOTATYPE;

typedef enum
{
	BOOT_SUCCEED = 0,
	BOOT_FAILED = 1,
}BOOTSTAT;

/**
 * struct fota_info - record fota update infomation
 * magic: fota info magic number(%FOTA_MAGIC)
 * activerootfs: rootfs was currently activated(A/B FOTA)
 * activebootimg: bootimg was currently activated(A/B FOTA)
 * activemodem: modem was currently activated(A/B FOTA)
 * padding1: reserved for expand,full zero
 * type: FOTA update type, which include A/B FOTA,full fota and diff FOTA
 * eventstat: stat of update,which include update,backup and expand stat for future
 * upcount: the count of fota update success
 * ecount: the count of the fota info update  
 * fcount: the count of fota failed,which will be cleared when one fota update success happened
 * padding2: reserved for expand,full zero
 * info_crc: fota_info crc code
 */
struct fota_info
{
    char magic[8];
    char activerootfs[32];
    char activebootimg[32];
    char activemodem[32];
	char backrootfs[32];
	char backbootimg[32];
	char backmodem[32];
    char padding1[32];
    unsigned int type;
    unsigned int eventstat[4];
    unsigned int upcount;
    unsigned int ecount;
    unsigned int fcount;
    char padding2[52];
	char package[128];
	char padding3[64];
    unsigned int info_crc;
};

#define FOTAINFO_SIZE (sizeof(struct fota_info))
#define FOTAINFO_CRCSIZE (FOTAINFO_SIZE-4)
extern int write_fota_info(struct MtdPartition *, struct fota_info *);
extern int get_fota_info(struct fota_info *);
extern int get_fota_stat(void);
extern int get_fota_package(char *package);

extern int fota_update_start(FOTATYPE);
extern int fota_update_backup(void);
extern int fota_update_end(void);

extern int fota_set_activepart(const char *, const char *, const char *);
extern int fota_set_packagepath(const char *path);
extern int fota_set_type(FOTATYPE type);
extern int fota_set_stat(unsigned int stat);

#endif
