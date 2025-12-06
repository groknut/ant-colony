

import pandas as pd
import matplotlib.pyplot as pp
import sys
from config import timeit

@timeit
def pp_graphic(csv_file):

    df = pd.read_csv(csv_file);

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

    pp.savefig('output/graphic.png')
    
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

    pp.savefig('output/mean-graphic.png')
    
    pp.show()
    
def main():

    if len(sys.argv) != 2:
        print(f"""
Usage: python {sys.argv[0]} <output_csv_file>
Example: python {sys.argv[0]} output.csv
        """)
    else:
        pp_graphic(sys.argv[1])

if __name__ == "__main__":
    main()
