
#include "operator_between.h"

#include <string>
#include <assert.h>
#include <string.h>
#include "commfun.h"
#include "log.h"

#ifdef WIN32

#include "strptime.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "windows.h"
#pragma comment(lib, "ws2_32.lib")

#define gmtime_r(a,b) gmtime_s(b,a)
#define localtime_r(a,b) localtime_s(b,a)

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#endif

#define SECONDS_IN_DAY (24*60*60)
#define SECONDS_IN_HOUR (60*60)
#define SECONDS_IN_MINUTE (60)




long local_time_zone();
bool parse_timestamp(const char *src, time_t *from_out, time_t *to_out);
bool parse_time(const char *src, time_t *from_out, time_t *to_out);




long local_time_zone() {
    time_t t = 0;
    struct tm p, p1;

    gmtime_r(&t, &p);
    localtime_r(&t, &p1);

    long ret = (p1.tm_min - p.tm_min) * SECONDS_IN_MINUTE + (p1.tm_hour - p.tm_hour) * SECONDS_IN_HOUR;
    if (ret > SECONDS_IN_DAY/2)
        ret -= SECONDS_IN_DAY;
    else if (ret < -(SECONDS_IN_DAY/2))
        ret += SECONDS_IN_DAY;
    return ret;
}

bool check_current_timestamp_between(time_t from, time_t to) {
    assert(from <= to);
    time_t cur = time(nullptr);
    Log::WriteLog(log_info, "timestamp@=%ld   policy:from=%ld, to=%ld", (long)cur, (long)from, (long)to);
    return cur >= from && cur <= to;
}

// (from, to)
bool check_current_time_between_(time_t from, time_t to) {
    time_t c = time(nullptr);
    c = c % SECONDS_IN_DAY;
    Log::WriteLog(log_info, "timestamp@=%ld   policy:from=%ld, to=%ld", (long)c, (long)from, (long)to);
    return c > from && c < to;
}

// [from, to]
bool check_current_time_between(time_t from, time_t to) {
    time_t c = time(nullptr);
    c = c % SECONDS_IN_DAY;
    Log::WriteLog(log_info, "timestamp@=%ld   policy:from=%ld, to=%ld", (long)c, (long)from, (long)to);
    return c >= from && c <= to;
}


/*
 * TIMESTAMP@ IS [NOT] 2010-02-15 02:30:15 ~ 2010-02-20 13:30:15 [ time_zone ]
 * TIME@ IS [NOT] 02:30:15 ~ 13:30:15 [ <time zone> ]
 * time_zone: UTC +8
 *              UTC -7
 *                 UTC +8:45
 *
 *
 *    2010-02-15 02:30:15 ~ 2010-02-20 13:30:15
 *
 *    TIMESTAMP@ IS [NOT] 2010-02-15 02:30:15 ~ 2010-02-20 13:30:15 UTC +8:45
 *
 *
 * */

bool parse_timestamp(const char *src, std::string& from_out, std::string& to_out, long& utc_out) {
    int i = 0;
    //struct tm from, to;
    //memset(&from, 0, sizeof(from));
    //memset(&to, 0, sizeof(to));
   // long timezone = 0;
    while (src[i] != '\0' && src[i] == ' ')
        ++i;
    if (src[i] == '\0')
        return false;
	 
    int start = i;
    while (src[i] != '\0' && src[i] != '~')
        ++i;
    if (src[i] == '\0')
        return false;
    std::string from_str(src+start, src+i);
    //strptime(from_str.c_str(), "%Y-%m-%d %H:%M:%S", &from);
    from_out = from_str;
    assert(src[i] == '~');
    ++i;
	while (src[i] != '\0' && src[i] == ' ')
		++i;
	if (src[i] == '\0')
		return false;
    start = i;
    while (src[i] != '\0' && src[i] != 'U')
        ++i;
    std::string to_str(src+start, src+i);
    //strptime(to_str.c_str(), "%Y-%m-%d %H:%M:%S", &to);
    to_out = to_str;
    if (src[i] == '\0') {
        utc_out = 0;
        return true;
    }
    else {
        assert(src[i] == 'U');
        ++i;
        if (src[i] != 'T')
            return false;
        ++i;
        if (src[i] != 'C')
            return false;
        ++i;
        while (src[i] != '\0' && src[i] == ' ')
            ++i;
        if (src[i] == '\0')
            return false;
        if (src[i] != '+' && src[i] != '-')
            return false;
        bool is_east = src[i] == '+';
        ++i;

        std::string tz_str = src+i;
        struct tm tz;
        tz.tm_hour = 0; tz.tm_min = 0;
        strptime(tz_str.c_str(), "%H:%M", &tz);

        long m = tz.tm_hour * SECONDS_IN_HOUR + tz.tm_min * SECONDS_IN_MINUTE;
        if (!is_east)
            m = -m;

        utc_out = m;

        return true;
    }
}

