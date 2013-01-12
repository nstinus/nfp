#ifndef __NFP__SHMSEGMENT_H__
#define __NFP__SHMSEGMENT_H__

#include <sys/types.h>

namespace NFP
{
    
namespace base
{

class ShmSegment
{

public:
    ShmSegment();
    ShmSegment(std::string, size_t);
    virtual ~ShmSegment();

    virtual int create();
    virtual int attach();
    virtual int detach();
    virtual int remove();
    
    std::string keyFileName() {return keyFileName_;}
    void keyFileName(std::string keyFileName) {if (keyFileName == "") keyFileName_ = keyFileName;}
    
    key_t key() {return key_;}
    void key(key_t key) {if (key_ == -1) key_ = key;}
    
    int shmid() {return shmid_;}
    void shmid(int shmid) {if (shmid_ == -1) shmid_ = shmid;}
    
    void* ptr() {return ptr_;}
    
    int size() {return size_;}
    void size(int size) {if (size_ == 0) size_ = size;}
    
    virtual int save_raw(const char* filename);
    virtual int load_raw(const char* filename);
    
    std::string info();
    
protected:
    std::string keyFileName_;
    key_t key_;
    int shmid_;
    size_t size_;
    void* ptr_;
};

}
}

#endif
