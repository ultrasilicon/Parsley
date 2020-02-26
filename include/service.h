#ifndef AGIO_SERVICE_H
#define AGIO_SERVICE_H

#include "loop.h"

A_NS_BEGIN

template <typename AType>
struct AgioServiceData
{
  AType* agioObj;
  void* data;
  // ...
};

template <typename UvType, typename AType>
class AgioService
    : public AgioObject<UvType, AType>
{
public:
  using AgioObjectT = AgioObject<UvType, AType>;
  using AgioServiceDataT = AgioServiceData<AType>;

  static AType* getAgioService(UvType* handle)
  {
    return static_cast<AgioServiceDataT*>(handle->data)->agioObj;
  }

  AgioService(Loop* loop, AType* agioObj)
    : AgioObjectT()
    , loop_(loop)
    , service_data_(new AgioServiceDataT{agioObj})
  {
    this->obj_->data = service_data_;
  }

  ~AgioService()
  {
    if(service_data_)
      delete service_data_;
  }

  template<typename AgioServiceDataT>
  AgioServiceDataT* serviceData()
  {
    return this->obj_->data;
  }

  Loop* loop()
  {
    return loop_;
  }

protected:
  //! This class keeps a pointer to AgioObject<UvType, AType>::obj_->data.
  //! Because uv_loop might not free the data pointer when destructing objects.
  //! Above is not proved, if crash on destruct happens, try removing the delete in ~AgioService().
  AgioServiceDataT* service_data_;
  Loop* loop_;
};


A_NS_END
#endif // AGIO_SERVICE_H
