#include <glog/logging.h>

#include <string>
#include <sys/shm.h>

#include "ShmSegment.h"

NFP::ShmSegment::ShmSegment(std::string keyFile, size_t size):
    keyFile_(keyFile),
    key_(-1),
    shmid_(-1),
    size_(size),
    ptr_(NULL)
{}

NFP::ShmSegment::~ShmSegment()
{
    detach();
}

void NFP::ShmSegment::create()
{
    key_ = ftok(keyFile_.c_str(), 'R');
    if (size_ == 0 || key_ == -1) {
        LOG(ERROR) << "Cannot create ShmSegment with "
            << "Size: " << size_ << " "
            << "Key: " << key_;
    } else {
        shmid_ = shmget(key_, size_, 0644 | IPC_CREAT);
        DLOG(INFO) << "ShmSegment created.";
    }
}

void NFP::ShmSegment::attach()
{
    if (shmid_ == -1) {
        ptr_ = shmat(shmid_, (void *)0, 0);
        if ((char*)(ptr_) == (char*)(-1))
            LOG(ERROR) << "Shm attach failed for "
                << keyFile_ << " "
                << "Size: " << size_ << " "
                << "Key: " << key_ << " "
                << "Shmid: " << shmid_ << " ";
        DLOG(INFO) << "ShmSegment attached: " << ptr_;
    } else
        LOG(ERROR) << "Attach failed. Perhaps the segment has not been created.";
}

void NFP::ShmSegment::detach()
{
    if(ptr_) {
        if (shmdt(ptr_))
            LOG(ERROR) << "Shm detach failed.";
        else {
            ptr_ = NULL;
            DLOG(INFO) << "ShmSegment detached.";
        }
    } else {
        LOG(ERROR) << "Cannot detach: ptr_ == NULL.";
    }
}

int NFP::ShmSegment::remove()
{
    detach();
    int ret = shmctl(shmid_, IPC_RMID, NULL);
    if (ret) {
        size_ = 0;
        shmid_ = -1;
            DLOG(INFO) << "ShmSegment removed.";
    } else
        LOG(ERROR) << "Failed to remove ShmSegment.";
    return ret;
}

