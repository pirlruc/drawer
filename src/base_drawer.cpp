#include <improc/drawer/engine/base_drawer.hpp>

/**
 * @brief Construct a new improc::BaseDrawer object
 */
improc::BaseDrawer::BaseDrawer() {};

/**
 * @brief Construct a new improc::BaseDrawer object
 * 
 * @param drawer_json - configuration json for drawer
 */
improc::BaseDrawer::BaseDrawer(const Json::Value& drawer_json) : improc::BaseDrawer() {};

/**
 * @brief Construct a new improc::BaseDrawer object
 * 
 * @param factory - drawer factory
 * @param drawer_json - configuration json for drawer
 * @return std::shared_ptr<improc::BaseDrawer> - drawer object pointer
 */
std::shared_ptr<improc::BaseDrawer> improc::BaseDrawer::Create(const improc::DrawerFactory& factory, const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating drawer using factory...");
    static const std::string kDrawerType = "drawer-type";
    static const std::string kDrawerArgs = "args";
    if (drawer_json.isMember(kDrawerType) == false) 
    {
        std::string error_message = fmt::format("Key {} is missing from json",kDrawerType);
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    std::string drawer_type {improc::json::ReadElement<std::string>(drawer_json[kDrawerType])};

    Json::Value drawer_args {};
    if (drawer_json.isMember(kDrawerArgs) == true) 
    {
        drawer_args = drawer_json[kDrawerArgs];
    }
    return std::move(factory).Create(std::move(drawer_type),std::move(drawer_args));
}