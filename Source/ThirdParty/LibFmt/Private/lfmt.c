#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>

#include <string.h>
#include <float.h>

#if LUA_VERSION_NUM == 501
static void luaL_tolstring(lua_State *L, int idx, size_t *len) {
    int tt; const char *kind; (void)len;
    if (luaL_callmeta(L, idx, "__tostring")) {
        if (!lua_isstring(L, -1))
            luaL_error(L, "'__tostring' must return a string");
        return;
    }
    switch (lua_type(L, idx)) {
    case LUA_TSTRING:
        lua_pushvalue(L, idx); break;
    case LUA_TBOOLEAN:
        lua_pushstring(L, (lua_toboolean(L, idx) ? "true" : "false"));
        break;
    case LUA_TNIL:
        lua_pushliteral(L, "nil");
        break;
    default: 
        tt = luaL_getmetafield(L, idx, "__name");
        kind = (tt == LUA_TSTRING) ? lua_tostring(L, -1) :
            luaL_typename(L, idx);
        lua_pushfstring(L, "%s: %p", kind, lua_topointer(L, idx));
        if (tt != LUA_TNIL) lua_remove(L, -2);
    }
}
#endif

#if LUA_VERSION_NUM < 502 && !defined(LUA_OK)
static lua_Integer lua_tointegerx(lua_State *L, int idx, int *isint) { 
    lua_Integer i = lua_tointeger(L, idx);
    *isint = i == 0 ? lua_type(L, idx)==LUA_TNUMBER : lua_tonumber(L, idx)==i;
    return i;
}
#endif

#if LUA_VERSION_NUM < 503
static void lua_geti(lua_State *L, int idx, int i)
{ lua_pushinteger(L, i); lua_gettable(L, idx); }

static int lua_isinteger(lua_State *L, int idx) {
    lua_Number v = lua_tonumber(L, idx);
    if (v == 0.0 && lua_type(L,idx) != LUA_TNUMBER) return 0;
    return (lua_Number)(lua_Integer)v == v;
}
#endif

typedef struct fmt_State {
    lua_State *L;
    luaL_Buffer B;
    int idx, top, zeroing;
    const char *p, *e;
} fmt_State;


/* read argid */

#define fmt_value(S,i)  ((S)->top+(i))
#define fmt_isdigit(ch) ((ch) >= '0' && (ch) <= '9')
#define fmt_isalpha(ch) ((ch) == '_' \
        || ((ch) >= 'A' && (ch) <= 'Z') \
        || ((ch) >= 'a' && (ch) <= 'z'))

static int fmt_integer(fmt_State *S, int *pv) {
    const char *p = S->p;
    unsigned idx = 0;
    while (p < S->e && fmt_isdigit(*p)) {
        if (idx > INT_MAX/10 || (idx == INT_MAX/10 && *p++ > INT_MAX%10))
            return luaL_error(S->L, "Too many decimal digits in format string");
        idx = idx*10 + (*p++ - '0');
    }
    if (p == S->p) return 0;
    if (pv) *pv = (int)idx;
    S->p = p;
    return 1;
}

static int fmt_identity(fmt_State *S) {
    const char *p = S->p;
    if (fmt_isalpha(*p))
        while (++p < S->e && (fmt_isalpha(*p) || fmt_isdigit(*p)))
            ;
    if (p == S->p) return 0;
    lua_pushlstring(S->L, S->p, p - S->p);
    S->p = p;
    return 1;
}

static int fmt_accessor(fmt_State *S, int to) {
    /* "." (number | identity) | "[" <anychar except ']'> "]" */
    while (*S->p == '.' || *S->p == '[') {
        int idx;
        const char *p = ++S->p;
        if (p[-1] == '.') {
            if (fmt_integer(S, &idx))
                lua_geti(S->L, to, idx);
            else if (fmt_identity(S))
                lua_gettable(S->L, to);
            else luaL_error(S->L, "unexpected '%c' in field name", *S->p);
        } else if (fmt_integer(S, &idx) && *S->p == ']')
            lua_geti(S->L, to, idx), ++S->p;
        else {
            while (p < S->e && *p != ']') ++p;
            if (p == S->e)
                return luaL_error(S->L, "expected '}' before end of string");
            lua_pushlstring(S->L, S->p, p - S->p);
            S->p = p + 1;
            lua_gettable(S->L, to);
        }
        lua_replace(S->L, to);
    }
    return 1;
}

