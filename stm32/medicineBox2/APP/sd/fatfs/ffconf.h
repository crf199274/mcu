/*---------------------------------------------------------------------------/
/  FatFs Functional Configurations---fatf��������
/---------------------------------------------------------------------------*/

#define FFCONF_DEF	86631	/* Revision ID ---�޶�ID */


/*---------------------------------------------------------------------------/
/ Function Configurations---��������
/---------------------------------------------------------------------------*/

#define FF_FS_READONLY	0
/* This option switches read-only configuration. (0:Read/Write or 1:Read-only)
/  Read-only configuration removes writing API functions, f_write(), f_sync(),
/  f_unlink(), f_mkdir(), f_chmod(), f_rename(), f_truncate(), f_getfree()
/  and optional writing functions as well. */

/*��ѡ���л�ֻ�����á� (0:��/д��1:ֻ��)  
/ֻ������ɾ��дAPI������f_write()�� f_sync()��  
/ f_unlink (), f_mkdir (), f_chmod (), f_rename (), f_truncate (), f_getfree ()  
/�Ϳ�ѡ��д�뺯����*/  

#define FF_FS_MINIMIZE	0
/* This option defines minimization level to remove some basic API functions.
/
/   0: Basic functions are fully enabled.
/   1: f_stat(), f_getfree(), f_unlink(), f_mkdir(), f_truncate() and f_rename()
/      are removed.
/   2: f_opendir(), f_readdir() and f_closedir() are removed in addition to 1.
/   3: f_lseek() function is removed in addition to 2. */

/*���ѡ�������С��������ɾ��һЩ������API������  
/  
/ 0:����������ȫ���á�  
/ 1: f_stat (), f_getfree (), f_unlink (), f_mkdir (), f_truncate()��f_rename ()��ɾ����  
/ 2: f_opendir()�� f_readdir()��f_closedir()��1�Ļ����ϱ�ɾ�����ˡ�  
/ 3:����f_lseek��2�Ļ����ϱ�ɾ����*/


#define FF_USE_FIND		0
/* This option switches filtered directory read functions, f_findfirst() and
/  f_findnext(). (0:Disable, 1:Enable 2:Enable with matching altname[] too) */

/*���ѡ���л����˺��Ŀ¼��ȡ������f_findfirst()��  
/ f_findnext()�� (0:Disable, 1:Enable 2:Enable with matching altname[] too) */  


#define FF_USE_MKFS		0
/* This option switches f_mkfs() function. (0:Disable or 1:Enable) */
/*���ѡ���л�f_mkfs()������ (0:���û�1:����)*/  


#define FF_USE_FASTSEEK	0
/* This option switches fast seek function. (0:Disable or 1:Enable) */
 /*���ѡ���л����ٲ��ҹ��ܡ� (0:���û�1:����)*/  


#define FF_USE_EXPAND	0
/* This option switches f_expand function. (0:Disable or 1:Enable) */
/*��ѡ���л�f_expand������ (0:���û�1:����)*/  


#define FF_USE_CHMOD	0
/* This option switches attribute manipulation functions, f_chmod() and f_utime().
/  (0:Disable or 1:Enable) Also FF_FS_READONLY needs to be 0 to enable this option. */

/*���ѡ������Բ���������f_chmod()��f_utime()��  
/ (0:Disable��1:Enable)����FF_FS_READONLYҲ��Ҫ����Ϊ0�������ø�ѡ�*/  


#define FF_USE_LABEL	0
/* This option switches volume label functions, f_getlabel() and f_setlabel().
/  (0:Disable or 1:Enable) */

/*�л����ǩ������f_getlabel()��f_setlabel()��  
/(0:���û�1:����)*/  

#define FF_USE_FORWARD	0
/* This option switches f_forward() function. (0:Disable or 1:Enable) */
/*���ѡ���л�f_forward()������ (0:���û�1:����)*/  


