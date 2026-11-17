//
// Created by Dom on 12.11.2026.
//

#ifndef WEBSERVER_RESOURCE_MANAGER_H
#define WEBSERVER_RESOURCE_MANAGER_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "../parsers/http_request.h"
#include "../parsers/http_response.h"

using json = nlohmann::json;

namespace resource_manager {

    struct result {
        http_response::StatusCode status_code;
        std::string body;
    };

    class FileManager {
    protected:
        const std::string FILE_PATH_ROOT = "../resources";
        std::string file_path;
    public:
        virtual ~FileManager() = default;
        virtual void setFilePath(const std::string& file_path) = 0;
        virtual result getRequest(const http_request::request& request_struct) = 0;
        virtual result postRequest(const http_request::request& request_struct) = 0;
        virtual result deleteRequest(const http_request::request& request_struct) = 0;
        virtual result putRequest(const http_request::request& request_struct) = 0;
        virtual result patchRequest(const http_request::request& request_struct) = 0;
    };

    class IRepository {
    public:
        virtual ~IRepository() = default;
        virtual result handleRequest(const http_request::request& request_struct) = 0;

    };
    class JsonManager: public FileManager {
        json getJsonFromFile() const;
    public:
        void setFilePath(const std::string &file_path) override;
        result getRequest(const http_request::request& request_struct) override;
        result postRequest(const http_request::request& request_struct) override;
        result deleteRequest(const http_request::request& request_struct) override;
        result putRequest(const http_request::request& request_struct) override;
        result patchRequest(const http_request::request& request_struct) override;
    };
    ///Class controling http requests to users repository
    class UsersRepository : public IRepository{
    private:
        FileManager& manager;
    public:
        UsersRepository(FileManager& manager): manager(manager){}

        /**
         * @brief Dispatches the request to the handler matching its HTTP method.
         * @param request_struct Parsed request (method, url, query, version...)
         * @return result containing status code and string body, created from given request or error message
         */
        result handleRequest(const http_request::request& request_struct) override;

    };
}
#endif //WEBSERVER_RESOURCE_MANAGER_H
