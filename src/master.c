#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "master.h"
#include "logger.h"
#include "config.h"

Hook executor_start_hook = NULL;

union Caster_to_function_pointer {
    void *ptr;
    void (*function_pointer)(void);
};

union Caster_to_function_pointer2 {
    void *ptr;
    int (*function_pointer)(void);
};

union Caster_to_function_pointer3 {
    void *ptr;
    const char *(*function_pointer)(void);
};

void execute_hook(void) {
    if (executor_start_hook) {
        executor_start_hook();
    }
    else {
        printf("No hook registered.\n");
    }
}

int main() {
    if (init_logger(NULL, -1)) {
        fprintf(stderr, "Failed to initialize the logger\n");
        return 1;
    }

    if (create_config_table()) {
        fprintf(stderr, "Failed to initialize the config\n");
        if (fini_logger() == 1) {
	    fprintf(stderr, "Couldn't shut down logger\n");
	    return 1;
	}
        return 1;
    }

    void *handle = dlopen("/home/anvea/Desktop/OSs/NSU-OSs/install/plugins/greeting.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Library couldn't be opened.\n\tLibrary's path is /home/anvea/Desktop/OSs/NSU-OSs/install/plugins/greeting.so\n\tdlopen: %s\n\tcheck plugins folder or rename library", dlerror());

        destroy_config_table();
        if (fini_logger() == 1) {
	    fprintf(stderr, "Couldn't shut down logger\n");
	    return 1;
	}
        return 1;
    }



    union Caster_to_function_pointer3 name_plugin_pointer;
    name_plugin_pointer.ptr = dlsym(handle, "name");

    const char *(*name_plugin)(void) = name_plugin_pointer.function_pointer;
    if (!name_plugin) {
	fprintf(stderr, "Library couldn't execute name.\n\tLibrary's name is greeting. Dlsym message: %s\n\tcheck plugins folder or rename library", dlerror());

	dlclose(handle);
	destroy_config_table();

	if (fini_logger() == 1) {
	    fprintf(stderr, "Couldn't shut down logger\n");
	    return 1;
	}
	return 1;
    }



    union Caster_to_function_pointer2 init_plugin_pointer;
    init_plugin_pointer.ptr = dlsym(handle, "init");

    int (*init_plugin)(void) = init_plugin_pointer.function_pointer;

    if (!init_plugin) {
        fprintf(stderr, "Library couldn't execute init.\n\tLibrary's name is greeting. Dlsym message: %s\n\tcheck plugins folder or rename library", dlerror());

        dlclose(handle);
        destroy_config_table();

        if (fini_logger() == 1) {
	    fprintf(stderr, "Couldn't shut down logger\n");
	    return 1;
	}
        return 1;
    }

    if (init_plugin() == 1) {
	dlclose(handle);
	destroy_config_table();

	if (fini_logger() == 1) {
	    return 1;
	}
	return 1;
    }
    execute_hook();



    union Caster_to_function_pointer2 fini_plugin_pointer;
    fini_plugin_pointer.ptr = dlsym(handle, "fini");
    
    int (*fini_plugin)(void) = fini_plugin_pointer.function_pointer;

    if (!fini_plugin) {
	fprintf(stderr, "Library couldn't execute fini.\n\tLibrary's name is greeting. Dlsym message: %s\n\tcheck plugins folder or rename library", dlerror());

	dlclose(handle);
	destroy_config_table();

	if (fini_logger() == 1) {
	    fprintf(stderr, "Couldn't shut down logger\n");
	    return 1;
	}
	return 1;
    }

    if (fini_plugin() == 1) {
	dlclose(handle);
	destroy_config_table();

	if (fini_logger() == 1) {
	    fprintf(stderr, "Couldn't shut down logger\n");
	    return 1;
	}
	return 1;
    }

    dlclose(handle);
    destroy_config_table();

    if (fini_logger() == 1) {
	fprintf(stderr, "Couldn't shut down logger\n");
	return 1;
    }

    return 0;
}