#define FF_USE_STRFUNC	0
#define FF_PRINT_LLI	0
#define FF_PRINT_FLOAT	0
#define FF_STRF_ENCODE	0
/* FF_USE_STRFUNC switches string functions, f_gets(), f_putc(), f_puts() and
/  f_printf().
/
/   0: Disable. FF_PRINT_LLI, FF_PRINT_FLOAT and FF_STRF_ENCODE have no effect.
/   1: Enable without LF-CRLF conversion.
/   2: Enable with LF-CRLF conversion.
/
/  FF_PRINT_LLI = 1 makes f_printf() support long long argument and FF_PRINT_FLOAT = 1/2
   makes f_printf() support floating point argument. These features want C99 or later.
/  When FF_LFN_UNICODE >= 1 with LFN enabled, string functions convert the character
/  encoding in it. FF_STRF_ENCODE selects assumption of character encoding ON THE FILE
/  to be read/written via those functions.
/
/   0: ANSI/OEM in current CP
/   1: Unicode in UTF-16LE
/   2: Unicode in UTF-16BE
/   3: Unicode in UTF-8
*/

/* FF_USE_STRFUNC�����ַ���������f_gets()�� f_putc()�� f_puts()��  
/ f_printf()��  
/  
/ 0:���á� FF_PRINT_LLI, FF_PRINT_FLOAT��FF_STRF_ENCODE��Ч��  
/ 1: Enable without LF-CRLF conversion��  
/ 2: Enable with LF-CRLF conversion��  
/  
/ FF_PRINT_LLI = 1ʹf_printf()֧�ֳ�������FF_PRINT_FLOAT = 1/2  
ʹf_printf()֧�ָ�������� ��Щ������ҪC99����߰汾��  
/������LFNʱFF_LFN_UNICODE >= 1���ַ�������ת���ַ�  
/���롣 FF_STRF_ENCODEѡ�����ļ��Ͻ����ַ�����ļ���  
/����ͨ����Щ������ȡ/д�롣  
/  
/ 0: ANSI/OEM�ڵ�ǰCP  
/ 1: Unicode UTF-16LE  
/ 2: UTF-16BE��ʽ��Unicode  
/ 3: UTF-8��ʽ��Unicode  
*/  

/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations---Locale��Namespace����
/---------------------------------------------------------------------------*/

#define FF_CODE_PAGE	437
/* This option specifies the OEM code page to be used on the target system.
/  Incorrect code page setting can cause a file open failure.
/
/   437 - U.S.
/   720 - Arabic
/   737 - Greek
/   771 - KBL
/   775 - Baltic
/   850 - Latin 1
/   852 - Latin 2
/   855 - Cyrillic
/   857 - Turkish
/   860 - Portuguese
/   861 - Icelandic
/   862 - Hebrew
/   863 - Canadian French
/   864 - Arabic
/   865 - Nordic
/   866 - Russian
/   869 - Greek 2
/   932 - Japanese (DBCS)
/   936 - Simplified Chinese (DBCS)
/   949 - Korean (DBCS)
/   950 - Traditional Chinese (DBCS)
/     0 - Include all code pages above and configured by f_setcp()
*/

/*��ѡ��ָ��Ҫ��Ŀ��ϵͳ��ʹ�õ�OEM����ҳ��  
/����ȷ�Ĵ���ҳ���ÿ��ܵ����ļ���ʧ�ܡ�  
/  
/ 437 -����  
/ 720 -��������  
/ 737 -ϣ����  
/ 771 -����  
775 -���޵ĺ�  
/ 850 -������  
/ 852 -������2  
/ 855 -�������  
/ 857 -��������  
/ 860 -��������  
/ 861 -������  
/ 862 -ϣ������  
/ 863 -���ô���  
/ 864 -��������  
/ 865 -��ŷ  
/ 866 -����  
/ 869 -ϣ����  
/ 932 -����(DBCS)  
/ 936 -��������(DBCS)  
/ 949 -����(DBCS)  
/ 950 -��������(DBCS)  
/ 0 -�������д���ҳ��������õ�f_setcp()  
*/  


#define FF_USE_LFN		0
#define FF_MAX_LFN		255
/* The FF_USE_LFN switches the support for LFN (long file name).
/
/   0: Disable LFN. FF_MAX_LFN has no effect.
/   1: Enable LFN with static  working buffer on the BSS. Always NOT thread-safe.
/   2: Enable LFN with dynamic working buffer on the STACK.
/   3: Enable LFN with dynamic working buffer on the HEAP.
/
/  To enable the LFN, ffunicode.c needs to be added to the project. The LFN function
/  requiers certain internal working buffer occupies (FF_MAX_LFN + 1) * 2 bytes and
/  additional (FF_MAX_LFN + 44) / 15 * 32 bytes when exFAT is enabled.
/  The FF_MAX_LFN defines size of the working buffer in UTF-16 code unit and it can
/  be in range of 12 to 255. It is recommended to be set it 255 to fully support LFN
/  specification.
/  When use stack for the working buffer, take care on stack overflow. When use heap
/  memory for the working buffer, memory management functions, ff_memalloc() and
/  ff_memfree() exemplified in ffsystem.c, need to be added to the project. */

