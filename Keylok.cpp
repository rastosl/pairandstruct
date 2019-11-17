#include "Keylok.h"

//#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

//#define BOOST_FILESYSTEM_NO_DEPRECATED
//#include <boost/filesystem.hpp>


Keylok::IpSync::IpSync(IpSync&& other) noexcept :
    ipMutex(std::move(other.ipMutex)),
    ipLock(std::move(other.ipLock))
{
}

Keylok::IpSync& Keylok::IpSync::operator=(IpSync&& other) noexcept
{
    if (this != &other) {
        ipMutex = std::move(other.ipMutex);
        ipLock = std::move(other.ipLock);
    }
    return *this;
}

Keylok::IpSync::~IpSync()
{
}

LockedKeylok Keylok::getInstance()
{
    //auto mutexLockPair = getMutexLockPair();
    auto ipSync = getIpSync();
    static Keylok fortress;
    //return LockedKeylok(std::move(mutexLockPair), &fortress);
    return LockedKeylok(std::move(ipSync), &fortress);
}

//Keylok::MutexLockPair Keylok::getMutexLockPair()
//{
//    MutexLockPair mutexLockPair;
//    mutexLockPair.first = std::make_unique<InterprocessMutex>(boost::interprocess::open_or_create, "phokeylok");
//    mutexLockPair.second = std::make_unique<InterprocessLock>(*mutexLockPair.first);
//    return mutexLockPair;
//}

Keylok::IpSync Keylok::getIpSync() {
    IpSync ipSync;
    ipSync.ipMutex = std::make_unique<InterprocessMutex>(boost::interprocess::open_or_create, "phokeylok");
    ipSync.ipLock = std::make_unique<InterprocessLock>(*ipSync.ipMutex);
    return ipSync;
}

//LockedKeylok::LockedKeylok(Keylok::MutexLockPair&& mutexLockPair, Keylok* keylokInstance) noexcept :
//    mutexLockPair(std::move(mutexLockPair)),
//    keylokInstance(keylokInstance) 
//{
//}

LockedKeylok::LockedKeylok(Keylok::IpSync&& ipSync, Keylok* keylokInstance) noexcept :
    ipSync(std::move(ipSync)),
    keylokInstance(keylokInstance)
{
}

LockedKeylok::LockedKeylok(LockedKeylok&& other) noexcept :
    //mutexLockPair(std::move(other.mutexLockPair)),
    ipSync(std::move(ipSync)),
    keylokInstance(other.keylokInstance)
{
    other.keylokInstance = nullptr;
}

LockedKeylok::~LockedKeylok()
{
}
