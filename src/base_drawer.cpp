#include <improc/drawer/engine/base_drawer.hpp>

improc::BaseDrawer::BaseDrawer() {};

improc::BaseDrawer::BaseDrawer(const Json::Value& drawer_json) : improc::BaseDrawer() {};

std::shared_ptr<improc::BaseDrawer> improc::BaseDrawer::Create(const improc::DrawerFactory& factory, const Json::Value& drawer_json)
{
    IMPROC_DRAWER_LOGGER_TRACE("Creating drawer using factory...");
    static const std::string kDrawerType = "drawer-type";
    static const std::string kDrawerArgs = "args";
    if (drawer_json.isMember(kDrawerType) == false) 
    {
        IMPROC_DRAWER_LOGGER_ERROR("ERROR_01: Drawer type missing.");
        throw improc::file_processing_error();
    }
    std::string drawer_type {improc::json::ReadElement<std::string>(drawer_json[kDrawerType])};

    Json::Value drawer_args {};
    if (drawer_json.isMember(kDrawerArgs) == true) 
    {
        drawer_args = drawer_json[kDrawerArgs];
    }
    return factory.Create(std::move(drawer_type),std::move(drawer_args));
}