time_t str2time_t(const char *str, const char *format, long utc) {
    struct tm p;
    memset(&p, 0, sizeof(p));
    strptime(str, format, &p);
    time_t t1 = mktime(&p);
#ifdef WIN32
    /* WIN32 not support the timepoint before the BASE */
    if (t1 == -1)
        assert(false);
#else
#endif
    return t1 + local_time_zone() - utc;
}

bool parse_timestamp(const char *src, time_t *from_out, time_t *to_out) {
    std::string from_str, to_str;
    long utc = 0;
    if (!parse_timestamp(src, from_str, to_str, utc))
        return false;
    *from_out = str2time_t(from_str.c_str(), "%Y-%m-%d %H:%M:%S", utc);
    *to_out = str2time_t(to_str.c_str(), "%Y-%m-%d %H:%M:%S", utc);
    return true;
}

long str2long(const char *str, long utc) {
    struct tm p;
    memset(&p, 0, sizeof(p));
    strptime(str, "%H:%M:%S", &p);

    long l = SECONDS_IN_HOUR*p.tm_hour + SECONDS_IN_MINUTE*p.tm_min + p.tm_sec;
    l = l - utc;
    if (l < 0)
        l = SECONDS_IN_DAY + l;
    else if (l >= SECONDS_IN_DAY)
        l = l - SECONDS_IN_DAY;
    return l;
}

bool parse_time(const char *src, time_t *from_out, time_t *to_out) {
    std::string from_str, to_str;
    long utc = 0;
    if (!parse_timestamp(src, from_str, to_str, utc))
        return false;
    *from_out = str2long(from_str.c_str(), utc);
    *to_out = str2long(to_str.c_str(), utc);
    return true;
}

BETWEEN_OP_RESULT cur_timestamp_between(const char *v) {
    time_t from, to;
    if (!parse_timestamp(v, &from, &to))
        return BETWEEN_OP_ERR;
    if (from > to)
        return BETWEEN_OP_ERR;
    if (check_current_timestamp_between(from, to))
        return BETWEEN_OP_TR;
    else
        return BETWEEN_OP_FL;
}

BETWEEN_OP_RESULT cur_time_between(const char *v) {
    time_t from, to;
    if (!parse_time(v, &from, &to))
        return BETWEEN_OP_ERR;

    if (from > to) {
        // cross one day
        // [from, to] means not (to, from)
        if (check_current_time_between_(to, from))
            return BETWEEN_OP_FL;
        else
            return BETWEEN_OP_TR;
    }

    if (check_current_time_between(from, to))
        return BETWEEN_OP_TR;
    else
        return BETWEEN_OP_FL;
}




