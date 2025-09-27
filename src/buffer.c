#include <hal/buffer.h>

bool_t hal_buffer_check(hal_buffer_t buffer)
{
    return buffer.data != NULL && buffer.length > 0;
}

bool_t hal_rx_tx_buffer_check(hal_rx_tx_buffer_t buffer)
{
    return hal_buffer_check(buffer.rx) && hal_buffer_check(buffer.tx);
}
