#pragma once
// mg使用辅助类
#include <sys\stat.h>
#include "mongoose.h"


#define MAX_OPTIONS 40
#define MAX_OPTION_LEN 40
#define PATH_MAX MAX_PATH
#define S_ISDIR(x) ((x) & _S_IFDIR)


class CmgManager
{
public:
    CmgManager(void);
    ~CmgManager(void);

    // mg官方c语言辅助函数 
public:
    // 链接启动参数,设置可以参考默认参数config_options
    static void set_option( char **options, const char *name, const char *value );

    // 复制字符串
    static char *sdup( const char *str );

    // 检查web目录
    static void verify_document_root( const char *root );

    // 异常结束
    static void die( const char *fmt, ... );

    // 回调函数
    static void *mongoose_callback( enum mg_event ev, struct mg_connection *conn );

public:
    // 启动web监听
    static bool start_mongoose( char **options );

    // mg信息
    static struct mg_context *ctx;

    // 停止web监听
    static void stop_mongoose();

    // 状态标志
    static bool bRunWeb;

};

extern CmgManager g_mgManager;