/*
  uWSGI plugin that creates custom json-escaped logging variables.
  
  build plugin with `uwsgi --build-plugin <filename.c>`
  and use it with `uwsgi --plugin <filename_plugin.so> ...`
*/
#include <uwsgi.h>

static ssize_t do_encode(struct wsgi_request *wsgi_req, char **buf, long pos, long pos_len) {
    char **var = (char **) (((char *) wsgi_req) + pos);
    uint16_t *varlen = (uint16_t *) (((char *) wsgi_req) + pos_len);

    char *e_json = uwsgi_malloc((*varlen * 2) + 1);
    escape_json(*var, *varlen, e_json);
    *buf = e_json;
    return strlen(*buf);
}

static ssize_t uwsgi_lf_json_uri(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, uri);
    long pos_len = offsetof(struct wsgi_request, uri_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}

static ssize_t uwsgi_lf_json_host(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, host);
    long pos_len = offsetof(struct wsgi_request, host_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}

// see https://github.com/unbit/uwsgi/blob/master/uwsgi.h#L1376
static ssize_t uwsgi_lf_json_uagent(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, user_agent);
    long pos_len = offsetof(struct wsgi_request, user_agent_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}

static ssize_t uwsgi_lf_json_method(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, method);
    long pos_len = offsetof(struct wsgi_request, method_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}

static ssize_t uwsgi_lf_json_addr(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, remote_addr);
    long pos_len = offsetof(struct wsgi_request, remote_addr_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}

static ssize_t uwsgi_lf_json_proto(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, protocol);
    long pos_len = offsetof(struct wsgi_request, protocol_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}

static ssize_t uwsgi_lf_json_referer(struct wsgi_request *wsgi_req, char **buf) {
    long pos = offsetof(struct wsgi_request, referer);
    long pos_len = offsetof(struct wsgi_request, referer_len);
    return do_encode(wsgi_req, buf, pos, pos_len);
}


static void register_logchunks() {
        uwsgi_register_logchunk("json_uri", uwsgi_lf_json_uri, 1);
        uwsgi_register_logchunk("json_host", uwsgi_lf_json_host, 1);
        uwsgi_register_logchunk("json_uagent", uwsgi_lf_json_uagent, 1);
        uwsgi_register_logchunk("json_method", uwsgi_lf_json_method, 1);
        uwsgi_register_logchunk("json_addr", uwsgi_lf_json_addr, 1);
        uwsgi_register_logchunk("json_proto", uwsgi_lf_json_proto, 1);
        uwsgi_register_logchunk("json_referer", uwsgi_lf_json_referer, 1);
}

struct uwsgi_plugin escape_json_plugin = {
        .name = "escape_json",
        .on_load = register_logchunks,
};
