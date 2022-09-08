/*---------------------------------------------------------------------------/
/  FatFs Functional Configurations---fatf功能配置
/---------------------------------------------------------------------------*/

#define FFCONF_DEF	86631	/* Revision ID ---修订ID */


/*---------------------------------------------------------------------------/
/ Function Configurations---功能配置
/---------------------------------------------------------------------------*/

#define FF_FS_READONLY	0
/* This option switches read-only configuration. (0:Read/Write or 1:Read-only)
/  Read-only configuration removes writing API functions, f_write(), f_sync(),
/  f_unlink(), f_mkdir(), f_chmod(), f_rename(), f_truncate(), f_getfree()
/  and optional writing functions as well. */

/*该选项切换只读配置。 (0:读/写或1:只读)  
/只读配置删除写API函数，f_write()， f_sync()，  
/ f_unlink (), f_mkdir (), f_chmod (), f_rename (), f_truncate (), f_getfree ()  
/和可选的写入函数。*/  

#define FF_FS_MINIMIZE	0
/* This option defines minimization level to remove some basic API functions.
/
/   0: Basic functions are fully enabled.
/   1: f_stat(), f_getfree(), f_unlink(), f_mkdir(), f_truncate() and f_rename()
/      are removed.
/   2: f_opendir(), f_readdir() and f_closedir() are removed in addition to 1.
/   3: f_lseek() function is removed in addition to 2. */

/*这个选项定义了最小化级别，以删除一些基本的API函数。  
/  
/ 0:基本功能完全启用。  
/ 1: f_stat (), f_getfree (), f_unlink (), f_mkdir (), f_truncate()和f_rename ()被删除。  
/ 2: f_opendir()， f_readdir()和f_closedir()在1的基础上被删除除了。  
/ 3:函数f_lseek在2的基础上被删除。*/


#define FF_USE_FIND		0
/* This option switches filtered directory read functions, f_findfirst() and
/  f_findnext(). (0:Disable, 1:Enable 2:Enable with matching altname[] too) */

/*这个选项切换过滤后的目录读取函数，f_findfirst()和  
/ f_findnext()。 (0:Disable, 1:Enable 2:Enable with matching altname[] too) */  


#define FF_USE_MKFS		0
/* This option switches f_mkfs() function. (0:Disable or 1:Enable) */
/*这个选项切换f_mkfs()函数。 (0:禁用或1:启用)*/  


#define FF_USE_FASTSEEK	0
/* This option switches fast seek function. (0:Disable or 1:Enable) */
 /*这个选项切换快速查找功能。 (0:禁用或1:启用)*/  


#define FF_USE_EXPAND	0
/* This option switches f_expand function. (0:Disable or 1:Enable) */
/*该选项切换f_expand函数。 (0:禁用或1:启用)*/  


#define FF_USE_CHMOD	0
/* This option switches attribute manipulation functions, f_chmod() and f_utime().
/  (0:Disable or 1:Enable) Also FF_FS_READONLY needs to be 0 to enable this option. */

/*这个选项开关属性操作函数，f_chmod()和f_utime()。  
/ (0:Disable或1:Enable)并且FF_FS_READONLY也需要设置为0才能启用该选项。*/  


#define FF_USE_LABEL	0
/* This option switches volume label functions, f_getlabel() and f_setlabel().
/  (0:Disable or 1:Enable) */

/*切换卷标签函数，f_getlabel()和f_setlabel()。  
/(0:禁用或1:启用)*/  

#define FF_USE_FORWARD	0
/* This option switches f_forward() function. (0:Disable or 1:Enable) */
/*这个选项切换f_forward()函数。 (0:禁用或1:启用)*/  


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

/* FF_USE_STRFUNC开关字符串函数，f_gets()， f_putc()， f_puts()和  
/ f_printf()。  
/  
/ 0:禁用。 FF_PRINT_LLI, FF_PRINT_FLOAT和FF_STRF_ENCODE无效。  
/ 1: Enable without LF-CRLF conversion。  
/ 2: Enable with LF-CRLF conversion。  
/  
/ FF_PRINT_LLI = 1使f_printf()支持长参数和FF_PRINT_FLOAT = 1/2  
使f_printf()支持浮点参数。 这些特性需要C99或更高版本。  
/当启用LFN时FF_LFN_UNICODE >= 1，字符串函数转换字符  
/编码。 FF_STRF_ENCODE选择在文件上进行字符编码的假设  
/可以通过这些函数读取/写入。  
/  
/ 0: ANSI/OEM在当前CP  
/ 1: Unicode UTF-16LE  
/ 2: UTF-16BE格式的Unicode  
/ 3: UTF-8格式的Unicode  
*/  

