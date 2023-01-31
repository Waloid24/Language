void _nodeInfo_ (node_t * node, int line, const char * file);
void _dumpCode_ (char * code, int line, const char * file);

#define $str(str, ...)          \
    printf (str, ##__VA_ARGS__);    \
    printf ("\nline: %d, %s\n\n", __LINE__, __FILE__);

#define $code(code) _dumpCode_ (code, __LINE__, __FILE__);

#define $node(node) _nodeInfo_ (node, __LINE__, __FILE__);

