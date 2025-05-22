#include <stcd-monitor/structures/itemvalues.h>

using namespace zab::mon;

ItemValues::ItemValues(std::vector<Value> values, Item item)
    : values(std::move(values))
    , item(std::move(item))
{
}
