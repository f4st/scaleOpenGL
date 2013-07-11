

#import "OpenGLView.h"


@implementation OpenGLView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}

- (void)setupContext {   
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    [EAGLContext setCurrentContext:_context];
}

- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];    
}

- (void)setupFrameBuffer {    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);   
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
}

- (void)setScale:(CGPoint)scale
{
    _viewer.setScale(scale.x, scale.y);
}

- (void)setOffset:(CGPoint)offset
{
    _viewer.setOffset(offset.x, offset.y);
}

- (void)render:(CADisplayLink*)displayLink
{
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glViewport(0,0,768.0,1004.0);
    _viewer.render();
    
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];    
}

- (void) updateTexture:(const unsigned char *)texture withRects:(const std::vector<fs::Rect>&)rects
{
    _viewer.updateImage(texture, rects);
}

- (void)setupTexture:(const unsigned char *)texture withWidth:(int) width andHeight:(int) height
{
    bool needResize = _viewer.didSizeChanged(width,height);
    bool wasCreatedBefore = _viewer.initTexture(width, height);
    
    if(!wasCreatedBefore || needResize)
    {
        _viewer.setupVBOs();
        _viewer.updateImage(texture, width, height);
    }
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {        
        [self setupLayer];
        [self setupContext];    
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        _viewer.compileShaders();
        [self setupDisplayLink];
    }
    return self;
}

- (void)dealloc
{
    [_context release];
    _context = nil;
    [super dealloc];
}


@end
