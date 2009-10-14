// Copyright (c) 2009 - Decho Corp.

#include "mordor/pch.h"

#include "stream.h"

#include <string.h>

namespace Mordor {

size_t
Stream::write(const char *sz)
{
    return write(sz, strlen(sz));
}

size_t
Stream::write(const void *b, size_t len)
{
    Buffer buf;
    buf.copyIn(b, len);
    return write(buf, len);
}

std::string
Stream::getDelimited(char delim, bool eofIsDelimiter)
{
    ptrdiff_t offset = find(delim, ~0, !eofIsDelimiter);
    eofIsDelimiter = offset < 0;
    if (offset < 0)
        offset = -offset - 1;
    Buffer buf;
#ifdef DEBUG
    size_t readResult = 
#endif
    read(buf, offset + (eofIsDelimiter ? 0 : 1));
    MORDOR_ASSERT((ptrdiff_t)readResult == offset + (eofIsDelimiter ? 0 : 1));
    // Don't copyOut the delimiter itself
    std::string result;
    result.resize(offset);
    buf.copyOut(const_cast<char*>(result.data()), offset);
    return result;
}

}