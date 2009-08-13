#ifndef __NOTIFY_STREAM_H__
#define __NOTIFY_STREAM_H__
// Copyright (c) 2009 - Decho Corp.

#include <boost/function.hpp>

#include "filter.h"

class NotifyStream : public FilterStream
{
public:
    NotifyStream(Stream::ptr parent, bool own = true)
        : FilterStream(parent, own)
    {}

    boost::function<void ()> notifyOnClose;
    boost::function<void ()> notifyOnEof;
    boost::function<void ()> notifyOnException;

    void close(CloseType type = BOTH)
    {
        try {
            parent()->close(type);
        } catch (...) {
            if (notifyOnException)
                notifyOnException();
            throw;
        }
        if (notifyOnClose)
            notifyOnClose();
    }

    size_t read(Buffer &b, size_t len)
    {
        size_t result;
        try {
            result = parent()->read(b, len);
        } catch(...) {
            if (notifyOnException)
                notifyOnException();
            throw;
        }
        if (result == 0 && notifyOnEof)
            notifyOnEof();
        return result;
    }

    size_t write(const Buffer &b, size_t len)
    {
        try {
            return parent()->write(b, len);
        } catch(...) {
            if (notifyOnException)
                notifyOnException();
            throw;
        }
    }
};

#endif