// src 2019-06-20 18:45:00 ~ 2019-06-21 18:23:00 UTC+8
// rhs 2019-06-20 10:45:00 ~ 2019-06-21 10:23:00 [UTC+0]
// bool compare_timestamp(const char *src, const char *rhs) {
//     time_t f_src, t_src, f_rhs, t_rhs;
//     bool s = parse_timestamp(src, &f_src, &t_src);
//     assert(s);
//     s = parse_timestamp(rhs, &f_rhs, &t_rhs);
//     assert(s);
//     return f_src == f_rhs && t_src == t_rhs;
// }
// 
// // src 18:45:00 ~ 18:23:00 UTC+8
// // rhs 10:45:00 ~ 10:23:00 [UTC+0]
// bool compare_time(const char *src, const char *rhs) {
//     time_t f_src, t_src, f_rhs, t_rhs;
//     bool s = parse_time(src, &f_src, &t_src);
//     assert(s);
//     s = parse_time(rhs, &f_rhs, &t_rhs);
//     assert(s);
//     return f_src == f_rhs && t_src == t_rhs;
// }

//ip parse and match
//-----------------------------------------------------------------------------------
#include <iostream>
#include <list>
using namespace std;

class AttrIP
{
public:
	AttrIP() :bt1(0), bt2(0), bt3(0), bt4(0) {  }

	void SetValue(int iIndex, int iValue) {
		switch (iIndex) {
		case 0:
			bt1 = (unsigned char)iValue;
			break;
		case 1:
			bt2 = (unsigned char)iValue;
			break;
		case 2:
			bt3 = (unsigned char)iValue;
			break;
		case 3:
			bt4 = (unsigned char)iValue;
			break;
		default:
			break;
		}
	}
	bool operator>=(const AttrIP &t) {
		unsigned int i = 0;
		i += bt1 << 24;
		i += bt2 << 16;
		i += bt3 << 8;
		i += bt4 << 0;
		unsigned int it = t.bt1 << 24;
		it += t.bt2 << 16;
		it += t.bt3 << 8;
		it += t.bt4 << 0;
		if (i >= it)
            return true;
		else
            return false;
	}
private:

	unsigned char  bt1;
	unsigned char  bt2;
	unsigned char  bt3;
	unsigned char  bt4;
};

 
bool get_local_ip(list<string> & list);
//std::string get_local_ip();
bool parse_ip(const std::string& strValue, AttrIP & ip);
bool parse_ip(const std::string& strValue, AttrIP & ip1, AttrIP & ip2);

BETWEEN_OP_RESULT cur_ip_between(const char *v)
{
	string strCondition = v;
	list<string> list;
	if (!get_local_ip(list))
        return BETWEEN_OP_ERR;

	BETWEEN_OP_RESULT ret = BETWEEN_OP_FL;
	//bool bBreak = false;
	for (auto pV : list) {
        Log::WriteLog(log_info, "ip@=%s  policy:ip=%s", pV.c_str(), v);
		if (pV.empty()) {
			ret = BETWEEN_OP_ERR;
			continue;
		}
		if (ret == BETWEEN_OP_FL) {
			AttrIP ipV;
			if (!parse_ip(pV, ipV))	{
				ret = BETWEEN_OP_ERR;
				continue;
			}
			AttrIP ipC1, ipC2;
			if (!parse_ip(strCondition, ipC1, ipC2)) {
				ret = BETWEEN_OP_ERR;
				continue;
			}
			if (ipC2 >= ipC1) {
				if (ipV >= ipC1 && ipC2 >= ipV)
                    ret = BETWEEN_OP_TR;
			}
			else
                ret = BETWEEN_OP_ERR;

        }
		
	}
	list.clear();
	
	return ret;
}

bool compare_with_cur_application(const char* cname){
	char cFile[256] = {0};
	#ifdef WIN32
	::GetModuleFileNameA(NULL,(CHAR*)cFile,256);
	#else
    int rslt = readlink("/proc/self/exe", cFile, 256);
    if (rslt < 0 || (rslt >= 256)) {
        return false;
    }
    #endif
    Log::WriteLog(log_info, "application@=%s  policy:app=%s", cFile, cname);
	std::string strname(cname);
	size_t iLen = strname.length();
	std::string sPath(cFile);
	size_t iAllLen = sPath.length();
	if(iAllLen < iLen)
		return false;
	std::string sCompare = sPath.substr(iAllLen-iLen,iLen);
	if(sCompare.compare(strname)==0)
		return true;
	return false;

}