static int fmt_checkidx(fmt_State *S, int haskey) {
    if (haskey) {
        if (S->idx > 1)
            return luaL_error(S->L,
                    "cannot switch from automatic field numbering"
                    " to manual field specification");
        return S->idx = 0;
    } else {
        if (S->idx == 0)
            return luaL_error(S->L,
                    "cannot switch from manual field specification"
                    " to automatic field numbering");
        return ++S->idx;
    }
}

static int fmt_argid(fmt_State *S, int to) {
    /* [(number | identity) [accessor]] */
    int idx;
    if (*S->p == ':' || *S->p == '}') {
        if (S->idx >= S->top)
            return luaL_error(S->L, "automatic index out of range");
        lua_pushvalue(S->L, fmt_checkidx(S, 0));
    } else if (fmt_integer(S, &idx)) {
        fmt_checkidx(S, 1);
        if (idx < 1 || idx > S->top)
            return luaL_error(S->L, "argument index out of range");
        lua_pushvalue(S->L, idx+1);
    } else {
        fmt_checkidx(S, 1);
        if (!fmt_identity(S))
            return luaL_error(S->L, "unexpected '%c' in field name", *S->p);
        lua_gettable(S->L, 2);
    }
    lua_replace(S->L, to);
    return fmt_accessor(S, to);
}


/* read spec */

static int fmt_checkspec(fmt_State *S)
{ return S->p < S->e ? 1 : luaL_error(S->L, "unmatched '{' in format spec"); }

static int fmt_readchar(fmt_State *S)
{ int ch = *S->p++; fmt_checkspec(S); return ch; }

typedef struct fmt_Spec {
    int fill;
    int align;
    int sign;
    int alter;
    int zero;
    int width;
    int grouping;
    int precision;
    int type;
} fmt_Spec;

static int fmt_readint(fmt_State *S, int required, const char *name) {
    int isint, v = 0;
    if (*S->p != '{') {
        if (!fmt_integer(S, &v) && required)
            return luaL_error(S->L, "Format specifier missing %s", name);
        fmt_checkspec(S);
    } else {
        ++S->p;
        fmt_argid(S, fmt_value(S, 2));
        if (*S->p != '}')
            return luaL_error(S->L, "unexpected '%c' in field name", *S->p);
        ++S->p;
        v = lua_tointegerx(S->L, fmt_value(S, 2), &isint);
        if (!isint)
            return luaL_error(S->L, "integer expected for %s, got %s",
                    name, luaL_typename(S->L, fmt_value(S, 2)));
    }
    return v;
}

static int fmt_spec(fmt_State *S, fmt_Spec *d) {
    /* [[fill]align][sign]["#"]["0"][width][grouping]["." precision][type] */
    if (*S->p == '<' || *S->p == '>' || *S->p == '^')
        d->align = fmt_readchar(S);
    else if (S->p[1] == '<' || S->p[1] == '>' || S->p[1] == '^')
        d->fill  = fmt_readchar(S), d->align = fmt_readchar(S);
    if (*S->p == '+' || *S->p == '-' || *S->p == ' ')
        d->sign = fmt_readchar(S);
    if (*S->p == '#') d->alter = fmt_readchar(S);
    if (*S->p == '0') d->zero  = fmt_readchar(S);
    d->width = fmt_readint(S, 0, "width");
    if (*S->p == '_' || *S->p == ',') d->grouping = fmt_readchar(S);
    if (*S->p == '.') ++S->p, d->precision = fmt_readint(S, 1, "precision");
    if (*S->p != '}') {
        const char *p = S->p++;
        d->type = *p;
        if (*S->p != '}') {
            while (S->p < S->e && *S->p != '}') ++S->p;
            fmt_checkspec(S);
            return luaL_error(S->L, "Invalid format specifier: '%s'", p);
        }
    }
    return 1;
}


/* write spec */

#define FMT_DELIMITPOS  3
#define FMT_UTF8BUFFSIZ 8
#define FMT_FMTLEN      10 /* "%#.99f" */
#define FMT_INTBUFFSIZ  100
#define FMT_FLTBUFFSIZ  (110 + FLT_MAX_10_EXP)

