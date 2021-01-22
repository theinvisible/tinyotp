#ifndef CONFIG_H
#define CONFIG_H

namespace tinyotp_config
{
    static const char __attribute__ ((unused)) *name = "tinyOTP";
    static const char __attribute__ ((unused)) *version = "0.2.0";
    static const char __attribute__ ((unused)) *file_main = "~/.tinyotp/main.conf";
    static const char __attribute__ ((unused)) *profiles_dir = "~/.tinyotp/profiles";
    static const char __attribute__ ((unused)) *initd_default = "/etc/init.d/tinyotp";

    static const char __attribute__ ((unused)) *password_manager_namespace = "tinyotp";
    static const char __attribute__ ((unused)) *validatorName = "^[a-zA-zäöüÄÖÜ0-9 \\-_]{3,}$";
}

#endif // CONFIG_H