/*---------------------------------------------------------------------------/
/ Locale and Namespace Configurations---Locale和Namespace配置
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

/*该选项指定要在目标系统上使用的OEM代码页。  
/不正确的代码页设置可能导致文件打开失败。  
/  
/ 437 -美国  
/ 720 -阿拉伯语  
/ 737 -希腊语  
/ 771 -九龙  
775 -波罗的海  
/ 850 -拉丁语  
/ 852 -拉丁语2  
/ 855 -西里尔文  
/ 857 -土耳其语  
/ 860 -葡萄牙语  
/ 861 -冰岛语  
/ 862 -希伯来语  
/ 863 -加拿大法语  
/ 864 -阿拉伯语  
/ 865 -北欧  
/ 866 -俄语  
/ 869 -希腊文  
/ 932 -日文(DBCS)  
/ 936 -简体中文(DBCS)  
/ 949 -韩语(DBCS)  
/ 950 -繁体中文(DBCS)  
/ 0 -包含所有代码页上面和配置的f_setcp()  
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

/* FF_USE_LFN开关支持LFN(长文件名)。  
/  
/ 0: Disable LFN。 FF_MAX_LFN无效。  
/ 1:在BSS上使能LFN with static working buffer。 总是不是线程安全的。  
/ 2: Enable LFN with dynamic working buffer on the STACK。  
/ 3:启用LFN with dynamic working buffer on the HEAP。  
/  
/要启用LFN，需要将ffunicode.c添加到项目中。 LFN函数  
/要求一定的内部工作缓冲区占用(FF_MAX_LFN + 1) * 2字节和  
/附加(FF_MAX_LFN + 44) / 15 * 32字节时，exFAT启用。  
/ FF_MAX_LFN以UTF-16编码单元定义了工作缓冲区的大小，它可以  
/的取值范围是12 ~ 255。 建议设置为255，以完全支持LFN  
/规范。  
/当使用堆栈作为工作缓冲区时，注意栈溢出。 当使用堆  
/内存工作缓冲区，内存管理函数，ff_memalloc()和  
/ ff_memfree()示例在ffsystem.c中，需要添加到项目中。 */  


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

/*当LFN被启用时，该选项在API上切换字符编码。  
/  
/ 0: ANSI/OEM在当前CP (TCHAR = char)  
/ 1: Unicode在UTF-16 (TCHAR = WCHAR)  
/ 2: Unicode在UTF-8 (TCHAR = char)  
/ 3: Unicode在UTF-32 (TCHAR = DWORD)  
/  
此外，字符串I/O函数的行为将受此选项的影响。  
/当LFN未启用时，该选项无效。 */ 


#define FF_LFN_BUF		255
#define FF_SFN_BUF		12
/* This set of options defines size of file name members in the FILINFO structure
/  which is used to read out directory items. These values should be suffcient for
/  the file names to read. The maximum possible length of the read file name depends
/  on character encoding. When LFN is not enabled, these options have no effect. */

/*这个选项集定义了FILINFO结构中文件名成员的大小  
/用来读出目录项。 这些值应该足够了  
/读取文件名。 读取文件名的最大可能长度取决于  
/打开字符编码。 当LFN未启用时，这些选项不起作用。 */  


#define FF_FS_RPATH		0
/* This option configures support for relative path.
/
/   0: Disable relative path and remove related functions.
/   1: Enable relative path. f_chdir() and f_chdrive() are available.
/   2: f_getcwd() function is available in addition to 1.
*/

/*该选项配置相对路径支持。  
/  
/ 0:禁用相对路径，移除相关功能。  
/ 1:启用相对路径。 F_chdir()和f_chdrive()是可用的。  
/ 2: f_getcwd()函数在1之外可用。  
*/  