/* FF_USE_LFN����֧��LFN(���ļ���)��  
/  
/ 0: Disable LFN�� FF_MAX_LFN��Ч��  
/ 1:��BSS��ʹ��LFN with static working buffer�� ���ǲ����̰߳�ȫ�ġ�  
/ 2: Enable LFN with dynamic working buffer on the STACK��  
/ 3:����LFN with dynamic working buffer on the HEAP��  
/  
/Ҫ����LFN����Ҫ��ffunicode.c��ӵ���Ŀ�С� LFN����  
/Ҫ��һ�����ڲ�����������ռ��(FF_MAX_LFN + 1) * 2�ֽں�  
/����(FF_MAX_LFN + 44) / 15 * 32�ֽ�ʱ��exFAT���á�  
/ FF_MAX_LFN��UTF-16���뵥Ԫ�����˹����������Ĵ�С��������  
/��ȡֵ��Χ��12 ~ 255�� ��������Ϊ255������ȫ֧��LFN  
/�淶��  
/��ʹ�ö�ջ��Ϊ����������ʱ��ע��ջ����� ��ʹ�ö�  
/�ڴ湤�����������ڴ��������ff_memalloc()��  
/ ff_memfree()ʾ����ffsystem.c�У���Ҫ��ӵ���Ŀ�С� */  


#define FF_LFN_UNICODE	0
/* This option switches the character encoding on the API when LFN is enabled.
/
/   0: ANSI/OEM in current CP (TCHAR = char)
/   1: Unicode in UTF-16 (TCHAR = WCHAR)
/   2: Unicode in UTF-8 (TCHAR = char)
/   3: Unicode in UTF-32 (TCHAR = DWORD)
/
/  Also behavior of string I/O functions will be affected by this option.
/  When LFN is not enabled, this option has no effect. */

/*��LFN������ʱ����ѡ����API���л��ַ����롣  
/  
/ 0: ANSI/OEM�ڵ�ǰCP (TCHAR = char)  
/ 1: Unicode��UTF-16 (TCHAR = WCHAR)  
/ 2: Unicode��UTF-8 (TCHAR = char)  
/ 3: Unicode��UTF-32 (TCHAR = DWORD)  
/  
���⣬�ַ���I/O��������Ϊ���ܴ�ѡ���Ӱ�졣  
/��LFNδ����ʱ����ѡ����Ч�� */ 


#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
/* This set of options defines size of file name members in the FILINFO structure
/  which is used to read out directory items. These values should be suffcient for
/  the file names to read. The maximum possible length of the read file name depends
/  on character encoding. When LFN is not enabled, these options have no effect. */

/*���ѡ�������FILINFO�ṹ���ļ�����Ա�Ĵ�С  
/��������Ŀ¼� ��ЩֵӦ���㹻��  
/��ȡ�ļ����� ��ȡ�ļ����������ܳ���ȡ����  
/���ַ����롣 ��LFNδ����ʱ����Щѡ������á� */  


#define FF_FS_RPATH		0
/* This option configures support for relative path.
/
/   0: Disable relative path and remove related functions.
/   1: Enable relative path. f_chdir() and f_chdrive() are available.
/   2: f_getcwd() function is available in addition to 1.
*/

/*��ѡ���������·��֧�֡�  
/  
/ 0:�������·�����Ƴ���ع��ܡ�  
/ 1:�������·���� F_chdir()��f_chdrive()�ǿ��õġ�  
/ 2: f_getcwd()������1֮����á�  
*/  


/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations---����/������
/---------------------------------------------------------------------------*/

#define FF_VOLUMES		1
/* Number of volumes (logical drives) to be used. (1-10) */
/*Ҫʹ�õľ���(�߼�������)�� (1) */  


