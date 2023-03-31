#include <improc/drawer/engine/page_drawer_type.hpp>

/**
 * @brief Construct a new improc::PageDrawerType object
 */
improc::PageDrawerType::PageDrawerType() : value_(improc::PageDrawerType::kPageDrawer) {};

/**
 * @brief Construct a new improc::PageDrawerType object
 * 
 * @param page_drawer_type_str - page drawer type description as string
 */
improc::PageDrawerType::PageDrawerType(const std::string& page_drawer_type_str)
{
    IMPROC_DRAWER_LOGGER_TRACE("Obtaining page drawer type from string {}...",page_drawer_type_str);
    static const std::unordered_map<std::string,PageDrawerType::Value> kToElemType = { {"page-drawer",PageDrawerType::Value::kPageDrawer }
                                                                                     , {"grid-drawer",PageDrawerType::Value::kGridDrawer }
                                                                                     };
    this->value_ = kToElemType.at(improc::String::ToLower(std::move(page_drawer_type_str)));
}
