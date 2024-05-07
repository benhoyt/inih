runtest = files(join_paths(meson.project_source_root(), 'tests', 'runtest.sh'))

tests = {
  'INIReaderExample': { 'args': [] },
}

foreach name, properties : tests
  exe = executable('unittest_' + name, src_inih, src_INIReader, 'INIReaderExample.cpp', cpp_args : ['-Wall', properties['args']])
  test('test_' + name, runtest, depends : [exe], args : [files('cpptest.txt'), exe.full_path()])
endforeach
