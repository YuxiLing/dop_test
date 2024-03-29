#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define min(val1, val2)  ((val1 > val2) ? (val2) : (val1))
#define NGX_HTTP_DISCARD_BUFFER_SIZE    4096

typedef struct chunk chunk;

struct chunk
{
    long long size;
    char* content;
};


char *user_input1() {
    return "<replacement>";
}

unsigned char *user_input2(){
    return "FFFF";
}

int parse_chunked(unsigned char* pos,
    chunk *ctx)
{
    unsigned char      c,ch;
    int   rc=0;
    for (int idx=0;idx<4;idx++)
    {
        ch=*pos;
        if (ch >= '0' && ch <= '9') {
            ctx->size = ctx->size * 16 + (ch - '0');
            rc=1;
        }
        else{
            c = (unsigned char) (ch | 0x20);
            if (c >= 'a' && c <= 'f') {
                ctx->size = ctx->size * 16 + (c - 'a' + 10);
                rc=1;
            }
            pos++;
        }
    
    }
    
    return rc;
    
}

static void hack_me(chunk *r)
{
    unsigned long     size;
    unsigned char     buffer[NGX_HTTP_DISCARD_BUFFER_SIZE];

    char *content = r->content;

    size = (unsigned long) min(r->size,
                                NGX_HTTP_DISCARD_BUFFER_SIZE);

    memcpy(buffer,content,size);

    return;
}

int main(){
    char *user_content=user_input1();
    struct chunk* r;
    r->content=user_content;
    r->size=sizeof(user_content);
    unsigned char* pos=user_input2();

    // chunk the request
    int rc=0;
    rc=parse_chunked(pos,r);
    if(rc==1){
        hack_me(r);
    }
}