static void fmt_addpadding(fmt_State *S, int ch, size_t len) {
    char *s;
    if (ch == 0) ch = ' ';
    while (len > LUAL_BUFFERSIZE) {
        s = luaL_prepbuffer(&S->B);
        memset(s, ch, LUAL_BUFFERSIZE);
        luaL_addsize(&S->B, LUAL_BUFFERSIZE);
        len -= LUAL_BUFFERSIZE;
    }
    s = luaL_prepbuffer(&S->B);
    memset(s, ch, len);
    luaL_addsize(&S->B, len);
}

static void fmt_addzeroing(fmt_State *S, const fmt_Spec *d, size_t len) {
    char *s;
    if (!d->zero || d->grouping == 0) {
         fmt_addpadding(S, d->fill ? d->fill : d->zero, len);
         return;
    }
    s = luaL_prepbuffer(&S->B);
    if (len > (size_t)S->zeroing) {
        int pref = (len - S->zeroing) % 4;
        if (pref > 2) *s++ = '0', luaL_addsize(&S->B, 1);
        if (pref > 0) *s++ = '0', *s++ = d->grouping, luaL_addsize(&S->B, 2);
        len -= pref;
        while (len > 4) {
            size_t curr = len > LUAL_BUFFERSIZE ? LUAL_BUFFERSIZE : len;
            s = luaL_prepbuffer(&S->B);
            while (curr > 4) {
                s[0] = s[1] = s[2] = '0', s[3] = d->grouping;
                s += 4, luaL_addsize(&S->B, 4), curr -= 4, len -= 4;
            }
        }
    }
    memset(s, '0', len), luaL_addsize(&S->B, len);
}

static int fmt_addstring(fmt_State *S, int shrink, size_t width, const fmt_Spec *d) {
    size_t len, plen;
    const char *s = lua_tolstring(S->L, fmt_value(S,1), &len);
    if (shrink && d->precision)
        len = len > (size_t)d->precision ? (size_t)d->precision : len;
    if (len > width) {
        lua_pushvalue(S->L, fmt_value(S,1));
        luaL_addvalue(&S->B);
        return 1;
    }
    plen = width - (int)len;
    switch (d->align) {
    case 0:
    case '<': fmt_addzeroing(S, d, plen);
              luaL_addlstring(&S->B, s, len); break;
    case '>': luaL_addlstring(&S->B, s, len);
              fmt_addpadding(S, d->fill, plen); break;
    case '^': fmt_addpadding(S, d->fill, plen/2);
              luaL_addlstring(&S->B, s, len);
              fmt_addpadding(S, d->fill, plen - plen/2); break;
    }
    return 1;
}

static int fmt_dumpstr(fmt_State *S, const fmt_Spec *d) {
    if (d->type != 0 && d->type != 's' && d->type != 'p')
        return luaL_error(S->L,
                "Unknown format code '%c' for object of type 'string'", d->type);
    if (d->sign)     return luaL_error(S->L,
            "Sign not allowed in string format specifier");
    if (d->alter)    return luaL_error(S->L,
            "Alternate form (#) not allowed in string format specifier");
    if (d->zero)     return luaL_error(S->L,
            "Zero form (0) not allowed in string format specifier");
    if (d->grouping) return luaL_error(S->L,
            "Grouping form (%c) not allowed in string format specifier",
            d->grouping);
    return fmt_addstring(S, 1, d->width, d);
}

static void fmt_pushutf8(fmt_State *S, unsigned long x) {
    int n = 1;
    char buff[FMT_UTF8BUFFSIZ];
    if (x < 0x80)
        buff[FMT_UTF8BUFFSIZ - 1] = (char)x;
    else {
        unsigned int mfb = 0x3f;
        do {
            buff[FMT_UTF8BUFFSIZ - (n++)] = (char)(0x80 | (x & 0x3f));
            x >>= 6;
            mfb >>= 1;
        } while (x > mfb);
        buff[FMT_UTF8BUFFSIZ - n] = (char)((~mfb << 1) | x);
    }
    lua_pushlstring(S->L, buff + FMT_UTF8BUFFSIZ - n, n);
}

