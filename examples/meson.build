runtest = files(join_paths(meson.project_source_root(), 'tests', 'runtest.sh'))

tests = {
  'INIReaderExample': { 'src': 'INIReaderExample.cpp', 'expected': 'cpptest.txt' },
  'INIReaderInteger': { 'src': 'INIReaderTest.cpp', 'expected': 'cpptestintegers.txt' },
}

foreach name, properties : tests
  exe = executable('unittest_' + name, src_inih, src_INIReader, properties['src'], cpp_args : ['-Wall'])
  test('test_' + name, runtest, depends : [exe], args : [files(properties['expected']), exe.full_path()])
endforeach
