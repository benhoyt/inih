runtest = files(join_paths(meson.project_source_root(), 'tests', 'runtest.sh'))

tests = {
  'multi_max_line': { 'args': ['-DINI_MAX_LINE=20'] },
}

foreach name, properties : tests
  test_src = 'src' in properties ? properties['src'] : 'unittest.c'
  exe = executable('unittest_' + name, src_inih, test_src, c_args : ['-Wall', properties['args']])
  test('test_' + name, runtest, depends : [exe], args : [files('baseline_' + name + '.txt'), exe.full_path()])
endforeach
