#include <glog/logging.h>

#include <string>
#include <sys/shm.h>

#include "ShmSegment.h"


NFP::ShmSegment::ShmSegment(std::string keyFile, size_t size):
    keyFile_(keyFile),
    size_(size)
{
    create();
    attach();
}

void NFP::ShmSegment::create()
{
    key_ = ftok(keyFile_.c_str(), 'R');
    shmid_ = shmget(key_, size_, 0644 | IPC_CREAT);
    DLOG(INFO) << "ShmSegment created.";
}

void NFP::ShmSegment::attach()
{
    ptr_ = shmat(shmid_, (void *)0, 0);
    if ((char*)(ptr_) == (char*)(-1))
        LOG(ERROR) << "Shm attach failed for ";
    DLOG(INFO) << "ShmSegment attached: " << ptr_;
}

void NFP::ShmSegment::detach()
{
    if (shmdt(ptr_))
        LOG(ERROR) << "Shm detach failed.";
    else {
        ptr_ = NULL;
        DLOG(INFO) << "ShmSegment detached.";
    }
        
}

int NFP::ShmSegment::remove()
{
    detach();
    return shmctl(shmid_, IPC_RMID, NULL);
    DLOG(INFO) << "ShmSegment removed.";
}

