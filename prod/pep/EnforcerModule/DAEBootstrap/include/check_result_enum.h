#ifndef __DAEBOOTSTRAP_CHECK_RESULT_ENUM_H__
#define __DAEBOOTSTRAP_CHECK_RESULT_ENUM_H__

enum class CheckResult {
    kSucceed = 0x0000,
    kConfigFailed = 0x0001,
    kFileMissingFailed = 0x0002,
    kQueryJpcFailed = 0x0004,
    kQueryCCPolicyCacheFailed = 0x0008
};

inline CheckResult operator|(const CheckResult& l, const CheckResult& r) {
	return static_cast<CheckResult>(static_cast<int>(l) | static_cast<int>(r));
}

inline CheckResult& operator|=(CheckResult& l, const CheckResult& r) {
    l = l | r;
    return l;
}

inline CheckResult operator&(const CheckResult& l, const CheckResult& r) {
	return static_cast<CheckResult>(static_cast<int>(l) & static_cast<int>(r));
}

inline CheckResult& operator&=(CheckResult& l, const CheckResult& r) {
    l = l & r;
    return l;
}

#endif//__DAEBOOTSTRAP_CHECK_RESULT_ENUM_H__