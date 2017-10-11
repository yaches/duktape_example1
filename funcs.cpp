#include "duktape.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <cstdio>

using namespace std;

static void push_file_as_string(duk_context *ctx, const char *filename) {
    FILE *f;
    size_t len;
    char buf[16384];

    f = fopen(filename, "rb");
    if (f) {
        len = fread((void *) buf, 1, sizeof(buf), f);
        fclose(f);
        duk_push_lstring(ctx, (const char *) buf, (duk_size_t) len);
    } else {
        duk_push_undefined(ctx);
    }
}

static duk_ret_t native_write_file(duk_context *ctx) {
	duk_size_t file_len;

	const char *file_fullname = duk_require_string(ctx, 0);
	const char *text = duk_require_lstring(ctx, 1, &file_len);
	
	if (!(file_fullname || text)) {
		duk_push_false(ctx);
		return 1;
	} 

	ofstream out;
	out.open(file_fullname);
	out << text << endl;
	out.close();

	duk_push_true(ctx); 

	return 1;
}

static duk_ret_t native_create_dir(duk_context *ctx) {
	const char *dirname = duk_require_string(ctx, 0);

	if (!dirname) {
		duk_push_false(ctx);
		return 1;
	}

	int result = mkdir(dirname, 0777);

	if (result == -1) {
		duk_push_false(ctx);
		return 1;
	}

	duk_push_true(ctx);
	return 1;
}

static duk_ret_t native_read_dir(duk_context *ctx) {
	const char *dirname = duk_require_string(ctx, 0);

	if (!dirname) {
		duk_push_undefined(ctx);
		return 1;
	}

	DIR *d;
	struct dirent *dir;
	d = opendir(dirname);

	if (d) {
		duk_idx_t arr_idx = duk_push_array(ctx);
		int i = 0;

		while ((dir = readdir(d)) != NULL) {
			if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
				duk_push_string(ctx, dir->d_name);
				duk_put_prop_index(ctx, arr_idx, i++);
			}
		}

		closedir(d);
	} else {
		duk_push_undefined(ctx);
	}

	return 1;
}

static duk_ret_t native_rm_file(duk_context *ctx) {
	const char *filename = duk_require_string(ctx, 0);

	if (!filename) {
		duk_push_false(ctx);
		return 1;
	}

	remove(filename);
	return 1;
}

static duk_ret_t native_rm_dir(duk_context *ctx) {
	return native_rm_file(ctx);
}

static duk_ret_t native_get_current_dir(duk_context *ctx) {
	char dirname[PATH_MAX];

	if (getcwd(dirname, sizeof(dirname)) != NULL) {
		duk_push_string(ctx, dirname);
	} else {
		duk_push_undefined(ctx);
	}

	return 1;
}