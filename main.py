

import pandas as pd
import matplotlib.pyplot as pp
import sys

    
 
def pp_graphic(csv_file):
    df = pd.read_csv(csv_file);

    print(
        df.head(10)
    )
    
    pp.plot(
        df['Iteration'], df['AntPathLength'], label='Длина пути, который прошел муравей'
    )

    pp.xlabel('Iteration')
    pp.ylabel('Ant Path Length');
    pp.title('Работа алгоритма')
    pp.legend(loc='upper right', framealpha=0.9)
    pp.show()

def main():

    if len(sys.argv) != 2:
        print(r"""
Usage: python main,py <output_csv_file>
Example: python main.py output.csv
        """)
        sys.exit(1);
    f = sys.argv[1]
    try:
        pp_graphic(f);
    except FileNotFoundError:
        print(f"Error: File '{f}' not found ")     
        sys.exit(1)   
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
    

if __name__ == "__main__":
    main()
