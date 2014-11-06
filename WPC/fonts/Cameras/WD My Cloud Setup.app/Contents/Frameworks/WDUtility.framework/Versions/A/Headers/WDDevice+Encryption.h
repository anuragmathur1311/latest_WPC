//
//  Encryption.h
//  DriveUnlock
//
//  Created by Bob on 3/12/09.
//  Copyright © 2014 Western Digital Technologies, Inc. All rights reserved.
//

#import "WDDevice.h"
#import "WDDMcommon.h"
#import <CommonCrypto/CommonDigest.h>

#define MAX_COOKED_PASSWORD_SIZE	32

//enum {
//	SEC_STAT_EncryptionOff      = 0,
//	SEC_STAT_Locked             = 1,
//	SEC_STAT_Unlocked           = 2,
//	SEC_STAT_UnlocksExceeded    = 6,
//	SEC_STAT_NoDEK              = 7
//};
//
//enum {
//    NO_CIPHER		= 0x00,
//	AES_128_ECB		= 0x10,
//	AES_128_CBC		= 0x12,
//	AES_128_XTS		= 0x18,
//
//	AES_256_ECB		= 0x20,
//	AES_256_CBC		= 0x22,
//	AES_256_XTS		= 0x28,
//    FULL_DISK_ENCRYPTION = 0x30
//};
//
//typedef struct {
//	UInt8		Signature;
//	UInt8		reserved1[2];
//	UInt8		SecurityState;
//	UInt8		CurrentCipherID;
//	UInt8		reserved2;
//	UInt16		PasswordLength;
//	UInt8		KeyResetEnabler[4];
//	UInt8		reserved3[3];
//	UInt8		NumberOfCiphers;
//	UInt8		CipherList[16];
//} EncryptStatusReturnData;

@interface WDDevice (Encryption)

- (int) setPassword: (NSString *) password error:(NSMutableDictionary **)errorDict;
- (int) changePasswordFrom: (NSString *) oldPassword to: (NSString *) newPassword error:(NSMutableDictionary **)errorDict;
- (int) clearPassword: (NSString *) password error:(NSMutableDictionary **)errorDict;
- (int) setPasswordHint: (NSString *) passwordHint error:(NSMutableDictionary **)errorDict;
- (int) unlockWithPassword: (NSString *) password error:(NSMutableDictionary **)errorDict;
- (NSString *) passwordHint:(NSMutableDictionary **)errorDict;
- (UInt8) encryptionStatus:(NSMutableDictionary **)errorDict;
- (UInt8) encryptionStatusForLun: (UInt32)lun error: (NSMutableDictionary **)errorDict;

- (int) setPasswordParametersInBlock: (int)blockNumber salt:(NSString *) saltString iterations:(UInt32)iterationCount;
- (int) cookPassword: (NSString *)password retpassword: (UInt8 *)cookedPassword;
- (NSString *) passwordSalt;
- (UInt32) passwordIterations;
- (UInt16) encryptionPasswordLengthInBytes;
- (void) EncryptWithSHA256:(void *)data length:(UInt32)length digest:(unsigned char *)md;
- (int) resetDEK:(NSMutableDictionary **)errorDict;
-(int) resetDEKonVolumeSet: (UInt32)volumeSetNumber error: (NSMutableDictionary **)errorDict;

@end
