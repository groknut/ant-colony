
import functools
import time

def timeit(func):
	@functools.wraps(func)
	def wrapper(*args, **kwargs):
		start = time.perf_counter()
		result = func(*args, **kwargs)
		print(f"{func.__name__}: {time.perf_counter() - start:.4f}с")
		return result
	return wrapper
