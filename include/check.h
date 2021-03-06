#ifndef PCHECK_H
#define PCHECK_H

#include "callback.h"
#include "service.h"

A_NS_BEGIN

class Check
    : public AgioService<uv_check_t, Check>
{
  static void checkCb(uv_check_t* r);

public:
  Callback<void()> onCalled;

  Check(Loop* l);
  ~Check();
  int start();
  int stop();
};



A_NS_END
#endif // PCHECK_H
