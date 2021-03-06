flags = [
        '-std=c++17',
        '-Wall',
        '-I/home/matt/usr/include/liblcf',
        '-I/home/matt/hh3-rm2k/dev/ext/GSL/include',
        '-I/home/matt/hh3-rm2k/dev/hh3core',
        ]

def FlagsForFile( file_name, **kwargs ):
    return {
        # We pass the compiler flags from the database unmodified.
        'flags': flags,

        # We always want to use ycmd's cache, as this significantly improves
        # performance.
        'do_cache': True
    }
