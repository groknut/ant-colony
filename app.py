

import configparser
import pandas as pd
import matplotlib.pyplot as pp
import functools
import time
from pathlib import Path
import sys
import os
import subprocess as sp

def timeit(func):
	@functools.wraps(func)
	def wrapper(*args, **kwargs):
		start = time.perf_counter()
		result = func(*args, **kwargs)
		print(f"{func.__name__}: {time.perf_counter() - start:.4f}с")
		return result
	return wrapper

class AntColonyApp:
	def __init__(self, config_file="config.ic"):
		self.config_file = Path(config_file)
		self.config = configparser.ConfigParser(
			delimiters=[':'],
			comment_prefixes=[';'],  # строка начинается с ;
			inline_comment_prefixes=[';']  # комментарий в конце строки
		)

		self.load_config()

		self.cpp_app = None
		self.os_name = os.name
		self.csv_file = None
		self.graphic_path = None
		self.mean_graphic_path = None
		

		self.init_from_config()

	def load_config(self):
		if self.config_file.exists():
			self.config.read(self.config_file)
			return {
				sect: dict(self.config.items(sect)) for sect in self.config.sections()
			}
		return {}


	def init_from_config(self):
		self.cpp_app = self.config['app'].get("name", "main")
		self.csv_file = self.config['output'].get("output_file", "./output/output.csv")
		self.graphic_path = self.config['output'].get("graphic", "./output/graphic.png")
		self.mean_graphic_path = self.config['output'].get("mean_graphic", "./output/mean_graphic.png")

	@timeit
	def compile_cpp(self):
		exe_path = str(Path(self.cpp_app).with_suffix('.exe'))  if os.name == "nt" else str(Path(self.cpp_app))
		if not os.path.exists(exe_path):
			sp.run(f"g++ main.cpp src/*.cpp -o {self.cpp_app}", shell=True)

	@timeit
	def start_cpp(self):
		exe_path = str(Path(self.cpp_app).with_suffix('.exe'))  if os.name == "nt" else str(Path(self.cpp_app))
		sp.run(
		f"{exe_path} {self.config_file}", shell=True
		)

	@timeit
	def pp_graphic(self):
		df = pd.read_csv(self.csv_file);

		param = df[df['CurrentBestLength'] != 1e6].empty

		if not param:
			df = df[df['CurrentBestLength'] != 1e6]

		# статистика
		print(
		rf"""
		Записей в таблице: {len(df)} (считаем от нуля)
		Количество неполных путей: {len(df[df['PathType'] == 0])}
		Количество полных путей: {len(df[df['PathType'] == 1])}
		Количество итераций: {max(df['Iteration'])+1}
		Количество муравьев на одну итерацию: {max(df[df['Iteration'] == 0]['AntId'])}
		Найденные лучшие пути: {df['CurrentBestLength'].unique()}
		Максимальное количество феромона на оптимальном пути: {max(df['PhersOptimal'])}
		Минимальное количество феромона на оптимальном пути: {min(df['PhersOptimal'])}
		"""
		)

		print(df)

		# рисуем 2 графика

		pp.figure(figsize=(14,6))

		pp.subplot(121)
		# Количество феромонов на оптимальном пути и пути, пройденном муравьем
		pp.plot(
		df['Iteration'], df['Phers'], label='Глобальное количество феромонов'
		)
		pp.plot(
			df['Iteration'], df['PhersOptimal'], label='Феромоны на оптимальном пути'
		)

		pp.xlabel("Количество итераций")
		pp.ylabel("Количество феромонов")
		pp.legend(fontsize=8)


		pp.subplot(122)
		# Длина оптимального пути по отношению к пройденному муравьем
		pp.plot(
			df['Iteration'], df['AntPathLength'], label='Длина пути, который прошел муравей'
		)
		pp.plot(
			df['Iteration'], df['CurrentBestLength'], label='Лучшая длина пути на данный момент'
		)

		pp.xlabel("Количество итераций")
		pp.ylabel("Длина пути")
		pp.legend(loc='upper right', framealpha=0.9, fontsize=8)

		pp.suptitle('Процесс работы алгоритма', fontsize=16, y=0.98)

		pp.subplots_adjust(wspace=0.6)

		pp.savefig(self.graphic_path)

		pp.show()

		df = df.groupby('Iteration').mean(numeric_only=True)

		print(df)

		# Длина оптимального пути по отношению к пройденному муравьем
		pp.plot(
			df.index, df['AntPathLength'], label='Длина пути, который прошел муравей'
		)

		pp.plot(
			df.index, df['CurrentBestLength'], label='Лучшая длина пути на данный момент'
		)
		pp.plot(
			df.index, df['PhersOptimal'], label='Феромоны на оптимальном пути'
		)

		pp.xlabel("Количество итераций")
		pp.ylabel("Параметры")

		pp.legend()

		pp.title("Процесс работы алгоритма по итерациям")    

		pp.savefig(self.mean_graphic_path)

		pp.show()

		
	def run(self):
		print("🚀 Start pipeline...")

		self.compile_cpp()
		self.start_cpp()
		self.pp_graphic()


if len(sys.argv) == 2:
	app = AntColonyApp(sys.argv[1])
	app.run()
else:
	app = AntColonyApp()
	app.run()
		




