#ifndef __NFP__SHMSEGMENT_H__
#define __NFP__SHMSEGMENT_H__

namespace NFP
{

class ShmSegment
{

public:
    ShmSegment();
    ShmSegment(std::string, size_t);
    virtual ~ShmSegment();

    virtual void create();
    virtual void attach();
    virtual void detach();
    virtual int remove();
    
    std::string keyFile() {return keyFile_;}
    void keyFile(std::string keyFile) {if (keyFile == "") keyFile_ = keyFile;}
    
    key_t key() {return key_;}
    void key(key_t key) {if (key_ == -1) key_ = key;}
    
    int shmid() {return shmid_;}
    void shmid(int shmid) {if (shmid_ == -1) shmid_ = shmid;}
    
    void* ptr() {return ptr_;}
    
    int size() {return size_;}
    void size(int size) {if (size_ == 0) size_ = size;}
    
    std::string info();
    
protected:
    std::string keyFile_;
    key_t key_;
    int shmid_;
    size_t size_;
    void* ptr_;
};

}

#endif