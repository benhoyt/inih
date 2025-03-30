runtest = files(join_paths(meson.project_source_root(), 'tests', 'runtest.sh'))

tests = {
  'multi': { 'args': [] },
  'multi_max_line': { 'args': ['-DINI_MAX_LINE=20'] },
  'single': { 'args': ['-DINI_ALLOW_MULTILINE=0'] },
  'disallow_inline_comments': { 'args': ['-DINI_ALLOW_INLINE_COMMENTS=0'] },
  'stop_on_first_error': { 'args': ['-DINI_STOP_ON_FIRST_ERROR=1'] },
  'handler_lineno': { 'args': ['-DINI_HANDLER_LINENO=1'] },
  'string': { 'src': 'unittest_string.c', 'args': ['-DINI_MAX_LINE=20'] },
  'heap': { 'args': ['-DINI_USE_STACK=0'] },
  'heap_max_line': { 'args': ['-DINI_USE_STACK=0', '-DINI_MAX_LINE=20', '-DINI_INITIAL_ALLOC=20'] },
  'heap_realloc': { 'args': ['-DINI_USE_STACK=0', '-DINI_ALLOW_REALLOC=1', '-DINI_INITIAL_ALLOC=5'] },
  'heap_realloc_max_line': { 'args': ['-DINI_USE_STACK=0', '-DINI_MAX_LINE=20', '-DINI_ALLOW_REALLOC=1', '-DINI_INITIAL_ALLOC=5'] },
  'heap_string': { 'src': 'unittest_string.c', 'args': ['-DINI_USE_STACK=0', '-DINI_MAX_LINE=20', '-DINI_INITIAL_ALLOC=20'] },
  'call_handler_on_new_section': { 'args': ['-DINI_CALL_HANDLER_ON_NEW_SECTION=1'] },
  'allow_no_value': { 'args': ['-DINI_ALLOW_NO_VALUE=1'] },
  'alloc': { 'src': 'unittest_alloc.c', 'args': ['-DINI_CUSTOM_ALLOCATOR=1', '-DINI_USE_STACK=0', '-DINI_ALLOW_REALLOC=1', '-DINI_INITIAL_ALLOC=12'] }
}

foreach name, properties : tests
  test_src = 'src' in properties ? properties['src'] : 'unittest.c'
  exe = executable('unittest_' + name, src_inih, test_src, c_args : ['-Wall', properties['args']])
  test('test_' + name, runtest, depends : [exe], args : [files('baseline_' + name + '.txt'), exe.full_path()])
endforeach
