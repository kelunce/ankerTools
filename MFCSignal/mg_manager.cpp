#include "StdAfx.h"
#include "mg_manager.h"
#include <mmsystem.h>

CmgManager g_mgManager;

CmgManager::CmgManager(void)
{
}

CmgManager::~CmgManager(void)
{
    if(NULL != ctx)
        stop_mongoose();
}

void CmgManager::set_option( char **options, const char *name, const char *value )
{
    int i;

    if ( !strcmp( name, "document_root" ) || !( strcmp( name, "r" ) ) )
    {
        verify_document_root( value );
    }

    for ( i = 0; i < MAX_OPTIONS - 3; i++ )
    {
        if ( options[i] == NULL )
        {
            options[i] = sdup( name );
            options[i + 1] = sdup( value );
            options[i + 2] = NULL;
            break;
        }
    }

    if ( i == MAX_OPTIONS - 3 )
    {
        die( "%s", "Too many options specified" );
    }
}

char * CmgManager::sdup( const char *str )
{
    char *p;
    if ( ( p = ( char * ) malloc( strlen( str ) + 1 ) ) != NULL )
    {
        strcpy( p, str );
    }
    return p;
}

void CmgManager::verify_document_root( const char *root )
{
    const char *p, *path;
    char buf[PATH_MAX];
    struct stat st;

    path = root;
    if ( ( p = strchr( root, ',' ) ) != NULL && ( size_t ) ( p - root ) < sizeof( buf ) )
    {
        memcpy( buf, root, p - root );
        buf[p - root] = '\0';
        path = buf;
    }

    if ( stat( path, &st ) != 0 || !S_ISDIR( st.st_mode ) )
    {
        die( "Invalid root directory: [%s]: %s", root, strerror( errno ) );
    }
}

void CmgManager::die( const char *fmt, ... )
{
    va_list ap;
    char msg[200];

    va_start( ap, fmt );
    vsnprintf( msg, sizeof( msg ), fmt, ap );
    va_end( ap );

#if defined(_WIN32)
    MessageBox( NULL, msg, "Error", MB_OK );
#else
    fprintf( stderr, "%s\n", msg );
#endif

    exit( EXIT_FAILURE );
}

void * CmgManager::mongoose_callback( enum mg_event ev, struct mg_connection *conn )
{
    static char szRet[MAX_PATH];

    if ( ev == MG_EVENT_LOG )
    {
        //printf( "%s\n", ( const char * ) mg_get_request_info( conn )->ev_data );
        char szTemp[128]={0};
        mg_request_info *pInfo = mg_get_request_info( conn );
        sprintf(szTemp,"uri:%s \t querystring:%s\n",pInfo->uri,pInfo->query_string);
        OutputDebugStr(szTemp);     
    }
    else if ( ev == MG_NEW_REQUEST)
    {
        ZeroMemory(szRet,sizeof(szRet));
        mg_request_info *pInfo = mg_get_request_info( conn );
        if(NULL == pInfo) return NULL;

        // 使用post方式发送数据
        if( stricmp(pInfo->request_method,"POST") == 0 )
        {
            char szPara[1024] = {0};
            char szDst[8196] = {0};
            int nRead = mg_read(conn,szDst,sizeof(szDst));

            char szValues[128] = {0};
            if(mg_get_var(szDst,nRead,"email",szValues,sizeof(szValues)) > 0)
            {
                strcat(szPara,"email=");
                strcat(szPara,szValues);
                strcat(szPara,";  ");
            }

            if(mg_get_var(szDst,nRead,"pwd",szValues,sizeof(szValues)) > 0)
            {
                strcat(szPara,"pwd=");
                strcat(szPara,szValues);
                strcat(szPara,";  ");
            }

            if(mg_get_var(szDst,nRead,"save_login",szValues,sizeof(szValues)) > 0)
            {
                strcat(szPara,"save_login=");
                strcat(szPara,szValues);
                strcat(szPara,";  ");
            }
            
            sprintf(szRet,"post uri:%s para:%s",pInfo->uri,szPara);
                                
        }
        else if( stricmp(pInfo->request_method,"GET") == 0 )
        {
            sprintf(szRet,"get uri:%s  para:%s",pInfo->uri,pInfo->query_string);
        }
        //mg_write_status( conn, szRet, strlen(szRet),200 );
        mg_write( conn, szRet, strlen(szRet) );
        return szRet;
    }
    else if ( ev == MG_REQUEST_COMPLETE )
    {
    }
   

    // Returning NULL marks request as not handled, signalling mongoose to
    // proceed with handling it.
    return NULL;
}

bool CmgManager::start_mongoose( char **options )
{
    if(bRunWeb)
        return true;

    ctx = mg_start( &mongoose_callback, NULL, ( const char ** ) options );
    if ( ctx == NULL )
    {
        return false;
        die( "%s", "Failed to start Mongoose." );
    }

    bRunWeb = true;

    return true;
}

void CmgManager::stop_mongoose()
{   
    if(bRunWeb)
        mg_stop( ctx );  // ctx不能多次关闭,会造成ABA问题

    bRunWeb = false;
}

bool CmgManager::bRunWeb = false;

struct mg_context * CmgManager::ctx = NULL;
