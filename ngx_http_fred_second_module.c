#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


#define HELLO_WORLD "im fredhehehe\r\n"

static char *ngx_http_fred_second(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_fred_second_handler(ngx_http_request_t *r);

static void* ngx_http_fred_second_create_loc_conf(ngx_conf_t *cf);

static char* ngx_http_fred_second_merge_loc_conf(ngx_conf_t *cf,
    void *parent, void *child);

static void ngx_http_variable_set_fred_mode(ngx_http_request_t *r,
     ngx_http_variable_value_t *v, uintptr_t data);

static void ngx_http_variable_set_fred_mode(ngx_http_request_t *r,
     ngx_http_variable_value_t *v, uintptr_t data);

static ngx_int_t ngx_http_fred_second_add_variables(ngx_conf_t *cf);

ngx_int_t
ngx_fred_parse_mode(ngx_str_t *line);

typedef struct {
    ngx_uint_t mode;
} ngx_http_fred_second_loc_conf_t;

// Defining my embedded variable
static ngx_http_variable_t  ngx_http_fred_second_variables[] = {

    { ngx_string("mode"), ngx_http_variable_set_fred_mode,
      NULL,
      0,
      NGX_HTTP_VAR_CHANGEABLE|NGX_HTTP_VAR_NOCACHEABLE, 0 },
    
     ngx_http_null_variable
};  

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
    ngx_http_fred_second_add_variables, /* preconfiguration */
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

ngx_int_t
ngx_fred_parse_mode(ngx_str_t *line)
{
    size_t   len;
    ngx_uint_t mode;

    len = line->len;

    if (len == 0) {
        return NGX_ERROR;
    }

    if (ngx_strcmp(line->data, "ua") == 0) {
        mode = 1;
        return mode;
    } 
    if (ngx_strcmp(line->data, "ip") == 0) {
        mode = 0;
        return mode;
    }
    return NGX_ERROR;
}

static void 
ngx_http_variable_set_fred_mode(ngx_http_request_t *r,
     ngx_http_variable_value_t *v, uintptr_t data) 
{
    ngx_http_fred_second_loc_conf_t  *fred_second_config;
    fred_second_config = ngx_http_get_module_loc_conf(r, ngx_http_fred_second_module);
    
    ngx_int_t    s;
    ngx_str_t  val;

    val.len = v->len;
    val.data = v->data;

    s = ngx_fred_parse_mode(&val); // need declare this 

    if (s == NGX_ERROR) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0,
                      "invalid $mode \"%V\"", &val);
        return;
    }

    fred_second_config->mode = (ngx_uint_t) s;
}

// Add it in preconfiguration
static ngx_int_t
ngx_http_fred_second_add_variables(ngx_conf_t *cf)
{
    ngx_http_variable_t  *var, *v;

    for (v = ngx_http_fred_second_variables; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }
        var->set_handler = v->set_handler;
        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    return NGX_OK;
}

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

    ngx_conf_merge_uint_value(conf->mode, prev->mode, 0);

    if (conf->mode > 2) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, 
            "mode variable set incorrectly");
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
    ngx_http_fred_second_loc_conf_t  *fred_second_config;
    fred_second_config = ngx_http_get_module_loc_conf(r, ngx_http_fred_second_module);
    // ngx_http_variable_value_t  *v;

    // v = ngx_http_get_flushed_variable(r, index);

    // if (v == NULL || v->not_found) {
    //     /* we failed to get value or there is no such variable, handle it */
    //     return NGX_ERROR;
    // }   

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

    
    
    if (fred_second_config->mode == 1) {
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
    } else if (fred_second_config->mode == 0) {
        ngx_str_t *ip;
        ip = &r->connection->addr_text;
        if (ip == NULL) {
            return NGX_DECLINED;
        }

        b->pos = ip->data; /* first position in memory of the data */
        b->last = ip->data + ip->len; /* last position in memory of the data */
        b->memory = 1; /* content is in read-only memory */
        b->last_buf = 1; /* there will be no more buffers in the request */

        /* Sending the headers for the reply. */
        r->headers_out.status = NGX_HTTP_OK; /* 200 status code */
        /* Get the content length of the body. */
        r->headers_out.content_length_n = ip->len;
    } else {
        ngx_str_t *host;     
        host = &r->headers_in.host->value;
        if (host == NULL) {
            return NGX_DECLINED;
        }

        b->pos = host->data; /* first position in memory of the data */
        b->last = host->data + host->len; /* last position in memory of the data */
        b->memory = 1; /* content is in read-only memory */
        b->last_buf = 1; /* there will be no more buffers in the request */

        /* Sending the headers for the reply. */
        r->headers_out.status = NGX_HTTP_OK; /* 200 status code */
        /* Get the content length of the body. */
        r->headers_out.content_length_n = host->len;
    }
    
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
