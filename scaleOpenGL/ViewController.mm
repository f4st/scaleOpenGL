//
//  ViewController.m
//  scaleOpenGL
//
//  Created by Mykola Farion on 5/7/13.
//  Copyright (c) 2013 Mykola Farion. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLView.h"
@interface ViewController ()

@property OpenGLView *glView;

@property CGFloat lastScale;
@property CGPoint previousOffset;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    _glView = [[OpenGLView alloc]initWithFrame:self.view.frame];
    [self.view addSubview:_glView];
    
    int height = self.view.frame.size.height;
    int width = self.view.frame.size.width;
    int i, j;//, c;
    unsigned char fullIm[width*height*4];
    
    for (i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            
            fullIm[(i*width+j)*4] = (GLubyte) 2*i;
            fullIm[(i*width+j)*4+1] = (GLubyte) 2*j;
            fullIm[(i*width+j)*4+2] = (GLubyte) (255-i)+(255-j);
            fullIm[(i*width+j)*4+3] = (GLubyte) 255;
        }
    }
    [_glView setupTexture:fullIm withWidth:width andHeight:height];
    UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(scale:)];
    [pinch setDelegate:self];
    [self.view addGestureRecognizer:pinch];
    
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(move:)];
    [pan setDelegate:self];
    [self.view addGestureRecognizer:pan];
	// Do any additional setup after loading the view, typically from a nib.
}

-(void)move:(UIPanGestureRecognizer *)sender {
    if((sender.state == UIGestureRecognizerStateChanged) || (sender.state == UIGestureRecognizerStateEnded))
    {
        CGPoint offset = [sender translationInView:_glView];
        CGPoint convertedOffset;
        convertedOffset.x = _previousOffset.x + offset.x/_glView.frame.size.width;
        convertedOffset.y = _previousOffset.y - offset.y/_glView.frame.size.height;
        [_glView setOffset:convertedOffset];
        _previousOffset = convertedOffset;
    }
}

-(void)scale:(UIPinchGestureRecognizer *)sender {
    //test
    /*if([(UIPinchGestureRecognizer*)sender state] == UIGestureRecognizerStateBegan) {
        _lastScale = 1.0;
    }
    CGFloat scale = 1.0 - (_lastScale - [(UIPinchGestureRecognizer*)sender scale]);*/
    
    if((sender.state == UIGestureRecognizerStateChanged) || (sender.state == UIGestureRecognizerStateEnded))
    {
        if(!_lastScale) {
            _lastScale = 1.0;
        }
        CGFloat scale = _lastScale*sender.scale;
        CGPoint scaleP;
        scaleP.x = scaleP.y = scale;
        [_glView setScale:scaleP];
        _lastScale = scale;
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