// std::string get_local_ip()
// {
// 	string svalue;
// 	WSADATA wsaData;
// 	WORD sockVersion = MAKEWORD(2, 2);
// 	::WSAStartup(sockVersion, &wsaData);
// 
// 	char               buf[100];
// 	int                ret = 0;
// 	struct addrinfo    hints;
// 	struct addrinfo    *res, *curr;
// 	struct sockaddr_in *sa;
// 
// 	memset(&hints, 0, sizeof(addrinfo));
// 	hints.ai_flags = AI_CANONNAME;
// 	hints.ai_family = AF_INET;
// 	hints.ai_socktype = SOCK_STREAM;
// 	int iRet = gethostname(buf, sizeof(buf));
// 	if (iRet == 0) {
// 
// 		if ((ret = getaddrinfo(buf, NULL, &hints, &res)) == 0) {
// 			curr = res;
// 			while (curr && curr->ai_canonname) {
// 				sa = (struct sockaddr_in *)curr->ai_addr;
// 
// 				string sname = curr->ai_canonname;//hostname
// 
// 			
// 				svalue = inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof(buf));
// 
// 				cout << "name:" << sname << endl;
// 				cout << "ip:" << svalue << endl;
// 
// 				curr = curr->ai_next;
// 			}
// 		}
// 	}
// 
// 	::WSACleanup();
// 
// 	return svalue;
// }

bool get_local_ip(list<string> & list) {

#ifdef WIN32
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	::WSAStartup(sockVersion, &wsaData);
#endif // WIN32



	char               buf[100];
	struct addrinfo    hints;
	struct addrinfo    *res, *curr;
	struct sockaddr_in *sa;

	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int iRet = gethostname(buf, sizeof(buf));
	if (iRet == 0) {
		if ((getaddrinfo(buf, NULL, &hints, &res)) == 0) {
			curr = res;
			while (curr ) {
				sa = (struct sockaddr_in *)curr->ai_addr;
				//string sname = curr->ai_canonname;//hostname
				std::string ip_ =inet_ntop(AF_INET, &sa->sin_addr.s_addr, buf, sizeof(buf));

				list.push_back(ip_);
				curr = curr->ai_next;
			}
			freeaddrinfo(res);
		}
	}
	else {
#ifdef WIN32
		::WSACleanup();
#endif // WIN32
		return false;
	}

#ifdef WIN32
	::WSACleanup();
#endif // WIN32

	return true;
}

bool parse_ip(const std::string& strValue, AttrIP & ip) {
	if (strValue.empty())
        return false;
    unsigned int i = 0;
	int iSubIpIndex = 0;
	std::string sSubIp;
	while (strValue.length() > i) {
		if ('0' <= strValue[i] && strValue[i] <= '9')
            sSubIp += strValue[i];
		else if (strValue[i] == '.') {
			int iValue = atoi(sSubIp.c_str());
			if (iValue < 0 || iValue > 255)
                return false;
			ip.SetValue(iSubIpIndex, iValue);
			iSubIpIndex++;
			sSubIp.clear();
		}
		else if (strValue[i] == ' ') {

        }
		else
            return false;
		i++;

	}
	int iValue = atoi(sSubIp.c_str());
	if (iValue < 0 || iValue > 255)
        return false;
	if (iSubIpIndex != 3)
		return false;

	ip.SetValue(iSubIpIndex, iValue);
	return true;
}

bool parse_ip(const std::string& strValue, AttrIP & ip1, AttrIP & ip2) {
    size_t iIndex = strValue.find('~');
	if (iIndex > 0)	{
		if (!parse_ip(strValue.substr(0, iIndex), ip1))
            return false;
		if (!parse_ip(strValue.substr(iIndex + 1), ip2))
            return false;

	}
	return true;
}