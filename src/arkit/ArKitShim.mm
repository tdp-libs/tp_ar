//#import <ARKitShim.h>
#import <ARKit/ARKit.h>

//@implementation ARKitShim



//@end


#include "ARKitShim.hpp"
#import "ARKitShim.h"
namespace abc
{
  struct WidgetImpl
  {
    ARSession* wrapped;
  };
  Widget::Widget() :
    impl(new WidgetImpl)
  {
    impl->wrapped = [[ARSession alloc] init];
  }
  Widget::~Widget()
  {
    if (impl)
      [impl->wrapped release];
    delete impl;
  }
  void Widget::Reticulate()
  {
    //[impl->wrapped reticulate];
  }
}
