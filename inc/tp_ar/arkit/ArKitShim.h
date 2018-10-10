#ifndef tp_ar_ArKitShim_h
#define tp_ar_ArKitShim_h

namespace tp_ar
{

//##################################################################################################
class ArKitShim
{
public:
  //################################################################################################
  ArKitShim();

  //################################################################################################
  virtual ~ArKitShim();

private:
  struct Private;
  Private* d;
  friend struct Private;
};

}

#endif