#define FF_STR_VOLUME_ID	0
#define FF_VOLUME_STRS		"RAM","NAND","CF","SD","SD2","USB","USB2","USB3"
/* FF_STR_VOLUME_ID switches support for volume ID in arbitrary strings.
/  When FF_STR_VOLUME_ID is set to 1 or 2, arbitrary strings can be used as drive
/  number in the path name. FF_VOLUME_STRS defines the volume ID strings for each
/  logical drives. Number of items must not be less than FF_VOLUMES. Valid
/  characters for the volume ID strings are A-Z, a-z and 0-9, however, they are
/  compared in case-insensitive. If FF_STR_VOLUME_ID >= 1 and FF_VOLUME_STRS is
/  not defined, a user defined volume string table needs to be defined as:
/
/  const char* VolumeStr[FF_VOLUMES] = {"ram","flash","sd","usb",...
*/

/* FF_STR_VOLUME_ID����֧�־�IDΪ�����ַ�����  
/��FF_STR_VOLUME_ID����Ϊ1��2ʱ����ʹ�������ַ�����Ϊ������  
·�����еġ�/ number���� FF_VOLUME_STRSΪÿ�������˾�ID�ַ���  
/�߼��������� ��Ŀ��������С��FF_VOLUMES�� ��Ч��  
��ID�ַ�����/�ַ���A-Z��A-Z��0-9�����ǣ�������  
/�Ƚϲ����ִ�Сд�� ���FF_STR_VOLUME_ID >= 1����FF_VOLUME_STRSΪ  
/δ���壬�û�����ľ��ַ�������Ҫ����Ϊ:  
/  
/ const char * VolumeStr [FF_VOLUMES] ={���ڴ桱,�����硱����sd������usb������  
*/  

#define FF_MULTI_PARTITION	0
/* This option switches support for multiple volumes on the physical drive.
/  By default (0), each logical drive number is bound to the same physical drive
/  number and only an FAT volume found on the physical drive will be mounted.
/  When this function is enabled (1), each logical drive number can be bound to
/  arbitrary physical drive and partition listed in the VolToPart[]. Also f_fdisk()
/  funciton will be available. */

/*��ѡ���л��������������϶�����֧�֡�  
Ĭ�������(0)��ÿ���߼��̺Ŷ��󶨵�ͬһ��������  
ֻ�����������������ҵ���FAT��Żᱻ���ء�  
/�����ù���ʱ(1)��ÿ���߼��̺Ŷ����Ա���  
volpartart[]���г���/���������������ͷ����� ��f_fdisk ()  
/���ܽ����á� */  


#define FF_MIN_SS		512
#define FF_MAX_SS		512
/* This set of options configures the range of sector size to be supported. (512,
/  1024, 2048 or 4096) Always set both 512 for most systems, generic memory card and
/  harddisk, but a larger value may be required for on-board flash memory and some
/  type of optical media. When FF_MAX_SS is larger than FF_MIN_SS, FatFs is configured
/  for variable sector size mode and disk_ioctl() function needs to implement
/  GET_SECTOR_SIZE command. */

/*���ѡ�����������С�ķ�Χ��֧�֡� (512��,  
/ 1024, 2048��4096)��������512Ϊ�����ϵͳ��ͨ�ü��俨��  
/Ӳ�̣���������Ҫ�����ֵΪ���������һЩ  
/��ѧ�������͡� ��FF_MAX_SS����FF_MIN_SSʱ������FatFs  
/Ϊ�ɱ�������Сģʽ��disk_ioctl()������Ҫʵ��  
/ GET_SECTOR_SIZE��� */  


#define FF_LBA64		0
/* This option switches support for 64-bit LBA. (0:Disable or 1:Enable)
/  To enable the 64-bit LBA, also exFAT needs to be enabled. (FF_FS_EXFAT == 1) */

/*��ѡ���л�֧��64λLBA�� (0:���û�1:����)  
/Ҫ����64λLBA������Ҫ����exFAT�� (ff_fs_exfat == 1) */  


#define FF_MIN_GPT		0x10000000
/* Minimum number of sectors to switch GPT as partitioning format in f_mkfs and
/  f_fdisk function. 0x100000000 max. This option has no effect when FF_LBA64 == 0. */

/*��f_mkfs�����л�GPTΪ������ʽ����С������  
/ f_fdisk������ 0 x100000000 max�� ��FF_LBA64 == 0ʱ����ѡ����Ч�� */  


