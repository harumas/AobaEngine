#include "ObjectService.h"
#include <memory>
#include <vector>

std::vector<std::shared_ptr<Object>> ObjectService::objects = std::vector<std::shared_ptr<Object>>();