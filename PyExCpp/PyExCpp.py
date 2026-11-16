
import TestCaps
print('hello world')
caps = TestCaps.new_test("hello world")
TestCaps.init_test(caps)
print(TestCaps.read_test(caps))