static int fmt_dumpchar(fmt_State *S, lua_Integer cp, const fmt_Spec *d) {
    if (d->sign)                return luaL_error(S->L,
            "Sign not allowed with integer format specifier 'c'");
    if (d->alter)               return luaL_error(S->L,
            "Alternate form (#) not allowed with integer format specifier 'c'");
    if (d->zero)                return luaL_error(S->L,
            "Zero form (0) not allowed with integer format specifier 'c'");
    if (d->grouping)            return luaL_error(S->L,
            "Cannot specify '%c' with 'c'", d->grouping);
    if (cp < 0 || cp > INT_MAX) return luaL_error(S->L,
            "'c' arg not in range(%d)", INT_MAX);
    fmt_pushutf8(S, (unsigned long)cp);
    lua_replace(S->L, fmt_value(S,1));
    return fmt_addstring(S, 0, d->width, d);
}

static int fmt_writesign(int sign, int dsign) {
    switch (dsign) {
    case '+': return sign ? '+' : '-';
    case ' ': return sign ? ' ' : '-';
    default:  return sign ?  0  : '-';
    }
}

static int fmt_writeint(char **pp, lua_Integer v, const fmt_Spec *d) {
    const char *hexa = "0123456789abcdef";
    int radix = 10, zeroing;
    char *p = *pp;
    switch (d->type) {
    case 'b': case 'B': radix =  2; break;
    case 'o': case 'O': radix =  8; break;
    case 'X': hexa = "0123456789ABCDEF"; /* FALLTHROUGH */
    case 'x':           radix = 16; break;
    }
    zeroing = d->grouping ? FMT_DELIMITPOS : 0;
    while (*--p = hexa[v % radix], v /= radix, --zeroing, v)
        if (!zeroing) zeroing = FMT_DELIMITPOS, *--p = d->grouping;
    *pp = p;
    return zeroing;
}

static int fmt_dumpint(fmt_State *S, lua_Integer v, const fmt_Spec *d) {
    char buff[FMT_INTBUFFSIZ], *p = buff + FMT_INTBUFFSIZ, *dp;
    int sign = !(v < 0), width = d->width;
    if (!sign) v = -v;
    S->zeroing = fmt_writeint(&p, v, d);
    dp = p;
    if (d->alter && d->type != 0 && d->type != 'd')
        *--p = d->type, *--p = '0';
    if ((p[-1] = fmt_writesign(sign, d->sign)) != 0) --p;
    if (!d->zero || d->width <= FMT_INTBUFFSIZ - (p-buff))
        lua_pushlstring(S->L, p, FMT_INTBUFFSIZ - (p-buff));
    else {
        luaL_addlstring(&S->B, p, dp - p);
        width -= (dp - p);
        lua_pushlstring(S->L, dp, FMT_INTBUFFSIZ - (dp-buff));
    }
    lua_replace(S->L, fmt_value(S,1));
    return fmt_addstring(S, 0, width, d);
}

static int fmt_writeflt(char *s, size_t n, lua_Number v, const fmt_Spec *d) {
    int type = d->type ? d->type : 'g';
    int (*ptr_snprintf)(char *s, size_t n, const char *fmt, ...) = snprintf;
    char fmt[FMT_FMTLEN];
    const char *percent = "";
    if (d->type == '%') type = 'f', v *= 100.0, percent = "%%";
    if (d->precision)
        ptr_snprintf(fmt, FMT_FMTLEN, "%%%s.%d%c%s",
                d->alter ? "#" : "", d->precision, type, percent);
    else if ((lua_Number)(lua_Integer)v == v)
        ptr_snprintf(fmt, FMT_FMTLEN, "%%.1f%s", percent);
    else
        ptr_snprintf(fmt, FMT_FMTLEN, "%%%s%c%s",
                d->alter ? "#" : "", type, percent);
    return ptr_snprintf(s, n, fmt, v);
}

static int fmt_dumpflt(fmt_State *S, lua_Number v, const fmt_Spec *d) {
    int sign = !(v < 0), len, width = d->width;
    char buff[FMT_FLTBUFFSIZ], *p = buff;
    if (d->precision > 99) return luaL_error(S->L,
            "precision specifier too large");
    if (d->grouping)       return luaL_error(S->L,
            "Grouping form (%c) not allowed in float format specifier",
            d->grouping);
    if (!sign) v = -v;
    if ((*p = fmt_writesign(sign, d->sign)) != 0) ++p;
    len = fmt_writeflt(p, FMT_FLTBUFFSIZ - (p-buff), v, d);
    if (!d->zero || width <= len)
        lua_pushlstring(S->L, buff, len + (p-buff));
    else {
        if (p != buff) luaL_addlstring(&S->B, buff, p-buff);
        width -= p-buff;
        lua_pushlstring(S->L, p, len);
    }
    lua_replace(S->L, fmt_value(S,1));
    return fmt_addstring(S, 0, width, d);
}