#define FF_USE_TRIM		0
/* This option switches support for ATA-TRIM. (0:Disable or 1:Enable)
/  To enable Trim function, also CTRL_TRIM command should be implemented to the
/  disk_ioctl() function. */

/*��ѡ���л���ATA-TRIM��֧�֡� (0:���û�1:����)  
Ҫʹ��Trim���ܣ�����Ҫ��ϵͳִ��CTRL_TRIM����  
/ disk_ioctl()������ */  



/*---------------------------------------------------------------------------/
/ System Configurations---ϵͳ����
/---------------------------------------------------------------------------*/

#define FF_FS_TINY		0
/* This option switches tiny buffer configuration. (0:Normal or 1:Tiny)
/  At the tiny configuration, size of file object (FIL) is shrinked FF_MAX_SS bytes.
/  Instead of private sector buffer eliminated from the file object, common sector
/  buffer in the filesystem object (FATFS) is used for the file data transfer. */

/*���ѡ���л�΢С�Ļ��������á� (0:������1:С)  
/��΢С�����£��ļ�����(FIL)�Ĵ�С��ѹ��ΪFF_MAX_SS�ֽڡ�  
/���ļ�������ɾ��˽�в��Ż���������������ͨ����  
�ļ�ϵͳ����(FATFS)�е�/ buffer�����ļ����ݴ��䡣 */  


#define FF_FS_EXFAT		0
/* This option switches support for exFAT filesystem. (0:Disable or 1:Enable)
/  To enable exFAT, also LFN needs to be enabled. (FF_USE_LFN >= 1)
/  Note that enabling exFAT discards ANSI C (C89) compatibility. */

/*���ѡ���л���exFAT�ļ�ϵͳ��֧�֡� (0:���û�1:����)  
/Ҫ����exFAT��Ҳ��Ҫ����LFN�� (FF_USE_LFN > = 1)  
/��ע�⣬����exFAT������ANSI C (C89)�����ԡ� */  


#define FF_FS_NORTC		0
#define FF_NORTC_MON	1
#define FF_NORTC_MDAY	1
#define FF_NORTC_YEAR	2020
/* The option FF_FS_NORTC switches timestamp functiton. If the system does not have
/  any RTC function or valid timestamp is not needed, set FF_FS_NORTC = 1 to disable
/  the timestamp function. Every object modified by FatFs will have a fixed timestamp
/  defined by FF_NORTC_MON, FF_NORTC_MDAY and FF_NORTC_YEAR in local time.
/  To enable timestamp function (FF_FS_NORTC = 0), get_fattime() function need to be
/  added to the project to read current time form real-time clock. FF_NORTC_MON,
/  FF_NORTC_MDAY and FF_NORTC_YEAR have no effect.
/  These options have no effect in read-only configuration (FF_FS_READONLY = 1). */

/*ѡ��FF_FS_NORTC����ʱ������ܡ� ���ϵͳû��  
/����Ҫ�κ�RTC��������Ч��ʱ�������FF_FS_NORTC = 1����Ϊdisable  
/ʱ��������� ��fatf�޸ĵ�ÿ�������й̶���ʱ���  
/�ɱ���ʱ���FF_NORTC_MON��FF_NORTC_MDAY��FF_NORTC_YEAR���塣  
/Ҫ����ʱ�������(FF_FS_NORTC = 0)�� get_fattime()������Ҫ��  
/��ӵ���Ŀ��ȡ��ǰʱ���ʵʱʱ�ӡ� FF_NORTC_MON,  
/ FF_NORTC_MDAY��FF_NORTC_YEAR��Ч��  
/��Щѡ����ֻ������(FF_FS_READONLY = 1)����Ч*/  


#define FF_FS_NOFSINFO	0
/* If you need to know correct free space on the FAT32 volume, set bit 0 of this
/  option, and f_getfree() function at first time after volume mount will force
/  a full FAT scan. Bit 1 controls the use of last allocated cluster number.
/
/  bit0=0: Use free cluster count in the FSINFO if available.
/  bit0=1: Do not trust free cluster count in the FSINFO.
/  bit1=0: Use last allocated cluster number in the FSINFO if available.
/  bit1=1: Do not trust last allocated cluster number in the FSINFO.
*/