/*---------------------------------------------------------------------------/
/ Drive/Volume Configurations---驱动/卷配置
/---------------------------------------------------------------------------*/

#define FF_VOLUMES		1
/* Number of volumes (logical drives) to be used. (1-10) */
/*要使用的卷数(逻辑驱动器)。 (1) */  


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

/* FF_STR_VOLUME_ID开关支持卷ID为任意字符串。  
/当FF_STR_VOLUME_ID设置为1或2时，可使用任意字符串作为驱动器  
路径名中的“/ number”。 FF_VOLUME_STRS为每个卷定义了卷ID字符串  
/逻辑驱动器。 项目数量不能小于FF_VOLUMES。 有效的  
卷ID字符串的/字符是A-Z、A-Z和0-9，但是，它们是  
/比较不区分大小写。 如果FF_STR_VOLUME_ID >= 1，且FF_VOLUME_STRS为  
/未定义，用户定义的卷字符串表需要定义为:  
/  
/ const char * VolumeStr [FF_VOLUMES] ={“内存”,“闪电”、“sd”、“usb”……  
*/  

#define FF_MULTI_PARTITION	0
/* This option switches support for multiple volumes on the physical drive.
/  By default (0), each logical drive number is bound to the same physical drive
/  number and only an FAT volume found on the physical drive will be mounted.
/  When this function is enabled (1), each logical drive number can be bound to
/  arbitrary physical drive and partition listed in the VolToPart[]. Also f_fdisk()
/  funciton will be available. */

/*该选项切换对物理驱动器上多个卷的支持。  
默认情况下(0)，每个逻辑盘号都绑定到同一个物理盘  
只有在物理驱动器上找到的FAT卷才会被挂载。  
/开启该功能时(1)，每个逻辑盘号都可以被绑定  
volpartart[]中列出的/任意物理驱动器和分区。 还f_fdisk ()  
/功能将可用。 */  


#define FF_MIN_SS		512
#define FF_MAX_SS		512
/* This set of options configures the range of sector size to be supported. (512,
/  1024, 2048 or 4096) Always set both 512 for most systems, generic memory card and
/  harddisk, but a larger value may be required for on-board flash memory and some
/  type of optical media. When FF_MAX_SS is larger than FF_MIN_SS, FatFs is configured
/  for variable sector size mode and disk_ioctl() function needs to implement
/  GET_SECTOR_SIZE command. */

/*这个选项集配置扇区大小的范围被支持。 (512年,  
/ 1024, 2048或4096)总是设置512为大多数系统，通用记忆卡和  
/硬盘，但可能需要更大的值为板载闪存和一些  
/光学介质类型。 当FF_MAX_SS大于FF_MIN_SS时，配置FatFs  
/为可变扇区大小模式和disk_ioctl()函数需要实现  
/ GET_SECTOR_SIZE命令。 */  


#define FF_LBA64		0
/* This option switches support for 64-bit LBA. (0:Disable or 1:Enable)
/  To enable the 64-bit LBA, also exFAT needs to be enabled. (FF_FS_EXFAT == 1) */

/*该选项切换支持64位LBA。 (0:禁用或1:启用)  
/要启用64位LBA，还需要启用exFAT。 (ff_fs_exfat == 1) */  


#define FF_MIN_GPT		0x10000000
/* Minimum number of sectors to switch GPT as partitioning format in f_mkfs and
/  f_fdisk function. 0x100000000 max. This option has no effect when FF_LBA64 == 0. */

/*在f_mkfs和中切换GPT为分区格式的最小扇区数  
/ f_fdisk函数。 0 x100000000 max。 当FF_LBA64 == 0时，此选项无效。 */  


#define FF_USE_TRIM		0
/* This option switches support for ATA-TRIM. (0:Disable or 1:Enable)
/  To enable Trim function, also CTRL_TRIM command should be implemented to the
/  disk_ioctl() function. */

/*此选项切换对ATA-TRIM的支持。 (0:禁用或1:启用)  
要使能Trim功能，还需要对系统执行CTRL_TRIM命令  
/ disk_ioctl()函数。 */  



/*---------------------------------------------------------------------------/
/ System Configurations---系统配置
/---------------------------------------------------------------------------*/

