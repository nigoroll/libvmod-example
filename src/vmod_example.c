#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define __USE_GNU
#include <dlfcn.h>

#include "cache/cache.h"
#include "vrt.h"
#include "vcl.h"
#include "vqueue.h"
#include "vapi/vsl.h"

#include "vtim.h"
#include "vcc_example_if.h"

const size_t infosz = 64;
char	     *info;

/*
 * handle vmod internal state, vmod init/fini and/or varnish callback
 * (un)registration here.
 *
 * malloc'ing the info buffer is only indended as a demonstration, for any
 * real-world vmod, a fixed-sized buffer should be a global variable
 */

int __match_proto__(vmod_event_f)
event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
	char	   ts[VTIM_FORMAT_SIZE];
	const char *event = NULL;

	(void) ctx;
	(void) priv;

	switch (e) {
	case VCL_EVENT_LOAD:
		info = malloc(infosz);
		if (! info)
			return (-1);
		event = "loaded";
		break;
	case VCL_EVENT_WARM:
		event = "warmed";
		break;
	case VCL_EVENT_COLD:
		event = "cooled";
		break;
	case VCL_EVENT_DISCARD:
		free(info);
		return (0);
		break;
	default:
		return (0);
	}
	AN(event);
	VTIM_format(VTIM_real(), ts);
	snprintf(info, infosz, "vmod_example %s at %s", event, ts);

	return (0);
}

VCL_STRING
vmod_info(VRT_CTX)
{
	(void) ctx;

	return (info);
}

VCL_STRING
vmod_hello(VRT_CTX, VCL_STRING name)
{
	char *p;
	unsigned u, v;

	u = WS_Reserve(ctx->ws, 0); /* Reserve some work space */
	p = ctx->ws->f;		/* Front of workspace area */
	v = snprintf(p, u, "Hello, %s", name);
	v++;
	if (v > u) {
		/* No space, reset and leave */
		WS_Release(ctx->ws, 0);
		return (NULL);
	}
	/* Update work space with what we've used */
	WS_Release(ctx->ws, v);
	return (p);
}

struct myvcl {
	unsigned                magic;
#define VCL_MAGIC               0x214188f2
	VTAILQ_ENTRY(vcl)       list;
	void                    *dlh;
};


VCL_STRING
vmod_call(VRT_CTX, VCL_STRING sym)
{
	vcl_func_f	*func;
	struct myvcl	*vcl;
	const int	l = 64;
	char		buf[l];
	const char	*prefix = "VGC_function_";

	CAST_OBJ_NOTNULL(vcl, (void *)ctx->vcl, VCL_MAGIC);

	assert (l > strlen(prefix));
	strcpy(buf, prefix);
	strncat(buf, sym, l - strlen(prefix) - 1);
	sym = buf;
	errno = 0;
	func = dlsym(vcl->dlh, sym);
	VSL(SLT_Debug, 0, "example.call: sub %s -> %p", sym, func);
	if (func)
		func(ctx);
	return(strerror(errno));
}
