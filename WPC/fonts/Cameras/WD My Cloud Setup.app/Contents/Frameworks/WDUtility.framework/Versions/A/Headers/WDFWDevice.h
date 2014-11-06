//
//  WDFWDevice.h
//  WDUtility
//
//  Created by Bob Hahn on 12/3/13.
//
//

#import <WDUtility/WDDevice.h>

@interface WDFWDevice : WDDevice

- (io_service_t)populateDeviceInformation: (WDDevice *) device;
- (io_service_t)populateDriveInformation;

@end
