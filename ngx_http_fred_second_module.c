#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


#define HELLO_WORLD "im fredhehehe\r\n"

static char *ngx_http_fred_second(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_fred_second_handler(ngx_http_request_t *r);

static void* ngx_http_fred_second_create_loc_conf(ngx_conf_t *cf);

static char* ngx_http_fred_second_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);

typedef struct {
    ngx_uint_t mode;
} ngx_http_fred_second_loc_conf_t;

/**
 * This module provided directive: hello world.
 *
 */
static ngx_command_t ngx_http_fred_second_commands[] = {

    { ngx_string("frederick"), /* directive */
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, /* location context and takes
                                            no arguments*/
      ngx_http_fred_second, /* configuration setup function */
      0, /* No offset. Only one context is supported. */
      0, /* No offset when storing the module configuration on struct. */
      NULL},
      
    { ngx_string("frederick_mode"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1, 
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_fred_second_loc_conf_t, mode),
      NULL},

    ngx_null_command /* command termination */
};

/* The hello world string. */
// static u_char ngx_hello_world[] = HELLO_WORLD;

/* The module context. */
static ngx_http_module_t ngx_http_fred_second_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_fred_second_create_loc_conf, /* create location configuration */
    ngx_http_fred_second_merge_loc_conf /* merge location configuration */
};

/* Module definition. */
ngx_module_t ngx_http_fred_second_module = {
    NGX_MODULE_V1,
    &ngx_http_fred_second_module_ctx, /* module context */
    ngx_http_fred_second_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

static void* ngx_http_fred_second_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_fred_second_loc_conf_t *conf; // pointer to the location config struct
    
    // allocate memory for the struct in the cf pool
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_fred_second_loc_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR; // Check for allocation error
    }
    conf->mode = NGX_CONF_UNSET_UINT; // tells the merging function that this value should be overriden
    return conf;
}

static char *
ngx_http_fred_second_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_fred_second_loc_conf_t *prev = parent;
    ngx_http_fred_second_loc_conf_t *conf = child;

    ngx_conf_merge_uint_value(conf->mode, prev->mode, 1);

    if (conf->mode < 1) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, 
            "value must be equal or more than 1");
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}

/**
 * Content handler.
 *
 * @param r
 *   Pointer to the request structure. See http_request.h.
 * @return
 *   The status of the response generation.
 */

//static ngx_int_t ngx_http_fred_second_handler(ngx_http_request_t *r)
//{    
//   ngx_buf_t *b;
//    ngx_chain_t out;

//    /* Set the Content-Type header. */
//    r->headers_out.content_type.len = sizeof("text/plain") - 1;
//    r->headers_out.content_type.data = (u_char *) "text/plain";

    /* Allocate a new buffer for sending out the reply. */
//    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

    /* Insertion in the buffer chain. */
//    out.buf = b;
//    out.next = NULL; /* just one buffer */

//    b->pos = ngx_hello_world; /* first position in memory of the data */
//    b->last = ngx_hello_world + sizeof(ngx_hello_world) - 1; /* last position in memory of the data */
//    b->memory = 1; /* content is in read-only memory */
    // b->last_buf = 1; /* there will be no more buffers in the request */
    
//    ngx_chain_t out2;
    
//    ngx_str_t *ua;
//    ua = &r->headers_in.user_agent->value;
//    if (ua == NULL) {
//        return NGX_DECLINED;
//    }
    
//    ngx_buf_t *b2;
//    b2 = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    
//    out.next = &out2;
//    out2.buf = b2;
//    out2.next = NULL;
    
//    b->pos = ua->data;
//    b->last = ua->data + sizeof(ua->data) - 1;
//    b->memory = 1;
//    b->last_buf = 1;

    /* Sending the headers for the reply. */
//    r->headers_out.status = NGX_HTTP_OK; /* 200 status code */
    /* Get the content length of the body. */
//    r->headers_out.content_length_n = sizeof(ngx_hello_world) - 1 + sizeof(ua->data) - 1;
//    ngx_http_send_header(r); /* Send the headers */

    /* Send the body, and return the status code of the output filter chain. */
//    return ngx_http_output_filter(r, &out);
//} /* ngx_http_hello_world_handler */

static ngx_int_t ngx_http_fred_second_handler(ngx_http_request_t *r)
{
    ngx_buf_t *b;
    ngx_chain_t out;

    /* Set the Content-Type header. */
    r->headers_out.content_type.len = sizeof("text/plain") - 1;
    r->headers_out.content_type.data = (u_char *) "text/plain";

    /* Allocate a new buffer for sending out the reply. */
    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

    /* Insertion in the buffer chain. */
    out.buf = b;
    out.next = NULL; /* just one buffer */
    
    ngx_str_t *ua;
    ua = &r->headers_in.user_agent->value;
    if (ua == NULL) {
        return NGX_DECLINED;
    }

    b->pos = ua->data; /* first position in memory of the data */
    b->last = ua->data + ua->len; /* last position in memory of the data */
    b->memory = 1; /* content is in read-only memory */
    b->last_buf = 1; /* there will be no more buffers in the request */

    /* Sending the headers for the reply. */
    r->headers_out.status = NGX_HTTP_OK; /* 200 status code */
    /* Get the content length of the body. */
    r->headers_out.content_length_n = ua->len;
    ngx_http_send_header(r); /* Send the headers */

    /* Send the body, and return the status code of the output filter chain. */
    return ngx_http_output_filter(r, &out);
} /* ngx_http_hello_world_handler */

/**
 * Configuration setup function that installs the content handler.
 *
 * @param cf
 *   Module configuration structure pointer.
 * @param cmd
 *   Module directives structure pointer.
 * @param conf
 *   Module configuration structure pointer.
 * @return string
 *   Status of the configuration setup.
 */
static char *ngx_http_fred_second(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf; /* pointer to core location configuration */

    /* Install the hello world handler. */
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_fred_second_handler;

    return NGX_CONF_OK;
} /* ngx_http_hello_world */
