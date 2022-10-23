#include "TalkWithCC.h"
#include "tool.h"
#include "log.h"



TalkWithCC *TalkWithCC::MakeTalk(const std::string& service, const std::string& port, const std::string& user, const std::string& pwd) {
    TalkWithCC *talk = nullptr;
    // step 1
    bool r = TalkWithCC::GetCASLoginParameters(service, port, user, pwd, talk);
    if (!r) {
        //Log::WriteLog(log_error, "Get CAS Login parameters failed");
        return nullptr;
    }
    assert(talk != nullptr);
    // step 2
    r = talk->LoginToCAS();
    if (!r) {
        Log::WriteLog(log_error, "Login To CAS failed");
        delete (talk); talk = nullptr;
        return nullptr;
    }
    // step 3
    http::response<http::string_body> res;
    r = talk->CasSecurityCheck(res);
    if (!r) {
        Log::WriteLog(log_error, "CAS Security Check failed");
        delete (talk); talk = nullptr;
        return nullptr;
    }
    return talk;
}

bool TalkWithCC::GetCASLoginParameters(const std::string& service, const std::string& port,
        const std::string& user, const std::string& pwd, TalkWithCC*& out) {
    NXLHttpClient *phttp_client = new NXLHttpClient(service.c_str(), port.c_str());
    NXLHttpClient& http_client = *phttp_client;
    std::string location;
    bool r = GetCASLoginUrl(http_client, service, port, location);
    if (!r) {
       Log::WriteLog(log_fatal, "Get CAS Login URL failed, please check [Policy]cchost/ccport." );
       return false;
    }

    http::request<http::string_body> req{ http::verb::get, location, 11 };

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient"); // ? if we need this
    req.set(http::field::connection, "keep-alive");

    //send request
    boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
    http::response<http::string_body> res;
    try {
        http_client.Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on Get CAS Login Parameters:%s", e.what() );
        return false;
    }
    catch (...)
    {
        Log::WriteLog(log_fatal, "excpetion on Get CAS Login Parameters" );
        return false;
    }

    //get response header
    if (res.result() == http::status::ok) {

        std::string login_cookie;
        std::string login_submit_path, login_param_lt, login_param_execution;

        //get cookie
        auto& headers = res.base();
        auto setcookieHdr = headers.find(http::field::set_cookie);
        if (setcookieHdr != headers.end()) {
            login_cookie = setcookieHdr->value().to_string();
        }

        //parse body
        auto& body = res.body();

        //parse result
        size_t nPosForm = body.find("<form");

        //get auth url
        {
            const std::string strKey = "action=\"";
            size_t nPosAction = body.find(strKey, nPosForm);
            if (nPosAction!=std::string::npos) {
                size_t nPosEnd = body.find("\"", nPosAction + strKey.length());
                login_submit_path = body.substr(nPosAction + strKey.length(), nPosEnd - nPosAction - strKey.length());
            }
        }

        //get lt
        {
            const std::string strKey = "name=\"lt\"";
            size_t nPosLtName = body.find(strKey, nPosForm);
            if (nPosLtName!=std::string::npos) {
                size_t nltBegin = body.find("\"", nPosLtName + strKey.length());
                size_t nLtEnd = body.find("\"", nltBegin + 1);
                login_param_lt = body.substr(nltBegin + 1, nLtEnd - nltBegin - 1);
            }
        }

        //get execute
        {
            const std::string strKey = "name=\"execution\"";
            size_t nPosExecName = body.find(strKey, nPosForm);
            if (nPosExecName!=std::string::npos) {
                size_t nExecBegin = body.find("\"", nPosExecName + strKey.length());
                size_t nExecEnd = body.find("\"", nExecBegin + 1);
                login_param_execution = body.substr(nExecBegin + 1, nExecEnd - nExecBegin - 1);
            }
        }

        //get CsrfToken, this introduce in CC9.1
        {
            const std::string strKey = "name=\"csrfToken\"";
            size_t nPosExecName = body.find(strKey, nPosForm);
            if (nPosExecName!=std::string::npos) {
                std::string login_csrf_token;
                size_t nExecBegin = body.find("\"", nPosExecName + strKey.length());
                size_t nExecEnd = body.find("\"", nExecBegin + 1);
                login_csrf_token = body.substr(nExecBegin + 1, nExecEnd - nExecBegin - 1);
                login_submit_path = location;

                out = new TalkWithCC91(&http_client, user, pwd, location, login_submit_path, login_param_lt, login_param_execution, login_csrf_token, login_cookie);
                Log::WriteLog(log_info, "TalkWithCC::GetCASLoginParameters create object TalkWithCC" );
                return true;
            } else {
                out = new TalkWithCCLower(&http_client, user, pwd, location, login_submit_path, login_param_lt, login_param_execution);
                Log::WriteLog(log_info, "TalkWithCC::GetCASLoginParameters create object TalkWithCCLower" );
                return true;
            }
        }
    }

    return false;

}

