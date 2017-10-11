#include "funcs.cpp"
#include "duktape.h"
#include "duk_print_alert.h"

#define CREATE_ARG "--create"
#define DELETE_ARG "--delete"


using namespace std;

void print_args_error() {
	cout << "First argument may be: " << endl << endl;
	cout << "'--create' to create directory withs some files;" << endl;
	cout << "'--delete' to delete directory" << endl;
}

int main(int argc, char const *argv[])
{
	if (argc <= 1) {
		print_args_error();
		return 0;
	}

	duk_context *ctx = duk_create_heap_default();
	duk_print_alert_init(ctx, 0);

	duk_push_global_object(ctx);
	duk_push_c_function(ctx, native_write_file, 2);
	duk_put_prop_string(ctx, -2, "writeFile");
	duk_push_c_function(ctx, native_create_dir, 1);
	duk_put_prop_string(ctx, -2, "createDir");
	duk_push_c_function(ctx, native_read_dir, 1);
	duk_put_prop_string(ctx, -2, "readDir");
	duk_push_c_function(ctx, native_rm_file, 1);
	duk_put_prop_string(ctx, -2, "rmFile");
	duk_push_c_function(ctx, native_rm_dir, 1);
	duk_put_prop_string(ctx, -2, "rmDir");
	duk_push_c_function(ctx, native_get_current_dir, 0);
	duk_put_prop_string(ctx, -2, "currentDir");

	if (strcmp(argv[1], CREATE_ARG) == 0) {
		push_file_as_string(ctx, "createdir.js");
	} else if (strcmp(argv[1], DELETE_ARG) == 0) {
		push_file_as_string(ctx, "removedir.js");
	} else {
		print_args_error();
		duk_destroy_heap(ctx);
		return 0;
	}

	if (duk_peval(ctx) != 0) {
        cout << "Error running: " << duk_safe_to_string(ctx, -1) << endl;
    }
	duk_pop(ctx);

	duk_destroy_heap(ctx);

	return 0;
}