static int fmt_dumpnumber(fmt_State *S, const fmt_Spec *d) {
    int type = d->type;
    if (type == 0) type = lua_isinteger(S->L, fmt_value(S,1)) ? 'd' : 'g';
    switch (type) {
    case 'c':
        return fmt_dumpchar(S, lua_tointeger(S->L, fmt_value(S,1)), d);
    case 'd': case 'b': case 'B': case 'o': case 'O': case 'x': case 'X':
        return fmt_dumpint(S, lua_tointeger(S->L, fmt_value(S,1)), d);
    case 'e': case 'E': case 'f': case 'F': case 'g': case 'G': case '%':
        return fmt_dumpflt(S, lua_tonumber(S->L, fmt_value(S,1)), d);
    default: return luaL_error(S->L,
                     "Unknown format code '%c' for object of type 'number'",
                     d->type);
    }
}

static int fmt_dump(fmt_State *S, const fmt_Spec *d) {
    int type = lua_type(S->L, fmt_value(S,1));
    if (type == LUA_TNUMBER)
        return fmt_dumpnumber(S, d);
    else {
        if (d->type != 'p')
            luaL_tolstring(S->L, fmt_value(S,1), NULL);
        else {
            if (type == LUA_TNIL || type == LUA_TBOOLEAN)
                return luaL_error(S->L,
                        "Unknown format code '%c' for object of type '%s'",
                        d->type, lua_typename(S->L, type));
            lua_pushfstring(S->L, "%p", lua_topointer(S->L, fmt_value(S,1)));
        }
        lua_replace(S->L, fmt_value(S,1));
        return fmt_dumpstr(S, d);
    }
    return 1;
}


/* format */

static int fmt_parse(fmt_State *S, fmt_Spec *d) {
    /* "{" [arg_id] [":" format_spec] "}" */
    fmt_argid(S, fmt_value(S, 1));
    if (*S->p == ':' && ++S->p < S->e)
        fmt_spec(S, d);
    if (S->p >= S->e || *S->p != '}') return luaL_error(S->L,
            "expected '}' before end of string");
    ++S->p;
    return 1;
}

static int fmt_format(fmt_State *S) {
    lua_settop(S->L, fmt_value(S, 2));
    luaL_buffinit(S->L, &S->B);
    while (S->p < S->e) {
        const char *p = S->p;
        while (p < S->e && *p != '{' && *p != '}') ++p;
        luaL_addlstring(&S->B, S->p, p - S->p), S->p = p;
        if (*S->p == S->p[1])
            luaL_addchar(&S->B, *S->p), S->p += 2;
        else {
            fmt_Spec d;
            if (*S->p++ == '}' || S->p >= S->e) return luaL_error(S->L,
                    "Single '%c' encountered in format string", S->p[-1]);
            memset(&d, 0, sizeof(d));
            fmt_parse(S, &d);
            fmt_dump(S, &d);
        }
    }
    luaL_pushresult(&S->B);
    return 1;
}

static int Lformat(lua_State *L) {
    size_t len;
    fmt_State S;
    S.p   = luaL_checklstring(L, 1, &len);
    S.e   = S.p + len;
    S.L   = L;
    S.idx = 1;
    S.top = lua_gettop(L);
    return fmt_format(&S);
}

LUALIB_API int luaopen_fmt(lua_State *L) {
    lua_pushcfunction(L, Lformat);
    return 1;
}

/* cc: flags+='-O3 --coverage -pedantic -I/usr/local/include/luajit-2.0'
 * unixcc: flags+='-shared -fPIC' output='fmt.so'
 * maccc: flags+='-undefined dynamic_lookup'
 * win32cc: flags+='-s -mdll -DLUA_BUILD_AS_DLL '
 * win32cc: libs+='-llua53' output='fmt.dll' */