bool TalkWithCC::GetCASLoginUrl(NXLHttpClient& http_client, const std::string& /*service*/, const std::string& /*port*/, std::string& location) {
    http::request<http::string_body> req{ http::verb::get, "/console/", 11 };

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");

    //send request
    boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
    http::response<http::string_body> res;
    try {
        http_client.Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on Get CAS Login Url:%s", e.what() );
        //printf(e.what());// 
        return false;
    }
    catch (...){
        Log::WriteLog(log_fatal, "excpetion on Get CAS Login Url" );
        return false;
    }

    //get response header
    if (res.result() == http::status::found) {
        auto& headers = res.base();
        auto LocationHdr = headers.find(http::field::location);
        if(LocationHdr != headers.end()) {
            location = LocationHdr->value().to_string();
            location = GetRequestPathFromFullUrl(location);
            Log::WriteLog(log_info, "CCLoginHelper::GetCASLoginUrl:%s", location.c_str());
            return true;
        }
    }
    return false;
}

TalkWithCC::TalkWithCC(NXLHttpClient *http_client, const std::string& user, const std::string& pwd) :
_http_client(http_client), _user(user), _pwd(CommonFun::UrlEncode(pwd)) {}

std::string TalkWithCC::GetRequestPathFromFullUrl(const std::string& strFullurl)
{
    std::string strRequestPath = strFullurl; //set full url as default

    size_t nHostBegin = strFullurl.find("://", 0);
    if ((std::string::npos!=nHostBegin) && (strFullurl.size()>nHostBegin+3))  {
        size_t nPathPos = strFullurl.find('/', nHostBegin + 3);
        if (std::string::npos != nPathPos)  {
            strRequestPath = strFullurl.substr(nPathPos);
        }
    }

    return strRequestPath;
}

TalkWithCCLower::TalkWithCCLower(NXLHttpClient *http_client, const std::string& user, const std::string& pwd,
    const std::string& cas_login_url, const std::string& login_submit_path,
    const std::string& login_param_lt, const std::string& login_param_execution) : TalkWithCC(http_client, user, pwd),
_cas_login_url(cas_login_url), _login_submit_path(login_submit_path), _login_param_lt(login_param_lt),
_login_param_execution(CommonFun::UrlEncode(login_param_execution)) {}

bool TalkWithCCLower::LoginToCAS() {
    auto req = ConstructLoginRequest();
    boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
    http::response<http::string_body> res;
    try {
        _http_client->Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on Login To CAS:%s", e.what() );
        return false;
    }
    catch (...) {
        Log::WriteLog(log_fatal, "excpetion on Login To CAS:..." );
        return false;
    }

    //get response header
    if (res.result() == http::status::found) {
        auto& headers = res.base();
        auto LocationHdr = headers.find(http::field::location);
        if(LocationHdr != headers.end()) {
            _cas_security_check_url = LocationHdr->value().to_string(); 
            _cas_security_check_url = GetRequestPathFromFullUrl(_cas_security_check_url);

            Log::WriteLog(log_info, "CCLoginHelper::LoginToCAS:%s", _cas_security_check_url.c_str());
            return true;
        }
    }
    else
    {
        Log::WriteLog(log_fatal, "CC module initialization failed, fail to login to CAS by user [%s]", _user.c_str());
    }

    return false;
}

