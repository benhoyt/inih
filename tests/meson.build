# keep these in alphabetical order to not misalign return codes
inifiles = [
    'bad_comment.ini',
    'bad_multi.ini',
    'bad_section.ini',
    'bom.ini',
    'duplicate_sections.ini',
    'multi_line.ini',
    'normal.ini',
    'no_file.ini',
    'no_value.ini',
    'user_error.ini',
    ]

ini_cases = {
    'baseline': {
        'opts': [],
        'return codes': [1, 1, 3, 0, 0, 0, 0, -1, 2, 3, ],
        },
    'max line': {
        'opts': ['-DINI_MAX_LINE=20'],
        'return codes': [1, 1, 3, 0, 0, 5, 2, -1, 2, 3, ],
        },
    'single': {
        'opts': ['-DINI_ALLOW_MULTILINE=0'],
        'return codes': [1, 1, 3, 0, 0, 4, 0, -1, 2, 3, ],
        },
    'disallow inline comment': {
        'opts': ['-DINI_ALLOW_INLINE_COMMENTS=0'],
        'return codes': [1, 1, 3, 0, 0, 0, 0, -1, 2, 3, ],
        },
    'stop on first error': {
        'opts': ['-DINI_STOP_ON_FIRST_ERROR=1'],
        'return codes': [1, 1, 3, 0, 0, 0, 0, -1, 2, 3, ],
        },
    'handler lineno': {
        'opts': ['-DINI_HANDLER_LINENO=1'],
        'return codes': [1, 1, 3, 0, 0, 0, 0, -1, 2, 3, ],
        },
    'handler on new section': {
        'opts': ['-DINI_CALL_HANDLER_ON_NEW_SECTION=1'],
        'return codes': [1, 1, 3, 0, 0, 0, 0, -1, 2, 3, ],
        },
    'allow no value': {
        'opts': ['-DINI_ALLOW_NO_VALUE=1'],
        'return codes': [0, 0, 3, 0, 0, 0, 0, -1, 0, 3, ],
        },
    }

heap_extra_opts = [
    '-DINI_USE_STACK=0',
    '-DINI_INITIAL_ALLOC=20',
    '-DINI_ALLOW_REALLOC=1',
    ]

foreach name, case : ini_cases
    t = ' '.join(['utest_ini', name])

    utest = executable(t.underscorify(), ['../ini.c', 'unittest.c'],
        include_directories: inc_inih,
        c_args: case['opts'],
        )

    utest_heap = executable(t.underscorify() + '_heap', ['../ini.c', 'unittest.c'],
        include_directories: inc_inih,
        c_args: case['opts'] + heap_extra_opts,
        )

    i = 0
    foreach ini : inifiles
        test(name, utest,
            workdir: join_paths(meson.source_root(), 'tests'),
            args: [ini, case['return codes'][i].to_string()],
            suite: ['file', ini, 'stack'],
            )

        test(name, utest_heap,
            workdir: join_paths(meson.source_root(), 'tests'),
            args: [ini, case['return codes'][i].to_string()],
            suite: ['file', ini, 'heap'],
            )

        i += 1

    endforeach
endforeach