#define FF_FS_TINY		0
/* This option switches tiny buffer configuration. (0:Normal or 1:Tiny)
/  At the tiny configuration, size of file object (FIL) is shrinked FF_MAX_SS bytes.
/  Instead of private sector buffer eliminated from the file object, common sector
/  buffer in the filesystem object (FATFS) is used for the file data transfer. */

/*这个选项切换微小的缓冲区配置。 (0:正常或1:小)  
/在微小配置下，文件对象(FIL)的大小被压缩为FF_MAX_SS字节。  
/从文件对象中删除私有部门缓冲区，而不是普通部门  
文件系统对象(FATFS)中的/ buffer用于文件数据传输。 */  


#define FF_FS_EXFAT		0
/* This option switches support for exFAT filesystem. (0:Disable or 1:Enable)
/  To enable exFAT, also LFN needs to be enabled. (FF_USE_LFN >= 1)
/  Note that enabling exFAT discards ANSI C (C89) compatibility. */

/*这个选项切换对exFAT文件系统的支持。 (0:禁用或1:启用)  
/要启用exFAT，也需要启用LFN。 (FF_USE_LFN > = 1)  
/请注意，启用exFAT将丢弃ANSI C (C89)兼容性。 */  


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

/*选项FF_FS_NORTC开关时间戳功能。 如果系统没有  
/不需要任何RTC函数或有效的时间戳，将FF_FS_NORTC = 1设置为disable  
/时间戳函数。 被fatf修改的每个对象都有固定的时间戳  
/由本地时间的FF_NORTC_MON、FF_NORTC_MDAY和FF_NORTC_YEAR定义。  
/要启用时间戳函数(FF_FS_NORTC = 0)， get_fattime()函数需要是  
/添加到项目读取当前时间从实时时钟。 FF_NORTC_MON,  
/ FF_NORTC_MDAY和FF_NORTC_YEAR无效。  
/这些选项在只读配置(FF_FS_READONLY = 1)下无效*/  


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

/*如果你需要知道正确的空闲空间在FAT32卷，设置位0  
/选项，以及f_getfree()函数将强制挂载卷  
/全脂肪扫描。 第1位控制最后分配的集群号的使用。  
/  
/ bit0=0:如果可用，在FSINFO中使用空闲集群数。  
/ bit0=1: Do not trust free cluster count in the FSINFO。  
/ bit1=0:如果可用，使用FSINFO中最后分配的集群号。  
/ bit1=1:不信任FSINFO中最后一次分配的集群号。*/  


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

/*选项FF_FS_LOCK开关文件锁定功能，以控制重复文件打开  
/和打开对象的非法操作。 当FF_FS_READONLY时，该选项必须为0  
/是1。  
/  
/ 0: Disable file lock function。 为了避免卷损坏，应用程序  
/应该避免非法打开，删除并重命名打开的对象。  
/ >0: Enable file lock function。 该值定义了多少文件/子目录  
/可以在文件锁定控制下同时打开。 注意，该文件  
/ lock控制独立于重入。 */  


/* #include <somertos.h>	// O/S definitions */					// O/S定义*/
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

/*选项FF_FS_REENTRANT切换fatf的可重入(线程安全)  
/模块本身。 注意，不管这个选项是什么，文件访问不同  
/ volume总是可重入和音量控制函数，f_mount()， f_mkfs()  
/和f_fdisk()函数，总是不可重入的。 只有文件/目录的访问  
/到相同的音量都在这个功能的控制下。  
/  
/ 0: Disable re- entry。 FF_FS_TIMEOUT和FF_SYNC_t无效。  
/ 1:使能重入。 用户还提供了同步处理程序，  
/ ff_req_grant()， ff_rel_grant()， ff_del_syncobj()和ff_cre_syncobj()  
/功能，必须添加到项目中。 样本可于  
/选项/ syscall.c。  
/  
/ FF_FS_TIMEOUT以时间单位tick定义超时时间。  
/ FF_SYNC_t定义了O/S依赖的同步对象类型。 例如HANDLE, ID, OS_EVENT*，  
/ SemaphoreHandle_t等。 O/S定义需要一个头文件  
/包括在f.h范围内的某个地方。 */  



/*--- End of configuration options ---*/
/*――配置选项结束――*/
