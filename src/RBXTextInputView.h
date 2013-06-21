//
//  RBXTextInputView.h
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#import <Cocoa/Cocoa.h>
#import "rb_plugin.h"
#import "rb_plugin_cocoa.h"

@interface XOJTextInputView : NSView <NSTextInputClient> {
	REALcontrolInstance control;
}

- (id)initWithControlInstance:(REALcontrolInstance)controlInstance;
- (void)cleanup;

@end

