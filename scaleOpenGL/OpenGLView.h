

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "GLViewer.h"

@interface OpenGLView : UIView {
    CAEAGLLayer* _eaglLayer;
    EAGLContext* _context;
    GLuint _colorRenderBuffer;
    GLViewer _viewer;
}

- (void)setupTexture:(const unsigned char *)texture withWidth:(int) width andHeight:(int) height;

- (void)updateTexture:(const unsigned char *)texture withRects:(const std::vector<fs::Rect>&) rects;

- (void)setScale:(CGPoint)scale;

- (void)setOffset:(CGPoint)offset;

@end