bool TalkWithCCLower::CasSecurityCheck(http::response<http::string_body>& out) {
    http::request<http::string_body> req{ http::verb::get, _cas_security_check_url, 11};
    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");

    //send request
    boost::beast::flat_buffer buffer; // This buffer is used for reading and must be persisted
    http::response<http::string_body> res;
    try {
        _http_client->Request(req, buffer, res);
    }
    catch (boost::system::system_error const& e) {
        //disconnected
        Log::WriteLog(log_fatal, "excpetion on CAS Security Check:%s", e.what());
        return false;
    }
    catch (...) {
        Log::WriteLog(log_fatal, "excpetion on CAS Security Check");
        return false;
    }

    //get response header
    out = res;
    if (res.result() == http::status::found) {
        auto& headers = res.base();
        auto setCookieHdr = headers.find(http::field::set_cookie);
        if (setCookieHdr != headers.end()) {
            _set_cookie = setCookieHdr->value().to_string();
            Log::WriteLog(log_info, "CCLoginHelper::LoginToCAS set cookie:%s", _set_cookie.c_str());
            return true;
        }
    }
    return false;
}

std::string TalkWithCCLower::CtorLoginBody() {
    std::string body;
    body += "username=" + _user; body += "&password=" + _pwd;
    body += "&lt=" + _login_param_lt; body += "&execution=" + _login_param_execution;
    body += "&_eventId=submit&submit=Login";
    return body;
}

http::request<http::string_body> TalkWithCCLower::ConstructLoginRequest() {
    std::string body = CtorLoginBody();
    http::request<http::string_body> req{ http::verb::post, _login_submit_path, 11, body };

    //set headers
    req.set(http::field::user_agent, "SQLEnforcer-ODBCClient");
    req.set(http::field::connection, "keep-alive");
    req.set(http::field::content_length, body.length());
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    req.set(http::field::accept, "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
    return req;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TalkWithCC91::TalkWithCC91(NXLHttpClient *http_client, const std::string& user, const std::string& pwd,
const std::string& cas_login_url, const std::string& login_submit_path,
const std::string& login_param_lt, const std::string& login_param_execution,
const std::string& login_param_csrf_token, const std::string& login_cookie) :
    TalkWithCCLower(http_client, user, pwd, cas_login_url, login_submit_path, login_param_lt, login_param_execution),
    _login_param_csrf_token(login_param_csrf_token), _login_cookie(login_cookie) {}

bool TalkWithCC91::CasSecurityCheck(http::response<http::string_body>& /*out*/) {
    http::response<http::string_body> res;
    bool r = TalkWithCCLower::CasSecurityCheck(res);
    if (!r){
         Log::WriteLog(log_fatal, "CAS Security Check call parent failed.");
         return false;
    }
    if (res.result() == http::status::found) {
        auto& headers = res.base();
        //get X-CSRF-HEADER for cc91
        auto X_CSRF_HEADER_name = headers.find("X-CSRF-HEADER");
        if (X_CSRF_HEADER_name!=headers.end()) {
            std::string strXCSRFHdrName = X_CSRF_HEADER_name->value().to_string();
            if (!strXCSRFHdrName.empty()) {
                auto X_CSRF_TOKEN = headers.find(strXCSRFHdrName);
                if (X_CSRF_TOKEN!=headers.end()) {
                    _csfr_token = X_CSRF_TOKEN->value().to_string();
                    return true;
                }else{
                     Log::WriteLog(log_fatal, "CAS Security Check can't find header:%s", strXCSRFHdrName.c_str() );
                }


            }else{
                Log::WriteLog(log_fatal, "CAS Security Check value of X-CSRF-HEADER is empty.");
            }
        }else{
            Log::WriteLog(log_fatal, "CAS Security Check didn't find X-CSRF-HEADER header.");
        }

    }else{
         Log::WriteLog(log_fatal, "CAS Security Check http status not:http::status::found.");
    }
    return false;
}

std::string TalkWithCC91::CtorLoginBody() {
    std::string body = TalkWithCCLower::CtorLoginBody();
    body += "&csrfToken=" + _login_param_csrf_token;
    return body;
}

http::request<http::string_body> TalkWithCC91::ConstructLoginRequest() {
    auto req = TalkWithCCLower::ConstructLoginRequest();
    req.set(http::field::cookie, _login_cookie);
    return req;
}


