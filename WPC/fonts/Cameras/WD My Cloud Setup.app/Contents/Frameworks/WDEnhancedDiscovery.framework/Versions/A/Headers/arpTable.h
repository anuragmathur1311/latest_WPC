//
//  arpTable.h
//  WD Discovery
//
//  Created by Hugh Dinh on 7/9/12.
//  Copyright (c) 2012 Western Digital. All rights reserved.
//

#import "ARPDiscovery.h"
#import <Foundation/Foundation.h>

int dump(u_long addr);

@interface ARPDiscovery (arpTable)

-(void) parseArp;
@end
