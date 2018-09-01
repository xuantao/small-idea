#include "utility/ring_buffer.h"
#include "test.h"

UTILITY_USING_NAMESPACE

void test_ring_buffer()
{
    fixed_ring_buffer<512> buffer;

    size_t ws;
    void* buf;
    buffer.write_begin(10);
    buffer.write_end();

    buf = buffer.read_begin(&ws);
    buffer.read_end();
    assert(ws = 10);


}
