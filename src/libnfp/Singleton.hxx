#include <glog/logging.h>

namespace NFP
{
namespace base
{

template <typename T>
class Singleton
{
protected:
  // Constructeur/destructeur
  Singleton() {}
  ~Singleton() {}

public:
  // Interface publique
  static T* instance() {
      if (NULL == _singleton) _singleton = new T;
      return (static_cast<T*> (_singleton));
  }

  static void kill () {
      if (NULL != _singleton) {
          delete _singleton;
          _singleton = NULL;
      }
  }

private:
    static T* _singleton;
};

template <typename T>
T* Singleton<T>::_singleton = NULL;

}
}