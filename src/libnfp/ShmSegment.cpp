#include <glog/logging.h>

#include <string>
#include <sys/shm.h>
//#include <sys/ipc.h>
#include <errno.h>

#include "ShmSegment.h"

NFP::ShmSegment::ShmSegment() :
    keyFile_(""),
    key_(-1),
    shmid_(-1),
    size_(0),
    ptr_(NULL)
{}

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
        shmid_ = shmget(key_, size_, 0600 | IPC_CREAT);
        if (shmid_ == -1) {
            LOG(ERROR) << "Segment creation failed - "
                << strerror(errno) << " [" << errno << "]";
            LOG(ERROR) << info();
        }
        else
            DLOG(INFO) << "ShmSegment created.";
    }
}

void NFP::ShmSegment::attach()
{
    if (shmid_ != -1) {
        ptr_ = shmat(shmid_, (void *)0, 0);
        if ((char*)(ptr_) == (char*)(-1))
            LOG(ERROR) << "Shm attach failed for "
                << info();
        DLOG(INFO) << "ShmSegment attached: " << ptr_;
    } else
        LOG(ERROR) << "Attach failed. Perhaps the segment has not been created.";
}

void NFP::ShmSegment::detach()
{
    if(ptr_) {
        if (shmdt(ptr_)) {
            LOG(ERROR) << "Shm detach failed.";
            LOG(ERROR) << info();
        }
        else {
            ptr_ = NULL;
            DLOG(INFO) << "ShmSegment detached.";
        }
    }/* else {
        LOG(ERROR) << "Cannot detach: ptr_ == NULL.";
        LOG(ERROR) << info();
    }*/
}

int NFP::ShmSegment::remove()
{
    detach();
    int ret = shmctl(shmid_, IPC_RMID, NULL);
    if (ret == 0) {
        size_ = 0;
        shmid_ = -1;
            DLOG(INFO) << "ShmSegment removed.";
    } else {
        LOG(ERROR) << "Failed to remove ShmSegment. " << ret << " - "
            << strerror(errno) << " [" << errno << "]";
        LOG(ERROR) << info();
    }
    return ret;
}

std::string NFP::ShmSegment::info()
{
    char* msg = new char[255];
    sprintf(msg, "%s Size: %d Key: %d ShmId: %d",
        keyFile_.c_str(),
        (int)size_,
        (int)key_,
        (int)shmid_);
    return std::string(msg);
    //delete[] msg;
}

