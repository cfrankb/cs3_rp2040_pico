#ifndef __LEVELARCH_H
#define __LEVELARCH_H

#include <cinttypes>
#include <vector>
class CLevelArch 
{
public:
    CLevelArch();
    ~CLevelArch();

    bool open(const char *filename);
    bool fromMemory(uint8_t *ptr);
    int count();
    uint32_t at(int i);
    void debug();
    void forget();

protected:
    typedef std::vector<long> IndexVector;
    IndexVector m_index;

};

#endif