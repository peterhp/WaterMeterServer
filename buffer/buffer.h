#ifndef _QPT_HEADER_BUFFER_H_
#define _QPT_HEADER_BUFFER_H_

#include "types.h"
#include "util/macros.h"

#define BUFFER_SIZE_1K  (1024)
#define BUFFER_SIZE_2K  (BUFFER_SIZE_1K * 2)
#define BUFFER_SIZE_4K  (BUFFER_SIZE_2K * 2)
#define BUFFER_SIZE_8K  (BUFFER_SIZE_4K * 2)

#define BUFFER_SIZE_MIN     ALIGN_WIDTH
#define BUFFER_SIZE_DEFAULT BUFFER_SIZE_1K
#define BUFFER_SIZE_PAGE    BUFFER_SIZE_4K

class Buffer {
    public: 
        virtual ~Buffer() {}

        virtual uint buf_size() const = 0;
        virtual uint data_size() const = 0;
        virtual uint free_size() const = 0;

        virtual uint peek(byte *dst, uint dlen) const = 0;
        virtual uint read(byte *dst, uint dlen) = 0;
        virtual uint write(const byte *src, uint slen) = 0;
};

class BlockBuffer : public Buffer {
    public: 
        BlockBuffer(uint init_size = BUFFER_SIZE_DEFAULT);
        ~BlockBuffer();

        uint buf_size() const;
        uint data_size() const;
        uint free_size() const;

        uint peek(byte *dst, uint dlen) const;
        uint read(byte *dst, uint dlen);
        uint write(const byte *src, uint slen);

    private: 
        void expand(uint min_size);

    private:  
        byte *buf;
        uint size;
        uint max_size;
};

class CircularBuffer : public Buffer {
    public: 
        CircularBuffer(uint init_size = BUFFER_SIZE_DEFAULT);
        ~CircularBuffer();

        uint buf_size() const;
        uint data_size() const;
        uint free_size() const;

        uint peek(byte *dst, uint dlen) const;
        uint read(byte *dst, uint dlen);
        uint write(const byte *src, uint slen);

    private: 
        void expand(uint min_size);

    private:
        byte *buf;
        uint max_size;
        uint front;
        uint rear;
};

#endif // buffer.h
