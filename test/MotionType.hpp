#include <array>   // array
#include <cstddef> // size_t
#include <cstdint> // uint8_t
#include <string>  // string

namespace motion
{
    enum class cmd_t : std::uint8_t
    {
        connect,
        disconnect,
        reconnect,
        connection_check,
        jog_start,
        jog_stop,
        moving_abs,
    };

    enum class device_t : std::uint8_t
    {
        axis_x,
        axis_y,
        axis_z,
    };

    enum class direction_t : std::uint8_t
    {
        cw,
        ccw,
    };

}