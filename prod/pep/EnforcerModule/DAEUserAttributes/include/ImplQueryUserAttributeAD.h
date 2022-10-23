#ifndef AD_QUERY
#define AD_QUERY

/* this file is cross platform */

class IQueryUserAttribute;
class IUserAttr;
IQueryUserAttribute* make_ad_impl(const char *ldap, const char *u_name, const char *pwd);
IQueryUserAttribute* make_ad_impl2(const char *attrs);
bool query_ad_attr_2_user(IUserAttr * pUser, const char * user_attrs);
#endif