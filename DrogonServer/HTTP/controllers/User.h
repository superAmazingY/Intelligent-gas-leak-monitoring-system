#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class User : public drogon::HttpController<User>
{
  public:
    METHOD_LIST_BEGIN
       METHOD_ADD(User::loginInfo, "login?userId={1}&passwd={2}",Post,Options);
       //url: http://8.130.146.112:8099/user/login?userId={1}&passwd={2}(登录api）
       METHOD_ADD(User::registerInfo, "register?userId={1}&passwd={2}&phone_number={3}", Post,Options);
       //url: http://8.130.146.112:8099/user/register?userId={1}&passwd={2}&phone_number={3}(注册api)
       METHOD_ADD(User::getCoInfo,"CoInfo",Get,Options);
       //url: http://8.130.146.112:8099/user/CoInfo(一氧化碳含量api)
       METHOD_ADD(User::getCo2Info,"Co2Info",Get,Options);
       //url: http://8.130.146.112:8099/user/Co2Info(二氧化碳含量api)
       METHOD_ADD(User::getCH4Info,"CH4Info",Get,Options);
       //url: http://8.130.146.112:8099/user/CH4Info(甲烷含量api)
       METHOD_ADD(User::getH2Info,"H2Info",Get,Options);
       //url: http://8.130.146.112:8099/user/H2Info(氢气含量api)
       METHOD_ADD(User::getERRORInfo,"errorInfo",Get,Options);
       //url: http://8.130.146.112:8099/user/errorInfo(警报api)

    METHOD_LIST_END
       //post函数实现虚函数
       void loginInfo(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               std::string &&userId,
               const std::string &password);
       void registerInfo(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback,
               std::string &&userId,
               const std::string &password,
               const std::string &phone_number);

    //get函数实现虚函数
       void getCoInfo(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getCo2Info(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getCH4Info(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getH2Info(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);
       void getERRORInfo(const HttpRequestPtr &req,std::function<void(const HttpResponsePtr &)> &&callback);

};
