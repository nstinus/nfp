#ifndef __NFP__SHMSEGMENT_H__
#define __NFP__SHMSEGMENT_H__

namespace NFP
{

class ShmSegment
{

public:
    ShmSegment(std::string, size_t);
    virtual ~ShmSegment() {}

    virtual void create();
    virtual void attach();
    virtual void detach();
    virtual int remove();
    
    std::string keyFile() {return keyFile_;}
    key_t key() {return key_;}
    int shmid() {return shmid_;}
    void* ptr() {return ptr_;}
    int size() {return size_;}
    
private:
    std::string keyFile_;
    key_t key_;
    int shmid_;
    void* ptr_;
    size_t size_;
    
};

}

#endif