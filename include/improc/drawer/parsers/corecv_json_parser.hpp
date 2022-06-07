#ifndef IMPROC_DRAWER_CORECV_JSON_PARSER_HPP
#define IMPROC_DRAWER_CORECV_JSON_PARSER_HPP

//TODO: Move to core_cv
#include <improc/drawer/logger_drawer.hpp>
#include <improc/improc_defs.hpp>
#include <improc/infrastructure/parsers/json_parser.hpp>

#include <opencv2/core.hpp>
#include <json/json.h>

namespace improc
{
    namespace json
    {
        template<typename KeyType>
        IMPROC_EXPORTS KeyType                  ReadPositiveSize (const Json::Value& json_size);
    }
}

#include <improc/drawer/parsers/corecv_json_parser.tpp>

#endif
