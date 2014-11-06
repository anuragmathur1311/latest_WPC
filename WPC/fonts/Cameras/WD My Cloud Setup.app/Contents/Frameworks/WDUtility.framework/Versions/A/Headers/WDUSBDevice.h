//
//  WDUSBDevice.h
//  WDUtility
//
//  Created by Bob Hahn on 12/3/13.
//
//

#import <WDUtility/WDDevice.h>

@interface WDUSBDevice : WDDevice
{
    int             mRAIDMode;
    int             mRAIDStatus;
    NSString *		mRAIDSetName;
    NSString *		mRAIDFormat;
    NSNumber *		mRAIDRebuildProgress;
}

@property (readwrite,assign) int            mRAIDMode;
@property (readwrite,copy) NSString *		mRAIDSetName;
@property (readwrite,assign) int            mRAIDStatus;
@property (readwrite,copy) NSString *		mRAIDFormat;
@property (readwrite,copy) NSNumber *		mRAIDRebuildProgress;

- (io_service_t)findSESDevice: (WDDevice *) device;
- (io_service_t)populateDeviceInformation: (WDDevice *) device;
- (io_service_t)populateDriveInformation;

//- (BOOL)reEnumerateUSBDevice;
- (void) initalizeDiskWithVolumeName: (NSString *)volumeName format: (NSString *)volumeFormat;

@end
