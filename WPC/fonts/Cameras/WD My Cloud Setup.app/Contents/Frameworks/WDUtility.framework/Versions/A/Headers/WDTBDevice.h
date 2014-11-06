//
//  WDTBRAIDDevice.h
//  WDUtility
//
//  Created by Bob Hahn on 12/3/13.
//
//

#import <WDUtility/WDDevice.h>
#import "WDDiskSWRaid.h"

@interface WDTBDevice : WDDevice
{
//io_service_t				AppleAHCInterface;			// SWRAID - Start the search for disk drives here.
int							mThunderboltPortNumber;     //
}

//@property (readwrite,assign) io_service_t			AppleAHCInterface;
@property (readwrite,assign) int					mThunderboltPortNumber;

- (io_service_t) populateDriveInformation;

@end