/*�������Ҫ֪����ȷ�Ŀ��пռ���FAT32������λ0  
/ѡ��Լ�f_getfree()������ǿ�ƹ��ؾ�  
/ȫ֬��ɨ�衣 ��1λ����������ļ�Ⱥ�ŵ�ʹ�á�  
/  
/ bit0=0:������ã���FSINFO��ʹ�ÿ��м�Ⱥ����  
/ bit0=1: Do not trust free cluster count in the FSINFO��  
/ bit1=0:������ã�ʹ��FSINFO��������ļ�Ⱥ�š�  
/ bit1=1:������FSINFO�����һ�η���ļ�Ⱥ�š�*/  


#define FF_FS_LOCK		0
/* The option FF_FS_LOCK switches file lock function to control duplicated file open
/  and illegal operation to open objects. This option must be 0 when FF_FS_READONLY
/  is 1.
/
/  0:  Disable file lock function. To avoid volume corruption, application program
/      should avoid illegal open, remove and rename to the open objects.
/  >0: Enable file lock function. The value defines how many files/sub-directories
/      can be opened simultaneously under file lock control. Note that the file
/      lock control is independent of re-entrancy. */

/*ѡ��FF_FS_LOCK�����ļ��������ܣ��Կ����ظ��ļ���  
/�ʹ򿪶���ķǷ������� ��FF_FS_READONLYʱ����ѡ�����Ϊ0  
/��1��  
/  
/ 0: Disable file lock function�� Ϊ�˱�����𻵣�Ӧ�ó���  
/Ӧ�ñ���Ƿ��򿪣�ɾ�����������򿪵Ķ���  
/ >0: Enable file lock function�� ��ֵ�����˶����ļ�/��Ŀ¼  
/�������ļ�����������ͬʱ�򿪡� ע�⣬���ļ�  
/ lock���ƶ��������롣 */  


/* #include <somertos.h>	// O/S definitions */					// O/S����*/
#define FF_FS_REENTRANT	0
#define FF_FS_TIMEOUT	1000
#define FF_SYNC_t		HANDLE
/* The option FF_FS_REENTRANT switches the re-entrancy (thread safe) of the FatFs
/  module itself. Note that regardless of this option, file access to different
/  volume is always re-entrant and volume control functions, f_mount(), f_mkfs()
/  and f_fdisk() function, are always not re-entrant. Only file/directory access
/  to the same volume is under control of this function.
/
/   0: Disable re-entrancy. FF_FS_TIMEOUT and FF_SYNC_t have no effect.
/   1: Enable re-entrancy. Also user provided synchronization handlers,
/      ff_req_grant(), ff_rel_grant(), ff_del_syncobj() and ff_cre_syncobj()
/      function, must be added to the project. Samples are available in
/      option/syscall.c.
/
/  The FF_FS_TIMEOUT defines timeout period in unit of time tick.
/  The FF_SYNC_t defines O/S dependent sync object type. e.g. HANDLE, ID, OS_EVENT*,
/  SemaphoreHandle_t and etc. A header file for O/S definitions needs to be
/  included somewhere in the scope of ff.h. */

/*ѡ��FF_FS_REENTRANT�л�fatf�Ŀ�����(�̰߳�ȫ)  
/ģ�鱾�� ע�⣬�������ѡ����ʲô���ļ����ʲ�ͬ  
/ volume���ǿ�������������ƺ�����f_mount()�� f_mkfs()  
/��f_fdisk()���������ǲ�������ġ� ֻ���ļ�/Ŀ¼�ķ���  
/����ͬ����������������ܵĿ����¡�  
/  
/ 0: Disable re- entry�� FF_FS_TIMEOUT��FF_SYNC_t��Ч��  
/ 1:ʹ�����롣 �û����ṩ��ͬ���������  
/ ff_req_grant()�� ff_rel_grant()�� ff_del_syncobj()��ff_cre_syncobj()  
/���ܣ�������ӵ���Ŀ�С� ��������  
/ѡ��/ syscall.c��  
/  
/ FF_FS_TIMEOUT��ʱ�䵥λtick���峬ʱʱ�䡣  
/ FF_SYNC_t������O/S������ͬ���������͡� ����HANDLE, ID, OS_EVENT*��  
/ SemaphoreHandle_t�ȡ� O/S������Ҫһ��ͷ�ļ�  
/������f.h��Χ�ڵ�ĳ���ط��� */  



/*--- End of configuration options ---*/
/*�D�D����ѡ������D�D*/
