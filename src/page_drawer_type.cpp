#include <improc/drawer/page_drawer_type.hpp>

improc::PageDrawerType::PageDrawerType() : value_(improc::PageDrawerType::kPageDrawer) {};

improc::PageDrawerType::PageDrawerType(const std::string& page_drawer_type_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining page drawer type from string {}...",page_drawer_type_str);
    static const std::unordered_map<std::string,PageDrawerType::Value> kToElemType = { {"page-drawer",PageDrawerType::Value::kPageDrawer }
                                                                                     , {"grid-drawer",PageDrawerType::Value::kGridDrawer }
                                                                                     };
    this->value_ = kToElemType.at(improc::String::ToLower(page_drawer_type_str));
}
