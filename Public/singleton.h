#ifndef SINGLETON_H
#define SINGLETON_H
#include <assert.h>

/**
 * Singleton class facilitiates turning any class into a Singleton (one instance) object
 * Bad practice to throw something on the heap like that but some times it must be done
 */
template <class T>
class Singleton
{
public:
   /**
   * @brief Create or get the instance of the object in memory
   * @return
   */
  static T* Instance()
  {
      if(!m_pInstance) m_pInstance = new T;
      assert(m_pInstance != NULL);
      return m_pInstance;
  }

protected:
  Singleton();
  ~Singleton();
private:
  Singleton(Singleton const&);
  Singleton& operator=(Singleton const&);
  static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance=NULL;

#endif // SINGLETON_H
