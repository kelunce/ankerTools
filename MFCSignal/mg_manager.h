#pragma once
// mgʹ�ø�����
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

    // mg�ٷ�c���Ը������� 
public:
    // ������������,���ÿ��Բο�Ĭ�ϲ���config_options
    static void set_option( char **options, const char *name, const char *value );

    // �����ַ���
    static char *sdup( const char *str );

    // ���webĿ¼
    static void verify_document_root( const char *root );

    // �쳣����
    static void die( const char *fmt, ... );

    // �ص�����
    static void *mongoose_callback( enum mg_event ev, struct mg_connection *conn );

public:
    // ����web����
    static bool start_mongoose( char **options );

    // mg��Ϣ
    static struct mg_context *ctx;

    // ֹͣweb����
    static void stop_mongoose();

    // ״̬��־
    static bool bRunWeb;

};

extern CmgManager g_mgManager;